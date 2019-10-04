MODULES := obdclass llog_test

default: all

obdclass-all-objs := llog.o llog_cat.o llog_obd.o llog_swab.o llog_osd.o
obdclass-all-objs += class_obd.o debug.o genops.o llog_ioctl.o
obdclass-all-objs += lprocfs_status.o lprocfs_counters.o
obdclass-all-objs += lustre_handles.o lustre_peer.o local_storage.o
obdclass-all-objs += statfs_pack.o obdo.o obd_config.o obd_mount.o obd_sysfs.o
obdclass-all-objs += lu_object.o dt_object.o
obdclass-all-objs += cl_object.o cl_page.o cl_lock.o cl_io.o lu_ref.o
obdclass-all-objs += linkea.o
obdclass-all-objs += kernelcomm.o jobid.o
obdclass-all-objs += integrity.o obd_cksum.o
obdclass-all-objs += lu_qos.o
obdclass-all-objs += lu_tgt_descs.o

#obdclass-all-objs += acl.o
#obdclass-all-objs += idmap.o
#obdclass-all-objs += upcall_cache.o
#obdclass-all-objs += lprocfs_jobstats.o
#obdclass-all-objs += lprocfs_status_server.o
#obdclass-all-objs += lu_ucred.o
#obdclass-all-objs += md_attrs.o
#obdclass-all-objs += obd_mount_server.o
#obdclass-all-objs += obdo_server.o
#obdclass-all-objs += scrub.o

obdclass-objs := $(obdclass-all-objs)

EXTRA_PRE_CFLAGS := -I/fs -I@LDISKFS_DIR@ -I@LDISKFS_DIR@/ldiskfs

EXTRA_DIST = $(obdclass-all-objs:.o=.c) llog_test.c llog_internal.h
EXTRA_DIST += cl_internal.h local_storage.h

EXTRA_DIST += acl.c
EXTRA_DIST += idmap.c
EXTRA_DIST += upcall_cache.c
EXTRA_DIST += lprocfs_jobstats.c
EXTRA_DIST += lprocfs_status_server.c
EXTRA_DIST += lu_ucred.c
EXTRA_DIST += md_attrs.c
EXTRA_DIST += obd_mount_server.c
EXTRA_DIST += obdo_server.c
EXTRA_DIST += scrub.c

include /Users/crb002/github/lustre-release/Rules
