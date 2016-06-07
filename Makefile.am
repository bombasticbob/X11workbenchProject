bin_PROGRAMS = X11workbench
X11workbench_SOURCES = X11workbench.c X11workbench.h refactor.c refactor.h gizzard.c gizzard.h
X11workbench_SOURCES += application_icon.xpm splash.xpm
X11workbench_SOURCES += doxy_comments.dox doxy.txt

X11workbench_DEPENDENCIES = toolkit_lib

AM_CPPFLAGS = $(X_CFLAGS) -DGLOBAL_XPATH="\"$(GLOBAL_XPATH)/etc\"" -DGLOBAL_PATH="\"$(sysconfdir)\""


if HAVE_XPM_LIB
  X_EXTRA_LIBS += -lXpm
endif

# note:  was using AM_LDFLAGS but Ubuntu build borked, so using LIBS instead
#AM_LDFLAGS = $(X_LIBS) -lm -lX11 $(X_PRE_LIBS) $(X_EXTRA_LIBS)
LDADD=./lib/libX11workbenchToolkit.a
LIBS = -L./lib -lX11workbenchToolkit $(X_LIBS) -lm -lX11 $(X_PRE_LIBS) $(X_EXTRA_LIBS)

# any additional libraries go here
LIBS += -lpthread

if NEED_LRT
  LIBS += -lrt
endif

if NEED_LDL
  LIBS += -ldl
endif

# MAN pages
man1_MANS = @MAN_PAGES@

# Extra distribution files
EXTRA_DIST = BUILD X11workbench.1 X11workbench.interix $(man1_MANS)

# comment out this section to include external 'man2html' perl script
## man2html distribution files - man2html version 3.0.1 license under GPLv2
## web site http://www.oac.uci.edu/indiv/ehood/man2html.html
##
## what you see here is a copy of the contents of the tarball as GPL compliance
## I have to use this package directly since it might not exist and there
## are OTHER 'man2html' packages, such as those for debian, that do not
## work even remotely the same as this one
## This program is ONLY run as an external utility and is not 'compiled in'
## and is not necessary for X11workbench to run.  Only specific functionality
## relies on it, and it could be substituted with a different utility provided
## that the different utility gives you the same functionality
##
#EXTRA_DIST += man2html/man2html man2html/README man2html/COPYING
#EXTRA_DIST += man2html/CHANGES man2html/MANIFEST man2html/man.cgi
#EXTRA_DIST += man2html/doc/man2html.1 man2html/doc/man2html.html
#EXTRA_DIST += man2html/doc/man2html.txt man2html/doc/man2html.ps
#EXTRA_DIST += man2html/install.me
#
#bin_PROGRAMS += X11workbench_man2html
#
## build target for man2html, installed in the same dir as X11workbench
## following example from automake docs, use 'low res time' for dest
#
#.LOW_RESOLUTION_TIME: X11workbench_man2html$(EXEEXT)
#X11workbench_man2html$(EXEEXT):	man2html/man2html
#	cp -p man2html/man2html $@
#	sed -i".bak" 's/\/usr\/local\/bin\/perl/'`which perl | sed 's/\//\\\\\//g'`'/' $@
#	rm -f $@".bak"
#	chmod 755 $@
#

if NO_OPT
  NO_OPT_FLAGS = -O0
else
  NO_OPT_FLAGS =
endif

# cygwin does not support cetain compiler warnings
if NOT_CYGWIN

  EXTRA_WARN_FLAGS= -Waddress -Wpointer-sign \
   -Wstrict-overflow=1 -Wvolatile-register-var

else

  EXTRA_WARN_FLAGS=

endif

AM_CFLAGS = $(NO_OPT_FLAGS) -DX11WORKBENCH_PROJECT_BUILD

if NO_DEBUG

  AM_CFLAGS += -DNO_DEBUG -Wall -Werror -fno-strict-aliasing

else

  AM_CFLAGS += $(EXTRA_WARN_FLAGS) -Wimplicit-int \
   -Wimplicit-function-declaration -Wformat -Wmissing-braces \
   -Wnonnull -Wparentheses -Wreturn-type \
   -Wsequence-point -Wswitch -Wtrigraphs \
   -Wuninitialized -Wunknown-pragmas -Wunused-label \
   -fno-strict-aliasing

endif

# subdirectories containing additional sources
SUBDIRS = lib



# pseudo target for building the library

.PHONY: toolkit_lib

lib/libX11workbenchToolkit.a: toolkit_lib

toolkit_lib:
	$(MAKE) -C lib libX11workbenchToolkit.a


