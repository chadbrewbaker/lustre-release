/*
 * GPL HEADER START
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 only,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License version 2 for more details (a copy is included
 * in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with this program; If not, see
 * http://www.gnu.org/licenses/gpl-2.0.txt
 *
 * GPL HEADER END
 */
/*
 * Copyright (C) 2011 FUJITSU LIMITED.  All rights reserved.
 *
 * Copyright (c) 2013, Intel Corporation.
 */

#include <libcfs/libcfs.h>
#include <lustre_errno.h>

#ifdef LUSTRE_TRANSLATE_ERRNOS
#include <lustre_dlm.h>

/*
 * The two translation tables below must define a one-to-one mapping between
 * host and network errnos.
 *
 * EWOULDBLOCK is equal to EAGAIN on all architectures except for parisc, which
 * appears irrelevant.  Thus, existing references to EWOULDBLOCK are fine.
 *
 * EDEADLOCK is equal to EDEADLK on x86 but not on sparc, at least.  A sparc
 * host has no context-free way to determine if a LUSTRE_EDEADLK represents an
 * EDEADLK or an EDEADLOCK.  Therefore, all existing references to EDEADLOCK
 * that need to be transferred on wire have been replaced with EDEADLK.
 */
static int lustre_errno_hton_mapping[] = {
	[EPERM]			= LUSTRE_EPERM,
	[ENOENT]		= LUSTRE_ENOENT,
	[ESRCH]			= LUSTRE_ESRCH,
	[EINTR]			= LUSTRE_EINTR,
	[EIO]			= LUSTRE_EIO,
	[ENXIO]			= LUSTRE_ENXIO,
	[E2BIG]			= LUSTRE_E2BIG,
	[ENOEXEC]		= LUSTRE_ENOEXEC,
	[EBADF]			= LUSTRE_EBADF,
	[ECHILD]		= LUSTRE_ECHILD,
	[EAGAIN]		= LUSTRE_EAGAIN,
	[ENOMEM]		= LUSTRE_ENOMEM,
	[EACCES]		= LUSTRE_EACCES,
	[EFAULT]		= LUSTRE_EFAULT,
	[ENOTBLK]		= LUSTRE_ENOTBLK,
	[EBUSY]			= LUSTRE_EBUSY,
	[EEXIST]		= LUSTRE_EEXIST,
	[EXDEV]			= LUSTRE_EXDEV,
	[ENODEV]		= LUSTRE_ENODEV,
	[ENOTDIR]		= LUSTRE_ENOTDIR,
	[EISDIR]		= LUSTRE_EISDIR,
	[EINVAL]		= LUSTRE_EINVAL,
	[ENFILE]		= LUSTRE_ENFILE,
	[EMFILE]		= LUSTRE_EMFILE,
	[ENOTTY]		= LUSTRE_ENOTTY,
	[ETXTBSY]		= LUSTRE_ETXTBSY,
	[EFBIG]			= LUSTRE_EFBIG,
	[ENOSPC]		= LUSTRE_ENOSPC,
	[ESPIPE]		= LUSTRE_ESPIPE,
	[EROFS]			= LUSTRE_EROFS,
	[EMLINK]		= LUSTRE_EMLINK,
	[EPIPE]			= LUSTRE_EPIPE,
	[EDOM]			= LUSTRE_EDOM,
	[ERANGE]		= LUSTRE_ERANGE,
	[EDEADLK]		= LUSTRE_EDEADLK,
	[ENAMETOOLONG]		= LUSTRE_ENAMETOOLONG,
	[ENOLCK]		= LUSTRE_ENOLCK,
	[ENOSYS]		= LUSTRE_ENOSYS,
	[ENOTEMPTY]		= LUSTRE_ENOTEMPTY,
	[ELOOP]			= LUSTRE_ELOOP,
	[ENOMSG]		= LUSTRE_ENOMSG,
	[EIDRM]			= LUSTRE_EIDRM,
	[ECHRNG]		= LUSTRE_ECHRNG,
	[EL2NSYNC]		= LUSTRE_EL2NSYNC,
	[EL3HLT]		= LUSTRE_EL3HLT,
	[EL3RST]		= LUSTRE_EL3RST,
	[ELNRNG]		= LUSTRE_ELNRNG,
	[EUNATCH]		= LUSTRE_EUNATCH,
	[ENOCSI]		= LUSTRE_ENOCSI,
	[EL2HLT]		= LUSTRE_EL2HLT,
	[EBADE]			= LUSTRE_EBADE,
	[EBADR]			= LUSTRE_EBADR,
	[EXFULL]		= LUSTRE_EXFULL,
	[ENOANO]		= LUSTRE_ENOANO,
	[EBADRQC]		= LUSTRE_EBADRQC,
	[EBADSLT]		= LUSTRE_EBADSLT,
	[EBFONT]		= LUSTRE_EBFONT,
	[ENOSTR]		= LUSTRE_ENOSTR,
	[ENODATA]		= LUSTRE_ENODATA,
	[ETIME]			= LUSTRE_ETIME,
	[ENOSR]			= LUSTRE_ENOSR,
	[ENONET]		= LUSTRE_ENONET,
	[ENOPKG]		= LUSTRE_ENOPKG,
	[EREMOTE]		= LUSTRE_EREMOTE,
	[ENOLINK]		= LUSTRE_ENOLINK,
	[EADV]			= LUSTRE_EADV,
	[ESRMNT]		= LUSTRE_ESRMNT,
	[ECOMM]			= LUSTRE_ECOMM,
	[EPROTO]		= LUSTRE_EPROTO,
	[EMULTIHOP]		= LUSTRE_EMULTIHOP,
	[EDOTDOT]		= LUSTRE_EDOTDOT,
	[EBADMSG]		= LUSTRE_EBADMSG,
	[EOVERFLOW]		= LUSTRE_EOVERFLOW,
	[ENOTUNIQ]		= LUSTRE_ENOTUNIQ,
	[EBADFD]		= LUSTRE_EBADFD,
	[EREMCHG]		= LUSTRE_EREMCHG,
	[ELIBACC]		= LUSTRE_ELIBACC,
	[ELIBBAD]		= LUSTRE_ELIBBAD,
	[ELIBSCN]		= LUSTRE_ELIBSCN,
	[ELIBMAX]		= LUSTRE_ELIBMAX,
	[ELIBEXEC]		= LUSTRE_ELIBEXEC,
	[EILSEQ]		= LUSTRE_EILSEQ,
	[ERESTART]		= LUSTRE_ERESTART,
	[ESTRPIPE]		= LUSTRE_ESTRPIPE,
	[EUSERS]		= LUSTRE_EUSERS,
	[ENOTSOCK]		= LUSTRE_ENOTSOCK,
	[EDESTADDRREQ]		= LUSTRE_EDESTADDRREQ,
	[EMSGSIZE]		= LUSTRE_EMSGSIZE,
	[EPROTOTYPE]		= LUSTRE_EPROTOTYPE,
	[ENOPROTOOPT]		= LUSTRE_ENOPROTOOPT,
	[EPROTONOSUPPORT]	= LUSTRE_EPROTONOSUPPORT,
	[ESOCKTNOSUPPORT]	= LUSTRE_ESOCKTNOSUPPORT,
	[EOPNOTSUPP]		= LUSTRE_EOPNOTSUPP,
	[EPFNOSUPPORT]		= LUSTRE_EPFNOSUPPORT,
	[EAFNOSUPPORT]		= LUSTRE_EAFNOSUPPORT,
	[EADDRINUSE]		= LUSTRE_EADDRINUSE,
	[EADDRNOTAVAIL]		= LUSTRE_EADDRNOTAVAIL,
	[ENETDOWN]		= LUSTRE_ENETDOWN,
	[ENETUNREACH]		= LUSTRE_ENETUNREACH,
	[ENETRESET]		= LUSTRE_ENETRESET,
	[ECONNABORTED]		= LUSTRE_ECONNABORTED,
	[ECONNRESET]		= LUSTRE_ECONNRESET,
	[ENOBUFS]		= LUSTRE_ENOBUFS,
	[EISCONN]		= LUSTRE_EISCONN,
	[ENOTCONN]		= LUSTRE_ENOTCONN,
	[ESHUTDOWN]		= LUSTRE_ESHUTDOWN,
	[ETOOMANYREFS]		= LUSTRE_ETOOMANYREFS,
	[ETIMEDOUT]		= LUSTRE_ETIMEDOUT,
	[ECONNREFUSED]		= LUSTRE_ECONNREFUSED,
	[EHOSTDOWN]		= LUSTRE_EHOSTDOWN,
	[EHOSTUNREACH]		= LUSTRE_EHOSTUNREACH,
	[EALREADY]		= LUSTRE_EALREADY,
	[EINPROGRESS]		= LUSTRE_EINPROGRESS,
	[ESTALE]		= LUSTRE_ESTALE,
	[EUCLEAN]		= LUSTRE_EUCLEAN,
	[ENOTNAM]		= LUSTRE_ENOTNAM,
	[ENAVAIL]		= LUSTRE_ENAVAIL,
	[EISNAM]		= LUSTRE_EISNAM,
	[EREMOTEIO]		= LUSTRE_EREMOTEIO,
	[EDQUOT]		= LUSTRE_EDQUOT,
	[ENOMEDIUM]		= LUSTRE_ENOMEDIUM,
	[EMEDIUMTYPE]		= LUSTRE_EMEDIUMTYPE,
	[ECANCELED]		= LUSTRE_ECANCELED,
	[ENOKEY]		= LUSTRE_ENOKEY,
	[EKEYEXPIRED]		= LUSTRE_EKEYEXPIRED,
	[EKEYREVOKED]		= LUSTRE_EKEYREVOKED,
	[EKEYREJECTED]		= LUSTRE_EKEYREJECTED,
	[EOWNERDEAD]		= LUSTRE_EOWNERDEAD,
	[ENOTRECOVERABLE]	= LUSTRE_ENOTRECOVERABLE,
	[ERESTARTSYS]		= LUSTRE_ERESTARTSYS,
	[ERESTARTNOINTR]	= LUSTRE_ERESTARTNOINTR,
	[ERESTARTNOHAND]	= LUSTRE_ERESTARTNOHAND,
	[ENOIOCTLCMD]		= LUSTRE_ENOIOCTLCMD,
	[ERESTART_RESTARTBLOCK]	= LUSTRE_ERESTART_RESTARTBLOCK,
	[EBADHANDLE]		= LUSTRE_EBADHANDLE,
	[ENOTSYNC]		= LUSTRE_ENOTSYNC,
	[EBADCOOKIE]		= LUSTRE_EBADCOOKIE,
	[ENOTSUPP]		= LUSTRE_ENOTSUPP,
	[ETOOSMALL]		= LUSTRE_ETOOSMALL,
	[ESERVERFAULT]		= LUSTRE_ESERVERFAULT,
	[EBADTYPE]		= LUSTRE_EBADTYPE,
	[EJUKEBOX]		= LUSTRE_EJUKEBOX,
	[EIOCBQUEUED]		= LUSTRE_EIOCBQUEUED,

	/*
	 * The ELDLM errors are Lustre specific errors whose ranges
	 * lie in the middle of the above system errors. The ELDLM
	 * numbers must be preserved to avoid LU-9793.
	 */
	[ELDLM_LOCK_CHANGED]	= ELDLM_LOCK_CHANGED,
	[ELDLM_LOCK_ABORTED]	= ELDLM_LOCK_ABORTED,
	[ELDLM_LOCK_REPLACED]	= ELDLM_LOCK_REPLACED,
	[ELDLM_NO_LOCK_DATA]	= ELDLM_NO_LOCK_DATA,
	[ELDLM_LOCK_WOULDBLOCK]	= ELDLM_LOCK_WOULDBLOCK,
	[ELDLM_NAMESPACE_EXISTS]= ELDLM_NAMESPACE_EXISTS,
	[ELDLM_BAD_NAMESPACE]	= ELDLM_BAD_NAMESPACE
};

static int lustre_errno_ntoh_mapping[] = {
	[LUSTRE_EPERM]			= EPERM,
	[LUSTRE_ENOENT]			= ENOENT,
	[LUSTRE_ESRCH]			= ESRCH,
	[LUSTRE_EINTR]			= EINTR,
	[LUSTRE_EIO]			= EIO,
	[LUSTRE_ENXIO]			= ENXIO,
	[LUSTRE_E2BIG]			= E2BIG,
	[LUSTRE_ENOEXEC]		= ENOEXEC,
	[LUSTRE_EBADF]			= EBADF,
	[LUSTRE_ECHILD]			= ECHILD,
	[LUSTRE_EAGAIN]			= EAGAIN,
	[LUSTRE_ENOMEM]			= ENOMEM,
	[LUSTRE_EACCES]			= EACCES,
	[LUSTRE_EFAULT]			= EFAULT,
	[LUSTRE_ENOTBLK]		= ENOTBLK,
	[LUSTRE_EBUSY]			= EBUSY,
	[LUSTRE_EEXIST]			= EEXIST,
	[LUSTRE_EXDEV]			= EXDEV,
	[LUSTRE_ENODEV]			= ENODEV,
	[LUSTRE_ENOTDIR]		= ENOTDIR,
	[LUSTRE_EISDIR]			= EISDIR,
	[LUSTRE_EINVAL]			= EINVAL,
	[LUSTRE_ENFILE]			= ENFILE,
	[LUSTRE_EMFILE]			= EMFILE,
	[LUSTRE_ENOTTY]			= ENOTTY,
	[LUSTRE_ETXTBSY]		= ETXTBSY,
	[LUSTRE_EFBIG]			= EFBIG,
	[LUSTRE_ENOSPC]			= ENOSPC,
	[LUSTRE_ESPIPE]			= ESPIPE,
	[LUSTRE_EROFS]			= EROFS,
	[LUSTRE_EMLINK]			= EMLINK,
	[LUSTRE_EPIPE]			= EPIPE,
	[LUSTRE_EDOM]			= EDOM,
	[LUSTRE_ERANGE]			= ERANGE,
	[LUSTRE_EDEADLK]		= EDEADLK,
	[LUSTRE_ENAMETOOLONG]		= ENAMETOOLONG,
	[LUSTRE_ENOLCK]			= ENOLCK,
	[LUSTRE_ENOSYS]			= ENOSYS,
	[LUSTRE_ENOTEMPTY]		= ENOTEMPTY,
	[LUSTRE_ELOOP]			= ELOOP,
	[LUSTRE_ENOMSG]			= ENOMSG,
	[LUSTRE_EIDRM]			= EIDRM,
	[LUSTRE_ECHRNG]			= ECHRNG,
	[LUSTRE_EL2NSYNC]		= EL2NSYNC,
	[LUSTRE_EL3HLT]			= EL3HLT,
	[LUSTRE_EL3RST]			= EL3RST,
	[LUSTRE_ELNRNG]			= ELNRNG,
	[LUSTRE_EUNATCH]		= EUNATCH,
	[LUSTRE_ENOCSI]			= ENOCSI,
	[LUSTRE_EL2HLT]			= EL2HLT,
	[LUSTRE_EBADE]			= EBADE,
	[LUSTRE_EBADR]			= EBADR,
	[LUSTRE_EXFULL]			= EXFULL,
	[LUSTRE_ENOANO]			= ENOANO,
	[LUSTRE_EBADRQC]		= EBADRQC,
	[LUSTRE_EBADSLT]		= EBADSLT,
	[LUSTRE_EBFONT]			= EBFONT,
	[LUSTRE_ENOSTR]			= ENOSTR,
	[LUSTRE_ENODATA]		= ENODATA,
	[LUSTRE_ETIME]			= ETIME,
	[LUSTRE_ENOSR]			= ENOSR,
	[LUSTRE_ENONET]			= ENONET,
	[LUSTRE_ENOPKG]			= ENOPKG,
	[LUSTRE_EREMOTE]		= EREMOTE,
	[LUSTRE_ENOLINK]		= ENOLINK,
	[LUSTRE_EADV]			= EADV,
	[LUSTRE_ESRMNT]			= ESRMNT,
	[LUSTRE_ECOMM]			= ECOMM,
	[LUSTRE_EPROTO]			= EPROTO,
	[LUSTRE_EMULTIHOP]		= EMULTIHOP,
	[LUSTRE_EDOTDOT]		= EDOTDOT,
	[LUSTRE_EBADMSG]		= EBADMSG,
	[LUSTRE_EOVERFLOW]		= EOVERFLOW,
	[LUSTRE_ENOTUNIQ]		= ENOTUNIQ,
	[LUSTRE_EBADFD]			= EBADFD,
	[LUSTRE_EREMCHG]		= EREMCHG,
	[LUSTRE_ELIBACC]		= ELIBACC,
	[LUSTRE_ELIBBAD]		= ELIBBAD,
	[LUSTRE_ELIBSCN]		= ELIBSCN,
	[LUSTRE_ELIBMAX]		= ELIBMAX,
	[LUSTRE_ELIBEXEC]		= ELIBEXEC,
	[LUSTRE_EILSEQ]			= EILSEQ,
	[LUSTRE_ERESTART]		= ERESTART,
	[LUSTRE_ESTRPIPE]		= ESTRPIPE,
	[LUSTRE_EUSERS]			= EUSERS,
	[LUSTRE_ENOTSOCK]		= ENOTSOCK,
	[LUSTRE_EDESTADDRREQ]		= EDESTADDRREQ,
	[LUSTRE_EMSGSIZE]		= EMSGSIZE,
	[LUSTRE_EPROTOTYPE]		= EPROTOTYPE,
	[LUSTRE_ENOPROTOOPT]		= ENOPROTOOPT,
	[LUSTRE_EPROTONOSUPPORT]	= EPROTONOSUPPORT,
	[LUSTRE_ESOCKTNOSUPPORT]	= ESOCKTNOSUPPORT,
	[LUSTRE_EOPNOTSUPP]		= EOPNOTSUPP,
	[LUSTRE_EPFNOSUPPORT]		= EPFNOSUPPORT,
	[LUSTRE_EAFNOSUPPORT]		= EAFNOSUPPORT,
	[LUSTRE_EADDRINUSE]		= EADDRINUSE,
	[LUSTRE_EADDRNOTAVAIL]		= EADDRNOTAVAIL,
	[LUSTRE_ENETDOWN]		= ENETDOWN,
	[LUSTRE_ENETUNREACH]		= ENETUNREACH,
	[LUSTRE_ENETRESET]		= ENETRESET,
	[LUSTRE_ECONNABORTED]		= ECONNABORTED,
	[LUSTRE_ECONNRESET]		= ECONNRESET,
	[LUSTRE_ENOBUFS]		= ENOBUFS,
	[LUSTRE_EISCONN]		= EISCONN,
	[LUSTRE_ENOTCONN]		= ENOTCONN,
	[LUSTRE_ESHUTDOWN]		= ESHUTDOWN,
	[LUSTRE_ETOOMANYREFS]		= ETOOMANYREFS,
	[LUSTRE_ETIMEDOUT]		= ETIMEDOUT,
	[LUSTRE_ECONNREFUSED]		= ECONNREFUSED,
	[LUSTRE_EHOSTDOWN]		= EHOSTDOWN,
	[LUSTRE_EHOSTUNREACH]		= EHOSTUNREACH,
	[LUSTRE_EALREADY]		= EALREADY,
	[LUSTRE_EINPROGRESS]		= EINPROGRESS,
	[LUSTRE_ESTALE]			= ESTALE,
	[LUSTRE_EUCLEAN]		= EUCLEAN,
	[LUSTRE_ENOTNAM]		= ENOTNAM,
	[LUSTRE_ENAVAIL]		= ENAVAIL,
	[LUSTRE_EISNAM]			= EISNAM,
	[LUSTRE_EREMOTEIO]		= EREMOTEIO,
	[LUSTRE_EDQUOT]			= EDQUOT,
	[LUSTRE_ENOMEDIUM]		= ENOMEDIUM,
	[LUSTRE_EMEDIUMTYPE]		= EMEDIUMTYPE,
	[LUSTRE_ECANCELED]		= ECANCELED,
	[LUSTRE_ENOKEY]			= ENOKEY,
	[LUSTRE_EKEYEXPIRED]		= EKEYEXPIRED,
	[LUSTRE_EKEYREVOKED]		= EKEYREVOKED,
	[LUSTRE_EKEYREJECTED]		= EKEYREJECTED,
	[LUSTRE_EOWNERDEAD]		= EOWNERDEAD,
	[LUSTRE_ENOTRECOVERABLE]	= ENOTRECOVERABLE,
	[LUSTRE_ERESTARTSYS]		= ERESTARTSYS,
	[LUSTRE_ERESTARTNOINTR]		= ERESTARTNOINTR,
	[LUSTRE_ERESTARTNOHAND]		= ERESTARTNOHAND,
	[LUSTRE_ENOIOCTLCMD]		= ENOIOCTLCMD,
	[LUSTRE_ERESTART_RESTARTBLOCK]	= ERESTART_RESTARTBLOCK,
	[LUSTRE_EBADHANDLE]		= EBADHANDLE,
	[LUSTRE_ENOTSYNC]		= ENOTSYNC,
	[LUSTRE_EBADCOOKIE]		= EBADCOOKIE,
	[LUSTRE_ENOTSUPP]		= ENOTSUPP,
	[LUSTRE_ETOOSMALL]		= ETOOSMALL,
	[LUSTRE_ESERVERFAULT]		= ESERVERFAULT,
	[LUSTRE_EBADTYPE]		= EBADTYPE,
	[LUSTRE_EJUKEBOX]		= EJUKEBOX,
	[LUSTRE_EIOCBQUEUED]		= EIOCBQUEUED,

	/*
	 * The ELDLM errors are Lustre specific errors whose ranges
	 * lie in the middle of the above system errors. The ELDLM
	 * numbers must be preserved to avoid LU-9793.
	 */
	[ELDLM_LOCK_CHANGED]		= ELDLM_LOCK_CHANGED,
	[ELDLM_LOCK_ABORTED]		= ELDLM_LOCK_ABORTED,
	[ELDLM_LOCK_REPLACED]		= ELDLM_LOCK_REPLACED,
	[ELDLM_NO_LOCK_DATA]		= ELDLM_NO_LOCK_DATA,
	[ELDLM_LOCK_WOULDBLOCK]		= ELDLM_LOCK_WOULDBLOCK,
	[ELDLM_NAMESPACE_EXISTS]	= ELDLM_NAMESPACE_EXISTS,
	[ELDLM_BAD_NAMESPACE]		= ELDLM_BAD_NAMESPACE
};

unsigned int lustre_errno_hton(unsigned int h)
{
	unsigned int n;

	if (h == 0) {
		n = 0;
	} else if (h < ARRAY_SIZE(lustre_errno_hton_mapping)) {
		n = lustre_errno_hton_mapping[h];
		if (n == 0)
			goto generic;
	} else {
generic:
		/*
		 * A generic errno is better than the unknown one that could
		 * mean anything to a different host.
		 */
		n = LUSTRE_EIO;
	}

	return n;
}
EXPORT_SYMBOL(lustre_errno_hton);

unsigned int lustre_errno_ntoh(unsigned int n)
{
	unsigned int h;

	if (n == 0) {
		h = 0;
	} else if (n < ARRAY_SIZE(lustre_errno_ntoh_mapping)) {
		h = lustre_errno_ntoh_mapping[n];
		if (h == 0)
			goto generic;
	} else {
generic:
		/*
		 * Similar to the situation in lustre_errno_hton(), an unknown
		 * network errno could coincide with anything.  Hence, it is
		 * better to return a generic errno.
		 */
		h = EIO;
	}

	return h;
}
EXPORT_SYMBOL(lustre_errno_ntoh);

#endif /* LUSTRE_TRANSLATE_ERRNOS */
