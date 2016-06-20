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
AC_PROG_CC
AC_PROG_RANLIB
AC_PATH_X
AC_PATH_XTRA
AC_TYPE_SIGNAL
AC_PROG_AWK
AC_PROG_LN_S


# --enable-release (default has debugging enabled)
# --enable-noopt (default is optimization enabled)
# --enable-libXpm (default is NOT to link to libXpm)
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

AC_SUBST([CFLAGS])
AC_SUBST([CPPFLAGS])
AC_SUBST([CXXFLAGS])


#################################################################
# header file and function checks (some recommended by autoscan)
#################################################################

AC_CHECK_HEADER([unistd.h],[],AC_MSG_ERROR([missing unistd.h]))
AC_CHECK_HEADER([X11/Xlib.h],[],AC_MSG_ERROR([missing Xlib.h]))
AC_CHECK_HEADER([X11/Xmd.h],[],AC_MSG_ERROR([missing Xmd.h]))
AC_CHECK_HEADER([X11/Xutil.h],[],AC_MSG_ERROR([missing Xutil.h]))
AC_CHECK_HEADER([X11/Xatom.h],[],AC_MSG_ERROR([missing Xatom.h]))


# This is where I check for the presence of libXpm - if the configure script uses the
# option '--enable-libXpm' then I will test for its presence.  Otherwise, the internal
# implementation will be used, at least as far as the libraries are concerned.  This
# would not prevent an application from using the pre-compiled libraries and ALSO use
# libXpm.  It would only prevent the X11 toolkit library itself from linking to it.

test_have_xpm=false
if test x$libXpm = xtrue ; then
  AC_CHECK_HEADER([X11/xpm.h],test_have_xpm=true,AC_MSG_WARN([missing xpm.h - you should consider installing libXpm]))
fi

AC_CHECK_FUNCS([printf])
AC_CHECK_FUNCS([alarm])
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([nanosleep])

# the 'clock_gettime' function might require '-lrt'
AC_CHECK_FUNCS([clock_gettime])

AC_CHECK_LIB(rt, clock_gettime, test_need_lrt=true)
AC_CHECK_LIB(dl, dlopen, test_need_ldl=true)

# custom function checks go here

# malloc_usable_size - mostly for debugging purposes, not supported everywhere

AC_CHECK_FUNC(malloc_usable_size, [AC_DEFINE(HAVE_MALLOC_USABLE_SIZE,[1],[whether or not malloc_usable_size is available] ) ])

############################################################################################
#    qsort_r - definition for GNU C and BSD differ, other platforms may not have it at all
#              best to use libc version if available; otherwise, internal implementation

AC_CHECK_FUNC(qsort_r, [AC_DEFINE(HAVE_QSORT_R,[1],[whether or not qsort_r is available] ) ])

if test "$ac_cv_func_qsort_r" == yes ; then
  AC_LANG_PUSH([C])
  AC_MSG_CHECKING([that qsort_r is the BSD version])
  AC_RUN_IFELSE(
  [AC_LANG_SOURCE([[
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
    }]] )],
    [
      AC_MSG_RESULT([yes])
      AC_DEFINE([QSORT_R_BSD],[1],[BSD version of qsort_r])
    ],
    [
      AC_MSG_RESULT([no])
      AC_DEFINE([QSORT_R_GNUC],[1],[GNUC version of qsort_r])
    ] )
    AC_LANG_POP([C])
else
  AC_MSG_WARN([qsort_r not compatible; using internal qsort_r implementation])
fi

############################################################################################

AC_C_CONST
AC_HEADER_STDC

AC_CANONICAL_HOST


# optional things for Interix vs the rest of the world
case $host in
  i*86-pc-interix*)
    MAN_PAGES="X11workbench.interix"
    ;;
  *)
    MAN_PAGES="X11workbench.1"
    ;;
esac

if test x$test_have_xpm = xtrue ; then
  AC_DEFINE([X11WORKBENCH_TOOLKIT_HAVE_XPM],[1],[whether or not XPM header and libraries are available])
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

AM_CONDITIONAL(IS_CYGWIN, test x$is_cygwin = xtrue)
AM_CONDITIONAL(NOT_CYGWIN, test x$is_cygwin = xfalse)

########################
## LIBRARY LINKAGE STUFF
########################

# XPM library (program will use its own implementation when libXpm is not available)
# note that this conditional defines the linkage, not the library include symbols
AM_CONDITIONAL(HAVE_XPM_LIB, test x$test_have_xpm = xtrue )

# additional library linkages
AM_CONDITIONAL(NEED_LRT, test x$test_need_lrt = xtrue)
AM_CONDITIONAL(NEED_LDL, test x$test_need_ldl = xtrue)


GLOBAL_XPATH=`echo "$ac_x_libraries" | sed 's/\/lib//g'`
GLOBAL_XPATH="$GLOBAL_XPATH"

# additional substitutions for man pages and X11 libraries

AC_SUBST([MAN_PAGES])
AC_SUBST([GLOBAL_XPATH])
AC_SUBST([IS_CYGWIN])
AC_SUBST([NOT_CYGWIN])
AC_SUBST([HAVE_XPM_LIB])


# the end

AC_CONFIG_FILES([Makefile lib/Makefile])

AC_OUTPUT


