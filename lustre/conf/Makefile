# Makefile.in generated by automake 1.15 from Makefile.am.
# lustre/conf/Makefile.  Generated from Makefile.in by configure.

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
# Copyright (c) 2003, 2010, Oracle and/or its affiliates. All rights reserved.
# Use is subject to license terms.
#
# Copyright (c) 2014, 2015, Intel Corporation.
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
am__append_1 = lnet_routes.conf
#am__append_2 = 99-lustre-server.rules
##am__append_3 = ldev.conf
###am__append_4 = lsvcgss
subdir = lustre/conf
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
RECURSIVE_TARGETS = all-recursive check-recursive cscopelist-recursive \
	ctags-recursive dvi-recursive html-recursive info-recursive \
	install-data-recursive install-dvi-recursive \
	install-exec-recursive install-html-recursive \
	install-info-recursive install-pdf-recursive \
	install-ps-recursive install-recursive installcheck-recursive \
	installdirs-recursive pdf-recursive ps-recursive \
	tags-recursive uninstall-recursive
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
am__installdirs = "$(DESTDIR)$(modprobedir)" \
	"$(DESTDIR)$(requestkeydir)" "$(DESTDIR)$(sysconfdir)" \
	"$(DESTDIR)$(sysconfigdir)" "$(DESTDIR)$(udevrulesdir)"
DATA = $(modprobe_DATA) $(noinst_DATA) $(requestkey_DATA) \
	$(sysconf_DATA) $(sysconfig_DATA) $(udevrules_DATA)
RECURSIVE_CLEAN_TARGETS = mostlyclean-recursive clean-recursive	\
  distclean-recursive maintainer-clean-recursive
am__recursive_targets = \
  $(RECURSIVE_TARGETS) \
  $(RECURSIVE_CLEAN_TARGETS) \
  $(am__extra_recursive_targets)
AM_RECURSIVE_TARGETS = $(am__recursive_targets:-recursive=) TAGS CTAGS \
	distdir
am__tagged_files = $(HEADERS) $(SOURCES) $(TAGS_FILES) $(LISP)
# Read a list of newline-separated strings from the standard input,
# and print each of them once, without duplicates.  Input order is
# *not* preserved.
am__uniquify_input = $(AWK) '\
  BEGIN { nonempty = 0; } \
  { items[$$0] = 1; nonempty = 1; } \
  END { if (nonempty) { for (i in items) print i; }; } \
'
# Make sure the list of sources is unique.  This is necessary because,
# e.g., the same source file might be shared among _SOURCES variables
# for different programs/libraries.
am__define_uniq_tagged_files = \
  list='$(am__tagged_files)'; \
  unique=`for i in $$list; do \
    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
  done | $(am__uniquify_input)`
ETAGS = etags
CTAGS = ctags
am__DIST_COMMON = $(srcdir)/Makefile.in
DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)
am__relativize = \
  dir0=`pwd`; \
  sed_first='s,^\([^/]*\)/.*$$,\1,'; \
  sed_rest='s,^[^/]*/*,,'; \
  sed_last='s,^.*/\([^/]*\)$$,\1,'; \
  sed_butlast='s,/*[^/]*$$,,'; \
  while test -n "$$dir1"; do \
    first=`echo "$$dir1" | sed -e "$$sed_first"`; \
    if test "$$first" != "."; then \
      if test "$$first" = ".."; then \
        dir2=`echo "$$dir0" | sed -e "$$sed_last"`/"$$dir2"; \
        dir0=`echo "$$dir0" | sed -e "$$sed_butlast"`; \
      else \
        first2=`echo "$$dir2" | sed -e "$$sed_first"`; \
        if test "$$first2" = "$$first"; then \
          dir2=`echo "$$dir2" | sed -e "$$sed_rest"`; \
        else \
          dir2="../$$dir2"; \
        fi; \
        dir0="$$dir0"/"$$first"; \
      fi; \
    fi; \
    dir1=`echo "$$dir1" | sed -e "$$sed_rest"`; \
  done; \
  reldir="$$dir2"
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
abs_builddir = /Users/crb002/github/lustre-release/lustre/conf
abs_srcdir = /Users/crb002/github/lustre-release/lustre/conf
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
SUBDIRS = resource
DIST_SUBDIRS = resource
EXTRA_DIST = lustre.dtd lustre2ldif.xsl top.ldif \
             99-lustre.rules lustre ldev.conf ko2iblnd.conf lsvcgss \
             lnet_routes.conf lnet.conf lgssc.conf 99-lustre-server.rules

sysconf_DATA = lnet.conf $(am__append_1) $(am__append_3)
noinst_DATA = top.ldif lustre2ldif.xsl
udevrulesdir = $(sysconfdir)/udev/rules.d
udevrules_DATA = 99-lustre.rules $(am__append_2)
##sysconfigdir = $(sysconfdir)/sysconfig
##sysconfig_DATA = lustre $(am__append_4)
#requestkeydir = $(sysconfdir)/request-key.d
#requestkey_DATA = lgssc.conf
modprobedir = $(sysconfdir)/modprobe.d
modprobe_DATA = ko2iblnd.conf
all: all-recursive

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
	echo ' cd $(top_srcdir) && $(AUTOMAKE) --gnu lustre/conf/Makefile'; \
	$(am__cd) $(top_srcdir) && \
	  $(AUTOMAKE) --gnu lustre/conf/Makefile
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
install-modprobeDATA: $(modprobe_DATA)
	@$(NORMAL_INSTALL)
	@list='$(modprobe_DATA)'; test -n "$(modprobedir)" || list=; \
	if test -n "$$list"; then \
	  echo " $(MKDIR_P) '$(DESTDIR)$(modprobedir)'"; \
	  $(MKDIR_P) "$(DESTDIR)$(modprobedir)" || exit 1; \
	fi; \
	for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  echo "$$d$$p"; \
	done | $(am__base_list) | \
	while read files; do \
	  echo " $(INSTALL_DATA) $$files '$(DESTDIR)$(modprobedir)'"; \
	  $(INSTALL_DATA) $$files "$(DESTDIR)$(modprobedir)" || exit $$?; \
	done

uninstall-modprobeDATA:
	@$(NORMAL_UNINSTALL)
	@list='$(modprobe_DATA)'; test -n "$(modprobedir)" || list=; \
	files=`for p in $$list; do echo $$p; done | sed -e 's|^.*/||'`; \
	dir='$(DESTDIR)$(modprobedir)'; $(am__uninstall_files_from_dir)
install-requestkeyDATA: $(requestkey_DATA)
	@$(NORMAL_INSTALL)
	@list='$(requestkey_DATA)'; test -n "$(requestkeydir)" || list=; \
	if test -n "$$list"; then \
	  echo " $(MKDIR_P) '$(DESTDIR)$(requestkeydir)'"; \
	  $(MKDIR_P) "$(DESTDIR)$(requestkeydir)" || exit 1; \
	fi; \
	for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  echo "$$d$$p"; \
	done | $(am__base_list) | \
	while read files; do \
	  echo " $(INSTALL_DATA) $$files '$(DESTDIR)$(requestkeydir)'"; \
	  $(INSTALL_DATA) $$files "$(DESTDIR)$(requestkeydir)" || exit $$?; \
	done

uninstall-requestkeyDATA:
	@$(NORMAL_UNINSTALL)
	@list='$(requestkey_DATA)'; test -n "$(requestkeydir)" || list=; \
	files=`for p in $$list; do echo $$p; done | sed -e 's|^.*/||'`; \
	dir='$(DESTDIR)$(requestkeydir)'; $(am__uninstall_files_from_dir)
install-sysconfDATA: $(sysconf_DATA)
	@$(NORMAL_INSTALL)
	@list='$(sysconf_DATA)'; test -n "$(sysconfdir)" || list=; \
	if test -n "$$list"; then \
	  echo " $(MKDIR_P) '$(DESTDIR)$(sysconfdir)'"; \
	  $(MKDIR_P) "$(DESTDIR)$(sysconfdir)" || exit 1; \
	fi; \
	for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  echo "$$d$$p"; \
	done | $(am__base_list) | \
	while read files; do \
	  echo " $(INSTALL_DATA) $$files '$(DESTDIR)$(sysconfdir)'"; \
	  $(INSTALL_DATA) $$files "$(DESTDIR)$(sysconfdir)" || exit $$?; \
	done

uninstall-sysconfDATA:
	@$(NORMAL_UNINSTALL)
	@list='$(sysconf_DATA)'; test -n "$(sysconfdir)" || list=; \
	files=`for p in $$list; do echo $$p; done | sed -e 's|^.*/||'`; \
	dir='$(DESTDIR)$(sysconfdir)'; $(am__uninstall_files_from_dir)
install-sysconfigDATA: $(sysconfig_DATA)
	@$(NORMAL_INSTALL)
	@list='$(sysconfig_DATA)'; test -n "$(sysconfigdir)" || list=; \
	if test -n "$$list"; then \
	  echo " $(MKDIR_P) '$(DESTDIR)$(sysconfigdir)'"; \
	  $(MKDIR_P) "$(DESTDIR)$(sysconfigdir)" || exit 1; \
	fi; \
	for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  echo "$$d$$p"; \
	done | $(am__base_list) | \
	while read files; do \
	  echo " $(INSTALL_DATA) $$files '$(DESTDIR)$(sysconfigdir)'"; \
	  $(INSTALL_DATA) $$files "$(DESTDIR)$(sysconfigdir)" || exit $$?; \
	done

uninstall-sysconfigDATA:
	@$(NORMAL_UNINSTALL)
	@list='$(sysconfig_DATA)'; test -n "$(sysconfigdir)" || list=; \
	files=`for p in $$list; do echo $$p; done | sed -e 's|^.*/||'`; \
	dir='$(DESTDIR)$(sysconfigdir)'; $(am__uninstall_files_from_dir)
install-udevrulesDATA: $(udevrules_DATA)
	@$(NORMAL_INSTALL)
	@list='$(udevrules_DATA)'; test -n "$(udevrulesdir)" || list=; \
	if test -n "$$list"; then \
	  echo " $(MKDIR_P) '$(DESTDIR)$(udevrulesdir)'"; \
	  $(MKDIR_P) "$(DESTDIR)$(udevrulesdir)" || exit 1; \
	fi; \
	for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  echo "$$d$$p"; \
	done | $(am__base_list) | \
	while read files; do \
	  echo " $(INSTALL_DATA) $$files '$(DESTDIR)$(udevrulesdir)'"; \
	  $(INSTALL_DATA) $$files "$(DESTDIR)$(udevrulesdir)" || exit $$?; \
	done

uninstall-udevrulesDATA:
	@$(NORMAL_UNINSTALL)
	@list='$(udevrules_DATA)'; test -n "$(udevrulesdir)" || list=; \
	files=`for p in $$list; do echo $$p; done | sed -e 's|^.*/||'`; \
	dir='$(DESTDIR)$(udevrulesdir)'; $(am__uninstall_files_from_dir)

# This directory's subdirectories are mostly independent; you can cd
# into them and run 'make' without going through this Makefile.
# To change the values of 'make' variables: instead of editing Makefiles,
# (1) if the variable is set in 'config.status', edit 'config.status'
#     (which will cause the Makefiles to be regenerated when you run 'make');
# (2) otherwise, pass the desired values on the 'make' command line.
$(am__recursive_targets):
	@fail=; \
	if $(am__make_keepgoing); then \
	  failcom='fail=yes'; \
	else \
	  failcom='exit 1'; \
	fi; \
	dot_seen=no; \
	target=`echo $@ | sed s/-recursive//`; \
	case "$@" in \
	  distclean-* | maintainer-clean-*) list='$(DIST_SUBDIRS)' ;; \
	  *) list='$(SUBDIRS)' ;; \
	esac; \
	for subdir in $$list; do \
	  echo "Making $$target in $$subdir"; \
	  if test "$$subdir" = "."; then \
	    dot_seen=yes; \
	    local_target="$$target-am"; \
	  else \
	    local_target="$$target"; \
	  fi; \
	  ($(am__cd) $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
	  || eval $$failcom; \
	done; \
	if test "$$dot_seen" = "no"; then \
	  $(MAKE) $(AM_MAKEFLAGS) "$$target-am" || exit 1; \
	fi; test -z "$$fail"

ID: $(am__tagged_files)
	$(am__define_uniq_tagged_files); mkid -fID $$unique
tags: tags-recursive
TAGS: tags

tags-am: $(TAGS_DEPENDENCIES) $(am__tagged_files)
	set x; \
	here=`pwd`; \
	if ($(ETAGS) --etags-include --version) >/dev/null 2>&1; then \
	  include_option=--etags-include; \
	  empty_fix=.; \
	else \
	  include_option=--include; \
	  empty_fix=; \
	fi; \
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  if test "$$subdir" = .; then :; else \
	    test ! -f $$subdir/TAGS || \
	      set "$$@" "$$include_option=$$here/$$subdir/TAGS"; \
	  fi; \
	done; \
	$(am__define_uniq_tagged_files); \
	shift; \
	if test -z "$(ETAGS_ARGS)$$*$$unique"; then :; else \
	  test -n "$$unique" || unique=$$empty_fix; \
	  if test $$# -gt 0; then \
	    $(ETAGS) $(ETAGSFLAGS) $(AM_ETAGSFLAGS) $(ETAGS_ARGS) \
	      "$$@" $$unique; \
	  else \
	    $(ETAGS) $(ETAGSFLAGS) $(AM_ETAGSFLAGS) $(ETAGS_ARGS) \
	      $$unique; \
	  fi; \
	fi
ctags: ctags-recursive

CTAGS: ctags
ctags-am: $(TAGS_DEPENDENCIES) $(am__tagged_files)
	$(am__define_uniq_tagged_files); \
	test -z "$(CTAGS_ARGS)$$unique" \
	  || $(CTAGS) $(CTAGSFLAGS) $(AM_CTAGSFLAGS) $(CTAGS_ARGS) \
	     $$unique

GTAGS:
	here=`$(am__cd) $(top_builddir) && pwd` \
	  && $(am__cd) $(top_srcdir) \
	  && gtags -i $(GTAGS_ARGS) "$$here"
cscopelist: cscopelist-recursive

cscopelist-am: $(am__tagged_files)
	list='$(am__tagged_files)'; \
	case "$(srcdir)" in \
	  [\\/]* | ?:[\\/]*) sdir="$(srcdir)" ;; \
	  *) sdir=$(subdir)/$(srcdir) ;; \
	esac; \
	for i in $$list; do \
	  if test -f "$$i"; then \
	    echo "$(subdir)/$$i"; \
	  else \
	    echo "$$sdir/$$i"; \
	  fi; \
	done >> $(top_builddir)/cscope.files

distclean-tags:
	-rm -f TAGS ID GTAGS GRTAGS GSYMS GPATH tags

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
	@list='$(DIST_SUBDIRS)'; for subdir in $$list; do \
	  if test "$$subdir" = .; then :; else \
	    $(am__make_dryrun) \
	      || test -d "$(distdir)/$$subdir" \
	      || $(MKDIR_P) "$(distdir)/$$subdir" \
	      || exit 1; \
	    dir1=$$subdir; dir2="$(distdir)/$$subdir"; \
	    $(am__relativize); \
	    new_distdir=$$reldir; \
	    dir1=$$subdir; dir2="$(top_distdir)"; \
	    $(am__relativize); \
	    new_top_distdir=$$reldir; \
	    echo " (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) top_distdir="$$new_top_distdir" distdir="$$new_distdir" \\"; \
	    echo "     am__remove_distdir=: am__skip_length_check=: am__skip_mode_fix=: distdir)"; \
	    ($(am__cd) $$subdir && \
	      $(MAKE) $(AM_MAKEFLAGS) \
	        top_distdir="$$new_top_distdir" \
	        distdir="$$new_distdir" \
		am__remove_distdir=: \
		am__skip_length_check=: \
		am__skip_mode_fix=: \
	        distdir) \
	      || exit 1; \
	  fi; \
	done
check-am: all-am
check: check-recursive
all-am: Makefile $(DATA)
installdirs: installdirs-recursive
installdirs-am:
	for dir in "$(DESTDIR)$(modprobedir)" "$(DESTDIR)$(requestkeydir)" "$(DESTDIR)$(sysconfdir)" "$(DESTDIR)$(sysconfigdir)" "$(DESTDIR)$(udevrulesdir)"; do \
	  test -z "$$dir" || $(MKDIR_P) "$$dir"; \
	done
install: install-recursive
install-exec: install-exec-recursive
install-data: install-data-recursive
uninstall: uninstall-recursive

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

installcheck: installcheck-recursive
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

distclean-generic:
	-test -z "$(CONFIG_CLEAN_FILES)" || rm -f $(CONFIG_CLEAN_FILES)
	-test . = "$(srcdir)" || test -z "$(CONFIG_CLEAN_VPATH_FILES)" || rm -f $(CONFIG_CLEAN_VPATH_FILES)

maintainer-clean-generic:
	@echo "This command is intended for maintainers to use"
	@echo "it deletes files that may require special tools to rebuild."
clean: clean-recursive

clean-am: clean-generic clean-libtool mostlyclean-am

distclean: distclean-recursive
	-rm -f Makefile
distclean-am: clean-am distclean-generic distclean-tags

dvi: dvi-recursive

dvi-am:

html: html-recursive

html-am:

info: info-recursive

info-am:

install-data-am: install-modprobeDATA install-requestkeyDATA \
	install-sysconfigDATA install-udevrulesDATA

install-dvi: install-dvi-recursive

install-dvi-am:

install-exec-am: install-sysconfDATA

install-html: install-html-recursive

install-html-am:

install-info: install-info-recursive

install-info-am:

install-man:

install-pdf: install-pdf-recursive

install-pdf-am:

install-ps: install-ps-recursive

install-ps-am:

installcheck-am:

maintainer-clean: maintainer-clean-recursive
	-rm -f Makefile
maintainer-clean-am: distclean-am maintainer-clean-generic

mostlyclean: mostlyclean-recursive

mostlyclean-am: mostlyclean-generic mostlyclean-libtool

pdf: pdf-recursive

pdf-am:

ps: ps-recursive

ps-am:

uninstall-am: uninstall-modprobeDATA uninstall-requestkeyDATA \
	uninstall-sysconfDATA uninstall-sysconfigDATA \
	uninstall-udevrulesDATA

.MAKE: $(am__recursive_targets) install-am install-strip

.PHONY: $(am__recursive_targets) CTAGS GTAGS TAGS all all-am check \
	check-am clean clean-generic clean-libtool cscopelist-am ctags \
	ctags-am distclean distclean-generic distclean-libtool \
	distclean-tags distdir dvi dvi-am html html-am info info-am \
	install install-am install-data install-data-am install-dvi \
	install-dvi-am install-exec install-exec-am install-html \
	install-html-am install-info install-info-am install-man \
	install-modprobeDATA install-pdf install-pdf-am install-ps \
	install-ps-am install-requestkeyDATA install-strip \
	install-sysconfDATA install-sysconfigDATA \
	install-udevrulesDATA installcheck installcheck-am installdirs \
	installdirs-am maintainer-clean maintainer-clean-generic \
	mostlyclean mostlyclean-generic mostlyclean-libtool pdf pdf-am \
	ps ps-am tags tags-am uninstall uninstall-am \
	uninstall-modprobeDATA uninstall-requestkeyDATA \
	uninstall-sysconfDATA uninstall-sysconfigDATA \
	uninstall-udevrulesDATA

.PRECIOUS: Makefile


# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
