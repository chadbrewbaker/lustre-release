MODULES := lustre
lustre-objs := dcache.o dir.o file.o llite_lib.o llite_nfs.o
lustre-objs += rw.o lproc_llite.o namei.o symlink.o llite_mmap.o
lustre-objs += xattr.o xattr_cache.o
lustre-objs += rw26.o super25.o statahead.o xattr_security.o
lustre-objs += glimpse.o
lustre-objs += lcommon_cl.o
lustre-objs += lcommon_misc.o
lustre-objs += vvp_dev.o vvp_page.o vvp_io.o vvp_object.o
lustre-objs += range_lock.o pcc.o

EXTRA_DIST := $(lustre-objs:.o=.c) xattr.c rw26.c super25.c
EXTRA_DIST += llite_internal.h vvp_internal.h range_lock.h pcc.h

include /Users/crb002/github/lustre-release/Rules
