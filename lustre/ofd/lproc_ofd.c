/*
 * GPL HEADER START
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 only,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License version 2 for more details (a copy is included
 * in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with this program; If not, see
 * http://www.gnu.org/licenses/gpl-2.0.html
 *
 * GPL HEADER END
 */
/*
 * Copyright (c) 2009, 2010, Oracle and/or its affiliates. All rights reserved.
 * Use is subject to license terms.
 *
 * Copyright (c) 2012, 2017, Intel Corporation.
 */
/*
 * This file is part of Lustre, http://www.lustre.org/
 * Lustre is a trademark of Sun Microsystems, Inc.
 *
 * lustre/ofd/lproc_ofd.c
 *
 * This file provides functions of procfs interface for OBD Filter Device (OFD).
 *
 * Author: Andreas Dilger <andreas.dilger@intel.com>
 * Author: Mikhail Pershin <mike.pershin@intel.com>
 * Author: Johann Lombardi <johann.lombardi@intel.com>
 * Author: Fan Yong <fan.yong@intel.com>
 */

#define DEBUG_SUBSYSTEM S_CLASS

#include <obd.h>
#include <lprocfs_status.h>
#include <linux/seq_file.h>
#include <lustre_lfsck.h>

#include "ofd_internal.h"

#ifdef CONFIG_PROC_FS

/**
 * Show number of FID allocation sequences.
 *
 * \param[in] m		seq_file handle
 * \param[in] data	unused for single entry
 *
 * \retval		0 on success
 * \retval		negative value on error
 */
static ssize_t seqs_allocated_show(struct kobject *kobj, struct attribute *attr,
				   char *buf)
{
	struct obd_device *obd = container_of(kobj, struct obd_device,
					      obd_kset.kobj);
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);

	return sprintf(buf, "%u\n", ofd->ofd_seq_count);
}
LUSTRE_RO_ATTR(seqs_allocated);

/**
 * Show total number of grants for precreate.
 *
 * \param[in] m		seq_file handle
 * \param[in] data	unused for single entry
 *
 * \retval		0 on success
 * \retval		negative value on error
 */
static ssize_t grant_precreate_show(struct kobject *kobj,
				    struct attribute *attr,
				    char *buf)
{
	struct obd_device *obd = container_of(kobj, struct obd_device,
					      obd_kset.kobj);

	return sprintf(buf, "%ld\n",
		       obd->obd_self_export->exp_target_data.ted_grant);
}
LUSTRE_RO_ATTR(grant_precreate);

/**
 * Show number of precreates allowed in a single transaction.
 *
 * \param[in] m		seq_file handle
 * \param[in] data	unused for single entry
 *
 * \retval		0 on success
 * \retval		negative value on error
 */
static ssize_t precreate_batch_show(struct kobject *kobj,
				    struct attribute *attr,
				    char *buf)
{
	struct obd_device *obd = container_of(kobj, struct obd_device,
					      obd_kset.kobj);
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);

	return sprintf(buf, "%d\n", ofd->ofd_precreate_batch);
}

/**
 * Change number of precreates allowed in a single transaction.
 *
 * \param[in] file	proc file
 * \param[in] buffer	string which represents maximum number
 * \param[in] count	\a buffer length
 * \param[in] off	unused for single entry
 *
 * \retval		\a count on success
 * \retval		negative number on error
 */
static ssize_t precreate_batch_store(struct kobject *kobj,
				     struct attribute *attr,
				     const char *buffer, size_t count)
{
	struct obd_device *obd = container_of(kobj, struct obd_device,
					      obd_kset.kobj);
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);
	unsigned int val;
	int rc;

	rc = kstrtouint(buffer, 0, &val);
	if (rc)
		return rc;

	if (val < 1 || val > 65536)
		return -EINVAL;

	spin_lock(&ofd->ofd_batch_lock);
	ofd->ofd_precreate_batch = val;
	spin_unlock(&ofd->ofd_batch_lock);
	return count;
}
LUSTRE_RW_ATTR(precreate_batch);

/**
 * Show the last used ID for each FID sequence used by OFD.
 *
 * \param[in] m		seq_file handle
 * \param[in] data	unused for single entry
 *
 * \retval		0 on success
 * \retval		negative value on error
 */
static int ofd_last_id_seq_show(struct seq_file *m, void *data)
{
	struct obd_device	*obd = m->private;
	struct ofd_device	*ofd;
	struct ofd_seq		*oseq = NULL;

	if (obd == NULL)
		return 0;

	ofd = ofd_dev(obd->obd_lu_dev);

	read_lock(&ofd->ofd_seq_list_lock);
	list_for_each_entry(oseq, &ofd->ofd_seq_list, os_list) {
		__u64 seq;

		seq = ostid_seq(&oseq->os_oi) == 0 ?
		      fid_idif_seq(ostid_id(&oseq->os_oi),
				   ofd->ofd_lut.lut_lsd.lsd_osd_index) :
		      ostid_seq(&oseq->os_oi);
		seq_printf(m, DOSTID"\n", seq, ostid_id(&oseq->os_oi));
	}
	read_unlock(&ofd->ofd_seq_list_lock);
	return 0;
}

LPROC_SEQ_FOPS_RO(ofd_last_id);

/**
 * Show if the OFD is in degraded mode.
 *
 * Degraded means OFD has a failed drive or is undergoing RAID rebuild.
 * The MDS will try to avoid using this OST for new object allocations
 * to reduce the impact to global IO performance when clients writing to
 * this OST are slowed down.  It also reduces the contention on the OST
 * RAID device, allowing it to rebuild more quickly.
 *
 * \param[in] m		seq_file handle
 * \param[in] data	unused for single entry
 *
 * \retval		0 on success
 * \retval		negative value on error
 */
static ssize_t degraded_show(struct kobject *kobj, struct attribute *attr,
			     char *buf)
{
	struct obd_device *obd = container_of(kobj, struct obd_device,
					      obd_kset.kobj);
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);

	return sprintf(buf, "%u\n", ofd->ofd_raid_degraded);
}

/**
 * Set OFD to degraded mode.
 *
 * This is used to interface to userspace administrative tools for
 * the underlying RAID storage, so that they can mark an OST
 * as having degraded performance.
 *
 * \param[in] file	proc file
 * \param[in] buffer	string which represents mode
 *			1: set degraded mode
 *			0: unset degraded mode
 * \param[in] count	\a buffer length
 * \param[in] off	unused for single entry
 *
 * \retval		\a count on success
 * \retval		negative number on error
 */
static ssize_t degraded_store(struct kobject *kobj, struct attribute *attr,
			      const char *buffer, size_t count)
{
	struct obd_device *obd = container_of(kobj, struct obd_device,
					      obd_kset.kobj);
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);
	bool val;
	int rc;

	rc = kstrtobool(buffer, &val);
	if (rc)
		return rc;

	spin_lock(&ofd->ofd_flags_lock);
	ofd->ofd_raid_degraded = val;
	spin_unlock(&ofd->ofd_flags_lock);
	return count;
}
LUSTRE_RW_ATTR(degraded);

/**
 * Show OFD filesystem type.
 *
 * \param[in] m		seq_file handle
 * \param[in] data	unused for single entry
 *
 * \retval		0 on success
 * \retval		negative value on error
 */
static ssize_t fstype_show(struct kobject *kobj, struct attribute *attr,
			   char *buf)
{
	struct obd_device *obd = container_of(kobj, struct obd_device,
					      obd_kset.kobj);
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);
	struct lu_device  *d;

	LASSERT(ofd->ofd_osd);
	d = &ofd->ofd_osd->dd_lu_dev;
	LASSERT(d->ld_type);
	return sprintf(buf, "%s\n", d->ld_type->ldt_name);
}
LUSTRE_RO_ATTR(fstype);

/**
 * Show journal handling mode: synchronous or asynchronous.
 *
 * When running in asynchronous mode the journal transactions are not
 * committed to disk before the RPC is replied back to the client.
 * This will typically improve client performance when only a small number
 * of clients are writing, since the client(s) can have more write RPCs
 * in flight. However, it also means that the client has to handle recovery
 * on bulk RPCs, and will have to keep more dirty pages in cache before they
 * are committed on the OST.
 *
 * \param[in] m		seq_file handle
 * \param[in] data	unused for single entry
 *
 * \retval		0 on success
 * \retval		negative value on error
 */
static ssize_t sync_journal_show(struct kobject *kobj, struct attribute *attr,
				char *buf)
{
	struct obd_device *obd = container_of(kobj, struct obd_device,
					      obd_kset.kobj);
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);

	return sprintf(buf, "%u\n", ofd->ofd_sync_journal);
}

/**
 * Set journal mode to synchronous or asynchronous.
 *
 * \param[in] file	proc file
 * \param[in] buffer	string which represents mode
 *			1: synchronous mode
 *			0: asynchronous mode
 * \param[in] count	\a buffer length
 * \param[in] off	unused for single entry
 *
 * \retval		\a count on success
 * \retval		negative number on error
 */
static ssize_t sync_journal_store(struct kobject *kobj, struct attribute *attr,
				 const char *buffer, size_t count)
{
	struct obd_device *obd = container_of(kobj, struct obd_device,
					      obd_kset.kobj);
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);
	bool val;
	int rc;

	rc = kstrtobool(buffer, &val);
	if (rc)
		return rc;

	spin_lock(&ofd->ofd_flags_lock);
	ofd->ofd_sync_journal = val;
	ofd_slc_set(ofd);
	spin_unlock(&ofd->ofd_flags_lock);

	return count;
}
LUSTRE_RW_ATTR(sync_journal);

static int ofd_brw_size_seq_show(struct seq_file *m, void *data)
{
	struct obd_device	*obd = m->private;
	struct ofd_device	*ofd = ofd_dev(obd->obd_lu_dev);

	seq_printf(m, "%u\n", ofd->ofd_brw_size / ONE_MB_BRW_SIZE);
	return 0;
}

static ssize_t
ofd_brw_size_seq_write(struct file *file, const char __user *buffer,
		       size_t count, loff_t *off)
{
	struct seq_file	*m = file->private_data;
	struct obd_device *obd = m->private;
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);
	__s64 val;
	int rc;

	rc = lprocfs_str_with_units_to_s64(buffer, count, &val, 'M');
	if (rc)
		return rc;

	if (val <= 0)
		return -EINVAL;

	if (val > DT_MAX_BRW_SIZE ||
	    val < (1 << ofd->ofd_lut.lut_tgd.tgd_blockbits))
		return -ERANGE;

	spin_lock(&ofd->ofd_flags_lock);
	ofd->ofd_brw_size = val;
	spin_unlock(&ofd->ofd_flags_lock);

	return count;
}

LPROC_SEQ_FOPS(ofd_brw_size);

/**
 * Show the limit of soft sync RPCs.
 *
 * This value defines how many IO RPCs with OBD_BRW_SOFT_SYNC flag
 * are allowed before sync update will be triggered.
 *
 * \param[in] m		seq_file handle
 * \param[in] data	unused for single entry
 *
 * \retval		0 on success
 * \retval		negative value on error
 */
static ssize_t soft_sync_limit_show(struct kobject *kobj,
				    struct attribute *attr, char *buf)
{
	struct obd_device *obd = container_of(kobj, struct obd_device,
					      obd_kset.kobj);
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);

	return sprintf(buf, "%u\n", ofd->ofd_soft_sync_limit);
}

/**
 * Change the limit of soft sync RPCs.
 *
 * Define how many IO RPCs with OBD_BRW_SOFT_SYNC flag
 * allowed before sync update will be done.
 *
 * This limit is global across all exports.
 *
 * \param[in] file	proc file
 * \param[in] buffer	string which represents limit
 * \param[in] count	\a buffer length
 * \param[in] off	unused for single entry
 *
 * \retval		\a count on success
 * \retval		negative number on error
 */
static ssize_t soft_sync_limit_store(struct kobject *kobj,
				     struct attribute *attr,
				     const char *buffer, size_t count)
{
	struct obd_device *obd = container_of(kobj, struct obd_device,
					      obd_kset.kobj);
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);
	unsigned int val;
	int rc;

	rc = kstrtouint(buffer, 0, &val);
	if (rc < 0)
		return rc;

	ofd->ofd_soft_sync_limit = val;
	return 0;
}
LUSTRE_RW_ATTR(soft_sync_limit);

/**
 * Show the LFSCK speed limit.
 *
 * The maximum number of items scanned per second.
 *
 * \param[in] m		seq_file handle
 * \param[in] data	unused for single entry
 *
 * \retval		0 on success
 * \retval		negative value on error
 */
static ssize_t lfsck_speed_limit_show(struct kobject *kobj,
				      struct attribute *attr, char *buf)
{
	struct obd_device *obd = container_of(kobj, struct obd_device,
					      obd_kset.kobj);
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);

	return lfsck_get_speed(buf, ofd->ofd_osd);
}

/**
 * Change the LFSCK speed limit.
 *
 * Limit number of items that may be scanned per second.
 *
 * \param[in] file	proc file
 * \param[in] buffer	string which represents limit
 * \param[in] count	\a buffer length
 * \param[in] off	unused for single entry
 *
 * \retval		\a count on success
 * \retval		negative number on error
 */
static ssize_t lfsck_speed_limit_store(struct kobject *kobj,
				       struct attribute *attr,
				       const char *buffer, size_t count)
{
	struct obd_device *obd = container_of(kobj, struct obd_device,
					      obd_kset.kobj);
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);
	unsigned int val;
	int rc;

	rc = kstrtouint(buffer, 0, &val);
	if (rc != 0)
		return rc;

	rc = lfsck_set_speed(ofd->ofd_osd, val);

	return rc != 0 ? rc : count;
}
LUSTRE_RW_ATTR(lfsck_speed_limit);

/**
 * Show LFSCK layout verification stats from the most recent LFSCK run.
 *
 * \param[in] m		seq_file handle
 * \param[in] data	unused for single entry
 *
 * \retval		0 on success
 * \retval		negative value on error
 */
static int ofd_lfsck_layout_seq_show(struct seq_file *m, void *data)
{
	struct obd_device *obd = m->private;
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);

	return lfsck_dump(m, ofd->ofd_osd, LFSCK_TYPE_LAYOUT);
}

LPROC_SEQ_FOPS_RO(ofd_lfsck_layout);

/**
 * Show if LFSCK performed parent FID verification.
 *
 * \param[in] m		seq_file handle
 * \param[in] data	unused for single entry
 *
 * \retval		0 on success
 * \retval		negative value on error
 */
static int ofd_lfsck_verify_pfid_seq_show(struct seq_file *m, void *data)
{
	struct obd_device *obd = m->private;
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);

	seq_printf(m, "switch: %s\ndetected: %llu\nrepaired: %llu\n",
		   ofd->ofd_lfsck_verify_pfid ? "on" : "off",
		   ofd->ofd_inconsistency_self_detected,
		   ofd->ofd_inconsistency_self_repaired);
	return 0;
}

/**
 * Set the LFSCK behavior to verify parent FID correctness.
 *
 * If flag ofd_lfsck_verify_pfid is set then LFSCK does parent FID
 * verification during read/write operations.
 *
 * \param[in] file	proc file
 * \param[in] buffer	string which represents behavior
 *			1: verify parent FID
 *			0: don't verify parent FID
 * \param[in] count	\a buffer length
 * \param[in] off	unused for single entry
 *
 * \retval		\a count on success
 * \retval		negative number on error
 */
static ssize_t
ofd_lfsck_verify_pfid_seq_write(struct file *file, const char __user *buffer,
				size_t count, loff_t *off)
{
	struct seq_file *m = file->private_data;
	struct obd_device *obd = m->private;
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);
	bool val;
	int rc;

	rc = kstrtobool_from_user(buffer, count, &val);
	if (rc)
		return rc;

	ofd->ofd_lfsck_verify_pfid = val;
	if (!ofd->ofd_lfsck_verify_pfid) {
		ofd->ofd_inconsistency_self_detected = 0;
		ofd->ofd_inconsistency_self_repaired = 0;
	}

	return count;
}

LPROC_SEQ_FOPS(ofd_lfsck_verify_pfid);

static int ofd_site_stats_seq_show(struct seq_file *m, void *data)
{
	struct obd_device *obd = m->private;

	return lu_site_stats_seq_print(obd->obd_lu_dev->ld_site, m);
}

LPROC_SEQ_FOPS_RO(ofd_site_stats);

/**
 * Show if the OFD enforces T10PI checksum.
 *
 * \param[in] m		seq_file handle
 * \param[in] data	unused for single entry
 *
 * \retval		0 on success
 * \retval		negative value on error
 */
static ssize_t checksum_t10pi_enforce_show(struct kobject *kobj,
					   struct attribute *attr,
					   char *buf)
{
	struct obd_device *obd = container_of(kobj, struct obd_device,
					      obd_kset.kobj);
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);

	return sprintf(buf, "%u\n", ofd->ofd_checksum_t10pi_enforce);
}

/**
 * Force specific T10PI checksum modes to be enabled
 *
 * If T10PI *is* supported in hardware, allow only the supported T10PI type
 * to be used. If T10PI is *not* supported by the OSD, setting the enforce
 * parameter forces all T10PI types to be enabled (even if slower) for
 * testing.
 *
 * The final determination of which algorithm to be used depends whether
 * the client supports T10PI or not, and is handled at client connect time.
 *
 * \param[in] file	proc file
 * \param[in] buffer	string which represents mode
 *			1: set T10PI checksums enforced
 *			0: unset T10PI checksums enforced
 * \param[in] count	\a buffer length
 * \param[in] off	unused for single entry
 *
 * \retval		\a count on success
 * \retval		negative number on error
 */
static ssize_t checksum_t10pi_enforce_store(struct kobject *kobj,
					    struct attribute *attr,
					    const char *buffer, size_t count)
{
	struct obd_device *obd = container_of(kobj, struct obd_device,
					      obd_kset.kobj);
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);
	bool enforce;
	int rc;

	rc = kstrtobool(buffer, &enforce);
	if (rc)
		return rc;

	spin_lock(&ofd->ofd_flags_lock);
	ofd->ofd_checksum_t10pi_enforce = enforce;
	spin_unlock(&ofd->ofd_flags_lock);
	return count;
}
LUSTRE_RW_ATTR(checksum_t10pi_enforce);

#if LUSTRE_VERSION_CODE < OBD_OCD_VERSION(2, 14, 53, 0)
static bool max_file_warned;
static bool rd_cache_warned;
static bool wr_cache_warned;

static ssize_t read_cache_enable_show(struct kobject *kobj,
				      struct attribute *attr,
				      char *buf)
{
	struct obd_device *obd = container_of(kobj, struct obd_device,
					      obd_kset.kobj);
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);

	if (!rd_cache_warned) {
		rd_cache_warned = true;
		pr_info("ofd: 'obdfilter.*.read_cache_enabled' is deprecated, use 'osd-*.read_cache_enabled' instead\n");
	}

	if (!ofd->ofd_read_cache_enable)
		return -EOPNOTSUPP;

	return lustre_attr_show(&ofd->ofd_osd->dd_kobj,
				ofd->ofd_read_cache_enable, buf);
}

static ssize_t read_cache_enable_store(struct kobject *kobj,
				       struct attribute *attr,
				       const char *buffer, size_t count)
{
	struct obd_device *obd = container_of(kobj, struct obd_device,
					      obd_kset.kobj);
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);

	if (!rd_cache_warned) {
		rd_cache_warned = true;
		pr_info("ofd: 'obdfilter.*.read_cache_enabled' is deprecated, use 'osd-*.read_cache_enabled' instead\n");
	}

	if (!ofd->ofd_read_cache_enable)
		return -EOPNOTSUPP;

	return lustre_attr_store(&ofd->ofd_osd->dd_kobj,
				 ofd->ofd_read_cache_enable, buffer, count);
}
LUSTRE_RW_ATTR(read_cache_enable);

static ssize_t readcache_max_filesize_show(struct kobject *kobj,
					   struct attribute *attr,
					   char *buf)
{
	struct obd_device *obd = container_of(kobj, struct obd_device,
					      obd_kset.kobj);
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);

	if (!max_file_warned) {
		max_file_warned = true;
		pr_info("ofd: 'obdfilter.*.readcache_max_filesize' is deprecated, use 'osd-*.readcache_max_filesize' instead\n");
	}

	if (!ofd->ofd_read_cache_max_filesize)
		return -EOPNOTSUPP;

	return lustre_attr_show(&ofd->ofd_osd->dd_kobj,
				ofd->ofd_read_cache_max_filesize, buf);
}

static ssize_t readcache_max_filesize_store(struct kobject *kobj,
					    struct attribute *attr,
					    const char *buffer, size_t count)
{
	struct obd_device *obd = container_of(kobj, struct obd_device,
					      obd_kset.kobj);
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);

	if (!max_file_warned) {
		max_file_warned = true;
		pr_info("ofd: 'obdfilter.*.readcache_max_filesize' is deprecated, use 'osd-*.readcache_max_filesize' instead\n");
	}

	if (!ofd->ofd_read_cache_max_filesize)
		return -EOPNOTSUPP;

	return lustre_attr_store(&ofd->ofd_osd->dd_kobj,
				 ofd->ofd_read_cache_max_filesize,
				 buffer, count);
}
LUSTRE_RW_ATTR(readcache_max_filesize);

static ssize_t writethrough_cache_enable_show(struct kobject *kobj,
					      struct attribute *attr,
					      char *buf)
{
	struct obd_device *obd = container_of(kobj, struct obd_device,
					      obd_kset.kobj);
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);

	if (!wr_cache_warned) {
		wr_cache_warned = true;
		pr_info("ofd: 'obdfilter.*.writethrough_cache_enabled' is deprecated, use 'osd-*.writethrough_cache_enabled' instead\n");
	}

	if (!ofd->ofd_write_cache_enable)
		return -EOPNOTSUPP;

	return lustre_attr_show(&ofd->ofd_osd->dd_kobj,
				ofd->ofd_write_cache_enable, buf);
}

static ssize_t writethrough_cache_enable_store(struct kobject *kobj,
					       struct attribute *attr,
					       const char *buffer, size_t count)
{
	struct obd_device *obd = container_of(kobj, struct obd_device,
					      obd_kset.kobj);
	struct ofd_device *ofd = ofd_dev(obd->obd_lu_dev);

	if (!ofd->ofd_write_cache_enable)
		return -EOPNOTSUPP;

	return lustre_attr_store(&ofd->ofd_osd->dd_kobj,
				 ofd->ofd_write_cache_enable,
				 buffer, count);
}
LUSTRE_RW_ATTR(writethrough_cache_enable);
#endif

LPROC_SEQ_FOPS_RO_TYPE(ofd, recovery_status);
LUSTRE_RW_ATTR(recovery_time_hard);
LUSTRE_RW_ATTR(recovery_time_soft);
LUSTRE_RW_ATTR(ir_factor);

LPROC_SEQ_FOPS_WR_ONLY(ofd, evict_client);
LPROC_SEQ_FOPS_RW_TYPE(ofd, checksum_dump);
LUSTRE_RW_ATTR(job_cleanup_interval);

LUSTRE_RO_ATTR(tot_dirty);
LUSTRE_RO_ATTR(tot_granted);
LUSTRE_RO_ATTR(tot_pending);
LUSTRE_RW_ATTR(grant_compat_disable);
LUSTRE_RO_ATTR(instance);

LUSTRE_RO_ATTR(num_exports);

struct lprocfs_vars lprocfs_ofd_obd_vars[] = {
	{ .name =	"last_id",
	  .fops =	&ofd_last_id_fops		},
	{ .name =	"recovery_status",
	  .fops =	&ofd_recovery_status_fops	},
	{ .name =	"evict_client",
	  .fops =	&ofd_evict_client_fops		},
	{ .name =	"brw_size",
	  .fops =	&ofd_brw_size_fops		},
	{ .name =	"checksum_dump",
	  .fops =	&ofd_checksum_dump_fops		},
	{ .name =	"lfsck_layout",
	  .fops =	&ofd_lfsck_layout_fops		},
	{ .name	=	"lfsck_verify_pfid",
	  .fops	=	&ofd_lfsck_verify_pfid_fops	},
	{ .name =	"site_stats",
	  .fops =	&ofd_site_stats_fops		},
	{ NULL }
};

/**
 * Initialize OFD statistics counters
 *
 * param[in] stats	statistics counters
 */
void ofd_stats_counter_init(struct lprocfs_stats *stats)
{
	LASSERT(stats && stats->ls_num >= LPROC_OFD_STATS_LAST);

	lprocfs_counter_init(stats, LPROC_OFD_STATS_READ,
			     LPROCFS_CNTR_AVGMINMAX, "read_bytes", "bytes");
	lprocfs_counter_init(stats, LPROC_OFD_STATS_WRITE,
			     LPROCFS_CNTR_AVGMINMAX, "write_bytes", "bytes");
	lprocfs_counter_init(stats, LPROC_OFD_STATS_GETATTR,
			     0, "getattr", "reqs");
	lprocfs_counter_init(stats, LPROC_OFD_STATS_SETATTR,
			     0, "setattr", "reqs");
	lprocfs_counter_init(stats, LPROC_OFD_STATS_PUNCH,
			     0, "punch", "reqs");
	lprocfs_counter_init(stats, LPROC_OFD_STATS_SYNC,
			     0, "sync", "reqs");
	lprocfs_counter_init(stats, LPROC_OFD_STATS_DESTROY,
			     0, "destroy", "reqs");
	lprocfs_counter_init(stats, LPROC_OFD_STATS_CREATE,
			     0, "create", "reqs");
	lprocfs_counter_init(stats, LPROC_OFD_STATS_STATFS,
			     0, "statfs", "reqs");
	lprocfs_counter_init(stats, LPROC_OFD_STATS_GET_INFO,
			     0, "get_info", "reqs");
	lprocfs_counter_init(stats, LPROC_OFD_STATS_SET_INFO,
			     0, "set_info", "reqs");
	lprocfs_counter_init(stats, LPROC_OFD_STATS_QUOTACTL,
			     0, "quotactl", "reqs");
}

LPROC_SEQ_FOPS(lprocfs_nid_stats_clear);

static struct attribute *ofd_attrs[] = {
	&lustre_attr_tot_dirty.attr,
	&lustre_attr_tot_granted.attr,
	&lustre_attr_tot_pending.attr,
	&lustre_attr_grant_compat_disable.attr,
	&lustre_attr_instance.attr,
	&lustre_attr_recovery_time_hard.attr,
	&lustre_attr_recovery_time_soft.attr,
	&lustre_attr_ir_factor.attr,
	&lustre_attr_num_exports.attr,
	&lustre_attr_seqs_allocated.attr,
	&lustre_attr_grant_precreate.attr,
	&lustre_attr_precreate_batch.attr,
	&lustre_attr_degraded.attr,
	&lustre_attr_fstype.attr,
	&lustre_attr_sync_journal.attr,
	&lustre_attr_soft_sync_limit.attr,
	&lustre_attr_lfsck_speed_limit.attr,
	&lustre_attr_job_cleanup_interval.attr,
	&lustre_attr_checksum_t10pi_enforce.attr,
#if LUSTRE_VERSION_CODE < OBD_OCD_VERSION(2, 14, 53, 0)
	&lustre_attr_read_cache_enable.attr,
	&lustre_attr_readcache_max_filesize.attr,
	&lustre_attr_writethrough_cache_enable.attr,
#endif
	NULL,
};

/**
 * Initialize all needed procfs entries for OFD device.
 *
 * \param[in] ofd	OFD device
 *
 * \retval		0 if successful
 * \retval		negative value on error
 */
int ofd_tunables_init(struct ofd_device *ofd)
{
	struct obd_device *obd = ofd_obd(ofd);
	struct proc_dir_entry *entry;
	int rc = 0;

	ENTRY;
	/* lprocfs must be setup before the ofd so state can be safely added
	 * to /proc incrementally as the ofd is setup
	 */
	obd->obd_ktype.default_attrs = ofd_attrs;
	obd->obd_vars = lprocfs_ofd_obd_vars;
	rc = lprocfs_obd_setup(obd, false);
	if (rc) {
		CERROR("%s: lprocfs_obd_setup failed: %d.\n",
		       obd->obd_name, rc);
		RETURN(rc);
	}

	rc = tgt_tunables_init(&ofd->ofd_lut);
	if (rc) {
		CERROR("%s: tgt_tunables_init failed: rc = %d\n",
		       obd->obd_name, rc);
		GOTO(obd_cleanup, rc);
	}

	rc = lprocfs_alloc_obd_stats(obd, LPROC_OFD_STATS_LAST);
	if (rc) {
		CERROR("%s: lprocfs_alloc_obd_stats failed: %d.\n",
		       obd->obd_name, rc);
		GOTO(tgt_cleanup, rc);
	}

	entry = lprocfs_register("exports", obd->obd_proc_entry, NULL, NULL);
	if (IS_ERR(entry)) {
		rc = PTR_ERR(entry);
		CERROR("%s: error %d setting up lprocfs for %s\n",
		       obd->obd_name, rc, "exports");
		GOTO(obd_free_stats, rc);
	}
	obd->obd_proc_exports_entry = entry;

	entry = lprocfs_add_simple(obd->obd_proc_exports_entry, "clear",
				   obd, &lprocfs_nid_stats_clear_fops);
	if (IS_ERR(entry)) {
		rc = PTR_ERR(entry);
		CERROR("%s: add proc entry 'clear' failed: %d.\n",
		       obd->obd_name, rc);
		GOTO(obd_free_stats, rc);
	}

	ofd_stats_counter_init(obd->obd_stats);

	rc = lprocfs_job_stats_init(obd, LPROC_OFD_STATS_LAST,
				    ofd_stats_counter_init);
	if (rc)
		GOTO(obd_free_stats, rc);

	RETURN(0);

obd_free_stats:
	lprocfs_free_obd_stats(obd);
tgt_cleanup:
	tgt_tunables_fini(&ofd->ofd_lut);
obd_cleanup:
	lprocfs_obd_cleanup(obd);

	return rc;
}
#endif /* CONFIG_PROC_FS */
