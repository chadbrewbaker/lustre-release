%bcond_without servers
%bcond_without zfs
%bcond_with ldiskfs

# Set the package name prefix
%if %{with servers}
    %if %{with zfs}
	%if %{with ldiskfs}
	    %define module lustre-all
	%else
	    %define module lustre-zfs
	%endif
    %else
	%if %{without ldiskfs}
	    %define module lustre-BADSTATE
	%else
	    %define module lustre-ldiskfs
	%endif
    %endif
    %define lustre_name lustre

%else
    %define module lustre-client
    %define lustre_name lustre-client
%endif

%if %{_vendor}=="redhat" || %{_vendor}=="fedora"
	%global kmod_name kmod-%{lustre_name}
	%define mkconf_options %{nil}
%else	#for Suse / Ubuntu
	%global kmod_name %{lustre_name}-kmp
	%define mkconf_options "-k updates"
%endif

%define buildid 1
%define mkconf  lustre/scripts/dkms.mkconf

# There should be a better (non-arch dependent) way to require ext4
# sources
%define ext4_source_rpm kernel-debuginfo-common-x86_64

Name:           %{module}-dkms

Version:        2.12.58_104_g279c264_dirty
Release:        %{buildid}%{?dist}
Summary:        Kernel module(s) (dkms)

Group:          System Environment/Kernel
License:        GPLv2+
URL:            http://lustre.opensfs.org/
Source0:        lustre-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildArch:      noarch

# DKMS >= 2.2.0.3-28.git.7c3e7c5 to fully support inter-modules deps
# (ie, "BUILD_DEPENDS[#]=<pkg>"), and have latest DKMS fixes integrated
# for bugs that prevented our module to build/install.
Requires:       dkms >= 2.2.0.3-28.git.7c3e7c5
# for lnetctl
Requires:	libyaml-devel
Requires:	zlib-devel
%if %{with servers}
# If client package is installed when installing server, remove it since
# the server package also includes the client.  This can be removed if/when
# the packages are split into independent client/server/common packages.
Obsoletes:	lustre-client < %{version}
%if %{with zfs}
Requires:       zfs-dkms >= 0.6.5
Requires:	lustre-osd-zfs-mount
Conflicts:	lustre-ldiskfs-dkms
Conflicts:	lustre-client-dkms
# lustre-zfs-dkms replicates the functionality old lustre-dkms package
Provides:	lustre-dkms
Obsoletes:	lustre-dkms
%endif
%if %{with ldiskfs}
Requires:	patch
Requires:	%{ext4_source_rpm}
Requires:	lustre-osd-ldiskfs-mount
Conflicts:	lustre-zfs-dkms
Conflicts:	lustre-client-dkms
%if "%{module}" != "lustre-all"
Conflicts:	lustre-dkms
%endif
%endif
%if "%{module}" != "lustre-all"
Conflicts:	lustre-all-dkms
%endif
%endif
Requires:       gcc, make, perl
Requires:       kernel-devel
Provides:	%{kmod_name} = %{version}
Provides:	lustre-modules = %{version}
%if %{with servers}
%if %{with zfs}
Provides:	lustre-osd-zfs = %{version}
%endif
%if %{with ldiskfs}
Provides:	lustre-osd-ldiskfs = %{version}
%endif
Provides:	lustre-osd
%else
Provides:	lustre-client
%endif

%description
This package contains the dkms Lustre kernel modules.
%if %{with ldiskfs}

The required %{ext4_source_rpm} package is available from
the repository with other debuginfo rpms.
%endif

%prep
%setup -q -n lustre-%{version}

%build
%{mkconf} -n %{module} -v %{version} -f dkms.conf %{mkconf_options}

%install
if [ "$RPM_BUILD_ROOT" != "/" ]; then
    rm -rf $RPM_BUILD_ROOT
fi
mkdir -p $RPM_BUILD_ROOT/usr/src/
cp -rfp ${RPM_BUILD_DIR}/lustre-%{version} $RPM_BUILD_ROOT/usr/src/
mv $RPM_BUILD_ROOT/usr/src/lustre-%{version} $RPM_BUILD_ROOT/usr/src/%{module}-%{version}

%clean
if [ "$RPM_BUILD_ROOT" != "/" ]; then
    rm -rf $RPM_BUILD_ROOT
fi

%files
%defattr(-,root,root)
/usr/src/%{module}-%{version}

%post
for POSTINST in /usr/lib/dkms/common.postinst; do
    if [ -f $POSTINST ]; then
        $POSTINST %{module} %{version}
        exit $?
    fi
    echo "WARNING: $POSTINST does not exist."
done
echo -e "ERROR: DKMS version is too old and %{module} was not"
echo -e "built with legacy DKMS support."
echo -e "You must either rebuild %{module} with legacy postinst"
echo -e "support or upgrade DKMS to a more current version."
exit 1

%preun
dkms remove -m %{module} -v %{version} --all --rpm_safe_upgrade
exit 0

%changelog
* Wed May 16 2018 Joe Grund <joe.grund@intel.com>
- Add patch requirement
* Mon Aug  1 2016 Nathaniel Clark <nathaniel.l.clark@intel.com>
- Add option to build either ldiskfs or zfs flavour of server version
* Sat Jan 23 2016 Bruno Faccini <bruno.faccini@intel.com>
 - detect and handle cases where [spl,zfs]-dkms packages are not built
 - also handle on-target configure issues
* Wed Oct  7 2015 Bruno Faccini <bruno.faccini@intel.com>
 - adapted for Lustre Client DKMS creation
 - allow for on-target reconfig to prevent static deps requires
* Mon Apr  8 2013 Brian Behlendorf <behlendorf1@llnl.gov> - 2.3.63-1
- First DKMS packages.
