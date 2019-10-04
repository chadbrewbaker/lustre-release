default: all

MODULES := ldiskfs

# copy makefile over to not break patches
backfs_extra := $(wildcard /fs/ext4/Makefile)

backfs_headers := $(wildcard /*.h)
linux_headers := $(wildcard /include/linux/ext4*.h)
linux_new_headers := htree_lock.h
trace_headers := $(wildcard /include/trace/events/ext4*.h)

backfs_sources := $(filter-out %.mod.c,$(wildcard /*.c))

new_sources := mmp.c htree_lock.c
new_headers :=

ldiskfs_patched_sources := $(notdir $(backfs_sources) $(backfs_headers)) $(new_sources) $(new_headers)
ldiskfs_sources := $(ldiskfs_patched_sources)

ldiskfs-objs := $(filter %.o,$(ldiskfs_sources:.c=.o))

EXTRA_PRE_CFLAGS := -I/fs -I/Users/crb002/github/lustre-release/ldiskfs

include /Users/crb002/github/lustre-release/Rules
