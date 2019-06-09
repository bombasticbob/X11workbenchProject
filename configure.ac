AC_INIT(X11workbench, 1.0, bobf@mrp3.com, X11workbench)
# check/get base source directory based on a single unique file name
AC_CONFIG_SRCDIR(X11workbench.c)
# output defines to a header file 'X11workbenchToolkit_config.h'
AC_CONFIG_HEADERS([include/X11workbenchToolkit_config.h])
# enable automake using 'more modern' syntax where AC_INIT defines the project and version
AM_INIT_AUTOMAKE


# prereq autoconf 2.53 or later
AC_PREREQ(2.53)
# FYI assuming automake >= 15

# order of search for CC - use 'cc' first, then 'gcc', then 'cl'
# this way the system-defined C compiler will be invoked by default
# TODO:  add a configure option to force gcc, clang, Microsoft C, etc.
AC_PROG_CC([cc gcc cl])

# AM_PROG_CC_C_O required based on a warning on a linux box for per-target
# flags on pixmap_helper.c with automake 1.11,and it must be AFTER AC_PROG_CC
AM_PROG_CC_C_O

AC_PROG_RANLIB
AC_PATH_X
AC_PATH_XTRA
AC_TYPE_SIGNAL
AC_PROG_AWK
AC_PROG_LN_S

# check for presence of doxygen
ifdef([XORG_WITH_DOXYGEN],
[
  XORG_WITH_DOXYGEN([1.7])
  # this sets have_doxygen to yes if doxygen is present
],
[
  have_doxygen=no
  AM_CONDITIONAL([HAVE_DOXYGEN], [test x$have_doxygen = xyes])
])

# see if I have multiple CPUs (this works on FreeBSD and Linux, not sure about others)

AC_MSG_CHECKING([how many cores the host CPU has])

if test -e conftest.out ; then
  rm -f conftest.out
fi
if test -e /proc/cpuinfo ; then
  grep -i "^processor" /proc/cpuinfo | wc -l >conftest.out
else
  sysctl hw.ncpu | awk '{ print $2; }' >conftest.out
fi

if test -s conftest.out ; then
  ac_num_cores=`cat conftest.out`
  ac_status=0
  rm conftest.out

  echo $ac_num_cores
else
  ac_status=1
  ac_num_cores=1
  echo "cannot determine; assuming 1"
fi


# --enable-release (default has debugging enabled)
# --enable-noopt (default is optimization enabled)
# --enable-libXpm (default is NOT to link to libXpm)
# --enable-libXft (default is to link to libXft)
# --enable-libXext (default is to link to libXext)
# --enable-fatalwarn (default is warnings NOT fatal)


AC_ARG_ENABLE(release,
              [  --enable-release        CUSTOM - Turn off all debugging for release version],
              [case "${enableval}" in
               yes) release=true ; AC_MSG_RESULT([CONFIGURE INFORMATION:  building a RELEASE version]) ;;
               no)  release=false ;;
               *) AC_MSG_ERROR(bad value ${enableval} for --enable-release) ;;
               esac],
              [release=false])

AC_ARG_ENABLE(noopt,
              [  --enable-noopt          CUSTOM - removes optimized code, useful for debug],
              [case "${enableval}" in
               yes) noopt=true ; AC_MSG_WARN([code optimization disabled; for debug only]) ;;
               no)  noopt=false ;;
               *) AC_MSG_ERROR(bad value ${enableval} for --enable-noopt) ;;
               esac],
              [noopt=false])

AC_ARG_ENABLE(libXpm,
              [  --enable-libXpm         CUSTOM - use libXpm, rather than the (faster) internal implementation],
              [case "${enableval}" in
               yes)  libXpm=true ; AC_MSG_WARN([linking with libXpm - this could impact application performance]) ;;
               no)   libXpm=false ;;
               *) AC_MSG_ERROR(bad value ${enableval} for --enable-libXpm) ;;
               esac],
              [libXpm=false])

AC_ARG_ENABLE(libXft,
              [  --disable-libXft        CUSTOM - do NOT use libXft for 'FreeType' font support and rendering],
              [case "${enableval}" in
               yes)  libXft=true ;;
               no)   libXft=false ; AC_MSG_WARN([NOT linking with libXft]) ;;
               *) AC_MSG_ERROR(bad value ${enableval} for --enable-libXft) ;;
               esac],
              [libXft=true])

AC_ARG_ENABLE(libXext,
              [  --disable-libXext       CUSTOM - do NOT use libXext extensions (such as the shared memory extension)],
              [case "${enableval}" in
               yes)  libXext=true ;;
               no)   libXext=false ; AC_MSG_WARN([NOT linking with libXext]) ;;
               *) AC_MSG_ERROR(bad value ${enableval} for --enable-libXext) ;;
               esac],
              [libXext=true])

AC_ARG_ENABLE(fatalwarn,
              [  --enable-fatalwarn      CUSTOM - cause warnings to be treated as errors],
              [case "${enableval}" in
               yes)  fatalwarn=true ; AC_MSG_RESULT([CONFIGURE INFORMATION:  Warnings are now fatal, treated as errors]) ;;
               no)   fatalwarn=false ;;
               *) AC_MSG_ERROR(bad value ${enableval} for --enable-fatalwarn) ;;
               esac],
              [fatalwarn=false])


AM_CONDITIONAL(NO_DEBUG, test x$release = xtrue)
AM_CONDITIONAL(NO_OPT, test x$noopt = xtrue)
AM_CONDITIONAL(FATAL_WARN, test x$fatalwarn = xtrue)


local_incdir="\$(top_srcdir)/include"
local_xpmdir="\$(top_srcdir)/xpm"

if test x$noopt = xtrue ; then
  CFLAGS="$CFLAGS -O0 -Wformat $_X_CFLAGS -I$local_incdir -I$local_xpmdir"
else
  CFLAGS="$CFLAGS -Wformat $X_CFLAGS -I$local_incdir -I$local_xpmdir"
fi

#CFLAGS="$CFLAGS -Wformat $X_CFLAGS"
CPPFLAGS="$CPPFLAGS $X_CFLAGS -I$local_incdir -I$local_xpmdir"
CXXFLAGS="$CXXFLAGS $X_CFLAGS -I$local_incdir -I$local_xpmdir"


# TODO:  do I need AC_SUBST for CFLAGS, etc. here?


#################################################################
# header file and function checks (some recommended by autoscan)
#################################################################

AC_CHECK_HEADER([unistd.h],[],AC_MSG_ERROR([missing unistd.h]))
AC_CHECK_HEADER([X11/Xlib.h],[],AC_MSG_ERROR([missing Xlib.h - you should install 'xorg-dev' or similar]))
AC_CHECK_HEADER([X11/Xmd.h],[],AC_MSG_ERROR([missing Xmd.h - you need to install the X11 developer packages]))
AC_CHECK_HEADER([X11/Xutil.h],[],AC_MSG_ERROR([missing Xutil.h - you need to install the X11 developer packages]))
AC_CHECK_HEADER([X11/Xatom.h],[],AC_MSG_ERROR([missing Xatom.h - you need to install the X11 developer packages]))

#autoscan recommends
AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([limits.h])
AC_CHECK_HEADERS([locale.h])
AC_CHECK_HEADERS([malloc.h])
AC_CHECK_HEADERS([netinet/in.h])
AC_CHECK_HEADERS([sys/param.h])
AC_CHECK_HEADERS([sys/time.h])

#autoscan recommends
AC_CHECK_HEADER_STDBOOL

#autoscan recommends
AC_C_INLINE
AC_FUNC_CHOWN
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

#autoscan recommends
AC_CHECK_FUNCS([bzero])
AC_CHECK_FUNCS([dup2])
AC_CHECK_FUNCS([ftruncate])
AC_CHECK_FUNCS([getcwd])
AC_CHECK_FUNCS([gettimeofday])
AC_CHECK_FUNCS([mblen])
AC_CHECK_FUNCS([memchr])
AC_CHECK_FUNCS([memmove])
AC_CHECK_FUNCS([mkdir])
AC_CHECK_FUNCS([select])
AC_CHECK_FUNCS([setlocale])
AC_CHECK_FUNCS([strcasecmp])
AC_CHECK_FUNCS([strchr])
AC_CHECK_FUNCS([strncasecmp])
AC_CHECK_FUNCS([strrchr])
AC_CHECK_FUNCS([strstr])


# This is where I check for the presence of libXpm - if the configure script uses the
# option '--enable-libXpm' then I will test for its presence.  Otherwise, the internal
# implementation will be used, at least as far as the libraries are concerned.  This
# would not prevent an application from using the pre-compiled libraries and ALSO use
# libXpm.  It would only prevent the X11 toolkit library itself from linking to it.

test_have_xpm=false
if test x$libXpm = xtrue ; then
  AC_CHECK_HEADER([X11/xpm.h],test_have_xpm=true,AC_MSG_WARN([missing xpm.h - you should consider installing libXpm]))
fi


# This is where I check for libXft - the header will be X11/Xft/Xft.h
# First, make sure freetype2 is available.  this also modifies CFLAGS and LDFLAGS

test_have_xft=false
ifdef([AC_CHECK_FT2],
[
  if test x$libXft = xtrue ; then
    AC_CHECK_FT2([7.0.1],,[libXft=false; AC_MSG_WARN([freetype missing or incompatible version; will not link with libXft])])
  fi

  if test x$libXft = xtrue ; then
    my_save_CFLAGS="$CFLAGS"
    my_save_CPPFLAGS="$CPPFLAGS"
    CFLAGS="$CFLAGS $FT2_CFLAGS"
    CPPFLAGS="$CPPFLAGS $FT2_CFLAGS"
    AC_CHECK_HEADER([X11/Xft/Xft.h],test_have_xft=true,
    [
      AC_MSG_WARN([***************************************************************************])
      AC_MSG_WARN([missing Xft.h - will not be building the toolkit with libXft])
      AC_MSG_WARN([                you should consider installing the developer libXft package])
      AC_MSG_WARN([***************************************************************************])
    ])
    CFLAGS="$my_save_CFLAGS"
    CPPFLAGS="$my_save_CPPFLAGS"
  fi
],[AC_MSG_WARN([Freetype M4 macros not installed - do you need the developer package?])])

test_have_xext=false
if test x$libXext = xtrue ; then
  my_save_LDFLAGS="$LDFLAGS"
  LDFLAGS="$LDFLAGS -L$ac_x_libraries"
  AC_CHECK_LIB(Xext, XShmQueryExtension,
  [
    test_have_xext=true
    LDFLAGS="$LDFLAGS -lX11 -lXext"
    AC_CHECK_HEADER([X11/extensions/XShm.h],
    [
      AC_CHECK_FUNCS([XShmQueryExtension],
      [
        AC_DEFINE(X11WORKBENCH_TOOLKIT_HAVE_XSHM_EXTENSION,[1],[whether or not libXext and XShm extensions are available] )
      ],
      [
        AC_MSG_WARN([Unable to properly link with libXext for XShmQueryExtension test - are the lib dependencies ok?])
      ])
    ],
    [
      AC_MSG_WARN([***************************************************************************])
      AC_MSG_WARN([missing XShm.h - will not be building the toolkit with libXext.  You should])
      AC_MSG_WARN([                 consider installing the developer libXext package])
      AC_MSG_WARN([***************************************************************************])
    ],
    [
      #include "X11/Xlib.h"
      #include "sys/ipc.h"
      #include "sys/shm.h"
    ])
  ],
  [
    AC_MSG_WARN([***************************************************************************])
    AC_MSG_WARN([missing libXext - you should consider installing it])
    AC_MSG_WARN([***************************************************************************])
  ])
  LDFLAGS="$my_save_LDFLAGS"
fi



# check for a few functions that I want to verify exist
AC_CHECK_FUNCS([printf])
AC_CHECK_FUNCS([alarm])
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([nanosleep])
AC_CHECK_FUNCS([dlopen])

# the 'clock_gettime' function might require '-lrt'
AC_CHECK_LIB(rt, clock_gettime,
[
  test_need_lrt=true
  my_save_LDFLAGS="$LDFLAGS"
  LDFLAGS="$LDLAGS -lrt"
  AC_CHECK_FUNCS([clock_gettime])
  LDFLAGS="$my_save_LDFLAGS"
],
[
  # doing it this way is simpler
  AC_CHECK_FUNCS([clock_gettime])
])

# the 'dlopen' function might require '-ldl'
AC_CHECK_LIB(dl, dlopen,
[
  test_need_ldl=true
  my_save_LDLAGS="$LDFLAGS"
  LDFLAGS="$LDLAGS -ldl"
  AC_CHECK_FUNCS([dlopen])
  LDFLAGS="$my_save_LDLAGS"
],
[
  # doing it this way is simpler
  AC_CHECK_FUNCS([dlopen])
])


################################
# custom function checks go here
################################

# malloc_usable_size - mostly for debugging purposes, not supported everywhere
AC_CHECK_FUNC(malloc_usable_size, [AC_DEFINE(HAVE_MALLOC_USABLE_SIZE,[1],[whether or not malloc_usable_size is available] ) ])


############################################################################################
# __builtin_expect - see if compiler supports it

my_save_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS -Werror"
AC_LANG_PUSH([C])
AC_MSG_CHECKING([if the compiler supports __builtin_expect])
AC_LINK_IFELSE(
[AC_LANG_SOURCE([[
  #include <stdio.h>
  #include <stdlib.h>
  int main()
  {
    if(__builtin_expect(1 == 1, 1))
    {
      return 0;
    }

    exit(3);
    return 0;
  }]] )],
  [
    AC_MSG_RESULT([yes])
    AC_DEFINE([COMPILER_SUPPORTS_BUILTIN_EXPECT],[1],[Compiler supports __builtin_expect])
  ],
  [
    AC_MSG_RESULT([no])
  ] )
  AC_LANG_POP([C])
CFLAGS="$my_save_CFLAGS"

############################################################################################
# __attribute__((unused)) - see if compiler supports it

my_save_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS -Werror"
AC_LANG_PUSH([C])
AC_MSG_CHECKING([if the compiler supports __attribute__((unused))])
AC_LINK_IFELSE(
[AC_LANG_SOURCE([[
  #include <stdio.h>
  #include <stdlib.h>
  int main()
  {
    int iTemp __attribute__((unused)) = 0;

    return 0;
  }]] )],
  [
    AC_MSG_RESULT([yes])
    AC_DEFINE([COMPILER_SUPPORTS_UNUSED_ATTRIBUTE],[1],[Compiler supports __attribute__((unused))])
  ],
  [
    AC_MSG_RESULT([no])
  ] )
  AC_LANG_POP([C])
CFLAGS="$my_save_CFLAGS"

############################################################################################
# #pragma GCC diagnostic error "-Wincompatible-pointer-types"

my_save_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS -Werror"
AC_LANG_PUSH([C])
AC_MSG_CHECKING([if the compiler supports pragma for GCC diagnostic error -Wincompatible-pointer-types])
AC_LINK_IFELSE(
[AC_LANG_SOURCE([[
  #pragma GCC diagnostic error "-Wincompatible-pointer-types"
  #include <stdio.h>
  #include <stdlib.h>
  int main()
  {
    return 0;
  }]] )],
  [
    AC_MSG_RESULT([yes])
    compiler_has_pragma_for_incompatible_pointer_types=1
    AC_DEFINE([COMPILER_HAS_PRAGMA_FOR_INCOMPATIBLE_POINTER_TYPES],[1],[Compiler supports pragma for GCC diagnostic error -Wincompatible-pointer-types])
  ],
  [
    AC_MSG_RESULT([no])
    compiler_has_pragma_for_incompatible_pointer_types=0
  ] )
  AC_LANG_POP([C])
CFLAGS="$my_save_CFLAGS"

############################################################################################
#    qsort_r - definition for GNU C and BSD differ, other platforms may not have it at all
#              best to use libc version if available; otherwise, internal implementation

AC_CHECK_FUNC(qsort_r, [AC_DEFINE(HAVE_QSORT_R,[1],[whether or not qsort_r is available] ) ])

# if it does have qsort_r then make sure it has the BSD-ordered parameters (Linux typically does not)
# if it doesn't, then I use my already written equivalent version; otherwise use libc's version
if test "$ac_cv_func_qsort_r" == yes ; then
  my_save_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS -Werror"

  # if I have support for the pointer mismatch pragma, add this to CFLAGS
  if test $compiler_has_pragma_for_incompatible_pointer_types -ne 0 ; then
    CFLAGS="$CFLAGS -DCOMPILER_HAS_PRAGMA_FOR_INCOMPATIBLE_POINTER_TYPES"
  fi
  AC_LANG_PUSH([C])
  AC_MSG_CHECKING([that qsort_r is the BSD version])
  AC_LINK_IFELSE(
  [
    AC_LANG_SOURCE(
    [[
      #ifdef COMPILER_HAS_PRAGMA_FOR_INCOMPATIBLE_POINTER_TYPES
      #pragma GCC diagnostic error "-Wincompatible-pointer-types"
      #else
      #error For now we disallow this, to protect QSORT_R calls, and only use the internal version
      #endif // COMPILER_HAS_PRAGMA_FOR_INCOMPATIBLE_POINTER_TYPES

      #include <stdio.h>
      #include <stdlib.h>
      int unsorted[16]={1,3,5,7,9,11,13,15,2,4,6,8,10,12,14,16};
      int bsd_sort_compare(void *a, const void *b, const void *c)
      {
        const int *p1, *p2;
        if(a != (void *)unsorted)
        {
          exit(2);
        }

        p1 = (const int *)b;
        p2 = (const int *)c;
        if(*p1 > *p2)
        {
          return 1;
        }
        else if(*p2 > *p1)
        {
          return -1;
        }

        return 0;
      }
      int main()
      {
        int i1;
        qsort_r(unsorted, 16, sizeof(unsorted[0]), (void *)unsorted, bsd_sort_compare);

        for(i1=1; i1 < 16; i1++)
        {
          if(unsorted[i1 - 1] + 1 != unsorted[i1])
          {
            exit(3);
          }
        }

        return 0;
      }
    ]] )
  ],
  [
    AC_MSG_RESULT([yes])
    AC_DEFINE([QSORT_R_BSD],[1],[BSD version of qsort_r])
  ],
  [
    AC_MSG_RESULT([no])
    AC_DEFINE([QSORT_R_GNUC],[1],[GNUC version of qsort_r])
  ] )
  AC_LANG_POP([C])

  CFLAGS="$my_save_CFLAGS"
else
  AC_MSG_WARN([qsort_r not compatible; using internal qsort_r implementation])
fi

############################################################################################


############################################################################################
#    LONG LONG DATATYPE - checking for support (simple)
#

AC_LANG_PUSH([C])
  AC_MSG_CHECKING([that the 'long long' data type is supported])
  AC_LINK_IFELSE(
  [AC_LANG_SOURCE([[
    #include <stdio.h>
    #include <stdlib.h>
    int main()
    {
      static long long llTemp1 = 0;
      static unsigned long long ullTemp2 = 0;

      return 0;
    }]] )],
    [
      AC_MSG_RESULT([yes!!! (supported)])
      AC_DEFINE([HAVE_LONGLONG],[1],[ defined whenever 'long long' is supported ])
    ],
    [
      AC_MSG_RESULT([no (not supported)])
    ] )
AC_LANG_POP([C])

############################################################################################


AC_C_CONST
AC_HEADER_STDC

AC_CANONICAL_HOST


# optional things for Interix vs the rest of the world
# Interix is deprecated and I should get rid of this
case $host in
  i*86-pc-interix*)
    MAN_PAGES="X11workbench.interix"
    ;;
  *)
    MAN_PAGES="X11workbench.1"
    ;;
esac

if test x$test_have_xpm = xtrue ; then
  AC_DEFINE([X11WORKBENCH_TOOLKIT_HAVE_XPM],[1],[whether or not libXpm should be used])
fi

if test x$test_have_xft = xtrue ; then
  AC_DEFINE([X11WORKBENCH_TOOLKIT_HAVE_XFT],[1],[whether or not libXft should be used])
  # NOTE:  the AC_CHECK_FT2 macro SHOULD HAVE already modified CFLAGS and LIBS
  #        however, whenever I look at the output, I don't see it.  So...
  CFLAGS="$CFLAGS $FT2_CFLAGS"
  CXXFLAGS="$CXXFLAGS $FT2_CFLAGS"
  CPPFLAGS="$CPPFLAGS $FT2_CFLAGS"
  LIBS="$LIBS $FT2_LIBS"
fi

if test x$test_have_xext = xtrue ; then
  AC_DEFINE([X11WORKBENCH_TOOLKIT_HAVE_XEXT],[1],[whether or not libXext should be used])
fi



# optional things for cygwin vs the rest of the world
case $host in
# typically i686-pc-cygwin
  *-*-cygwin)
    is_cygwin=true
    ;;
  *)
    is_cygwin=false
    ;;
esac



#################################################################
# ADDITIONAL GLOBAL DEFINES (things to export)

# CONDITIONALS

AM_CONDITIONAL(IS_CYGWIN, test x$is_cygwin = xtrue)
AM_CONDITIONAL(NOT_CYGWIN, test x$is_cygwin = xfalse)

## LIBRARY LINKAGE STUFF

# XPM library (program will use its own implementation when libXpm is not available)
# note that this conditional defines the linkage, not the library include symbols
AM_CONDITIONAL(HAVE_XPM_LIB, test x$test_have_xpm = xtrue )

# XFT library - use libXft for certain rendering operations when available
AM_CONDITIONAL(HAVE_XFT_LIB, test x$test_have_xft = xtrue )

# XExt library - use libXext for things like XShm API functions
AM_CONDITIONAL(HAVE_XEXT_LIB, test x$test_have_xext = xtrue)

# additional library linkages
AM_CONDITIONAL(NEED_LRT, test x$test_need_lrt = xtrue)
AM_CONDITIONAL(NEED_LDL, test x$test_need_ldl = xtrue)


# Additional global defines go here

NUM_CORES="$ac_num_cores"

GLOBAL_XPATH=`echo "$ac_x_libraries" | sed 's/\/lib//g'`
GLOBAL_XPATH="$GLOBAL_XPATH"
#################################################################



#############################
# MULTI-CORE BUILDS via '-j'
#############################

# Whenever the host (build) system has more than one core, this
# will automatically invoke '-j' to use one job per available core.
# experimentation shows that specifying the '-j' argument on
# the make command line overrides this...

if test $NUM_CORES -gt 1 ; then
  AM_MAKEFLAGS="-j \$(NUM_CORES) $AM_MAKEFLAGS"
fi





#################################################################
# Makefile variable SUBSTITUTIONS (so they go into Makefile)

# After all of that, I need the variable substitutions.  They go here.
# This is for stuff I did not actually create a var already for, that
# needs to be defined in Makefile

# additional substitutions for man pages and X11 libraries

AC_SUBST([MAN_PAGES])
AC_SUBST([GLOBAL_XPATH])
AC_SUBST([NUM_CORES])
AC_SUBST([IS_CYGWIN])
AC_SUBST([NOT_CYGWIN])
AC_SUBST([HAVE_XPM_LIB])
AC_SUBST([HAVE_XFT_LIB])
AC_SUBST([HAVE_XEXT_LIB])

AC_SUBST([CFLAGS])
AC_SUBST([CPPFLAGS])
AC_SUBST([CXXFLAGS])
AC_SUBST([LIBS])
AC_SUBST([AM_MAKEFLAGS])


########################################################################


# the end

AC_CONFIG_FILES([Makefile lib/Makefile])

AC_OUTPUT


