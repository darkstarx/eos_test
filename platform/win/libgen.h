#ifndef _LIBGEN_H_
/* 
 * libgen.h
 *
 * $Id: libgen.h,v 1.2 2007/06/23 07:34:15 dannysmith Exp $
 *
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is a part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * Functions for splitting pathnames into dirname and basename components.
 *
 */
#define _LIBGEN_H_

#ifdef __cplusplus
extern "C" {
#endif

extern char *basename (char *);

#ifdef __cplusplus
}
#endif

#endif	/* _LIBGEN_H_: end of file */