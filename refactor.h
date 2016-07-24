/////////////////////////////////////////////////////////////////////////////////////////////
//                               __               _                _                       //
//                   _ __  ___  / _|  __ _   ___ | |_  ___   _ __ | |__                    //
//                  | '__|/ _ \| |_  / _` | / __|| __|/ _ \ | '__|| '_ \                   //
//                  | |  |  __/|  _|| (_| || (__ | |_| (_) || | _ | | | |                  //
//                  |_|   \___||_|   \__,_| \___| \__|\___/ |_|(_)|_| |_|                  //
//                                                                                         //
//                  Code 're-factor' utility - stand-alone or utility API                  //
//                                                                                         //
/////////////////////////////////////////////////////////////////////////////////////////////

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


#ifndef _REFACTOR_H_INCLUDED_
#define _REFACTOR_H_INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


enum
{
    refactor_symbol_functions = 1,
    refactor_symbol_variables = 2,
    refactor_symbol_macros = 4,
    refactor_symbol_classes = 8,
    refactor_symbol_members = 0x10,

    refactor_symbol_all = 0x7fffffff,
    refactor_symbol_nocase = 0x80000000
};

enum
{
    refactor_indent_width_mask = 0xf,
    refactor_indent_split_ll   = 0x10,         // split long lines (lineup with '(' or soft tab depending on flags)
    refactor_indent_hard_tab   = 0x20,
    refactor_indent_split_str  = 0x40,         // split long strings as well as long lines of code
    refactor_indent_soft_tab_line_cont = 0x80, // 'soft tab' line continuations (or '{}' depending on other flags)
    refactor_indent_KandR      = 0x100,
    refactor_indent_1TBS       = 0x200,  // K&R with } else {
    refactor_indent_Allman     = 0x400,
    refactor_indent_BSD        = 0x400,  // alias for 'Allman'
    refactor_indent_BSD_KNF    = 0x4a8,  // BSD kernel normal format, ITBS with 8 char hard tab, 4 char soft tab for line continuations
                                         // (don't include this with any other flags)
    refactor_indent_Whitesmith = 0x800,  // BSD with tabbed '{}' for control/scope, function and struct body '{}' not tabbed over
    refactor_indent_GNU        = 0x880,  // half-indent '{}'

    refactor_reserved          = 0xf000, // reserved flags

    refactor_lang_MASK         = 0xf0000,
    refactor_lang_C            = 0x10000,
    refactor_lang_CPP          = 0x20000,
    refactor_lang_Pascal       = 0x30000,
    refactor_lang_Java         = 0x40000,

    // other languages reserved


};

// workbench refactor tools - RESERVED

int refactor_symbol(const char *szSymbol, const char *szFile, int iFlags);
int refactor_indent(const char *szFile, int iFlags); // TODO:  calls 'indent' application?

#ifdef __cplusplus
};
#endif // __cplusplus


#endif // _REFACTOR_H_INCLUDED_

