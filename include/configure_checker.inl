/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                             //
//                       __  _                                _            _                     _         _   //
//    ___  ___   _ __   / _|(_)  __ _  _   _  _ __  ___      | |__    ___ | | _ __    ___  _ __ (_) _ __  | |  //
//   / __|/ _ \ | '_ \ | |_ | | / _` || | | || '__|/ _ \     | '_ \  / _ \| || '_ \  / _ \| '__|| || '_ \ | |  //
//  | (__| (_) || | | ||  _|| || (_| || |_| || |  |  __/     | | | ||  __/| || |_) ||  __/| | _ | || | | || |  //
//   \___|\___/ |_| |_||_|  |_| \__, | \__,_||_|   \___|_____|_| |_| \___||_|| .__/  \___||_|(_)|_||_| |_||_|  //
//                              |___/                  |_____|               |_|                               //
//                                                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                             //
//                                      Copyright (c) 2025 by S.F.T. Inc.                                      //
//                    Use, copying, and distribution of this software are licensed according                   //
//                      to the GPLv2, LGPLv2, or BSD license, as appropriate (see COPYING)                     //
//                                                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// #include this to check configure script and do some windows compatibility things

#ifndef _CONFIGURE_CHECKER_INL_INCLUDED_
#define _CONFIGURE_CHECKER_INL_INCLUDED_

/* CONFIGURE SCRIPT SUPPORT = GNUC and CLANG */
#if defined(__GNUC__) || defined(__DOXYGEN__)

/** \ingroup platform_definitions
  * \brief Defined in 'platform_helper.h' when the compiler supports C99 initializers
**/
#define WB_C99_INITIALIZERS /* allow C99-style initializers - gcc and clang */


// ======================================================================================
// this section is used for doxygen documentation using the X11 definitions
// and will compile whenever GNU or clang compiler is being used.
// both BNUC and 'clang' compilers define '__GNUC__' and will use this section
// ======================================================================================

#ifdef X11WORKBENCH_PROJECT_BUILD

// Globally include the output of the configure script for GNUC compiles for "the project"
#include "X11workbenchToolkit_config.h" // the 'generated' version'

#else // !X11WORKBENCH_PROJECT_BUILD

// This is for the installed version, a subset of the things defined by the configure output script.
// NOTE:  this must be installed in the appropriate package directory along with platform_helper.h
#include "X11workbenchToolkit_install_config.h"

#endif // X11WORKBENCH_PROJECT_BUILD


// TESTING THE CONFIGURATION - TODO:  for some, provide alternates?
// NOTE that for '_MSC_VER' defined, certain features won't be checked

// basic checks - test them all up front, then individual errors if any of these
//                are not defined in the appropriate header files
#if !defined( HAVE_ALARM ) || !defined( HAVE_CHOWN ) || \
    !defined( HAVE_CLOCK_GETTIME ) || !defined( HAVE_DUP2 ) || !defined( HAVE_FORK ) || \
    !defined( HAVE_FTRUNCATE ) || !defined( HAVE_GETCWD ) || !defined( HAVE_GETTIMEOFDAY ) || \
    !defined( HAVE_MALLOC ) || !defined( HAVE_MBLEN ) || !defined( HAVE_MEMCHR ) || \
    !defined( HAVE_MEMMOVE ) || !defined( HAVE_MEMSET ) || !defined( HAVE_MKDIR ) || \
    !defined( HAVE_PRINTF ) || !defined( HAVE_REALLOC ) || \
    !defined( HAVE_SELECT ) || !defined( HAVE_SETLOCALE ) || !defined( HAVE_STRCASECMP ) || \
    !defined( HAVE_STRCHR ) || !defined( HAVE_STRNCASECMP ) || !defined( HAVE_STRRCHR ) || \
    !defined( HAVE_STRSTR ) || !defined( HAVE__BOOL ) || \
    ( !defined(_MSC_VER) && ( !defined( LSTAT_FOLLOWS_SLASHED_SYMLINK ) || !defined( HAVE_VFORK ) || \
                              !defined( HAVE_WORKING_FORK ) || !defined( HAVE_WORKING_VFORK ) ))
// *************************************************************
#error
#error configure script feature check 1 fail
#error critical features missing
#error
// --------------------------------------------------------------
// this next section spits out errors based on what's not defined.
// --------------------------------------------------------------
#ifndef HAVE_ALARM
#error  HAVE_ALARM is not defined
#endif //  HAVE_ALARM
#ifndef HAVE_CHOWN
#error  HAVE_CHOWN is not defined
#endif //  HAVE_CHOWN
#ifndef HAVE_CLOCK_GETTIME
#error  HAVE_CLOCK_GETTIME is not defined
#endif //  HAVE_CLOCK_GETTIME
#ifndef HAVE_DUP2
#error  HAVE_DUP2 is not defined
#endif //  HAVE_DUP2
#ifndef HAVE_FORK
#error  HAVE_FORK is not defined
#endif //  HAVE_FORK
#ifndef HAVE_FTRUNCATE
#error  HAVE_FTRUNCATE is not defined
#endif //  HAVE_FTRUNCATE
#ifndef HAVE_GETCWD
#error  HAVE_GETCWD is not defined
#endif //  HAVE_GETCWD
#ifndef HAVE_GETTIMEOFDAY
#error  HAVE_GETTIMEOFDAY is not defined
#endif //  HAVE_GETTIMEOFDAY
#ifndef HAVE_MALLOC
#error  HAVE_MALLOC is not defined
#endif //  HAVE_MALLOC
#ifndef HAVE_MBLEN
#error  HAVE_MBLEN is not defined
#endif //  HAVE_MBLEN
#ifndef HAVE_MEMCHR
#error  HAVE_MEMCHR is not defined
#endif //  HAVE_MEMCHR
#ifndef HAVE_MEMMOVE
#error  HAVE_MEMMOVE is not defined
#endif //  HAVE_MEMMOVE
#ifndef HAVE_MEMSET
#error  HAVE_MEMSET is not defined
#endif //  HAVE_MEMSET
#ifndef HAVE_MKDIR
#error  HAVE_MKDIR is not defined
#endif //  HAVE_MKDIR
#ifndef HAVE_PRINTF
#error  HAVE_PRINTF is not defined
#endif //  HAVE_PRINTF
#ifndef HAVE_REALLOC
#error  HAVE_REALLOC is not defined
#endif //  HAVE_REALLOC
#ifndef HAVE_SELECT
#error  HAVE_SELECT is not defined
#endif //  HAVE_SELECT
#ifndef HAVE_SETLOCALE
#error  HAVE_SETLOCALE is not defined
#endif //  HAVE_SETLOCALE
#ifndef HAVE_STRCASECMP
#error  HAVE_STRCASECMP is not defined
#endif //  HAVE_STRCASECMP
#ifndef HAVE_STRCHR
#error  HAVE_STRCHR is not defined
#endif //  HAVE_STRCHR
#ifndef HAVE_STRNCASECMP
#error  HAVE_STRNCASECMP is not defined
#endif //  HAVE_STRNCASECMP
#ifndef HAVE_STRRCHR
#error  HAVE_STRRCHR is not defined
#endif //  HAVE_STRRCHR
#ifndef HAVE_STRSTR
#error  HAVE_STRSTR is not defined
#endif //  HAVE_STRSTR
#ifndef HAVE__BOOL
#error  HAVE__BOOL is not defined
#endif //  HAVE__BOOL
#ifndef LSTAT_FOLLOWS_SLASHED_SYMLINK
#error  LSTAT_FOLLOWS_SLASHED_SYMLINK is not defined
#endif //  LSTAT_FOLLOWS_SLASHED_SYMLINK
#ifndef HAVE_VFORK
#error  HAVE_VFORK is not defined
#endif //  HAVE_VFORK
#ifndef HAVE_WORKING_FORK
#error  HAVE_WORKING_FORK is not defined
#endif //  HAVE_WORKING_FORK
#ifndef HAVE_WORKING_VFORK
#error  HAVE_WORKING_VFORK is not defined
#endif //  HAVE_WORKING_VFORK
#error

// *************************************************************
// the end of the 'check not defined' error section
// *************************************************************

// check to see that all are defined as non-zero values
// -------------------------------------------------------------
#elif !( HAVE_ALARM ) || !( HAVE_CHOWN ) || \
      !( HAVE_CLOCK_GETTIME ) || !( HAVE_DUP2 ) || !( HAVE_FORK ) || \
      !( HAVE_FTRUNCATE ) || !( HAVE_GETCWD ) || !( HAVE_GETTIMEOFDAY ) || \
      !( HAVE_MALLOC ) || !( HAVE_MBLEN ) || !( HAVE_MEMCHR ) || \
      !( HAVE_MEMMOVE ) || !( HAVE_MEMSET ) || !( HAVE_MKDIR ) || \
      !( HAVE_PRINTF ) || !( HAVE_REALLOC ) || \
      !( HAVE_SELECT ) || !( HAVE_SETLOCALE ) || !( HAVE_STRCASECMP ) || \
      !( HAVE_STRCHR ) || !( HAVE_STRNCASECMP ) || !( HAVE_STRRCHR ) || \
      !( HAVE_STRSTR ) || !( HAVE__BOOL ) || \
      ( !defined(_MSC_VER) && ( !( LSTAT_FOLLOWS_SLASHED_SYMLINK ) || !( HAVE_VFORK ) || \
                                !( HAVE_WORKING_FORK ) || !( HAVE_WORKING_VFORK ) ))
#error
#error configure script feature check 2 fail
#error critical features missing
#error

// --------------------------------------------------------------
// this next section spits out errors based on what's not defined.
// --------------------------------------------------------------
#if !HAVE_ALARM
#error  HAVE_ALARM defined as zero
#endif //  HAVE_ALARM
#if !HAVE_CHOWN
#error  HAVE_CHOWN defined as zero
#endif //  HAVE_CHOWN
#if !HAVE_CLOCK_GETTIME
#error  HAVE_CLOCK_GETTIME defined as zero
#endif //  HAVE_CLOCK_GETTIME
#if !HAVE_DUP2
#error  HAVE_DUP2 defined as zero
#endif //  HAVE_DUP2
#if !HAVE_FORK
#error  HAVE_FORK defined as zero
#endif //  HAVE_FORK
#if !HAVE_FTRUNCATE
#error  HAVE_FTRUNCATE defined as zero
#endif //  HAVE_FTRUNCATE
#if !HAVE_GETCWD
#error  HAVE_GETCWD defined as zero
#endif //  HAVE_GETCWD
#if !HAVE_GETTIMEOFDAY
#error  HAVE_GETTIMEOFDAY defined as zero
#endif //  HAVE_GETTIMEOFDAY
#if !HAVE_MALLOC
#error  HAVE_MALLOC defined as zero
#endif //  HAVE_MALLOC
#if !HAVE_MBLEN
#error  HAVE_MBLEN defined as zero
#endif //  HAVE_MBLEN
#if !HAVE_MEMCHR
#error  HAVE_MEMCHR defined as zero
#endif //  HAVE_MEMCHR
#if !HAVE_MEMMOVE
#error  HAVE_MEMMOVE defined as zero
#endif //  HAVE_MEMMOVE
#if !HAVE_MEMSET
#error  HAVE_MEMSET defined as zero
#endif //  HAVE_MEMSET
#if !HAVE_MKDIR
#error  HAVE_MKDIR defined as zero
#endif //  HAVE_MKDIR
#if !HAVE_PRINTF
#error  HAVE_PRINTF defined as zero
#endif //  HAVE_PRINTF
#if !HAVE_REALLOC
#error  HAVE_REALLOC defined as zero
#endif //  HAVE_REALLOC
#if !HAVE_SELECT
#error  HAVE_SELECT defined as zero
#endif //  HAVE_SELECT
#if !HAVE_SETLOCALE
#error  HAVE_SETLOCALE defined as zero
#endif //  HAVE_SETLOCALE
#if !HAVE_STRCASECMP
#error  HAVE_STRCASECMP defined as zero
#endif //  HAVE_STRCASECMP
#if !HAVE_STRCHR
#error  HAVE_STRCHR defined as zero
#endif //  HAVE_STRCHR
#if !HAVE_STRNCASECMP
#error  HAVE_STRNCASECMP defined as zero
#endif //  HAVE_STRNCASECMP
#if !HAVE_STRRCHR
#error  HAVE_STRRCHR defined as zero
#endif //  HAVE_STRRCHR
#if !HAVE_STRSTR
#error  HAVE_STRSTR defined as zero
#endif //  HAVE_STRSTR
#if !HAVE__BOOL
#error  HAVE__BOOL defined as zero
#endif //  HAVE__BOOL
#if !LSTAT_FOLLOWS_SLASHED_SYMLINK
#error  LSTAT_FOLLOWS_SLASHED_SYMLINK defined as zero
#endif //  LSTAT_FOLLOWS_SLASHED_SYMLINK
#if !HAVE_VFORK
#error  HAVE_VFORK defined as zero
#endif //  HAVE_VFORK
#if !HAVE_WORKING_FORK
#error  HAVE_WORKING_FORK defined as zero
#endif //  HAVE_WORKING_FORK
#if !HAVE_WORKING_VFORK
#error  HAVE_WORKING_VFORK defined as zero
#endif //  HAVE_WORKING_VFORK
#error

// *************************************************************
// the end of the 'check defined as zero' error section
// *************************************************************

#endif // check for 'defined' as missing or zero as zero


#ifndef HAVE_BZERO
  #define HAVE_BZERO 0
#endif // !HAVE_BZERO

#if !( HAVE_BZERO )
// swap this in for 'bzero' using memset - it was checked for earlier
  #define bzero(X,Y) memset((X),0,(Y))
#endif

//-------------------------------------------------
// branch optimization macros - gcc and clang only
//-------------------------------------------------

#if defined(COMPILER_SUPPORTS_BUILTIN_EXPECT) || defined(__DOXYGEN__)

/** \brief optimization for code branching when condition is 'unlikely'.  use within conditionals
 **
 ** \code
    if(WB_UNLIKELY(x == 5))
    {
      // branch optimized to bypass this code when condition is FALSE
    }
 ** \endcode
**/
#define WB_UNLIKELY(x) (__builtin_expect (!!(x), 0))
/** \brief optimization for code branching when condition is 'likely'.  use within conditionals
 **
 ** \code
    if(WB_LIKELY(x == 5))
    {
      // branch optimized to execute this code when condition is TRUE
    }
 ** \endcode
**/
#define WB_LIKELY(x) (__builtin_expect (!!(x), 1))

#else // !COMPILER_SUPPORTS_BUILTIN_EXPECT

#define WB_UNLIKELY(x) (x)
#define WB_LIKELY(x) (x)

#endif // COMPILER_SUPPORTS_BUILTIN_EXPECT

#if defined(COMPILER_SUPPORTS_UNUSED_ATTRIBUTE) || defined(__DOXYGEN__)

/** \brief marks a variable as likely to be 'unused'.  warning abatement.  Place macro directly after the variable name
 **
 ** \code

    int an_unused_var WB_UNUSED = 5; // no compiler warnings if I don't actually use this variable

 ** \endcode
 **
 ** \b NOTE:  may also be used for function parameters.  In some cases a callback function may have unused\n
 **        parameters passed to it.  Marking them with 'WB_UNUSED' helps prevent unnecessary compiler warnings.
**/
#define WB_UNUSED __attribute__((unused))

#else // !COMPILER_SUPPORTS_UNUSED_ATTRIBUTE

#define WB_UNUSED

#endif // COMPILER_SUPPORTS_UNUSED_ATTRIBUTE



\
// TODO - any additional feature tests go here

// ======================================================================================
// end of section for clang and gcc compilers (and also doxygen definitions)
// ======================================================================================

#else // windows compiler


#if defined(_MSC_VER) /* Microsoft C/C++ compiler */
// ======================================================================================
// this section is dedicated to Microsoft compilers, which do things a bit different...

// TODO:  add proper support for MS compiler
#if _MSC_VER > 1300

// TODO:  specify correct version in which C99 initializers are supported
//        as of MSC for ".Net" 2010 it's _NOT_ supported (still!)
//#define WB_C99_INITIALIZERS

// see which MS C compiler supports C99 style initializers
//#define WB_C99_INITIALIZERS

// anything else?

#endif // _MSC_VER > xxxx

#define WB_UNLIKELY(x) (x)
#define WB_LIKELY(x) (x)
#define WB_UNUSED

#define WB_UNLIKELY(x) (x)
#define WB_LIKELY(x) (x)

#ifndef __inline__
#define __inline__ inline /* this assumes 'inline' is supported (MSC should support this) */
#endif // __inline__

// TODO:  sizeof(int) sizeof(long) - long is 32-bit for MSC

// TODO:  compatibility definition file for winders

#define __PACKED__ /* TODO: a definition for packing in MS-land */

// end of section specific to Microsoft compilers
// ======================================================================================

// neither MS nor GNUC compatible compiler
#error unknown and/or unsupported WIN32 compiler

#endif // _MSC_VER
#endif // __GNUC__,WIN32 - config script feature tests, MS C compiler-specific preliminary stuff
// *******************************
// end of compiler-specific stoff
// *******************************

#endif //  _CONFIGURE_CHECKER_INL_INCLUDED_

