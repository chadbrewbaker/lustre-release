MODULES := osd_ldiskfs
osd_ldiskfs-objs = osd_handler.o osd_oi.o osd_lproc.o osd_iam.o \
		   osd_iam_lfix.o osd_iam_lvar.o osd_io.o osd_compat.o \
		   osd_scrub.o osd_dynlocks.o osd_quota.o osd_quota_fmt.o

osd_ldiskfs-objs += osd_integrity.o

EXTRA_PRE_CFLAGS := -I/fs -I/Users/crb002/github/lustre-release -I/Users/crb002/github/lustre-release/ldiskfs

include /Users/crb002/github/lustre-release/Rules
