MODULES := fid
fid-objs := fid_request.o lproc_fid.o
#fid-objs += fid_handler.o fid_store.o fid_lib.o

EXTRA_DIST = $(fid-objs:%.o=%.c) fid_internal.h
EXTRA_DIST += fid_handler.c fid_store.c fid_lib.c

include /Users/crb002/github/lustre-release/Rules
