# Makefile.in generated by automake 1.15 from Makefile.am.
# lustre/doc/Makefile.  Generated from Makefile.in by configure.

# Copyright (C) 1994-2014 Free Software Foundation, Inc.

# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.



#
# GPL HEADER START
#
# DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 only,
# as published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License version 2 for more details (a copy is included
# in the LICENSE file that accompanied this code).
#
# You should have received a copy of the GNU General Public License
# version 2 along with this program; If not, see
# http://www.gnu.org/licenses/gpl-2.0.html
#
# GPL HEADER END
#

#
# Copyright (c) 2001, 2010, Oracle and/or its affiliates. All rights reserved.
# Use is subject to license terms.
#
# Copyright (c) 2011, 2017, Intel Corporation.
#

#
# This file is part of Lustre, http://www.lustre.org/
# Lustre is a trademark of Sun Microsystems, Inc.
#

am__is_gnu_make = { \
  if test -z '$(MAKELEVEL)'; then \
    false; \
  elif test -n '$(MAKE_HOST)'; then \
    true; \
  elif test -n '$(MAKE_VERSION)' && test -n '$(CURDIR)'; then \
    true; \
  else \
    false; \
  fi; \
}
am__make_running_with_option = \
  case $${target_option-} in \
      ?) ;; \
      *) echo "am__make_running_with_option: internal error: invalid" \
              "target option '$${target_option-}' specified" >&2; \
         exit 1;; \
  esac; \
  has_opt=no; \
  sane_makeflags=$$MAKEFLAGS; \
  if $(am__is_gnu_make); then \
    sane_makeflags=$$MFLAGS; \
  else \
    case $$MAKEFLAGS in \
      *\\[\ \	]*) \
        bs=\\; \
        sane_makeflags=`printf '%s\n' "$$MAKEFLAGS" \
          | sed "s/$$bs$$bs[$$bs $$bs	]*//g"`;; \
    esac; \
  fi; \
  skip_next=no; \
  strip_trailopt () \
  { \
    flg=`printf '%s\n' "$$flg" | sed "s/$$1.*$$//"`; \
  }; \
  for flg in $$sane_makeflags; do \
    test $$skip_next = yes && { skip_next=no; continue; }; \
    case $$flg in \
      *=*|--*) continue;; \
        -*I) strip_trailopt 'I'; skip_next=yes;; \
      -*I?*) strip_trailopt 'I';; \
        -*O) strip_trailopt 'O'; skip_next=yes;; \
      -*O?*) strip_trailopt 'O';; \
        -*l) strip_trailopt 'l'; skip_next=yes;; \
      -*l?*) strip_trailopt 'l';; \
      -[dEDm]) skip_next=yes;; \
      -[JT]) skip_next=yes;; \
    esac; \
    case $$flg in \
      *$$target_option*) has_opt=yes; break;; \
    esac; \
  done; \
  test $$has_opt = yes
am__make_dryrun = (target_option=n; $(am__make_running_with_option))
am__make_keepgoing = (target_option=k; $(am__make_running_with_option))
pkgdatadir = $(datadir)/lustre
pkgincludedir = $(includedir)/lustre
pkglibdir = $(libdir)/lustre
pkglibexecdir = $(libexecdir)/lustre
am__cd = CDPATH="$${ZSH_VERSION+.}$(PATH_SEPARATOR)" && cd
install_sh_DATA = $(install_sh) -c -m 644
install_sh_PROGRAM = $(install_sh) -c
install_sh_SCRIPT = $(install_sh) -c
INSTALL_HEADER = $(INSTALL_DATA)
transform = $(program_transform_name)
NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
build_triplet = x86_64-apple-darwin18.7.0
host_triplet = x86_64-apple-darwin18.7.0
target_triplet = x86_64-apple-darwin18.7.0
#am__append_1 = $(SERVER_MANFILES)
subdir = lustre/doc
ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
am__aclocal_m4_deps = $(top_srcdir)/configure.ac
am__configure_deps = $(am__aclocal_m4_deps) $(CONFIGURE_DEPENDENCIES) \
	$(ACLOCAL_M4)
DIST_COMMON = $(srcdir)/Makefile.am $(am__DIST_COMMON)
mkinstalldirs = $(install_sh) -d
CONFIG_HEADER = $(top_builddir)/config.h
CONFIG_CLEAN_FILES =
CONFIG_CLEAN_VPATH_FILES =
AM_V_P = $(am__v_P_$(V))
am__v_P_ = $(am__v_P_$(AM_DEFAULT_VERBOSITY))
am__v_P_0 = false
am__v_P_1 = :
AM_V_GEN = $(am__v_GEN_$(V))
am__v_GEN_ = $(am__v_GEN_$(AM_DEFAULT_VERBOSITY))
am__v_GEN_0 = @echo "  GEN     " $@;
am__v_GEN_1 = 
AM_V_at = $(am__v_at_$(V))
am__v_at_ = $(am__v_at_$(AM_DEFAULT_VERBOSITY))
am__v_at_0 = @
am__v_at_1 = 
SOURCES =
DIST_SOURCES =
am__can_run_installinfo = \
  case $$AM_UPDATE_INFO_DIR in \
    n|no|NO) false;; \
    *) (install-info --version) >/dev/null 2>&1;; \
  esac
am__vpath_adj_setup = srcdirstrip=`echo "$(srcdir)" | sed 's|.|.|g'`;
am__vpath_adj = case $$p in \
    $(srcdir)/*) f=`echo "$$p" | sed "s|^$$srcdirstrip/||"`;; \
    *) f=$$p;; \
  esac;
am__strip_dir = f=`echo $$p | sed -e 's|^.*/||'`;
am__install_max = 40
am__nobase_strip_setup = \
  srcdirstrip=`echo "$(srcdir)" | sed 's/[].[^$$\\*|]/\\\\&/g'`
am__nobase_strip = \
  for p in $$list; do echo "$$p"; done | sed -e "s|$$srcdirstrip/||"
am__nobase_list = $(am__nobase_strip_setup); \
  for p in $$list; do echo "$$p $$p"; done | \
  sed "s| $$srcdirstrip/| |;"' / .*\//!s/ .*/ ./; s,\( .*\)/[^/]*$$,\1,' | \
  $(AWK) 'BEGIN { files["."] = "" } { files[$$2] = files[$$2] " " $$1; \
    if (++n[$$2] == $(am__install_max)) \
      { print $$2, files[$$2]; n[$$2] = 0; files[$$2] = "" } } \
    END { for (dir in files) print dir, files[dir] }'
am__base_list = \
  sed '$$!N;$$!N;$$!N;$$!N;$$!N;$$!N;$$!N;s/\n/ /g' | \
  sed '$$!N;$$!N;$$!N;$$!N;s/\n/ /g'
am__uninstall_files_from_dir = { \
  test -z "$$files" \
    || { test ! -d "$$dir" && test ! -f "$$dir" && test ! -r "$$dir"; } \
    || { echo " ( cd '$$dir' && rm -f" $$files ")"; \
         $(am__cd) "$$dir" && rm -f $$files; }; \
  }
man1dir = $(mandir)/man1
am__installdirs = "$(DESTDIR)$(man1dir)" "$(DESTDIR)$(man3dir)" \
	"$(DESTDIR)$(man5dir)" "$(DESTDIR)$(man7dir)" \
	"$(DESTDIR)$(man8dir)"
man3dir = $(mandir)/man3
man5dir = $(mandir)/man5
man7dir = $(mandir)/man7
man8dir = $(mandir)/man8
NROFF = nroff
MANS = $(man_MANS)
am__tagged_files = $(HEADERS) $(SOURCES) $(TAGS_FILES) $(LISP)
am__DIST_COMMON = $(srcdir)/Makefile.in
DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)
ACLOCAL = ${SHELL} /Users/crb002/github/lustre-release/config/missing aclocal-1.15
AMTAR = $${TAR-tar}
AM_DEFAULT_VERBOSITY = 1
AR = ar
AUTOCONF = ${SHELL} /Users/crb002/github/lustre-release/config/missing autoconf
AUTOCONF_HDIR = 
AUTOHEADER = ${SHELL} /Users/crb002/github/lustre-release/config/missing autoheader
AUTOMAKE = ${SHELL} /Users/crb002/github/lustre-release/config/missing automake-1.15
AWK = awk
BASH_COMPLETION_DIR = ${datarootdir}/bash-completion/completions
CC = gcc
CCAS = gcc
CCASDEPMODE = depmode=gcc3
CCASFLAGS = -Wall -fPIC -D_GNU_SOURCE
CCDEPMODE = depmode=gcc3
CFLAGS = -g -O2 -Wall -Werror
CONFIG_INCLUDE = 
CPP = gcc -E
CPPFLAGS = -include /Users/crb002/github/lustre-release/undef.h -include /Users/crb002/github/lustre-release/config.h -I/Users/crb002/github/lustre-release/libcfs/include -I/Users/crb002/github/lustre-release/lnet/include -I/Users/crb002/github/lustre-release/lnet/include/uapi -I/Users/crb002/github/lustre-release/lustre/include -I/Users/crb002/github/lustre-release/lustre/include/uapi 
CYGPATH_W = echo
DEBUGFS = debugfs
DEFS = -DHAVE_CONFIG_H
DEPDIR = .deps
DLLTOOL = false
DSYMUTIL = dsymutil
DUMPBIN = 
DUMPE2FS = dumpe2fs
E2FSCK = e2fsck
E2FSPROGS = e2fsprogs
E2LABEL = e2label
ECHO_C = \c
ECHO_N = 
ECHO_T = 
EGREP = /usr/bin/grep -E
ENABLEO2IB = 
ENABLE_DOC = 0
ENABLE_GSS = 
ENABLE_LDISKFS = no
ENABLE_SERVER = no
ENABLE_ZFS = no
EXEEXT = 
EXT4_SRC_DIR = 
EXTRA_KCFLAGS = -include /Users/crb002/github/lustre-release/undef.h -include /Users/crb002/github/lustre-release/config.h  -g -I/Users/crb002/github/lustre-release/libcfs/include -I/Users/crb002/github/lustre-release/lnet/include -I/Users/crb002/github/lustre-release/lustre/include -Wno-format-truncation -Wno-stringop-truncation -Wno-stringop-overflow
EXTRA_OFED_CONFIG = 
EXTRA_OFED_INCLUDE = 
EXTRA_SYMBOLS = 
FGREP = /usr/bin/grep -F
GNICPPFLAGS = 
GNILND = 
GREP = /usr/bin/grep
INCLUDE_RULES = include /Users/crb002/github/lustre-release/Rules
INSTALL = /usr/local/bin/ginstall -c
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_PROGRAM = ${INSTALL}
INSTALL_SCRIPT = ${INSTALL}
INSTALL_STRIP_PROGRAM = $(install_sh) -c -s
K5VERS = 
KMODEXT = 
KMP_MODDIR = 
KRBCFLAGS = 
KRBDIR = 
KRBLDFLAGS = 
KRBLIBS = 
LD = /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ld
LDFLAGS = 
LDISKFS_SERIES = 
LDISKFS_SUBDIR = 
LDLIBMOUNT = 
LIBCFS_INCLUDE_DIR = libcfs/include
LIBCFS_SUBDIR = libcfs
LIBEFENCE = 
LIBOBJS = 
LIBREADLINE = -lreadline
LIBS = 
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LINUX = 
LINUXRELEASE = 
LINUX_CONFIG = 
LINUX_OBJ = 
LIPO = lipo
LN_S = ln -s
LTLIBOBJS = 
LT_SYS_LIBRARY_PATH = 
LUSTRE = /Users/crb002/github/lustre-release/lustre
LUSTREIOKIT_SUBDIR = lustre-iokit
MAINT = 
MAKEINFO = ${SHELL} /Users/crb002/github/lustre-release/config/missing makeinfo
MANIFEST_TOOL = :
MKDIR_P = /usr/local/bin/gmkdir -p
MKE2FS = mke2fs
MODULE_TARGET = 
MOSTLYCLEANFILES = .*.cmd .*.flags *.o *.ko *.mod.c .depend .*.1.* Modules.symvers Module.symvers
MPICC_WRAPPER = mpicc
MPI_BIN = /usr/local/bin
NET_SNMP_CFLAGS = -DNETSNMP_ENABLE_IPV6 -fno-strict-aliasing -g -Os -pipe -DBUILD=161 -DMACOSX_DEPLOYMENT_TARGET=10.14 -F/BuildRoot/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.Internal.sdk/System/Library/PrivateFrameworks/ -F/BuildRoot/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.Internal.sdk/System/Library/Frameworks/ -I/usr/local/libressl/include -Udarwin18 -Ddarwin18=darwin18 -I/usr/local/include
NET_SNMP_CONFIG = net-snmp-config
NET_SNMP_LIBS = -lnetsnmpmibs -lnetsnmpagent -lnetsnmp -lcrypto.35
NM = /usr/bin/nm -B
NMEDIT = nmedit
O2IBLND = 
O2IBPATH = 
OBJDUMP = objdump
OBJEXT = o
OSDADDON = 
OTOOL = otool
OTOOL64 = :
PACKAGE = lustre
PACKAGE_BUGREPORT = https://jira.whamcloud.com/
PACKAGE_NAME = Lustre
PACKAGE_STRING = Lustre 2.12.58_104_g279c264_dirty
PACKAGE_TARNAME = lustre
PACKAGE_URL = 
PACKAGE_VERSION = 2.12.58_104_g279c264_dirty
PATCHED_INTEGRITY_INTF = 
PATH_SEPARATOR = :
PFSCK = fsck
PKG_CONFIG = /usr/local/bin/pkg-config
PKG_CONFIG_LIBDIR = 
PKG_CONFIG_PATH = 
PTHREAD_LIBS = -lpthread
RANLIB = ranlib
RPMBUILD = 
RPMBUILD_BINARY_ARGS =  --without lustre_modules --without servers --without ldiskfs --without snmp
SED = /usr/bin/sed
SELINUX = 
SET_MAKE = 
SHELL = /bin/sh
SNMP_DIST_SUBDIR = snmp
SNMP_SUBDIR = 
SPL = 
SPL_OBJ = 
SPL_VERSION = 
STRIP = strip
SYMVERFILE = 
TUNE2FS = tune2fs
VERSION = 2.12.58_104_g279c264_dirty
VERSION_HDIR = 
ZFS = 
ZFS_LIBZFS_INCLUDE = 
ZFS_LIBZFS_LDFLAGS = 
ZFS_LIBZFS_LIBS = 
ZFS_OBJ = 
ZFS_VERSION = 
abs_builddir = /Users/crb002/github/lustre-release/lustre/doc
abs_srcdir = /Users/crb002/github/lustre-release/lustre/doc
abs_top_builddir = /Users/crb002/github/lustre-release
abs_top_srcdir = /Users/crb002/github/lustre-release
ac_configure_args = 
ac_ct_AR = ar
ac_ct_CC = gcc
ac_ct_DUMPBIN = 
agentdir = ${pkglibdir}/snmp
am__include = include
am__leading_dot = .
am__quote = 
am__tar = tar --format=ustar -chf - "$$tardir"
am__untar = tar -xf -
bindir = ${exec_prefix}/bin
build = x86_64-apple-darwin18.7.0
build_alias = 
build_cpu = x86_64
build_os = darwin18.7.0
build_vendor = apple
builddir = .
datadir = ${datarootdir}
datarootdir = ${prefix}/share
demodir = $(docdir)/demo
docdir = ${datadir}/doc/$(PACKAGE)
dvidir = ${docdir}
exec_prefix = ${prefix}
host = x86_64-apple-darwin18.7.0
host_alias = 
host_cpu = x86_64
host_os = darwin18.7.0
host_vendor = apple
htmldir = ${docdir}
includedir = ${prefix}/include
infodir = ${datarootdir}/info
install_sh = ${SHELL} /Users/crb002/github/lustre-release/config/install-sh
lb_target_os = 
libdir = ${exec_prefix}/lib
libexecdir = ${exec_prefix}/libexec
localedir = ${datarootdir}/locale
localstatedir = ${prefix}/var
mandir = ${datarootdir}/man
mibdir = ${pkgdatadir}/snmp/mibs
mkdir_p = $(MKDIR_P)
modulefsdir = 
modulenetdir = 
oldincludedir = /usr/include
patch_avail = 
pdfdir = ${docdir}
pkgexampledir = ${pkgdatadir}/examples
prefix = /usr
program_transform_name = s,x,x,
psdir = ${docdir}
quilt_avail = 
rootsbindir = /sbin
sbindir = ${exec_prefix}/sbin
sharedstatedir = ${prefix}/com
srcdir = .
sysconfdir = /etc
systemdsystemunitdir = 
target = x86_64-apple-darwin18.7.0
target_alias = 
target_cpu = x86_64
target_os = darwin18.7.0
target_vendor = apple
top_build_prefix = ../../
top_builddir = ../..
top_srcdir = ../..
MANFILES = lctl.8 lctl-get_param.8 lctl-network.8 lctl-set_param.8 \
	ldev.8 ldev.conf.5 lfs.1 lfs-df.1 lfs-fid2path.1 lfs-find.1 \
	lfs-getstripe.1 lfs-getdirstripe.1 lfs-heat.1 lfs-hsm.1 \
	lfs-ladvise.1 lfs_migrate.1 lfs-migrate.1 lfs-mirror-create.1 \
	lfs-mirror-extend.1 lfs-mirror-resync.1 lfs-mirror-split.1 \
	lfs-mirror-verify.1 lfs-mkdir.1 lfs-path2fid.1 lfs-pcc.1 \
	lfs-project.1 lfs-setdirstripe.1 lfs-setstripe.1 \
	lfs-setquota.1 l_getidentity.8 lgss_sk.8 lhbadm.8 \
	llapi_create_volatile_param.3 llapi_fd2parent.3 \
	llapi_file_create.3 llapi_file_create_foreign.3 \
	llapi_file_get_stripe.3 llapi_file_open.3 llapi_group_lock.3 \
	llapi_group_unlock.3 llapi_heat_get.3 llapi_heat_set.3 \
	llapi_hsm_action_begin.3 llapi_hsm_action_end.3 \
	llapi_hsm_action_get_dfid.3 llapi_hsm_action_get_fd.3 \
	llapi_hsm_action_progress.3 llapi_hsm_copytool_get_fd.3 \
	llapi_hsm_copytool_recv.3 llapi_hsm_copytool_register.3 \
	llapi_hsm_copytool_unregister.3 llapi_hsm_state_get.3 \
	llapi_hsm_state_set.3 llapi_ladvise.3 llapi_layout.7 \
	llapi_layout_alloc.3 llapi_layout_comp_add.3 \
	llapi_layout_comp_del.3 llapi_layout_comp_extent_get.3 \
	llapi_layout_comp_extent_set.3 llapi_layout_comp_flags_clear.3 \
	llapi_layout_comp_flags_get.3 llapi_layout_comp_flags_set.3 \
	llapi_layout_comp_id_get.3 llapi_layout_comp_use.3 \
	llapi_layout_comp_use_id.3 llapi_layout_file_comp_add.3 \
	llapi_layout_file_comp_del.3 llapi_layout_file_create.3 \
	llapi_layout_file_open.3 llapi_layout_free.3 \
	llapi_layout_get_by_fd.3 llapi_layout_get_by_fid.3 \
	llapi_layout_get_by_path.3 llapi_layout_get_by_xattr.3 \
	llapi_layout_ost_index_get.3 llapi_layout_ost_index_set.3 \
	llapi_layout_pattern_get.3 llapi_layout_pattern_set.3 \
	llapi_layout_pool_name_get.3 llapi_layout_pool_name_set.3 \
	llapi_layout_stripe_count_get.3 \
	llapi_layout_stripe_count_set.3 llapi_layout_stripe_size_get.3 \
	llapi_layout_stripe_size_set.3 llapi_path2fid.3 \
	llapi_path2parent.3 llapi_quotactl.3 ll_decode_linkea.8 \
	llobdstat.8 llog_reader.8 llsom_sync.8 llstat.8 lnetctl.8 \
	lst.8 lustre.7 lustreapi.7 lustre_routes_config.8 \
	lustre_routes_conversion.8 lustre_rsync.8 mount.lustre.8 \
	mount.lustre_tgt.8 nids.5 plot-llstat.8 routerstat.8 \
	$(am__append_1)
SERVER_MANFILES = \
	lctl-barrier.8				\
	lctl-lcfg.8				\
	lctl-lfsck-query.8			\
	lctl-lfsck-start.8			\
	lctl-lfsck-stop.8			\
	lctl-llog_cancel.8			\
	lctl-llog_catlist.8			\
	lctl-llog_info.8			\
	lctl-llog_print.8			\
	lctl-nodemap-activate.8			\
	lctl-nodemap-add.8			\
	lctl-nodemap-add-idmap.8		\
	lctl-nodemap-add-range.8		\
	lctl-nodemap-del.8			\
	lctl-nodemap-del-idmap.8		\
	lctl-nodemap-del-range.8		\
	lctl-nodemap-modify.8			\
	lctl-nodemap-set-fileset.8		\
	lctl-nodemap-set-sepol.8		\
	lctl-snapshot-create.8			\
	lctl-snapshot-destroy.8			\
	lctl-snapshot-list.8			\
	lctl-snapshot-modify.8			\
	lctl-snapshot-mount.8			\
	lctl-snapshot-umount.8			\
	ll_decode_filter_fid.8			\
	llverdev.8				\
	lshowmount.8				\
	mkfs.lustre.8				\
	tunefs.lustre.8

DISTONLY_MANFILES = $(SERVER_MANFILES)
man_MANS = $(MANFILES)
CLEANFILES = *.aux *.tex *.log *.pdf
EXTRA_DIST = $(MANFILES) $(DISTONLY_MANFILES)
all: all-am

.SUFFIXES:
$(srcdir)/Makefile.in:  $(srcdir)/Makefile.am  $(am__configure_deps)
	@for dep in $?; do \
	  case '$(am__configure_deps)' in \
	    *$$dep*) \
	      ( cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh ) \
	        && { if test -f $@; then exit 0; else break; fi; }; \
	      exit 1;; \
	  esac; \
	done; \
	echo ' cd $(top_srcdir) && $(AUTOMAKE) --gnu lustre/doc/Makefile'; \
	$(am__cd) $(top_srcdir) && \
	  $(AUTOMAKE) --gnu lustre/doc/Makefile
Makefile: $(srcdir)/Makefile.in $(top_builddir)/config.status
	@case '$?' in \
	  *config.status*) \
	    cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh;; \
	  *) \
	    echo ' cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@ $(am__depfiles_maybe)'; \
	    cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@ $(am__depfiles_maybe);; \
	esac;

$(top_builddir)/config.status: $(top_srcdir)/configure $(CONFIG_STATUS_DEPENDENCIES)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh

$(top_srcdir)/configure:  $(am__configure_deps)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh
$(ACLOCAL_M4):  $(am__aclocal_m4_deps)
	cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh
$(am__aclocal_m4_deps):

mostlyclean-libtool:
	-rm -f *.lo

clean-libtool:
	-rm -rf .libs _libs
install-man1: $(man_MANS)
	@$(NORMAL_INSTALL)
	@list1=''; \
	list2='$(man_MANS)'; \
	test -n "$(man1dir)" \
	  && test -n "`echo $$list1$$list2`" \
	  || exit 0; \
	echo " $(MKDIR_P) '$(DESTDIR)$(man1dir)'"; \
	$(MKDIR_P) "$(DESTDIR)$(man1dir)" || exit 1; \
	{ for i in $$list1; do echo "$$i"; done;  \
	if test -n "$$list2"; then \
	  for i in $$list2; do echo "$$i"; done \
	    | sed -n '/\.1[a-z]*$$/p'; \
	fi; \
	} | while read p; do \
	  if test -f $$p; then d=; else d="$(srcdir)/"; fi; \
	  echo "$$d$$p"; echo "$$p"; \
	done | \
	sed -e 'n;s,.*/,,;p;h;s,.*\.,,;s,^[^1][0-9a-z]*$$,1,;x' \
	      -e 's,\.[0-9a-z]*$$,,;$(transform);G;s,\n,.,' | \
	sed 'N;N;s,\n, ,g' | { \
	list=; while read file base inst; do \
	  if test "$$base" = "$$inst"; then list="$$list $$file"; else \
	    echo " $(INSTALL_DATA) '$$file' '$(DESTDIR)$(man1dir)/$$inst'"; \
	    $(INSTALL_DATA) "$$file" "$(DESTDIR)$(man1dir)/$$inst" || exit $$?; \
	  fi; \
	done; \
	for i in $$list; do echo "$$i"; done | $(am__base_list) | \
	while read files; do \
	  test -z "$$files" || { \
	    echo " $(INSTALL_DATA) $$files '$(DESTDIR)$(man1dir)'"; \
	    $(INSTALL_DATA) $$files "$(DESTDIR)$(man1dir)" || exit $$?; }; \
	done; }

uninstall-man1:
	@$(NORMAL_UNINSTALL)
	@list=''; test -n "$(man1dir)" || exit 0; \
	files=`{ for i in $$list; do echo "$$i"; done; \
	l2='$(man_MANS)'; for i in $$l2; do echo "$$i"; done | \
	  sed -n '/\.1[a-z]*$$/p'; \
	} | sed -e 's,.*/,,;h;s,.*\.,,;s,^[^1][0-9a-z]*$$,1,;x' \
	      -e 's,\.[0-9a-z]*$$,,;$(transform);G;s,\n,.,'`; \
	dir='$(DESTDIR)$(man1dir)'; $(am__uninstall_files_from_dir)
install-man3: $(man_MANS)
	@$(NORMAL_INSTALL)
	@list1=''; \
	list2='$(man_MANS)'; \
	test -n "$(man3dir)" \
	  && test -n "`echo $$list1$$list2`" \
	  || exit 0; \
	echo " $(MKDIR_P) '$(DESTDIR)$(man3dir)'"; \
	$(MKDIR_P) "$(DESTDIR)$(man3dir)" || exit 1; \
	{ for i in $$list1; do echo "$$i"; done;  \
	if test -n "$$list2"; then \
	  for i in $$list2; do echo "$$i"; done \
	    | sed -n '/\.3[a-z]*$$/p'; \
	fi; \
	} | while read p; do \
	  if test -f $$p; then d=; else d="$(srcdir)/"; fi; \
	  echo "$$d$$p"; echo "$$p"; \
	done | \
	sed -e 'n;s,.*/,,;p;h;s,.*\.,,;s,^[^3][0-9a-z]*$$,3,;x' \
	      -e 's,\.[0-9a-z]*$$,,;$(transform);G;s,\n,.,' | \
	sed 'N;N;s,\n, ,g' | { \
	list=; while read file base inst; do \
	  if test "$$base" = "$$inst"; then list="$$list $$file"; else \
	    echo " $(INSTALL_DATA) '$$file' '$(DESTDIR)$(man3dir)/$$inst'"; \
	    $(INSTALL_DATA) "$$file" "$(DESTDIR)$(man3dir)/$$inst" || exit $$?; \
	  fi; \
	done; \
	for i in $$list; do echo "$$i"; done | $(am__base_list) | \
	while read files; do \
	  test -z "$$files" || { \
	    echo " $(INSTALL_DATA) $$files '$(DESTDIR)$(man3dir)'"; \
	    $(INSTALL_DATA) $$files "$(DESTDIR)$(man3dir)" || exit $$?; }; \
	done; }

uninstall-man3:
	@$(NORMAL_UNINSTALL)
	@list=''; test -n "$(man3dir)" || exit 0; \
	files=`{ for i in $$list; do echo "$$i"; done; \
	l2='$(man_MANS)'; for i in $$l2; do echo "$$i"; done | \
	  sed -n '/\.3[a-z]*$$/p'; \
	} | sed -e 's,.*/,,;h;s,.*\.,,;s,^[^3][0-9a-z]*$$,3,;x' \
	      -e 's,\.[0-9a-z]*$$,,;$(transform);G;s,\n,.,'`; \
	dir='$(DESTDIR)$(man3dir)'; $(am__uninstall_files_from_dir)
install-man5: $(man_MANS)
	@$(NORMAL_INSTALL)
	@list1=''; \
	list2='$(man_MANS)'; \
	test -n "$(man5dir)" \
	  && test -n "`echo $$list1$$list2`" \
	  || exit 0; \
	echo " $(MKDIR_P) '$(DESTDIR)$(man5dir)'"; \
	$(MKDIR_P) "$(DESTDIR)$(man5dir)" || exit 1; \
	{ for i in $$list1; do echo "$$i"; done;  \
	if test -n "$$list2"; then \
	  for i in $$list2; do echo "$$i"; done \
	    | sed -n '/\.5[a-z]*$$/p'; \
	fi; \
	} | while read p; do \
	  if test -f $$p; then d=; else d="$(srcdir)/"; fi; \
	  echo "$$d$$p"; echo "$$p"; \
	done | \
	sed -e 'n;s,.*/,,;p;h;s,.*\.,,;s,^[^5][0-9a-z]*$$,5,;x' \
	      -e 's,\.[0-9a-z]*$$,,;$(transform);G;s,\n,.,' | \
	sed 'N;N;s,\n, ,g' | { \
	list=; while read file base inst; do \
	  if test "$$base" = "$$inst"; then list="$$list $$file"; else \
	    echo " $(INSTALL_DATA) '$$file' '$(DESTDIR)$(man5dir)/$$inst'"; \
	    $(INSTALL_DATA) "$$file" "$(DESTDIR)$(man5dir)/$$inst" || exit $$?; \
	  fi; \
	done; \
	for i in $$list; do echo "$$i"; done | $(am__base_list) | \
	while read files; do \
	  test -z "$$files" || { \
	    echo " $(INSTALL_DATA) $$files '$(DESTDIR)$(man5dir)'"; \
	    $(INSTALL_DATA) $$files "$(DESTDIR)$(man5dir)" || exit $$?; }; \
	done; }

uninstall-man5:
	@$(NORMAL_UNINSTALL)
	@list=''; test -n "$(man5dir)" || exit 0; \
	files=`{ for i in $$list; do echo "$$i"; done; \
	l2='$(man_MANS)'; for i in $$l2; do echo "$$i"; done | \
	  sed -n '/\.5[a-z]*$$/p'; \
	} | sed -e 's,.*/,,;h;s,.*\.,,;s,^[^5][0-9a-z]*$$,5,;x' \
	      -e 's,\.[0-9a-z]*$$,,;$(transform);G;s,\n,.,'`; \
	dir='$(DESTDIR)$(man5dir)'; $(am__uninstall_files_from_dir)
install-man7: $(man_MANS)
	@$(NORMAL_INSTALL)
	@list1=''; \
	list2='$(man_MANS)'; \
	test -n "$(man7dir)" \
	  && test -n "`echo $$list1$$list2`" \
	  || exit 0; \
	echo " $(MKDIR_P) '$(DESTDIR)$(man7dir)'"; \
	$(MKDIR_P) "$(DESTDIR)$(man7dir)" || exit 1; \
	{ for i in $$list1; do echo "$$i"; done;  \
	if test -n "$$list2"; then \
	  for i in $$list2; do echo "$$i"; done \
	    | sed -n '/\.7[a-z]*$$/p'; \
	fi; \
	} | while read p; do \
	  if test -f $$p; then d=; else d="$(srcdir)/"; fi; \
	  echo "$$d$$p"; echo "$$p"; \
	done | \
	sed -e 'n;s,.*/,,;p;h;s,.*\.,,;s,^[^7][0-9a-z]*$$,7,;x' \
	      -e 's,\.[0-9a-z]*$$,,;$(transform);G;s,\n,.,' | \
	sed 'N;N;s,\n, ,g' | { \
	list=; while read file base inst; do \
	  if test "$$base" = "$$inst"; then list="$$list $$file"; else \
	    echo " $(INSTALL_DATA) '$$file' '$(DESTDIR)$(man7dir)/$$inst'"; \
	    $(INSTALL_DATA) "$$file" "$(DESTDIR)$(man7dir)/$$inst" || exit $$?; \
	  fi; \
	done; \
	for i in $$list; do echo "$$i"; done | $(am__base_list) | \
	while read files; do \
	  test -z "$$files" || { \
	    echo " $(INSTALL_DATA) $$files '$(DESTDIR)$(man7dir)'"; \
	    $(INSTALL_DATA) $$files "$(DESTDIR)$(man7dir)" || exit $$?; }; \
	done; }

uninstall-man7:
	@$(NORMAL_UNINSTALL)
	@list=''; test -n "$(man7dir)" || exit 0; \
	files=`{ for i in $$list; do echo "$$i"; done; \
	l2='$(man_MANS)'; for i in $$l2; do echo "$$i"; done | \
	  sed -n '/\.7[a-z]*$$/p'; \
	} | sed -e 's,.*/,,;h;s,.*\.,,;s,^[^7][0-9a-z]*$$,7,;x' \
	      -e 's,\.[0-9a-z]*$$,,;$(transform);G;s,\n,.,'`; \
	dir='$(DESTDIR)$(man7dir)'; $(am__uninstall_files_from_dir)
install-man8: $(man_MANS)
	@$(NORMAL_INSTALL)
	@list1=''; \
	list2='$(man_MANS)'; \
	test -n "$(man8dir)" \
	  && test -n "`echo $$list1$$list2`" \
	  || exit 0; \
	echo " $(MKDIR_P) '$(DESTDIR)$(man8dir)'"; \
	$(MKDIR_P) "$(DESTDIR)$(man8dir)" || exit 1; \
	{ for i in $$list1; do echo "$$i"; done;  \
	if test -n "$$list2"; then \
	  for i in $$list2; do echo "$$i"; done \
	    | sed -n '/\.8[a-z]*$$/p'; \
	fi; \
	} | while read p; do \
	  if test -f $$p; then d=; else d="$(srcdir)/"; fi; \
	  echo "$$d$$p"; echo "$$p"; \
	done | \
	sed -e 'n;s,.*/,,;p;h;s,.*\.,,;s,^[^8][0-9a-z]*$$,8,;x' \
	      -e 's,\.[0-9a-z]*$$,,;$(transform);G;s,\n,.,' | \
	sed 'N;N;s,\n, ,g' | { \
	list=; while read file base inst; do \
	  if test "$$base" = "$$inst"; then list="$$list $$file"; else \
	    echo " $(INSTALL_DATA) '$$file' '$(DESTDIR)$(man8dir)/$$inst'"; \
	    $(INSTALL_DATA) "$$file" "$(DESTDIR)$(man8dir)/$$inst" || exit $$?; \
	  fi; \
	done; \
	for i in $$list; do echo "$$i"; done | $(am__base_list) | \
	while read files; do \
	  test -z "$$files" || { \
	    echo " $(INSTALL_DATA) $$files '$(DESTDIR)$(man8dir)'"; \
	    $(INSTALL_DATA) $$files "$(DESTDIR)$(man8dir)" || exit $$?; }; \
	done; }

uninstall-man8:
	@$(NORMAL_UNINSTALL)
	@list=''; test -n "$(man8dir)" || exit 0; \
	files=`{ for i in $$list; do echo "$$i"; done; \
	l2='$(man_MANS)'; for i in $$l2; do echo "$$i"; done | \
	  sed -n '/\.8[a-z]*$$/p'; \
	} | sed -e 's,.*/,,;h;s,.*\.,,;s,^[^8][0-9a-z]*$$,8,;x' \
	      -e 's,\.[0-9a-z]*$$,,;$(transform);G;s,\n,.,'`; \
	dir='$(DESTDIR)$(man8dir)'; $(am__uninstall_files_from_dir)
tags TAGS:

ctags CTAGS:

cscope cscopelist:


distdir: $(DISTFILES)
	@srcdirstrip=`echo "$(srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	topsrcdirstrip=`echo "$(top_srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	list='$(DISTFILES)'; \
	  dist_files=`for file in $$list; do echo $$file; done | \
	  sed -e "s|^$$srcdirstrip/||;t" \
	      -e "s|^$$topsrcdirstrip/|$(top_builddir)/|;t"`; \
	case $$dist_files in \
	  */*) $(MKDIR_P) `echo "$$dist_files" | \
			   sed '/\//!d;s|^|$(distdir)/|;s,/[^/]*$$,,' | \
			   sort -u` ;; \
	esac; \
	for file in $$dist_files; do \
	  if test -f $$file || test -d $$file; then d=.; else d=$(srcdir); fi; \
	  if test -d $$d/$$file; then \
	    dir=`echo "/$$file" | sed -e 's,/[^/]*$$,,'`; \
	    if test -d "$(distdir)/$$file"; then \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    if test -d $(srcdir)/$$file && test $$d != $(srcdir); then \
	      cp -fpR $(srcdir)/$$file "$(distdir)$$dir" || exit 1; \
	      find "$(distdir)/$$file" -type d ! -perm -700 -exec chmod u+rwx {} \;; \
	    fi; \
	    cp -fpR $$d/$$file "$(distdir)$$dir" || exit 1; \
	  else \
	    test -f "$(distdir)/$$file" \
	    || cp -p $$d/$$file "$(distdir)/$$file" \
	    || exit 1; \
	  fi; \
	done
check-am: all-am
check: check-am
all-am: Makefile $(MANS)
installdirs:
	for dir in "$(DESTDIR)$(man1dir)" "$(DESTDIR)$(man3dir)" "$(DESTDIR)$(man5dir)" "$(DESTDIR)$(man7dir)" "$(DESTDIR)$(man8dir)"; do \
	  test -z "$$dir" || $(MKDIR_P) "$$dir"; \
	done
install: install-am
install-exec: install-exec-am
install-data: install-data-am
uninstall: uninstall-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

installcheck: installcheck-am
install-strip:
	if test -z '$(STRIP)'; then \
	  $(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	    install_sh_PROGRAM="$(INSTALL_STRIP_PROGRAM)" INSTALL_STRIP_FLAG=-s \
	      install; \
	else \
	  $(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	    install_sh_PROGRAM="$(INSTALL_STRIP_PROGRAM)" INSTALL_STRIP_FLAG=-s \
	    "INSTALL_PROGRAM_ENV=STRIPPROG='$(STRIP)'" install; \
	fi
mostlyclean-generic:
	-test -z "$(MOSTLYCLEANFILES)" || rm -f $(MOSTLYCLEANFILES)

clean-generic:
	-test -z "$(CLEANFILES)" || rm -f $(CLEANFILES)

distclean-generic:
	-test -z "$(CONFIG_CLEAN_FILES)" || rm -f $(CONFIG_CLEAN_FILES)
	-test . = "$(srcdir)" || test -z "$(CONFIG_CLEAN_VPATH_FILES)" || rm -f $(CONFIG_CLEAN_VPATH_FILES)

maintainer-clean-generic:
	@echo "This command is intended for maintainers to use"
	@echo "it deletes files that may require special tools to rebuild."
clean: clean-am

clean-am: clean-generic clean-libtool mostlyclean-am

distclean: distclean-am
	-rm -f Makefile
distclean-am: clean-am distclean-generic

dvi: dvi-am

dvi-am:

html: html-am

html-am:

info: info-am

info-am:

install-data-am: install-man

install-dvi: install-dvi-am

install-dvi-am:

install-exec-am:

install-html: install-html-am

install-html-am:

install-info: install-info-am

install-info-am:

install-man: install-man1 install-man3 install-man5 install-man7 \
	install-man8

install-pdf: install-pdf-am

install-pdf-am:

install-ps: install-ps-am

install-ps-am:

installcheck-am:

maintainer-clean: maintainer-clean-am
	-rm -f Makefile
maintainer-clean-am: distclean-am maintainer-clean-generic

mostlyclean: mostlyclean-am

mostlyclean-am: mostlyclean-generic mostlyclean-libtool

pdf: pdf-am

pdf-am:

ps: ps-am

ps-am:

uninstall-am: uninstall-man

uninstall-man: uninstall-man1 uninstall-man3 uninstall-man5 \
	uninstall-man7 uninstall-man8

.MAKE: install-am install-strip

.PHONY: all all-am check check-am clean clean-generic clean-libtool \
	cscopelist-am ctags-am distclean distclean-generic \
	distclean-libtool distdir dvi dvi-am html html-am info info-am \
	install install-am install-data install-data-am install-dvi \
	install-dvi-am install-exec install-exec-am install-html \
	install-html-am install-info install-info-am install-man \
	install-man1 install-man3 install-man5 install-man7 \
	install-man8 install-pdf install-pdf-am install-ps \
	install-ps-am install-strip installcheck installcheck-am \
	installdirs maintainer-clean maintainer-clean-generic \
	mostlyclean mostlyclean-generic mostlyclean-libtool pdf pdf-am \
	ps ps-am tags-am uninstall uninstall-am uninstall-man \
	uninstall-man1 uninstall-man3 uninstall-man5 uninstall-man7 \
	uninstall-man8

.PRECIOUS: Makefile


all:

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
