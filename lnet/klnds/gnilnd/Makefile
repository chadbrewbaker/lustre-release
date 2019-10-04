MODULES := kgnilnd
kgnilnd-objs := gnilnd.o gnilnd_cb.o gnilnd_modparams.o gnilnd_debug.o gnilnd_proc.o \
		gnilnd_sysctl.o gnilnd_stack.o gnilnd_conn.o

EXTRA_POST_CFLAGS := -D"SVN_CODE_REV=KBUILD_STR(${SVN_CODE_REV})" 

EXTRA_DIST = $(kgnilnd-objs:%.o=%.c) gnilnd.h gnilnd_api_wrap.h

include /Users/crb002/github/lustre-release/Rules
