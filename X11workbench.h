//////////////////////////////////////////////////////////////////////////////////////////
//  __  __ _  _                        _     _                         _         _      //
//  \ \/ // |/ |__      __ ___   _ __ | | __| |__    ___  _ __    ___ | |__     | |__   //
//   \  / | || |\ \ /\ / // _ \ | '__|| |/ /| '_ \  / _ \| '_ \  / __|| '_ \    | '_ \  //
//   /  \ | || | \ V  V /| (_) || |   |   < | |_) ||  __/| | | || (__ | | | | _ | | | | //
//  /_/\_\|_||_|  \_/\_/  \___/ |_|   |_|\_\|_.__/  \___||_| |_| \___||_| |_|(_)|_| |_| //
//                                                                                      //
//                          'main' source for X11workbench                              //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////

/*****************************************************************************

    X11workbench - X11 programmer's 'work bench' application and toolkit
    Copyright (c) 2010-2018 by Bob Frazier (aka 'Big Bad Bombastic Bob')
                           all rights reserved

  DISCLAIMER:  The X11workbench application and toolkit software are supplied
               'as-is', with no warranties, either implied or explicit.

  BSD-like license:

  There is no restriction as to what you can do with this software, so long
  as you include the above copyright notice and DISCLAIMER for any distributed
  work that is linked with, equivalent to, or derived from any portion of this
  software, along with this paragraph that explains the terms of the license if
  the source is also being made available.  "Linked with" includes the use of a
  portion of any of the source and/or header files, or their compiled binary
  output, as a part of your application or library.   A "derived work"
  describes a work that uses a significant portion of the source files or the
  algorithms that are included with this software.

  EXCLUSIONS

  Specifically excluded from this requirement are files that were generated by
  the software, or anything that is included with the software that is part of
  another package (such as files that were created or added during the
  'configure' process).

  DISTRIBUTION

  The license also covers the use of part or all of any of the X11 workbench
  toolkit source or header files in your distributed application, in source or
  binary form.  If you do not ship the source, the above copyright statement
  and DISCLAIMER is still required to be placed in a reasonably prominent
  place, such as documentation, splash screens, and/or 'about the application'
  dialog boxes.

  Use and distribution are in accordance with GPL, LGPL, and/or the above
  BSD-like license.  See COPYING and README.md files for more information.

  Additionally, this software, in source or binary form, and in whole or in
  part, may be used by explicit permission from the author, without the need
  of a license.

  Additional information at http://sourceforge.net/projects/X11workbench
  and http://bombasticbob.github.io/X11workbench/

******************************************************************************/


// common includes (this also includes 'X' headers)
#include "window_helper.h"


#define APP_NAME "X11workbench" /* for dialog boxes and similar things */


typedef enum __WBFILE_TYPE__
{
  WBFILE_TYPE_NONE     = 0,
  WBFILE_TYPE_TEXT     = 1,
  WBFILE_TYPE_CPROG    = 2,
  WBFILE_TYPE_CPP      = 3,
  WBFILE_TYPE_CHEADER  = 4,
  WBFILE_TYPE_MAKEFILE = 5,
  WBFILE_TYPE_AUTOCONF = 6,
  WBFILE_TYPE_PROJECT  = 7,
  WBFILE_TYPE_RESOURCE = 8,
  WBFILE_TYPE_PIXMAP   = 9,
  WBFILE_TYPE_DIALOG   = 10,
  WBFILE_TYPE_MENU     = 11,
  WBFILE_TYPE_TOOLBAR  = 12,
  WBFILE_TYPE_SHELL    = 13,
  WBFILE_TYPE_PYTHON   = 14,
  WBFILE_TYPE_PERL     = 15,
  WBFILE_TYPE_ASM      = 16,
  WBFILE_TYPE_HTML     = 17,
  WBFILE_TYPE_XML      = 18,
  WBFILE_TYPE_JAVA     = 19,
  WBFILE_TYPE_JS       = 20,
  WBFILE_TYPE_PHP      = 21,
  WBFILE_TYPE_ARDUINO  = 22, // PDE or INO file for Arduino script

  WBFILE_TYPE_LAST     = 22 // update this if I add more types
} WBFILE_TYPE;


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                       //
//   _____        _                             _                          _      _     ____  ___        //
//  | ____|__  __| |_  ___  _ __  _ __    __ _ | | ___    __ _  _ __    __| |    / \   |  _ \|_ _| ___   //
//  |  _|  \ \/ /| __|/ _ \| '__|| '_ \  / _` || |/ __|  / _` || '_ \  / _` |   / _ \  | |_) || | / __|  //
//  | |___  >  < | |_|  __/| |   | | | || (_| || |\__ \ | (_| || | | || (_| |  / ___ \ |  __/ | | \__ \  //
//  |_____|/_/\_\ \__|\___||_|   |_| |_| \__,_||_||___/  \__,_||_| |_| \__,_| /_/   \_\|_|   |___||___/  //
//                                                                                                       //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

// external types (prototype definitions for functions)
typedef struct __WB_FRAME_WINDOW__ WBFrameWindow; // see frame_window.h


// global variables

// TODO:  any globals?  get/set methods are better, like GetFrameWindowID() below


Window GetFrameWindowID(); // get the frame window's window ID (for dialog boxen and such)

// general purpose utilities to be called by external things

int DoFileOpen(WBFrameWindow *pMainFrame, const char *szFileName);
WBFILE_TYPE GetFileType(const char *szFileName);
const char *GetFileTypeDesc(WBFILE_TYPE nFileType);
int GetDefaultTabSetting(WBFILE_TYPE nFileType);
int GetDefaultLineEnding(WBFILE_TYPE nFileType);
const char *GetFileTypeHighlightInfo(WBFILE_TYPE nFileType);





