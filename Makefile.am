bin_PROGRAMS = X11workbench
X11workbench_SOURCES = X11workbench.c X11workbench.h refactor.c refactor.h gizzard.c gizzard.h
X11workbench_SOURCES += gdb_helper.c gdb_helper.h
X11workbench_SOURCES += application_icon.xpm splash.xpm textfiledoc.xpm newdoc.xpm clangdoc.xpm makefiledoc.xpm
X11workbench_SOURCES += doxy_comments.dox doxy.txt

X11workbench_DEPENDENCIES = toolkit_lib

# allow multiple simultaneous jobs to speed up automake process
AUTOMAKE_JOBS = 4

AM_CPPFLAGS = $(X_CFLAGS) -DGLOBAL_XPATH="\"$(GLOBAL_XPATH)/etc\"" -DGLOBAL_PATH="\"$(sysconfdir)\""


# this allows me to eliminate a few files, like README  Since I have included
# 'README.md' in EXTRA_DIST (see below), I'll get THAT one and any others
# that are present.  But automake won't ENFORCE them.
AUTOMAKE_OPTIONS = foreign


# presence of XPM library.  use --enable-libXpm in configure to enable it
if HAVE_XPM_LIB
  X_EXTRA_LIBS += -lXpm
endif

# note:  was using AM_LDFLAGS but Ubuntu build borked, so using LIBS instead
#AM_LDFLAGS = $(X_LIBS) -lm -lX11 $(X_PRE_LIBS) $(X_EXTRA_LIBS)
LDADD=./lib/libX11workbenchToolkit.a
LIBS = -L./lib -lX11workbenchToolkit $(X_LIBS) -lm -lX11 $(X_PRE_LIBS) $(X_EXTRA_LIBS)

# any additional libraries go here
LIBS += -lpthread -lm

if NEED_LRT
  LIBS += -lrt
endif

if NEED_LDL
  LIBS += -ldl
endif

# MAN pages
man1_MANS = @MAN_PAGES@

# Extra distribution files
EXTRA_DIST = README.md BUILD X11workbench.1 X11workbench.interix $(man1_MANS)


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

  AM_CFLAGS += -DNO_DEBUG -Wall -fno-strict-aliasing

if FATAL_WARN

  AM_CFLAGS += -Werror

endif

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


