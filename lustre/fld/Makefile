MODULES := fld
fld-objs := fld_request.o fld_cache.o lproc_fld.o
#fld-objs += fld_handler.o fld_index.o

EXTRA_PRE_CFLAGS := -I/Users/crb002/github/lustre-release/lustre
EXTRA_DIST := $(fld-objs:%.o=%.c) fld_internal.h
EXTRA_DIST += fld_handler.c fld_index.c

include /Users/crb002/github/lustre-release/Rules
