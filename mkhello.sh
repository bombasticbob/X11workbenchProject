#!/bin/sh

CURDIR=`pwd`

gcc -Wall -o hello\
    -I. -I${CURDIR}/include -I${CURDIR}/xpm -I/usr/local/include/X11 -I/usr/local/include \
    lib/clipboard_helper.c lib/dialog_support.c lib/file_help.c lib/menu_bar.c lib/text_object.c \
    lib/conf_help.c lib/dialog_window.c lib/font_helper.c lib/menu_popup.c lib/window_dressing.c \
    lib/dialog_controls.c lib/draw_text.c lib/frame_window.c lib/pixmap_helper.c lib/window_helper.c \
    lib/dialog_impl.c lib/edit_window.c lib/menu.c  lib/platform_helper.c lib/child_frame.c \
    -DGLOBAL_PATH='"/usr/local/etc"' -DGLOBAL_XPATH='"/usr/local/etc"' \
    -DHAVE_XPM -L/usr/local/lib -lX11 -lXpm -lXRes -lm -lpthread -lrt  hello.c


