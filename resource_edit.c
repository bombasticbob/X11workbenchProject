///////////////////////////////////////////////////////////////////////////////////////
//                                                                                   //
//                                                               _  _  _             //
//   _ __  ___  ___   ___   _   _  _ __  ___  ___       ___   __| |(_)| |_     ___   //
//  | '__|/ _ \/ __| / _ \ | | | || '__|/ __|/ _ \     / _ \ / _` || || __|   / __|  //
//  | |  |  __/\__ \| (_) || |_| || |  | (__|  __/    |  __/| (_| || || |_  _| (__   //
//  |_|   \___||___/ \___/  \__,_||_|   \___|\___|_____\___| \__,_||_| \__|(_)\___|  //
//                                               |_____|                             //
//                                                                                   //
///////////////////////////////////////////////////////////////////////////////////////

/*****************************************************************************

    X11workbench - X11 programmer's 'work bench' application and toolkit
    Copyright (c) 2010-2019 by Bob Frazier (aka 'Big Bad Bombastic Bob')
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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <memory.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

// project includes
#include "X11workbench.h"
#include "resource_edit.h"


// Define resources as entries within the code marked by defined constants
//
// example - menu resource
//
// #define BEGIN_MENU_RESOURCE(X) static const char X##__menurc[] =
// #define END_MENU_RESOURCE(X) ;
//
// ...
//
// BEGIN_MENU_RESOURCE(main_menu)
//   "1\n"
//   "_File\tpopup\t2\n"
//   "_Edit\tpopup\t4\n"
//   "_Tools\tpopup\t5\n"
//   "\tseparator\n"
//   "_Window\tpopup\t6\n"
//   "\tseparator\n"
//   "_Help\tpopup\t3\n"
//   "\n"
//
//   ...
//
// END_MENU_RESOURCE(main_menu)
//
//
// The code in THIS file could then search through the program for such definitions,
// and edit them 'in-place'.  There would be a set of definitions "someplace" in the
// toolkit that would define menu resources, dialog resources, toolbar resources, and
// possibly pixmap and icon resources.
//
// as a side benefit, the editor could add macros at the ends of the 'BEGIN_xxx_RESOURCE'
// lines that indicates it was edited with the resource editor.



