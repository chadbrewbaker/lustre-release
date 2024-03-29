/*
 * GPL HEADER START
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 only,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.  A copy is
 * included in the COPYING file that accompanied this code.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * GPL HEADER END
 */
/*
 * Copyright  2009 Sun Microsystems, Inc. All rights reserved
 * Use is subject to license terms.
 *
 * Copyright (c) 2012, 2017, Intel Corporation.
 */
/*
 * This file is part of Lustre, http://www.lustre.org/
 * Lustre is a trademark of Sun Microsystems, Inc.
 *
 * lustre/lod/lod_qos.c
 *
 * Implementation of different allocation algorithm used
 * to distribute objects and data among OSTs.
 */

#define DEBUG_SUBSYSTEM S_LOV

#include <asm/div64.h>
#include <linux/random.h>

#include <libcfs/libcfs.h>
#include <uapi/linux/lustre/lustre_idl.h>
#include <lustre_swab.h>
#include <obd_class.h>

#include "lod_internal.h"

/*
 * force QoS policy (not RR) to be used for testing purposes
 */
#define FORCE_QOS_

#define D_QOS   D_OTHER

#define QOS_DEBUG(fmt, ...)     CDEBUG(D_QOS, fmt, ## __VA_ARGS__)
#define QOS_CONSOLE(fmt, ...)   LCONSOLE(D_QOS, fmt, ## __VA_ARGS__)

#define TGT_BAVAIL(i) (OST_TGT(lod,i)->ltd_statfs.os_bavail * \
		       OST_TGT(lod,i)->ltd_statfs.os_bsize)

/**
 * Check whether the target is available for new OST objects.
 *
 * Request statfs data from the given target and verify it's active and not
 * read-only. If so, then it can be used to place new OST objects. This
 * function also maintains the number of active/inactive targets and sets
 * dirty flags if those numbers change so others can run re-balance procedures.
 * No external locking is required.
 *
 * \param[in] env	execution environment for this thread
 * \param[in] d		LOD device
 * \param[in] index	index of OST target to check
 * \param[out] sfs	buffer for statfs data
 *
 * \retval 0		if the target is good
 * \retval negative	negated errno on error

 */
static int lod_statfs_and_check(const struct lu_env *env, struct lod_device *d,
				int index, struct obd_statfs *sfs)
{
	struct lod_tgt_desc *ost;
	int		     rc;
	ENTRY;

	LASSERT(d);
	ost = OST_TGT(d,index);
	LASSERT(ost);

	rc = dt_statfs(env, ost->ltd_ost, sfs);

	if (rc == 0 && ((sfs->os_state & OS_STATE_ENOSPC) ||
	    (sfs->os_state & OS_STATE_ENOINO && sfs->os_fprecreated == 0)))
		RETURN(-ENOSPC);

	if (rc && rc != -ENOTCONN)
		CERROR("%s: statfs: rc = %d\n", lod2obd(d)->obd_name, rc);

	/* If the OST is readonly then we can't allocate objects there */
	if (sfs->os_state & OS_STATE_READONLY)
		rc = -EROFS;

	/* object precreation is skipped on the OST with max_create_count=0 */
	if (sfs->os_state & OS_STATE_NOPRECREATE)
		rc = -ENOBUFS;

	/* check whether device has changed state (active, inactive) */
	if (rc != 0 && ost->ltd_active) {
		/* turned inactive? */
		spin_lock(&d->lod_lock);
		if (ost->ltd_active) {
			ost->ltd_active = 0;
			if (rc == -ENOTCONN)
				ost->ltd_connecting = 1;

			LASSERT(d->lod_desc.ld_active_tgt_count > 0);
			d->lod_desc.ld_active_tgt_count--;
			d->lod_qos.lq_dirty = 1;
			d->lod_qos.lq_rr.lqr_dirty = 1;
			CDEBUG(D_CONFIG, "%s: turns inactive\n",
			       ost->ltd_exp->exp_obd->obd_name);
		}
		spin_unlock(&d->lod_lock);
	} else if (rc == 0 && ost->ltd_active == 0) {
		/* turned active? */
		LASSERTF(d->lod_desc.ld_active_tgt_count < d->lod_ostnr,
			 "active tgt count %d, ost nr %d\n",
			 d->lod_desc.ld_active_tgt_count, d->lod_ostnr);
		spin_lock(&d->lod_lock);
		if (ost->ltd_active == 0) {
			ost->ltd_active = 1;
			ost->ltd_connecting = 0;
			d->lod_desc.ld_active_tgt_count++;
			d->lod_qos.lq_dirty = 1;
			d->lod_qos.lq_rr.lqr_dirty = 1;
			CDEBUG(D_CONFIG, "%s: turns active\n",
			       ost->ltd_exp->exp_obd->obd_name);
		}
		spin_unlock(&d->lod_lock);
	}

	RETURN(rc);
}

/**
 * Maintain per-target statfs data.
 *
 * The function refreshes statfs data for all the targets every N seconds.
 * The actual N is controlled via procfs and set to LOV_DESC_QOS_MAXAGE_DEFAULT
 * initially.
 *
 * \param[in] env	execution environment for this thread
 * \param[in] lod	LOD device
 */
void lod_qos_statfs_update(const struct lu_env *env, struct lod_device *lod)
{
	struct obd_device *obd = lod2obd(lod);
	struct ost_pool *osts = &(lod->lod_pool_info);
	time64_t max_age;
	unsigned int i;
	u64 avail;
	int idx;
	ENTRY;

	max_age = ktime_get_seconds() - 2 * lod->lod_desc.ld_qos_maxage;

	if (obd->obd_osfs_age > max_age)
		/* statfs data are quite recent, don't need to refresh it */
		RETURN_EXIT;

	down_write(&lod->lod_qos.lq_rw_sem);

	if (obd->obd_osfs_age > max_age)
		goto out;

	for (i = 0; i < osts->op_count; i++) {
		idx = osts->op_array[i];
		avail = OST_TGT(lod,idx)->ltd_statfs.os_bavail;
		if (lod_statfs_and_check(env, lod, idx,
					 &OST_TGT(lod, idx)->ltd_statfs))
			continue;
		if (OST_TGT(lod,idx)->ltd_statfs.os_bavail != avail)
			/* recalculate weigths */
			lod->lod_qos.lq_dirty = 1;
	}
	obd->obd_osfs_age = ktime_get_seconds();

out:
	up_write(&lod->lod_qos.lq_rw_sem);
	EXIT;
}

#define LOV_QOS_EMPTY ((__u32)-1)

/**
 * Calculate optimal round-robin order with regard to OSSes.
 *
 * Place all the OSTs from pool \a src_pool in a special array to be used for
 * round-robin (RR) stripe allocation.  The placement algorithm interleaves
 * OSTs from the different OSSs so that RR allocation can balance OSSs evenly.
 * Resorts the targets when the number of active targets changes (because of
 * a new target or activation/deactivation).
 *
 * \param[in] lod	LOD device
 * \param[in] src_pool	OST pool
 * \param[in] lqr	round-robin list
 *
 * \retval 0		on success
 * \retval -ENOMEM	fails to allocate the array
 */
static int lod_qos_calc_rr(struct lod_device *lod, struct ost_pool *src_pool,
			   struct lu_qos_rr *lqr)
{
	struct lu_svr_qos  *oss;
	struct lod_tgt_desc *ost;
	unsigned placed, real_count;
	unsigned int i;
	int rc;
	ENTRY;

	if (!lqr->lqr_dirty) {
		LASSERT(lqr->lqr_pool.op_size);
		RETURN(0);
	}

	/* Do actual allocation. */
	down_write(&lod->lod_qos.lq_rw_sem);

	/*
	 * Check again. While we were sleeping on @lq_rw_sem something could
	 * change.
	 */
	if (!lqr->lqr_dirty) {
		LASSERT(lqr->lqr_pool.op_size);
		up_write(&lod->lod_qos.lq_rw_sem);
		RETURN(0);
	}

	real_count = src_pool->op_count;

	/* Zero the pool array */
	/* alloc_rr is holding a read lock on the pool, so nobody is adding/
	   deleting from the pool. The lq_rw_sem insures that nobody else
	   is reading. */
	lqr->lqr_pool.op_count = real_count;
	rc = lod_ost_pool_extend(&lqr->lqr_pool, real_count);
	if (rc) {
		up_write(&lod->lod_qos.lq_rw_sem);
		RETURN(rc);
	}
	for (i = 0; i < lqr->lqr_pool.op_count; i++)
		lqr->lqr_pool.op_array[i] = LOV_QOS_EMPTY;

	/* Place all the OSTs from 1 OSS at the same time. */
	placed = 0;
	list_for_each_entry(oss, &lod->lod_qos.lq_svr_list, lsq_svr_list) {
		int j = 0;

		for (i = 0; i < lqr->lqr_pool.op_count; i++) {
			int next;

			if (!cfs_bitmap_check(lod->lod_ost_bitmap,
						src_pool->op_array[i]))
				continue;

			ost = OST_TGT(lod,src_pool->op_array[i]);
			LASSERT(ost && ost->ltd_ost);
			if (ost->ltd_qos.ltq_svr != oss)
				continue;

			/* Evenly space these OSTs across arrayspace */
			next = j * lqr->lqr_pool.op_count / oss->lsq_tgt_count;
			while (lqr->lqr_pool.op_array[next] != LOV_QOS_EMPTY)
				next = (next + 1) % lqr->lqr_pool.op_count;

			lqr->lqr_pool.op_array[next] = src_pool->op_array[i];
			j++;
			placed++;
		}
	}

	lqr->lqr_dirty = 0;
	up_write(&lod->lod_qos.lq_rw_sem);

	if (placed != real_count) {
		/* This should never happen */
		LCONSOLE_ERROR_MSG(0x14e, "Failed to place all OSTs in the "
				   "round-robin list (%d of %d).\n",
				   placed, real_count);
		for (i = 0; i < lqr->lqr_pool.op_count; i++) {
			LCONSOLE(D_WARNING, "rr #%d ost idx=%d\n", i,
				 lqr->lqr_pool.op_array[i]);
		}
		lqr->lqr_dirty = 1;
		RETURN(-EAGAIN);
	}

#if 0
	for (i = 0; i < lqr->lqr_pool.op_count; i++)
		QOS_CONSOLE("rr #%d ost idx=%d\n", i, lqr->lqr_pool.op_array[i]);
#endif

	RETURN(0);
}

/**
 * Instantiate and declare creation of a new object.
 *
 * The function instantiates LU representation for a new object on the
 * specified device. Also it declares an intention to create that
 * object on the storage target.
 *
 * Note lu_object_anon() is used which is a trick with regard to LU/OSD
 * infrastructure - in the existing precreation framework we can't assign FID
 * at this moment, we do this later once a transaction is started. So the
 * special method instantiates FID-less object in the cache and later it
 * will get a FID and proper placement in LU cache.
 *
 * \param[in] env	execution environment for this thread
 * \param[in] d		LOD device
 * \param[in] ost_idx	OST target index where the object is being created
 * \param[in] th	transaction handle
 *
 * \retval		object ptr on success, ERR_PTR() otherwise
 */
static struct dt_object *lod_qos_declare_object_on(const struct lu_env *env,
						   struct lod_device *d,
						   __u32 ost_idx,
						   struct thandle *th)
{
	struct lod_tgt_desc *ost;
	struct lu_object *o, *n;
	struct lu_device *nd;
	struct dt_object *dt;
	int               rc;
	ENTRY;

	LASSERT(d);
	LASSERT(ost_idx < d->lod_osts_size);
	ost = OST_TGT(d,ost_idx);
	LASSERT(ost);
	LASSERT(ost->ltd_ost);

	nd = &ost->ltd_ost->dd_lu_dev;

	/*
	 * allocate anonymous object with zero fid, real fid
	 * will be assigned by OSP within transaction
	 * XXX: to be fixed with fully-functional OST fids
	 */
	o = lu_object_anon(env, nd, NULL);
	if (IS_ERR(o))
		GOTO(out, dt = ERR_PTR(PTR_ERR(o)));

	n = lu_object_locate(o->lo_header, nd->ld_type);
	if (unlikely(n == NULL)) {
		CERROR("can't find slice\n");
		lu_object_put(env, o);
		GOTO(out, dt = ERR_PTR(-EINVAL));
	}

	dt = container_of(n, struct dt_object, do_lu);

	rc = lod_sub_declare_create(env, dt, NULL, NULL, NULL, th);
	if (rc < 0) {
		CDEBUG(D_OTHER, "can't declare creation on #%u: %d\n",
		       ost_idx, rc);
		lu_object_put(env, o);
		dt = ERR_PTR(rc);
	}

out:
	RETURN(dt);
}

/**
 * Calculate a minimum acceptable stripe count.
 *
 * Return an acceptable stripe count depending on flag LOV_USES_DEFAULT_STRIPE:
 * all stripes or 3/4 of stripes.
 *
 * \param[in] stripe_count	number of stripes requested
 * \param[in] flags		0 or LOV_USES_DEFAULT_STRIPE
 *
 * \retval			acceptable stripecount
 */
static int min_stripe_count(__u32 stripe_count, int flags)
{
	return (flags & LOV_USES_DEFAULT_STRIPE ?
		stripe_count - (stripe_count / 4) : stripe_count);
}

#define LOV_CREATE_RESEED_MULT 30
#define LOV_CREATE_RESEED_MIN  2000

/**
 * Initialize temporary OST-in-use array.
 *
 * Allocate or extend the array used to mark targets already assigned to a new
 * striping so they are not used more than once.
 *
 * \param[in] env	execution environment for this thread
 * \param[in] stripes	number of items needed in the array
 *
 * \retval 0		on success
 * \retval -ENOMEM	on error
 */
static inline int lod_qos_ost_in_use_clear(const struct lu_env *env,
					   __u32 stripes)
{
	struct lod_thread_info *info = lod_env_info(env);

	if (info->lti_ea_store_size < sizeof(int) * stripes)
		lod_ea_store_resize(info, stripes * sizeof(int));
	if (info->lti_ea_store_size < sizeof(int) * stripes) {
		CERROR("can't allocate memory for ost-in-use array\n");
		return -ENOMEM;
	}
	memset(info->lti_ea_store, -1, sizeof(int) * stripes);
	return 0;
}

/**
 * Remember a target in the array of used targets.
 *
 * Mark the given target as used for a new striping being created. The status
 * of an OST in a striping can be checked with lod_qos_is_ost_used().
 *
 * \param[in] env	execution environment for this thread
 * \param[in] idx	index in the array
 * \param[in] ost	OST target index to mark as used
 */
static inline void lod_qos_ost_in_use(const struct lu_env *env,
				      int idx, int ost)
{
	struct lod_thread_info *info = lod_env_info(env);
	int *osts = info->lti_ea_store;

	LASSERT(info->lti_ea_store_size >= idx * sizeof(int));
	osts[idx] = ost;
}

/**
 * Check is OST used in a striping.
 *
 * Checks whether OST with the given index is marked as used in the temporary
 * array (see lod_qos_ost_in_use()).
 *
 * \param[in] env	execution environment for this thread
 * \param[in] ost	OST target index to check
 * \param[in] stripes	the number of items used in the array already
 *
 * \retval 0		not used
 * \retval 1		used
 */
static int lod_qos_is_ost_used(const struct lu_env *env, int ost, __u32 stripes)
{
	struct lod_thread_info *info = lod_env_info(env);
	int *osts = info->lti_ea_store;
	__u32 j;

	for (j = 0; j < stripes; j++) {
		if (osts[j] == ost)
			return 1;
	}
	return 0;
}

static inline bool
lod_obj_is_ost_use_skip_cb(const struct lu_env *env, struct lod_object *lo,
			   int comp_idx, struct lod_obj_stripe_cb_data *data)
{
	struct lod_layout_component *comp = &lo->ldo_comp_entries[comp_idx];

	return comp->llc_ost_indices == NULL;
}

static inline int
lod_obj_is_ost_use_cb(const struct lu_env *env, struct lod_object *lo,
		      int comp_idx, struct lod_obj_stripe_cb_data *data)
{
	struct lod_layout_component *comp = &lo->ldo_comp_entries[comp_idx];
	int i;

	for (i = 0; i < comp->llc_stripe_count; i++) {
		if (comp->llc_ost_indices[i] == data->locd_ost_index) {
			data->locd_ost_index = -1;
			return -EEXIST;
		}
	}

	return 0;
}

/**
 * Check is OST used in a composite layout
 *
 * \param[in] lo	lod object
 * \param[in] ost	OST target index to check
 *
 * \retval false	not used
 * \retval true		used
 */
static inline bool lod_comp_is_ost_used(const struct lu_env *env,
				       struct lod_object *lo, int ost)
{
	struct lod_obj_stripe_cb_data data = { { 0 } };

	data.locd_ost_index = ost;
	data.locd_comp_skip_cb = lod_obj_is_ost_use_skip_cb;
	data.locd_comp_cb = lod_obj_is_ost_use_cb;

	(void)lod_obj_for_each_stripe(env, lo, NULL, &data);

	return data.locd_ost_index == -1;
}

static inline void lod_avoid_update(struct lod_object *lo,
				    struct lod_avoid_guide *lag)
{
	if (!lod_is_flr(lo))
		return;

	lag->lag_ost_avail--;
}

static inline bool lod_should_avoid_ost(struct lod_object *lo,
					struct lod_avoid_guide *lag,
					__u32 index)
{
	struct lod_device *lod = lu2lod_dev(lo->ldo_obj.do_lu.lo_dev);
	struct lod_tgt_desc *ost = OST_TGT(lod, index);
	struct lu_svr_qos *lsq = ost->ltd_qos.ltq_svr;
	bool used = false;
	int i;

	if (!cfs_bitmap_check(lod->lod_ost_bitmap, index)) {
		QOS_DEBUG("OST%d: been used in conflicting mirror component\n",
			  index);
		return true;
	}

	/**
	 * we've tried our best, all available OSTs have been used in
	 * overlapped components in the other mirror
	 */
	if (lag->lag_ost_avail == 0)
		return false;

	/* check OSS use */
	for (i = 0; i < lag->lag_oaa_count; i++) {
		if (lag->lag_oss_avoid_array[i] == lsq->lsq_id) {
			used = true;
			break;
		}
	}
	/**
	 * if the OSS which OST[index] resides has not been used, we'd like to
	 * use it
	 */
	if (!used)
		return false;

	/* if the OSS has been used, check whether the OST has been used */
	if (!cfs_bitmap_check(lag->lag_ost_avoid_bitmap, index))
		used = false;
	else
		QOS_DEBUG("OST%d: been used in conflicting mirror component\n",
			  index);
	return used;
}

static int lod_check_and_reserve_ost(const struct lu_env *env,
				     struct lod_object *lo,
				     struct lod_layout_component *lod_comp,
				     struct obd_statfs *sfs, __u32 ost_idx,
				     __u32 speed, __u32 *s_idx,
				     struct dt_object **stripe,
				     __u32 *ost_indices,
				     struct thandle *th,
				     bool *overstriped)
{
	struct lod_device *lod = lu2lod_dev(lo->ldo_obj.do_lu.lo_dev);
	struct lod_avoid_guide *lag = &lod_env_info(env)->lti_avoid;
	struct dt_object   *o;
	__u32 stripe_idx = *s_idx;
	int rc;
	ENTRY;

	rc = lod_statfs_and_check(env, lod, ost_idx, sfs);
	if (rc)
		RETURN(rc);

	/*
	 * We expect number of precreated objects in f_ffree at
	 * the first iteration, skip OSPs with no objects ready
	 */
	if (sfs->os_fprecreated == 0 && speed == 0) {
		QOS_DEBUG("#%d: precreation is empty\n", ost_idx);
		RETURN(rc);
	}

	/*
	 * try to use another OSP if this one is degraded
	 */
	if (sfs->os_state & OS_STATE_DEGRADED && speed < 2) {
		QOS_DEBUG("#%d: degraded\n", ost_idx);
		RETURN(rc);
	}

	/*
	 * try not allocate on OST which has been used by other
	 * component
	 */
	if (speed == 0 && lod_comp_is_ost_used(env, lo, ost_idx)) {
		QOS_DEBUG("iter %d: OST%d used by other component\n",
			  speed, ost_idx);
		RETURN(rc);
	}

	/**
	 * try not allocate OSTs used by conflicting component of other mirrors
	 * for the first and second time.
	 */
	if (speed < 2 && lod_should_avoid_ost(lo, lag, ost_idx)) {
		QOS_DEBUG("iter %d: OST%d used by conflicting mirror "
			  "component\n", speed, ost_idx);
		RETURN(rc);
	}

	/* do not put >1 objects on a single OST, except for overstriping */
	if (lod_qos_is_ost_used(env, ost_idx, stripe_idx)) {
		if (lod_comp->llc_pattern & LOV_PATTERN_OVERSTRIPING)
			*overstriped = true;
		else
			RETURN(rc);
	}

	o = lod_qos_declare_object_on(env, lod, ost_idx, th);
	if (IS_ERR(o)) {
		CDEBUG(D_OTHER, "can't declare new object on #%u: %d\n",
		       ost_idx, (int) PTR_ERR(o));
		rc = PTR_ERR(o);
		RETURN(rc);
	}

	/*
	 * We've successfully declared (reserved) an object
	 */
	lod_avoid_update(lo, lag);
	lod_qos_ost_in_use(env, stripe_idx, ost_idx);
	stripe[stripe_idx] = o;
	ost_indices[stripe_idx] = ost_idx;
	OBD_FAIL_TIMEOUT(OBD_FAIL_MDS_LOV_CREATE_RACE, 2);
	stripe_idx++;
	*s_idx = stripe_idx;

	RETURN(rc);
}

/**
 * Allocate a striping using round-robin algorithm.
 *
 * Allocates a new striping using round-robin algorithm. The function refreshes
 * all the internal structures (statfs cache, array of available OSTs sorted
 * with regard to OSS, etc). The number of stripes required is taken from the
 * object (must be prepared by the caller), but can change if the flag
 * LOV_USES_DEFAULT_STRIPE is supplied. The caller should ensure nobody else
 * is trying to create a striping on the object in parallel. All the internal
 * structures (like pools, etc) are protected and no additional locking is
 * required. The function succeeds even if a single stripe is allocated. To save
 * time we give priority to targets which already have objects precreated.
 * Full OSTs are skipped (see lod_qos_dev_is_full() for the details).
 *
 * \param[in] env		execution environment for this thread
 * \param[in] lo		LOD object
 * \param[out] stripe		striping created
 * \param[out] ost_indices	ost indices of striping created
 * \param[in] flags		allocation flags (0 or LOV_USES_DEFAULT_STRIPE)
 * \param[in] th		transaction handle
 * \param[in] comp_idx		index of ldo_comp_entries
 *
 * \retval 0		on success
 * \retval -ENOSPC	if not enough OSTs are found
 * \retval negative	negated errno for other failures
 */
static int lod_alloc_rr(const struct lu_env *env, struct lod_object *lo,
			struct dt_object **stripe, __u32 *ost_indices,
			int flags, struct thandle *th, int comp_idx)
{
	struct lod_layout_component *lod_comp;
	struct lod_device *m = lu2lod_dev(lo->ldo_obj.do_lu.lo_dev);
	struct obd_statfs *sfs = &lod_env_info(env)->lti_osfs;
	struct pool_desc  *pool = NULL;
	struct ost_pool   *osts;
	struct lu_qos_rr *lqr;
	unsigned int	i, array_idx;
	__u32 ost_start_idx_temp;
	__u32 stripe_idx = 0;
	__u32 stripe_count, stripe_count_min, ost_idx;
	int rc, speed = 0, ost_connecting = 0;
	int stripes_per_ost = 1;
	bool overstriped = false;
	ENTRY;

	LASSERT(lo->ldo_comp_cnt > comp_idx && lo->ldo_comp_entries != NULL);
	lod_comp = &lo->ldo_comp_entries[comp_idx];
	stripe_count = lod_comp->llc_stripe_count;
	stripe_count_min = min_stripe_count(stripe_count, flags);

	if (lod_comp->llc_pool != NULL)
		pool = lod_find_pool(m, lod_comp->llc_pool);

	if (pool != NULL) {
		down_read(&pool_tgt_rw_sem(pool));
		osts = &(pool->pool_obds);
		lqr = &(pool->pool_rr);
	} else {
		osts = &(m->lod_pool_info);
		lqr = &(m->lod_qos.lq_rr);
	}

	rc = lod_qos_calc_rr(m, osts, lqr);
	if (rc)
		GOTO(out, rc);

	rc = lod_qos_ost_in_use_clear(env, stripe_count);
	if (rc)
		GOTO(out, rc);

	down_read(&m->lod_qos.lq_rw_sem);
	spin_lock(&lqr->lqr_alloc);
	if (--lqr->lqr_start_count <= 0) {
		lqr->lqr_start_idx = prandom_u32_max(osts->op_count);
		lqr->lqr_start_count =
			(LOV_CREATE_RESEED_MIN / max(osts->op_count, 1U) +
			 LOV_CREATE_RESEED_MULT) * max(osts->op_count, 1U);
	} else if (stripe_count_min >= osts->op_count ||
			lqr->lqr_start_idx > osts->op_count) {
		/* If we have allocated from all of the OSTs, slowly
		 * precess the next start if the OST/stripe count isn't
		 * already doing this for us. */
		lqr->lqr_start_idx %= osts->op_count;
		if (stripe_count > 1 && (osts->op_count % stripe_count) != 1)
			++lqr->lqr_offset_idx;
	}
	ost_start_idx_temp = lqr->lqr_start_idx;

repeat_find:

	QOS_DEBUG("pool '%s' want %d start_idx %d start_count %d offset %d "
		  "active %d count %d\n",
		  lod_comp->llc_pool ? lod_comp->llc_pool : "",
		  stripe_count, lqr->lqr_start_idx, lqr->lqr_start_count,
		  lqr->lqr_offset_idx, osts->op_count, osts->op_count);

	if (lod_comp->llc_pattern & LOV_PATTERN_OVERSTRIPING)
		stripes_per_ost =
			(lod_comp->llc_stripe_count - 1)/osts->op_count + 1;

	for (i = 0; i < osts->op_count * stripes_per_ost
	     && stripe_idx < stripe_count; i++) {
		array_idx = (lqr->lqr_start_idx + lqr->lqr_offset_idx) %
				osts->op_count;
		++lqr->lqr_start_idx;
		ost_idx = lqr->lqr_pool.op_array[array_idx];

		QOS_DEBUG("#%d strt %d act %d strp %d ary %d idx %d\n",
			  i, lqr->lqr_start_idx, /* XXX: active*/ 0,
			  stripe_idx, array_idx, ost_idx);

		if ((ost_idx == LOV_QOS_EMPTY) ||
		    !cfs_bitmap_check(m->lod_ost_bitmap, ost_idx))
			continue;

		/* Fail Check before osc_precreate() is called
		   so we can only 'fail' single OSC. */
		if (OBD_FAIL_CHECK(OBD_FAIL_MDS_OSC_PRECREATE) && ost_idx == 0)
			continue;

		spin_unlock(&lqr->lqr_alloc);
		rc = lod_check_and_reserve_ost(env, lo, lod_comp, sfs, ost_idx,
					       speed, &stripe_idx, stripe,
					       ost_indices, th, &overstriped);
		spin_lock(&lqr->lqr_alloc);

		if (rc != 0 && OST_TGT(m, ost_idx)->ltd_connecting)
			ost_connecting = 1;
	}
	if ((speed < 2) && (stripe_idx < stripe_count_min)) {
		/* Try again, allowing slower OSCs */
		speed++;
		lqr->lqr_start_idx = ost_start_idx_temp;

		ost_connecting = 0;
		goto repeat_find;
	}

	spin_unlock(&lqr->lqr_alloc);
	up_read(&m->lod_qos.lq_rw_sem);

	/* If there are enough OSTs, a component with overstriping requested
	 * will not actually end up overstriped.  The comp should reflect this.
	 */
	if (!overstriped)
		lod_comp->llc_pattern &= ~LOV_PATTERN_OVERSTRIPING;

	if (stripe_idx) {
		lod_comp->llc_stripe_count = stripe_idx;
		/* at least one stripe is allocated */
		rc = 0;
	} else {
		/* nobody provided us with a single object */
		if (ost_connecting)
			rc = -EINPROGRESS;
		else
			rc = -ENOSPC;
	}

out:
	if (pool != NULL) {
		up_read(&pool_tgt_rw_sem(pool));
		/* put back ref got by lod_find_pool() */
		lod_pool_putref(pool);
	}

	RETURN(rc);
}

/**
 * Allocate a specific striping layout on a user defined set of OSTs.
 *
 * Allocates new striping using the OST index range provided by the data from
 * the lmm_obejcts contained in the lov_user_md passed to this method. Full
 * OSTs are not considered. The exact order of OSTs requested by the user
 * is respected as much as possible depending on OST status. The number of
 * stripes needed and stripe offset are taken from the object. If that number
 * can not be met, then the function returns a failure and then it's the
 * caller's responsibility to release the stripes allocated. All the internal
 * structures are protected, but no concurrent allocation is allowed on the
 * same objects.
 *
 * \param[in] env		execution environment for this thread
 * \param[in] lo		LOD object
 * \param[out] stripe		striping created
 * \param[out] ost_indices	ost indices of striping created
 * \param[in] th		transaction handle
 * \param[in] comp_idx		index of ldo_comp_entries
 *
 * \retval 0		on success
 * \retval -ENODEV	OST index does not exist on file system
 * \retval -EINVAL	requested OST index is invalid
 * \retval negative	negated errno on error
 */
static int lod_alloc_ost_list(const struct lu_env *env, struct lod_object *lo,
			      struct dt_object **stripe, __u32 *ost_indices,
			      struct thandle *th, int comp_idx)
{
	struct lod_layout_component *lod_comp;
	struct lod_device	*m = lu2lod_dev(lo->ldo_obj.do_lu.lo_dev);
	struct obd_statfs	*sfs = &lod_env_info(env)->lti_osfs;
	struct dt_object	*o;
	unsigned int		array_idx = 0;
	int			stripe_count = 0;
	int			i;
	int			rc = -EINVAL;
	ENTRY;

	/* for specific OSTs layout */
	LASSERT(lo->ldo_comp_cnt > comp_idx && lo->ldo_comp_entries != NULL);
	lod_comp = &lo->ldo_comp_entries[comp_idx];
	LASSERT(lod_comp->llc_ostlist.op_array);
	LASSERT(lod_comp->llc_ostlist.op_count);

	rc = lod_qos_ost_in_use_clear(env, lod_comp->llc_stripe_count);
	if (rc < 0)
		RETURN(rc);

	if (lod_comp->llc_stripe_offset == LOV_OFFSET_DEFAULT)
		lod_comp->llc_stripe_offset =
				lod_comp->llc_ostlist.op_array[0];

	for (i = 0; i < lod_comp->llc_stripe_count; i++) {
		if (lod_comp->llc_ostlist.op_array[i] ==
		    lod_comp->llc_stripe_offset) {
			array_idx = i;
			break;
		}
	}
	if (i == lod_comp->llc_stripe_count) {
		CDEBUG(D_OTHER,
		       "%s: start index %d not in the specified list of OSTs\n",
		       lod2obd(m)->obd_name, lod_comp->llc_stripe_offset);
		RETURN(-EINVAL);
	}

	for (i = 0; i < lod_comp->llc_stripe_count;
	     i++, array_idx = (array_idx + 1) % lod_comp->llc_stripe_count) {
		__u32 ost_idx = lod_comp->llc_ostlist.op_array[array_idx];

		if (!cfs_bitmap_check(m->lod_ost_bitmap, ost_idx)) {
			rc = -ENODEV;
			break;
		}

		/* do not put >1 objects on a single OST, except for
		 * overstriping
		 */
		if (lod_qos_is_ost_used(env, ost_idx, stripe_count) &&
		    !(lod_comp->llc_pattern & LOV_PATTERN_OVERSTRIPING)) {
			rc = -EINVAL;
			break;
		}

		rc = lod_statfs_and_check(env, m, ost_idx, sfs);
		if (rc < 0) /* this OSP doesn't feel well */
			break;

		o = lod_qos_declare_object_on(env, m, ost_idx, th);
		if (IS_ERR(o)) {
			rc = PTR_ERR(o);
			CDEBUG(D_OTHER,
			       "%s: can't declare new object on #%u: %d\n",
			       lod2obd(m)->obd_name, ost_idx, rc);
			break;
		}

		/*
		 * We've successfully declared (reserved) an object
		 */
		lod_qos_ost_in_use(env, stripe_count, ost_idx);
		stripe[stripe_count] = o;
		ost_indices[stripe_count] = ost_idx;
		stripe_count++;
	}

	RETURN(rc);
}

/**
 * Allocate a striping on a predefined set of OSTs.
 *
 * Allocates new layout starting from OST index in lo->ldo_stripe_offset.
 * Full OSTs are not considered. The exact order of OSTs is not important and
 * varies depending on OST status. The allocation procedure prefers the targets
 * with precreated objects ready. The number of stripes needed and stripe
 * offset are taken from the object. If that number cannot be met, then the
 * function returns an error and then it's the caller's responsibility to
 * release the stripes allocated. All the internal structures are protected,
 * but no concurrent allocation is allowed on the same objects.
 *
 * \param[in] env		execution environment for this thread
 * \param[in] lo		LOD object
 * \param[out] stripe		striping created
 * \param[out] ost_indices	ost indices of striping created
 * \param[in] flags		not used
 * \param[in] th		transaction handle
 * \param[in] comp_idx		index of ldo_comp_entries
 *
 * \retval 0		on success
 * \retval -ENOSPC	if no OST objects are available at all
 * \retval -EFBIG	if not enough OST objects are found
 * \retval -EINVAL	requested offset is invalid
 * \retval negative	errno on failure
 */
static int lod_alloc_specific(const struct lu_env *env, struct lod_object *lo,
			      struct dt_object **stripe, __u32 *ost_indices,
			      int flags, struct thandle *th, int comp_idx)
{
	struct lod_layout_component *lod_comp;
	struct lod_device *m = lu2lod_dev(lo->ldo_obj.do_lu.lo_dev);
	struct obd_statfs *sfs = &lod_env_info(env)->lti_osfs;
	struct dt_object *o;
	__u32 ost_idx;
	unsigned int i, array_idx, ost_count;
	int rc, stripe_num = 0;
	int speed = 0;
	struct pool_desc  *pool = NULL;
	struct ost_pool   *osts;
	int stripes_per_ost = 1;
	bool overstriped = false;
	ENTRY;

	LASSERT(lo->ldo_comp_cnt > comp_idx && lo->ldo_comp_entries != NULL);
	lod_comp = &lo->ldo_comp_entries[comp_idx];

	rc = lod_qos_ost_in_use_clear(env, lod_comp->llc_stripe_count);
	if (rc)
		GOTO(out, rc);

	if (lod_comp->llc_pool != NULL)
		pool = lod_find_pool(m, lod_comp->llc_pool);

	if (pool != NULL) {
		down_read(&pool_tgt_rw_sem(pool));
		osts = &(pool->pool_obds);
	} else {
		osts = &(m->lod_pool_info);
	}

	ost_count = osts->op_count;

repeat_find:
	/* search loi_ost_idx in ost array */
	array_idx = 0;
	for (i = 0; i < ost_count; i++) {
		if (osts->op_array[i] == lod_comp->llc_stripe_offset) {
			array_idx = i;
			break;
		}
	}
	if (i == ost_count) {
		CERROR("Start index %d not found in pool '%s'\n",
		       lod_comp->llc_stripe_offset,
		       lod_comp->llc_pool ? lod_comp->llc_pool : "");
		GOTO(out, rc = -EINVAL);
	}

	if (lod_comp->llc_pattern & LOV_PATTERN_OVERSTRIPING)
		stripes_per_ost =
			(lod_comp->llc_stripe_count - 1)/ost_count + 1;

	for (i = 0; i < ost_count * stripes_per_ost;
			i++, array_idx = (array_idx + 1) % ost_count) {
		ost_idx = osts->op_array[array_idx];

		if (!cfs_bitmap_check(m->lod_ost_bitmap, ost_idx))
			continue;

		/* Fail Check before osc_precreate() is called
		   so we can only 'fail' single OSC. */
		if (OBD_FAIL_CHECK(OBD_FAIL_MDS_OSC_PRECREATE) && ost_idx == 0)
			continue;

		/*
		 * do not put >1 objects on a single OST, except for
		 * overstriping, where it is intended
		 */
		if (lod_qos_is_ost_used(env, ost_idx, stripe_num)) {
			if (lod_comp->llc_pattern & LOV_PATTERN_OVERSTRIPING)
				overstriped = true;
			else
				continue;
		}

		/*
		 * try not allocate on the OST used by other component
		 */
		if (speed == 0 && i != 0 &&
		    lod_comp_is_ost_used(env, lo, ost_idx))
			continue;

		/* Drop slow OSCs if we can, but not for requested start idx.
		 *
		 * This means "if OSC is slow and it is not the requested
		 * start OST, then it can be skipped, otherwise skip it only
		 * if it is inactive/recovering/out-of-space." */

		rc = lod_statfs_and_check(env, m, ost_idx, sfs);
		if (rc) {
			/* this OSP doesn't feel well */
			continue;
		}

		/*
		 * We expect number of precreated objects at the first
		 * iteration.  Skip OSPs with no objects ready.  Don't apply
		 * this logic to OST specified with stripe_offset.
		 */
		if (i != 0 && sfs->os_fprecreated == 0 && speed == 0)
			continue;

		o = lod_qos_declare_object_on(env, m, ost_idx, th);
		if (IS_ERR(o)) {
			CDEBUG(D_OTHER, "can't declare new object on #%u: %d\n",
			       ost_idx, (int) PTR_ERR(o));
			continue;
		}

		/*
		 * We've successfully declared (reserved) an object
		 */
		lod_qos_ost_in_use(env, stripe_num, ost_idx);
		stripe[stripe_num] = o;
		ost_indices[stripe_num] = ost_idx;
		stripe_num++;

		/* We have enough stripes */
		if (stripe_num == lod_comp->llc_stripe_count)
			GOTO(out, rc = 0);
	}
	if (speed < 2) {
		/* Try again, allowing slower OSCs */
		speed++;
		goto repeat_find;
	}

	/* If we were passed specific striping params, then a failure to
	 * meet those requirements is an error, since we can't reallocate
	 * that memory (it might be part of a larger array or something).
	 */
	CERROR("can't lstripe objid "DFID": have %d want %u\n",
	       PFID(lu_object_fid(lod2lu_obj(lo))), stripe_num,
	       lod_comp->llc_stripe_count);
	rc = stripe_num == 0 ? -ENOSPC : -EFBIG;

	/* If there are enough OSTs, a component with overstriping requessted
	 * will not actually end up overstriped.  The comp should reflect this.
	 */
	if (rc == 0 && !overstriped)
		lod_comp->llc_pattern &= ~LOV_PATTERN_OVERSTRIPING;

out:
	if (pool != NULL) {
		up_read(&pool_tgt_rw_sem(pool));
		/* put back ref got by lod_find_pool() */
		lod_pool_putref(pool);
	}

	RETURN(rc);
}

/**
 * Allocate a striping using an algorithm with weights.
 *
 * The function allocates OST objects to create a striping. The algorithm
 * used is based on weights (currently only using the free space), and it's
 * trying to ensure the space is used evenly by OSTs and OSSs. The striping
 * configuration (# of stripes, offset, pool) is taken from the object and
 * is prepared by the caller.
 *
 * If LOV_USES_DEFAULT_STRIPE is not passed and prepared configuration can't
 * be met due to too few OSTs, then allocation fails. If the flag is passed
 * fewer than 3/4 of the requested number of stripes can be allocated, then
 * allocation fails.
 *
 * No concurrent allocation is allowed on the object and this must be ensured
 * by the caller. All the internal structures are protected by the function.
 *
 * The algorithm has two steps: find available OSTs and calculate their
 * weights, then select the OSTs with their weights used as the probability.
 * An OST with a higher weight is proportionately more likely to be selected
 * than one with a lower weight.
 *
 * \param[in] env		execution environment for this thread
 * \param[in] lo		LOD object
 * \param[out] stripe		striping created
 * \param[out] ost_indices	ost indices of striping created
 * \param[in] flags		0 or LOV_USES_DEFAULT_STRIPE
 * \param[in] th		transaction handle
 * \param[in] comp_idx		index of ldo_comp_entries
 *
 * \retval 0		on success
 * \retval -EAGAIN	not enough OSTs are found for specified stripe count
 * \retval -EINVAL	requested OST index is invalid
 * \retval negative	errno on failure
 */
static int lod_alloc_qos(const struct lu_env *env, struct lod_object *lo,
			 struct dt_object **stripe, __u32 *ost_indices,
			 int flags, struct thandle *th, int comp_idx)
{
	struct lod_layout_component *lod_comp;
	struct lod_device *lod = lu2lod_dev(lo->ldo_obj.do_lu.lo_dev);
	struct obd_statfs *sfs = &lod_env_info(env)->lti_osfs;
	struct lod_avoid_guide *lag = &lod_env_info(env)->lti_avoid;
	struct lod_tgt_desc *ost;
	struct dt_object *o;
	__u64 total_weight = 0;
	struct pool_desc *pool = NULL;
	struct ost_pool *osts;
	unsigned int i;
	__u32 nfound, good_osts, stripe_count, stripe_count_min;
	bool overstriped = false;
	int stripes_per_ost = 1;
	int rc = 0;
	ENTRY;

	LASSERT(lo->ldo_comp_cnt > comp_idx && lo->ldo_comp_entries != NULL);
	lod_comp = &lo->ldo_comp_entries[comp_idx];
	stripe_count = lod_comp->llc_stripe_count;
	stripe_count_min = min_stripe_count(stripe_count, flags);
	if (stripe_count_min < 1)
		RETURN(-EINVAL);

	if (lod_comp->llc_pool != NULL)
		pool = lod_find_pool(lod, lod_comp->llc_pool);

	if (pool != NULL) {
		down_read(&pool_tgt_rw_sem(pool));
		osts = &(pool->pool_obds);
	} else {
		osts = &(lod->lod_pool_info);
	}

	/* Detect -EAGAIN early, before expensive lock is taken. */
	if (!lqos_is_usable(&lod->lod_qos, lod->lod_desc.ld_active_tgt_count))
		GOTO(out_nolock, rc = -EAGAIN);

	if (lod_comp->llc_pattern & LOV_PATTERN_OVERSTRIPING)
		stripes_per_ost =
			(lod_comp->llc_stripe_count - 1)/osts->op_count + 1;

	/* Do actual allocation, use write lock here. */
	down_write(&lod->lod_qos.lq_rw_sem);

	/*
	 * Check again, while we were sleeping on @lq_rw_sem things could
	 * change.
	 */
	if (!lqos_is_usable(&lod->lod_qos, lod->lod_desc.ld_active_tgt_count))
		GOTO(out, rc = -EAGAIN);

	rc = lqos_calc_penalties(&lod->lod_qos, &lod->lod_ost_descs,
				 lod->lod_desc.ld_active_tgt_count,
				 lod->lod_desc.ld_qos_maxage, false);
	if (rc)
		GOTO(out, rc);

	rc = lod_qos_ost_in_use_clear(env, lod_comp->llc_stripe_count);
	if (rc)
		GOTO(out, rc);

	good_osts = 0;
	/* Find all the OSTs that are valid stripe candidates */
	for (i = 0; i < osts->op_count; i++) {
		if (!cfs_bitmap_check(lod->lod_ost_bitmap, osts->op_array[i]))
			continue;

		ost = OST_TGT(lod, osts->op_array[i]);
		ost->ltd_qos.ltq_usable = 0;

		rc = lod_statfs_and_check(env, lod, osts->op_array[i], sfs);
		if (rc) {
			/* this OSP doesn't feel well */
			continue;
		}

		if (sfs->os_state & OS_STATE_DEGRADED)
			continue;

		/* Fail Check before osc_precreate() is called
		   so we can only 'fail' single OSC. */
		if (OBD_FAIL_CHECK(OBD_FAIL_MDS_OSC_PRECREATE) &&
				   osts->op_array[i] == 0)
			continue;

		ost->ltd_qos.ltq_usable = 1;
		lqos_calc_weight(ost);
		total_weight += ost->ltd_qos.ltq_weight;

		good_osts++;
	}

	QOS_DEBUG("found %d good osts\n", good_osts);

	if (good_osts < stripe_count_min)
		GOTO(out, rc = -EAGAIN);

	/* If we do not have enough OSTs for the requested stripe count, do not
	 * put more stripes per OST than requested.
	 */
	if (stripe_count / stripes_per_ost > good_osts)
		stripe_count = good_osts * stripes_per_ost;

	/* Find enough OSTs with weighted random allocation. */
	nfound = 0;
	while (nfound < stripe_count) {
		u64 rand, cur_weight;

		cur_weight = 0;
		rc = -ENOSPC;

		rand = lu_prandom_u64_max(total_weight);

		/* On average, this will hit larger-weighted OSTs more often.
		 * 0-weight OSTs will always get used last (only when rand=0) */
		for (i = 0; i < osts->op_count; i++) {
			__u32 idx = osts->op_array[i];

			if (lod_should_avoid_ost(lo, lag, idx))
				continue;

			ost = OST_TGT(lod, idx);

			if (!ost->ltd_qos.ltq_usable)
				continue;

			cur_weight += ost->ltd_qos.ltq_weight;
			QOS_DEBUG("stripe_count=%d nfound=%d cur_weight=%llu "
				  "rand=%llu total_weight=%llu\n",
				  stripe_count, nfound, cur_weight, rand,
				  total_weight);

			if (cur_weight < rand)
				continue;

			QOS_DEBUG("stripe=%d to idx=%d\n", nfound, idx);
			/*
			 * do not put >1 objects on a single OST, except for
			 * overstriping
			 */
			if ((lod_comp_is_ost_used(env, lo, idx)) &&
			    !(lod_comp->llc_pattern & LOV_PATTERN_OVERSTRIPING))
				continue;

			if (lod_qos_is_ost_used(env, idx, nfound)) {
				if (lod_comp->llc_pattern &
				    LOV_PATTERN_OVERSTRIPING)
					overstriped = true;
				else
					continue;
			}

			o = lod_qos_declare_object_on(env, lod, idx, th);
			if (IS_ERR(o)) {
				QOS_DEBUG("can't declare object on #%u: %d\n",
					  idx, (int) PTR_ERR(o));
				continue;
			}

			lod_avoid_update(lo, lag);
			lod_qos_ost_in_use(env, nfound, idx);
			stripe[nfound] = o;
			ost_indices[nfound] = idx;
			lqos_recalc_weight(&lod->lod_qos, &lod->lod_ost_descs,
					   ost,
					   lod->lod_desc.ld_active_tgt_count,
					   &total_weight);
			nfound++;
			rc = 0;
			break;
		}

		if (rc) {
			/* no OST found on this iteration, give up */
			break;
		}
	}

	if (unlikely(nfound != stripe_count)) {
		/*
		 * when the decision to use weighted algorithm was made
		 * we had enough appropriate OSPs, but this state can
		 * change anytime (no space on OST, broken connection, etc)
		 * so it's possible OSP won't be able to provide us with
		 * an object due to just changed state
		 */
		QOS_DEBUG("%s: wanted %d objects, found only %d\n",
			  lod2obd(lod)->obd_name, stripe_count, nfound);
		for (i = 0; i < nfound; i++) {
			LASSERT(stripe[i] != NULL);
			dt_object_put(env, stripe[i]);
			stripe[i] = NULL;
		}

		/* makes sense to rebalance next time */
		lod->lod_qos.lq_dirty = 1;
		lod->lod_qos.lq_same_space = 0;

		rc = -EAGAIN;
	}

	/* If there are enough OSTs, a component with overstriping requessted
	 * will not actually end up overstriped.  The comp should reflect this.
	 */
	if (rc == 0 && !overstriped)
		lod_comp->llc_pattern &= ~LOV_PATTERN_OVERSTRIPING;

out:
	up_write(&lod->lod_qos.lq_rw_sem);

out_nolock:
	if (pool != NULL) {
		up_read(&pool_tgt_rw_sem(pool));
		/* put back ref got by lod_find_pool() */
		lod_pool_putref(pool);
	}

	RETURN(rc);
}

/**
 * Check stripe count the caller can use.
 *
 * For new layouts (no initialized components), check the total size of the
 * layout against the maximum EA size from the backing file system.  This
 * stops us from creating a layout which will be too large once initialized.
 *
 * For existing layouts (with initialized components):
 * Find the maximal possible stripe count not greater than \a stripe_count.
 * If the provided stripe count is 0, then the filesystem's default is used.
 *
 * \param[in] lod	LOD device
 * \param[in] lo	The lod_object
 * \param[in] stripe_count	count the caller would like to use
 *
 * \retval		the maximum usable stripe count
 */
__u16 lod_get_stripe_count(struct lod_device *lod, struct lod_object *lo,
			   __u16 stripe_count, bool overstriping)
{
	__u32 max_stripes = LOV_MAX_STRIPE_COUNT_OLD;
	/* max stripe count is based on OSD ea size */
	unsigned int easize = lod->lod_osd_max_easize;
	int i;


	if (!stripe_count)
		stripe_count = lod->lod_desc.ld_default_stripe_count;
	if (!stripe_count)
		stripe_count = 1;
	/* Overstriping allows more stripes than targets */
	if (stripe_count > lod->lod_desc.ld_active_tgt_count && !overstriping)
		stripe_count = lod->lod_desc.ld_active_tgt_count;

	if (lo->ldo_is_composite) {
		struct lod_layout_component *lod_comp;
		unsigned int header_sz = sizeof(struct lov_comp_md_v1);
		unsigned int init_comp_sz = 0;
		unsigned int total_comp_sz = 0;
		unsigned int comp_sz;

		header_sz += sizeof(struct lov_comp_md_entry_v1) *
				lo->ldo_comp_cnt;

		for (i = 0; i < lo->ldo_comp_cnt; i++) {
			lod_comp = &lo->ldo_comp_entries[i];
			comp_sz = lov_mds_md_size(lod_comp->llc_stripe_count,
						  LOV_MAGIC_V3);
			total_comp_sz += comp_sz;
			if (lod_comp->llc_flags & LCME_FL_INIT)
				init_comp_sz += comp_sz;
		}

		if (init_comp_sz > 0)
			total_comp_sz = init_comp_sz;

		header_sz += total_comp_sz;

		if (easize > header_sz)
			easize -= header_sz;
		else
			easize = 0;
	}

	max_stripes = lov_mds_md_max_stripe_count(easize, LOV_MAGIC_V3);

	return (stripe_count < max_stripes) ? stripe_count : max_stripes;
}

/**
 * Create in-core respresentation for a fully-defined striping
 *
 * When the caller passes a fully-defined striping (i.e. everything including
 * OST object FIDs are defined), then we still need to instantiate LU-cache
 * with the objects representing the stripes defined. This function completes
 * that task.
 *
 * \param[in] env	execution environment for this thread
 * \param[in] mo	LOD object
 * \param[in] buf	buffer containing the striping
 *
 * \retval 0		on success
 * \retval negative	negated errno on error
 */
int lod_use_defined_striping(const struct lu_env *env,
			     struct lod_object *mo,
			     const struct lu_buf *buf)
{
	struct lod_layout_component *lod_comp;
	struct lov_mds_md_v1   *v1 = buf->lb_buf;
	struct lov_mds_md_v3   *v3 = buf->lb_buf;
	struct lov_comp_md_v1  *comp_v1 = NULL;
	struct lov_ost_data_v1 *objs;
	__u32	magic;
	__u16	comp_cnt;
	__u16	mirror_cnt;
	int	rc = 0, i;
	ENTRY;

	mutex_lock(&mo->ldo_layout_mutex);
	lod_striping_free_nolock(env, mo);

	magic = le32_to_cpu(v1->lmm_magic) & ~LOV_MAGIC_DEFINED;

	if (magic != LOV_MAGIC_V1 && magic != LOV_MAGIC_V3 &&
	    magic != LOV_MAGIC_COMP_V1 && magic != LOV_MAGIC_FOREIGN)
		GOTO(unlock, rc = -EINVAL);

	if (magic == LOV_MAGIC_COMP_V1) {
		comp_v1 = buf->lb_buf;
		comp_cnt = le16_to_cpu(comp_v1->lcm_entry_count);
		if (comp_cnt == 0)
			GOTO(unlock, rc = -EINVAL);
		mirror_cnt = le16_to_cpu(comp_v1->lcm_mirror_count) + 1;
		mo->ldo_flr_state = le16_to_cpu(comp_v1->lcm_flags) &
					LCM_FL_FLR_MASK;
		mo->ldo_is_composite = 1;
	} else if (magic == LOV_MAGIC_FOREIGN) {
		struct lov_foreign_md *foreign;
		size_t length;

		if (buf->lb_len < offsetof(typeof(*foreign), lfm_value)) {
			CDEBUG(D_LAYOUT,
			       "buf len %zu < min lov_foreign_md size (%zu)\n",
			       buf->lb_len,
			       offsetof(typeof(*foreign), lfm_value));
			GOTO(out, rc = -EINVAL);
		}
		foreign = (struct lov_foreign_md *)buf->lb_buf;
		length = foreign_size_le(foreign);
		if (buf->lb_len < length) {
			CDEBUG(D_LAYOUT,
			       "buf len %zu < this lov_foreign_md size (%zu)\n",
			       buf->lb_len, length);
			GOTO(out, rc = -EINVAL);
		}

		/* just cache foreign LOV EA raw */
		rc = lod_alloc_foreign_lov(mo, length);
		if (rc)
			GOTO(out, rc);
		memcpy(mo->ldo_foreign_lov, buf->lb_buf, length);
		GOTO(out, rc);
	} else {
		mo->ldo_is_composite = 0;
		comp_cnt = 1;
		mirror_cnt = 0;
	}
	mo->ldo_layout_gen = le16_to_cpu(v1->lmm_layout_gen);

	rc = lod_alloc_comp_entries(mo, mirror_cnt, comp_cnt);
	if (rc)
		GOTO(unlock, rc);

	for (i = 0; i < comp_cnt; i++) {
		struct lu_extent *ext;
		char	*pool_name;
		__u32	offs;

		lod_comp = &mo->ldo_comp_entries[i];

		if (mo->ldo_is_composite) {
			offs = le32_to_cpu(comp_v1->lcm_entries[i].lcme_offset);
			v1 = (struct lov_mds_md_v1 *)((char *)comp_v1 + offs);
			v3 = (struct lov_mds_md_v3 *)v1;
			magic = le32_to_cpu(v1->lmm_magic);

			ext = &comp_v1->lcm_entries[i].lcme_extent;
			lod_comp->llc_extent.e_start =
				le64_to_cpu(ext->e_start);
			lod_comp->llc_extent.e_end = le64_to_cpu(ext->e_end);
			lod_comp->llc_flags =
				le32_to_cpu(comp_v1->lcm_entries[i].lcme_flags);
			if (lod_comp->llc_flags & LCME_FL_NOSYNC)
				lod_comp->llc_timestamp = le64_to_cpu(
					comp_v1->lcm_entries[i].lcme_timestamp);
			lod_comp->llc_id =
				le32_to_cpu(comp_v1->lcm_entries[i].lcme_id);
			if (lod_comp->llc_id == LCME_ID_INVAL)
				GOTO(out, rc = -EINVAL);
		}

		pool_name = NULL;
		if (magic == LOV_MAGIC_V1) {
			objs = &v1->lmm_objects[0];
		} else if (magic == LOV_MAGIC_V3) {
			objs = &v3->lmm_objects[0];
			if (v3->lmm_pool_name[0] != '\0')
				pool_name = v3->lmm_pool_name;
		} else {
			CDEBUG(D_LAYOUT, "Invalid magic %x\n", magic);
			GOTO(out, rc = -EINVAL);
		}

		lod_comp->llc_pattern = le32_to_cpu(v1->lmm_pattern);
		lod_comp->llc_stripe_size = le32_to_cpu(v1->lmm_stripe_size);
		lod_comp->llc_stripe_count = le16_to_cpu(v1->lmm_stripe_count);
		lod_comp->llc_layout_gen = le16_to_cpu(v1->lmm_layout_gen);
		/**
		 * The stripe_offset of an uninit-ed component is stored in
		 * the lmm_layout_gen
		 */
		if (mo->ldo_is_composite && !lod_comp_inited(lod_comp))
			lod_comp->llc_stripe_offset = lod_comp->llc_layout_gen;
		lod_obj_set_pool(mo, i, pool_name);

		if ((!mo->ldo_is_composite || lod_comp_inited(lod_comp)) &&
		    !(lod_comp->llc_pattern & LOV_PATTERN_F_RELEASED) &&
		    !(lod_comp->llc_pattern & LOV_PATTERN_MDT)) {
			rc = lod_initialize_objects(env, mo, objs, i);
			if (rc)
				GOTO(out, rc);
		}
	}

	rc = lod_fill_mirrors(mo);
	GOTO(out, rc);
out:
	if (rc)
		lod_striping_free_nolock(env, mo);
unlock:
	mutex_unlock(&mo->ldo_layout_mutex);

	RETURN(rc);
}

/**
 * Parse suggested striping configuration.
 *
 * The caller gets a suggested striping configuration from a number of sources
 * including per-directory default and applications. Then it needs to verify
 * the suggested striping is valid, apply missing bits and store the resulting
 * configuration in the object to be used by the allocator later. Must not be
 * called concurrently against the same object. It's OK to provide a
 * fully-defined striping.
 *
 * \param[in] env	execution environment for this thread
 * \param[in] lo	LOD object
 * \param[in] buf	buffer containing the striping
 *
 * \retval 0		on success
 * \retval negative	negated errno on error
 */
int lod_qos_parse_config(const struct lu_env *env, struct lod_object *lo,
			 const struct lu_buf *buf)
{
	struct lod_layout_component *lod_comp;
	struct lod_device *d = lu2lod_dev(lod2lu_obj(lo)->lo_dev);
	struct lov_desc *desc = &d->lod_desc;
	struct lov_user_md_v1 *v1 = NULL;
	struct lov_user_md_v3 *v3 = NULL;
	struct lov_comp_md_v1 *comp_v1 = NULL;
	struct lov_foreign_md *lfm = NULL;
	char def_pool[LOV_MAXPOOLNAME + 1];
	__u32 magic;
	__u16 comp_cnt;
	__u16 mirror_cnt;
	int i, rc;
	ENTRY;

	if (buf == NULL || buf->lb_buf == NULL || buf->lb_len == 0)
		RETURN(0);

	memset(def_pool, 0, sizeof(def_pool));
	if (lo->ldo_comp_entries != NULL)
		lod_layout_get_pool(lo->ldo_comp_entries, lo->ldo_comp_cnt,
				    def_pool, sizeof(def_pool));

	/* free default striping info */
	if (lo->ldo_is_foreign)
		lod_free_foreign_lov(lo);
	else
		lod_free_comp_entries(lo);

	rc = lod_verify_striping(d, lo, buf, false);
	if (rc)
		RETURN(-EINVAL);

	v3 = buf->lb_buf;
	v1 = buf->lb_buf;
	comp_v1 = buf->lb_buf;
	/* {lmm,lfm}_magic position/length work for all LOV formats */
	magic = v1->lmm_magic;

	if (unlikely(le32_to_cpu(magic) & LOV_MAGIC_DEFINED)) {
		/* try to use as fully defined striping */
		rc = lod_use_defined_striping(env, lo, buf);
		RETURN(rc);
	}

	switch (magic) {
	case __swab32(LOV_USER_MAGIC_V1):
		lustre_swab_lov_user_md_v1(v1);
		magic = v1->lmm_magic;
		/* fall through */
	case LOV_USER_MAGIC_V1:
		break;
	case __swab32(LOV_USER_MAGIC_V3):
		lustre_swab_lov_user_md_v3(v3);
		magic = v3->lmm_magic;
		/* fall through */
	case LOV_USER_MAGIC_V3:
		break;
	case __swab32(LOV_USER_MAGIC_SPECIFIC):
		lustre_swab_lov_user_md_v3(v3);
		lustre_swab_lov_user_md_objects(v3->lmm_objects,
						v3->lmm_stripe_count);
		magic = v3->lmm_magic;
		/* fall through */
	case LOV_USER_MAGIC_SPECIFIC:
		break;
	case __swab32(LOV_USER_MAGIC_COMP_V1):
		lustre_swab_lov_comp_md_v1(comp_v1);
		magic = comp_v1->lcm_magic;
		/* fall trhough */
	case LOV_USER_MAGIC_COMP_V1:
		break;
	case __swab32(LOV_USER_MAGIC_FOREIGN):
		lfm = buf->lb_buf;
		__swab32s(&lfm->lfm_magic);
		__swab32s(&lfm->lfm_length);
		__swab32s(&lfm->lfm_type);
		__swab32s(&lfm->lfm_flags);
		magic = lfm->lfm_magic;
		/* fall through */
	case LOV_USER_MAGIC_FOREIGN:
		if (!lfm)
			lfm = buf->lb_buf;
		rc = lod_alloc_foreign_lov(lo, foreign_size(lfm));
		if (rc)
			RETURN(rc);
		memcpy(lo->ldo_foreign_lov, buf->lb_buf, foreign_size(lfm));
		RETURN(0);
	default:
		CERROR("%s: unrecognized magic %X\n",
		       lod2obd(d)->obd_name, magic);
		RETURN(-EINVAL);
	}

	lustre_print_user_md(D_OTHER, v1, "parse config");

	if (magic == LOV_USER_MAGIC_COMP_V1) {
		comp_cnt = comp_v1->lcm_entry_count;
		if (comp_cnt == 0)
			RETURN(-EINVAL);
		mirror_cnt =  comp_v1->lcm_mirror_count + 1;
		if (mirror_cnt > 1)
			lo->ldo_flr_state = LCM_FL_RDONLY;
		lo->ldo_is_composite = 1;
	} else {
		comp_cnt = 1;
		mirror_cnt = 0;
		lo->ldo_is_composite = 0;
	}

	rc = lod_alloc_comp_entries(lo, mirror_cnt, comp_cnt);
	if (rc)
		RETURN(rc);

	LASSERT(lo->ldo_comp_entries);

	for (i = 0; i < comp_cnt; i++) {
		struct pool_desc	*pool;
		struct lu_extent	*ext;
		char	*pool_name;

		lod_comp = &lo->ldo_comp_entries[i];

		if (lo->ldo_is_composite) {
			v1 = (struct lov_user_md *)((char *)comp_v1 +
					comp_v1->lcm_entries[i].lcme_offset);
			ext = &comp_v1->lcm_entries[i].lcme_extent;
			lod_comp->llc_extent = *ext;
			lod_comp->llc_flags =
				comp_v1->lcm_entries[i].lcme_flags &
					LCME_CL_COMP_FLAGS;
		}

		pool_name = NULL;
		if (v1->lmm_magic == LOV_USER_MAGIC_V3 ||
		    v1->lmm_magic == LOV_USER_MAGIC_SPECIFIC) {
			v3 = (struct lov_user_md_v3 *)v1;
			if (v3->lmm_pool_name[0] != '\0')
				pool_name = v3->lmm_pool_name;

			if (v3->lmm_magic == LOV_USER_MAGIC_SPECIFIC) {
				rc = lod_comp_copy_ost_lists(lod_comp, v3);
				if (rc)
					GOTO(free_comp, rc);
			}
		}

		if (pool_name == NULL && def_pool[0] != '\0')
			pool_name = def_pool;

		if (v1->lmm_pattern == 0)
			v1->lmm_pattern = LOV_PATTERN_RAID0;
		if (lov_pattern(v1->lmm_pattern) != LOV_PATTERN_RAID0 &&
		    lov_pattern(v1->lmm_pattern) != LOV_PATTERN_MDT &&
		    lov_pattern(v1->lmm_pattern) !=
			(LOV_PATTERN_RAID0 | LOV_PATTERN_OVERSTRIPING)) {
			CDEBUG(D_LAYOUT, "%s: invalid pattern: %x\n",
			       lod2obd(d)->obd_name, v1->lmm_pattern);
			GOTO(free_comp, rc = -EINVAL);
		}

		lod_comp->llc_pattern = v1->lmm_pattern;
		lod_comp->llc_stripe_size = desc->ld_default_stripe_size;
		if (v1->lmm_stripe_size)
			lod_comp->llc_stripe_size = v1->lmm_stripe_size;

		lod_comp->llc_stripe_count = desc->ld_default_stripe_count;
		if (v1->lmm_stripe_count ||
		    lov_pattern(v1->lmm_pattern) == LOV_PATTERN_MDT)
			lod_comp->llc_stripe_count = v1->lmm_stripe_count;

		lod_comp->llc_stripe_offset = v1->lmm_stripe_offset;
		lod_obj_set_pool(lo, i, pool_name);

		LASSERT(ergo(lov_pattern(lod_comp->llc_pattern) ==
			     LOV_PATTERN_MDT, lod_comp->llc_stripe_count == 0));

		if (pool_name == NULL)
			continue;

		/* In the function below, .hs_keycmp resolves to
		 * pool_hashkey_keycmp() */
		/* coverity[overrun-buffer-val] */
		pool = lod_find_pool(d, pool_name);
		if (pool == NULL)
			continue;

		if (lod_comp->llc_stripe_offset != LOV_OFFSET_DEFAULT) {
			rc = lod_check_index_in_pool(
					lod_comp->llc_stripe_offset, pool);
			if (rc < 0) {
				lod_pool_putref(pool);
				CDEBUG(D_LAYOUT, "%s: invalid offset, %u\n",
				       lod2obd(d)->obd_name,
				       lod_comp->llc_stripe_offset);
				GOTO(free_comp, rc = -EINVAL);
			}
		}

		if (lod_comp->llc_stripe_count > pool_tgt_count(pool) &&
		    !(lod_comp->llc_pattern & LOV_PATTERN_OVERSTRIPING))
			lod_comp->llc_stripe_count = pool_tgt_count(pool);

		lod_pool_putref(pool);
	}

	RETURN(0);

free_comp:
	lod_free_comp_entries(lo);
	RETURN(rc);
}

/**
 * prepare enough OST avoidance bitmap space
 */
int lod_prepare_avoidance(const struct lu_env *env, struct lod_object *lo)
{
	struct lod_device *lod = lu2lod_dev(lo->ldo_obj.do_lu.lo_dev);
	struct lod_tgt_descs *ltds = &lod->lod_ost_descs;
	struct lod_avoid_guide *lag = &lod_env_info(env)->lti_avoid;
	struct cfs_bitmap *bitmap = NULL;
	__u32 *new_oss = NULL;

	lag->lag_ost_avail = ltds->ltd_tgtnr;

	/* reset OSS avoid guide array */
	lag->lag_oaa_count = 0;
	if (lag->lag_oss_avoid_array && lag->lag_oaa_size < ltds->ltd_tgtnr) {
		OBD_FREE(lag->lag_oss_avoid_array,
			 sizeof(__u32) * lag->lag_oaa_size);
		lag->lag_oss_avoid_array = NULL;
		lag->lag_oaa_size = 0;
	}

	/* init OST avoid guide bitmap */
	if (lag->lag_ost_avoid_bitmap) {
		if (ltds->ltd_tgtnr <= lag->lag_ost_avoid_bitmap->size) {
			CFS_RESET_BITMAP(lag->lag_ost_avoid_bitmap);
		} else {
			CFS_FREE_BITMAP(lag->lag_ost_avoid_bitmap);
			lag->lag_ost_avoid_bitmap = NULL;
		}
	}

	if (!lag->lag_ost_avoid_bitmap) {
		bitmap = CFS_ALLOCATE_BITMAP(ltds->ltd_tgtnr);
		if (!bitmap)
			return -ENOMEM;
	}

	if (!lag->lag_oss_avoid_array) {
		/**
		 * usually there are multiple OSTs in one OSS, but we don't
		 * know the exact OSS number, so we choose a safe option,
		 * using OST count to allocate the array to store the OSS
		 * id.
		 */
		OBD_ALLOC(new_oss, sizeof(*new_oss) * ltds->ltd_tgtnr);
		if (!new_oss) {
			CFS_FREE_BITMAP(bitmap);
			return -ENOMEM;
		}
	}

	if (new_oss) {
		lag->lag_oss_avoid_array = new_oss;
		lag->lag_oaa_size = ltds->ltd_tgtnr;
	}
	if (bitmap)
		lag->lag_ost_avoid_bitmap = bitmap;

	return 0;
}

/**
 * Collect information of used OSTs and OSSs in the overlapped components
 * of other mirrors
 */
void lod_collect_avoidance(struct lod_object *lo, struct lod_avoid_guide *lag,
			   int comp_idx)
{
	struct lod_device *lod = lu2lod_dev(lo->ldo_obj.do_lu.lo_dev);
	struct lod_layout_component *lod_comp = &lo->ldo_comp_entries[comp_idx];
	struct cfs_bitmap *bitmap = lag->lag_ost_avoid_bitmap;
	int i, j;

	/* iterate mirrors */
	for (i = 0; i < lo->ldo_mirror_count; i++) {
		struct lod_layout_component *comp;

		/**
		 * skip mirror containing component[comp_idx], we only
		 * collect OSTs info of conflicting component in other mirrors,
		 * so that during read, if OSTs of a mirror's component are
		 * not available, we still have other mirror with different
		 * OSTs to read the data.
		 */
		comp = &lo->ldo_comp_entries[lo->ldo_mirrors[i].lme_start];
		if (comp->llc_id != LCME_ID_INVAL &&
		    mirror_id_of(comp->llc_id) ==
						mirror_id_of(lod_comp->llc_id))
			continue;

		/* iterate components of a mirror */
		lod_foreach_mirror_comp(comp, lo, i) {
			/**
			 * skip non-overlapped or un-instantiated components,
			 * NOTE: don't use lod_comp_inited(comp) to judge
			 * whether @comp has been inited, since during
			 * declare phase, comp->llc_stripe has been allocated
			 * while it's init flag not been set until the exec
			 * phase.
			 */
			if (!lu_extent_is_overlapped(&comp->llc_extent,
						     &lod_comp->llc_extent) ||
			    !comp->llc_stripe)
				continue;

			/**
			 * collect used OSTs index and OSS info from a
			 * component
			 */
			for (j = 0; j < comp->llc_stripe_count; j++) {
				struct lod_tgt_desc *ost;
				struct lu_svr_qos *lsq;
				int k;

				ost = OST_TGT(lod, comp->llc_ost_indices[j]);
				lsq = ost->ltd_qos.ltq_svr;

				if (cfs_bitmap_check(bitmap, ost->ltd_index))
					continue;

				QOS_DEBUG("OST%d used in conflicting mirror "
					  "component\n", ost->ltd_index);
				cfs_bitmap_set(bitmap, ost->ltd_index);
				lag->lag_ost_avail--;

				for (k = 0; k < lag->lag_oaa_count; k++) {
					if (lag->lag_oss_avoid_array[k] ==
					    lsq->lsq_id)
						break;
				}
				if (k == lag->lag_oaa_count) {
					lag->lag_oss_avoid_array[k] =
								lsq->lsq_id;
					lag->lag_oaa_count++;
				}
			}
		}
	}
}

/**
 * Create a striping for an obejct.
 *
 * The function creates a new striping for the object. The function tries QoS
 * algorithm first unless free space is distributed evenly among OSTs, but
 * by default RR algorithm is preferred due to internal concurrency (QoS is
 * serialized). The caller must ensure no concurrent calls to the function
 * are made against the same object.
 *
 * \param[in] env	execution environment for this thread
 * \param[in] lo	LOD object
 * \param[in] attr	attributes OST objects will be declared with
 * \param[in] th	transaction handle
 * \param[in] comp_idx	index of ldo_comp_entries
 *
 * \retval 0		on success
 * \retval negative	negated errno on error
 */
int lod_qos_prep_create(const struct lu_env *env, struct lod_object *lo,
			struct lu_attr *attr, struct thandle *th,
			int comp_idx)
{
	struct lod_layout_component *lod_comp;
	struct lod_device      *d = lu2lod_dev(lod2lu_obj(lo)->lo_dev);
	int			stripe_len;
	int			flag = LOV_USES_ASSIGNED_STRIPE;
	int			i, rc = 0;
	struct lod_avoid_guide *lag = &lod_env_info(env)->lti_avoid;
	struct dt_object **stripe = NULL;
	__u32 *ost_indices = NULL;
	ENTRY;

	LASSERT(lo);
	LASSERT(lo->ldo_comp_cnt > comp_idx && lo->ldo_comp_entries != NULL);
	lod_comp = &lo->ldo_comp_entries[comp_idx];
	LASSERT(!(lod_comp->llc_flags & LCME_FL_EXTENSION));

	/* A released component is being created */
	if (lod_comp->llc_pattern & LOV_PATTERN_F_RELEASED)
		RETURN(0);

	/* A Data-on-MDT component is being created */
	if (lov_pattern(lod_comp->llc_pattern) == LOV_PATTERN_MDT)
		RETURN(0);

	if (likely(lod_comp->llc_stripe == NULL)) {
		/*
		 * no striping has been created so far
		 */
		LASSERT(lod_comp->llc_stripe_count);
		/*
		 * statfs and check OST targets now, since ld_active_tgt_count
		 * could be changed if some OSTs are [de]activated manually.
		 */
		lod_qos_statfs_update(env, d);
		stripe_len = lod_get_stripe_count(d, lo,
						  lod_comp->llc_stripe_count,
						  lod_comp->llc_pattern &
						  LOV_PATTERN_OVERSTRIPING);

		if (stripe_len == 0)
			GOTO(out, rc = -ERANGE);
		lod_comp->llc_stripe_count = stripe_len;
		OBD_ALLOC(stripe, sizeof(stripe[0]) * stripe_len);
		if (stripe == NULL)
			GOTO(out, rc = -ENOMEM);
		OBD_ALLOC(ost_indices, sizeof(*ost_indices) * stripe_len);
		if (!ost_indices)
			GOTO(out, rc = -ENOMEM);

		lod_getref(&d->lod_ost_descs);
		/* XXX: support for non-0 files w/o objects */
		CDEBUG(D_OTHER, "tgt_count %d stripe_count %d\n",
				d->lod_desc.ld_tgt_count, stripe_len);

		if (lod_comp->llc_ostlist.op_array &&
		    lod_comp->llc_ostlist.op_count) {
			rc = lod_alloc_ost_list(env, lo, stripe, ost_indices,
						th, comp_idx);
		} else if (lod_comp->llc_stripe_offset == LOV_OFFSET_DEFAULT) {
			/**
			 * collect OSTs and OSSs used in other mirrors whose
			 * components cross the ldo_comp_entries[comp_idx]
			 */
			rc = lod_prepare_avoidance(env, lo);
			if (rc)
				GOTO(put_ldts, rc);

			QOS_DEBUG("collecting conflict osts for comp[%d]\n",
				  comp_idx);
			lod_collect_avoidance(lo, lag, comp_idx);

			rc = lod_alloc_qos(env, lo, stripe, ost_indices, flag,
					   th, comp_idx);
			if (rc == -EAGAIN)
				rc = lod_alloc_rr(env, lo, stripe, ost_indices,
						  flag, th, comp_idx);
		} else {
			rc = lod_alloc_specific(env, lo, stripe, ost_indices,
						flag, th, comp_idx);
		}
put_ldts:
		lod_putref(d, &d->lod_ost_descs);
		if (rc < 0) {
			for (i = 0; i < stripe_len; i++)
				if (stripe[i] != NULL)
					dt_object_put(env, stripe[i]);
			lod_comp->llc_stripe_count = 0;
		} else {
			lod_comp->llc_stripe = stripe;
			lod_comp->llc_ost_indices = ost_indices;
			lod_comp->llc_stripes_allocated = stripe_len;
		}
	} else {
		/*
		 * lod_qos_parse_config() found supplied buf as a predefined
		 * striping (not a hint), so it allocated all the object
		 * now we need to create them
		 */
		for (i = 0; i < lod_comp->llc_stripe_count; i++) {
			struct dt_object  *o;

			o = lod_comp->llc_stripe[i];
			LASSERT(o);

			rc = lod_sub_declare_create(env, o, attr, NULL,
						    NULL, th);
			if (rc < 0) {
				CERROR("can't declare create: %d\n", rc);
				break;
			}
		}
		/**
		 * Clear LCME_FL_INIT for the component so that
		 * lod_striping_create() can create the striping objects
		 * in replay.
		 */
		lod_comp_unset_init(lod_comp);
	}

out:
	if (rc < 0) {
		if (stripe)
			OBD_FREE(stripe, sizeof(stripe[0]) * stripe_len);
		if (ost_indices)
			OBD_FREE(ost_indices,
				 sizeof(*ost_indices) * stripe_len);
	}
	RETURN(rc);
}

int lod_prepare_create(const struct lu_env *env, struct lod_object *lo,
		       struct lu_attr *attr, const struct lu_buf *buf,
		       struct thandle *th)

{
	struct lod_device *d = lu2lod_dev(lod2lu_obj(lo)->lo_dev);
	uint64_t size = 0;
	int i;
	int rc;
	ENTRY;

	LASSERT(lo);

	/* no OST available */
	/* XXX: should we be waiting a bit to prevent failures during
	 * cluster initialization? */
	if (d->lod_ostnr == 0)
		RETURN(-EIO);

	/*
	 * by this time, the object's ldo_stripe_count and ldo_stripe_size
	 * contain default value for striping: taken from the parent
	 * or from filesystem defaults
	 *
	 * in case the caller is passing lovea with new striping config,
	 * we may need to parse lovea and apply new configuration
	 */
	rc = lod_qos_parse_config(env, lo, buf);
	if (rc)
		RETURN(rc);

	if (attr->la_valid & LA_SIZE)
		size = attr->la_size;

	/**
	 * prepare OST object creation for the component covering file's
	 * size, the 1st component (including plain layout file) is always
	 * instantiated.
	 */
	for (i = 0; i < lo->ldo_comp_cnt; i++) {
		struct lod_layout_component *lod_comp;
		struct lu_extent *extent;

		lod_comp = &lo->ldo_comp_entries[i];
		extent = &lod_comp->llc_extent;
		QOS_DEBUG("comp[%d] %lld "DEXT"\n", i, size, PEXT(extent));
		if (!lo->ldo_is_composite || size >= extent->e_start) {
			rc = lod_qos_prep_create(env, lo, attr, th, i);
			if (rc)
				break;
		}
	}

	RETURN(rc);
}
