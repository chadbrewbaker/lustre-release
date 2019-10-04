MODULES = libcfs

libcfs-linux-objs := linux-tracefile.o linux-debug.o
libcfs-linux-objs += linux-prim.o
libcfs-linux-objs += linux-curproc.o
libcfs-linux-objs += linux-hash.o
libcfs-linux-objs += linux-module.o
libcfs-linux-objs += linux-crypto.o linux-crypto-adler.o
libcfs-linux-objs += linux-wait.o
#libcfs-linux-objs += linux-crypto-crc32.o
#libcfs-linux-objs += linux-crypto-crc32pclmul.o crc32-pclmul_asm.o
#libcfs-linux-objs += linux-crypto-crc32c-pclmul.o crc32c-pcl-intel-asm_64.o

default: all

libcfs-linux-objs := $(addprefix linux/,$(libcfs-linux-objs))

libcfs-all-objs := debug.o fail.o module.o tracefile.o \
		   libcfs_string.o hash.o \
		   workitem.o libcfs_cpu.o \
		   libcfs_mem.o libcfs_lock.o heap.o

libcfs-objs := $(libcfs-linux-objs) $(libcfs-all-objs)

EXTRA_PRE_CFLAGS := -I/Users/crb002/github/lustre-release/lustre/../libcfs/libcfs

include /Users/crb002/github/lustre-release/Rules
