//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                 _  _          _            _                             //
//       __ _   __| || |__      | |__    ___ | | _ __    ___  _ __  ___     //
//      / _` | / _` || '_ \     | '_ \  / _ \| || '_ \  / _ \| '__|/ __|    //
//     | (_| || (_| || |_) |    | | | ||  __/| || |_) ||  __/| | _| (__     //
//      \__, | \__,_||_.__/_____|_| |_| \___||_|| .__/  \___||_|(_)\___|    //
//      |___/             |_____|               |_|                         //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

/*****************************************************************************

    X11workbench - X11 programmer's 'work bench' application and toolkit
    Copyright (c) 2010-2016 by Bob Frazier (aka 'Big Bad Bombastic Bob')
                             all rights reserved

  DISCLAIMER:  The X11workbench application and toolkit software are supplied
               'as-is', with no warranties, either implied or explicit.
               Any claims to alleged functionality or features should be
               considered 'preliminary', and might not function as advertised.

  BSD-like license:

  There is no restriction as to what you can do with this software, so long
  as you include the above copyright notice and DISCLAIMER for any distributed
  work that is equal to or derived from this one, along with this paragraph
  that explains the terms of the license if the source is also being made
  available.  A "derived work" describes a work that uses a significant portion
  of the source files or algorithms that are included with this one.
  Specifically excluded from this are files that were generated by the software,
  or anything that is included with the software that is part of another package
  (such as files that were created or added during the 'configure' process).
  Specifically included is the use of part or all of any of the X11 workbench
  toolkit source or header files in your distributed application.  If you do not
  ship the source, the above copyright statement is still required to be placed
  in a reasonably prominent place, such as documentation, splash screens, and/or
  'about the application' dialog boxes.

  Use and distribution are in accordance with GPL, LGPL, and/or the above
  BSD-like license.  See COPYING and README files for more information.


  Additional information at http://sourceforge.net/projects/X11workbench

******************************************************************************/



#include "X11workbench.h"
#include "gdb_helper.h"

#include "platform_helper.h"


// NOTES:
//
// command line gdb {executable} where '{executable}' is the binary executable file
//
// (NOTE:  output of 'gdb' typically ends with the text "(gdb)" which then waits for input)
//
// commands from within gdb that should be useful:
//
// set height 0
//   this will eliminate the "Type <return> to continue, or q <return> to quit" prompting
//
//
// list ,main
//   this will set the 'main()' source file as "current source" and allow 'info source' to work
//
// info source
//   information on the 'current' source file being displayed.  output similar to this:
//     Current source file is X11workbench.c
//     Compilation directory is /d-drive/SecureStorage/root/source/X11workbench
//     Located in /d-drive/SecureStorage/root/source/X11workbench/X11workbench.c
//     Contains 2306 lines.
//     Source language is c.
//     Compiled with DWARF 2 debugging format.
//     Does not include preprocessor macro info.
//
//
// info sources
//   information on ALL source files.  output similar to this:
//     Source files for which symbols have been read in:
//
//     X11workbench.c, file_help.h, window_helper.h
//
//     Source files for which symbols will be read in on demand:
//
//     dialog_controls.c, dialog_window.c, clipboard_helper.c, text_object.c, platform_helper.c, window_dressing.c, draw_text.c,
//     file_help.c, conf_help.c, menu_popup.c, menu_bar.c, menu.c, dialog_support.c, dialog_impl.c, frame_window.c, child_frame.c,
//     edit_window.c, font_helper.c, window_helper.c, pixmap_helper.c, gizzard.c
//
//
// info function:
//
//   command:  info function WBInit\b   <-- the '\b' causes an exact match
//
//   output:
//
//     All functions matching regular expression "WBInit\b":
//
//     File window_helper.c:
//     Display *WBInit(const char *);
//
//
// info functions
//   this gives you a list of ALL functions and the source files that define them. example:
//
//     All defined functions:
//
//     File pixmap_helper.c:
//     Pixmap PXM_GetIconPixmap(int, XPM_ATTRIBUTES *, Pixmap *);
//     Pixmap PXM_GetIconPixmapFromAtom(Atom, XPM_ATTRIBUTES *, Pixmap *);
//       ...
//     static int clip255(int);
//
//     File font_helper.c:
//     XFontStruct *WBCopyFont(XFontStruct *);
//     XFontSet WBCopyModifyFontSet(Display *, XFontSet, int, int);
//     void WBDumpFontInfo(const char *);
//     int WBFontAvgCharWidth(Display *, const XFontStruct *);
//     ...
//
//     etc.
//
//   the list will include static functions appropriate to each source file, and ends with
//   a bunch of addresses labeled 'non-debugging symbols':
//
//     Non-debugging symbols:
//     0x0000000000404248  _init
//     0x000000000040426c  XSetInputFocus
//     0x000000000040427c  XFreePixmap
//     ...
//     0x0000000000405070  _start
//     0x0000000000405110  __do_global_dtors_aux
//     0x0000000000405150  frame_dummy
//     0x0000000000453f80  __do_global_ctors_aux
//     0x0000000000453fb4  _fini
//
//   Apparently, none of these have source available (typically contained in a linked-to library)
//
//
//
// other commands like list, backtrace, breakpoint, etc. can be applied as needed.
//
//




