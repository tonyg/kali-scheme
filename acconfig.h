/*
 * HAVE_SIGACTION is defined iff sigaction() is available.
 */
#undef	HAVE_SIGACTION

/*
 * HAVE_STRERROR is defined iff the standard libraries provide strerror().
 */
#undef	HAVE_STRERROR

/*
 * NLIST_HAS_N_NAME is defined iff a struct nlist has an n_name member.
 * If it doesn't then we assume it has an n_un member which, in turn,
 * has an n_name member.
 */
#undef	NLIST_HAS_N_NAME

/*
 * HAVE_SYS_SELECT_H is defined iff we have the include file sys/select.h.
 */
#undef	HAVE_SYS_SELECT_H

/*
 * HAVE_SYSEXITS_H is defined iff we have the include file sysexits.h.
 */
#undef	HAVE_SYSEXITS_H

/*
 * HAVE_DLOPEN is defined iff we have dlopen().
 */
#undef	HAVE_DLOPEN

/*
 * USCORE is defined iff C externals are prepended with an underscore.
 */
#undef	USCORE

/*
 * ENVIRON_NAME is defined to be the name of the environ variable.
 */
#undef ENVIRON_NAME

/*
 * We define this (as 'unsigned int') if the OS doesn't.
 */

#undef socklen_t

@BOTTOM@

#include "fake/sigact.h"
#include "fake/strerror.h"
#include "fake/sys-select.h"
#include "fake/environ.h"
#include "fake/sysexits.h"
