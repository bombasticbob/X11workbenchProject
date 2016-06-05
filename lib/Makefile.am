noinst_LIBRARIES = libX11WorkbenchToolkit.a
pkglib_LIBRARIES = libX11WorkbenchToolkit_debug.a libX11WorkbenchToolkit_release.a

libX11WorkbenchToolkit_a_SOURCES = pixmap_helper.c window_helper.c font_helper.c edit_window.c  child_frame.c
libX11WorkbenchToolkit_a_SOURCES += frame_window.c dialog_window.c dialog_impl.c dialog_controls.c dialog_support.c
libX11WorkbenchToolkit_a_SOURCES += menu.c menu_bar.c menu_popup.c conf_help.c file_help.c draw_text.c
libX11WorkbenchToolkit_a_SOURCES += window_dressing.c platform_helper.c text_object.c clipboard_helper.c

pkginclude_HEADERS = ../include/pixmap_helper.h ../include/edit_window.h ../include/child_frame.h
pkginclude_HEADERS += ../include/window_helper.h ../include/font_helper.h ../include/frame_window.h
pkginclude_HEADERS += ../include/dialog_window.h ../include/dialog_controls.h ../include/dialog_support.h
pkginclude_HEADERS += ../include/menu.h ../include/menu_bar.h ../include/menu_popup.h ../include/conf_help.h
pkginclude_HEADERS += ../include/file_help.h ../include/draw_text.h ../include/window_dressing.h
pkginclude_HEADERS += ../include/platform_helper.h ../include/text_object.h

libX11WorkbenchToolkit_a_SOURCES += $(pkginclude_HEADERS)

libX11WorkbenchToolkit_a_SOURCES += ../xpm/icon_app.xpm ../xpm/icon_bang.xpm ../xpm/icon_barney.xpm ../xpm/icon_bear.xpm 
libX11WorkbenchToolkit_a_SOURCES += ../xpm/icon_death.xpm ../xpm/icon_finger.xpm ../xpm/icon_ok.xpm ../xpm/icon_skull.xpm ../xpm/icon_splat.xpm
libX11WorkbenchToolkit_a_SOURCES += ../xpm/icon_stop.xpm ../xpm/icon_triangle.xpm ../xpm/icon_warn.xpm ../xpm/icon_what.xpm ../xpm/icon_what_bold.xpm
libX11WorkbenchToolkit_a_SOURCES += ../xpm/icon_wtf.xpm ../xpm/icon_thumbup.xpm ../xpm/icon_thumbdown.xpm


AM_CPPFLAGS = $(X_CFLAGS) -DGLOBAL_XPATH="\"$(GLOBAL_XPATH)/etc\"" -DGLOBAL_PATH="\"$(sysconfdir)\""

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

AM_CFLAGS = $(NO_OPT_FLAGS)

if NO_DEBUG

libX11WorkbenchToolkit_a_CFLAGS = \
   $(CFLAGS) -DNO_DEBUG -Wall -Werror -fno-strict-aliasing

else

libX11WorkbenchToolkit_a_CFLAGS = \
   $(CFLAGS) $(EXTRA_WARN_FLAGS) -Wimplicit-int \
   -Wimplicit-function-declaration -Wformat -Wmissing-braces \
   -Wnonnull -Wparentheses -Wreturn-type \
   -Wsequence-point -Wswitch -Wtrigraphs \
   -Wuninitialized -Wunknown-pragmas -Wunused-label \
   -fno-strict-aliasing

endif

libX11WorkbenchToolkit_release_a_SOURCES = $(libX11WorkbenchToolkit_a_SOURCES)
libX11WorkbenchToolkit_debug_a_SOURCES = $(libX11WorkbenchToolkit_a_SOURCES)


libX11WorkbenchToolkit_debug_a_CFLAGS = \
   $(CFLAGS) $(EXTRA_WARN_FLAGS) -Wimplicit-int \
   -Wimplicit-function-declaration -Wformat -Wmissing-braces \
   -Wnonnull -Wparentheses -Wreturn-type \
   -Wsequence-point -Wswitch -Wtrigraphs \
   -Wuninitialized -Wunknown-pragmas -Wunused-label \
   -fno-strict-aliasing

libX11WorkbenchToolkit_release_a_CFLAGS = \
   $(CFLAGS) -DNO_DEBUG -Wall -Werror -fno-strict-aliasing

# special targets - 'release-lib' and 'debug-lib'

release-lib: libX11WorkbenchToolkit_release.a

debug-lib: libX11WorkbenchToolkit_debug.a


