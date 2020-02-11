///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//                     _               __                                        //
//   _ __ ___    __ _ (_) _ __        / _| _ __  __ _  _ __ ___    ___     ___   //
//  | '_ ` _ \  / _` || || '_ \      | |_ | '__|/ _` || '_ ` _ \  / _ \   / __|  //
//  | | | | | || (_| || || | | |     |  _|| |  | (_| || | | | | ||  __/ _| (__   //
//  |_| |_| |_| \__,_||_||_| |_|_____|_|  |_|   \__,_||_| |_| |_| \___|(_)\___|  //
//                             |_____|                                           //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
//                                                                               //
//            Copyright (c) 2020 by S.F.T. Inc. - All rights reserved            //
//     Use, copying, and distribution of this software are licensed according    //
//       to the GPLv2, LGPLv2, or BSD license, as appropriate (see COPYING)      //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////


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
#include "context_help.h"
#include "main_frame.h"

// X11workbench toolkit library includes
#include "window_helper.h"
#include "pixmap_helper.h"  // pixmap helpers, including pre-defined icons
#include "frame_window.h"
#include "edit_window.h"    // edit (client) window
#include "dialog_window.h"
#include "file_help.h"
#include "conf_help.h"
#include "draw_text.h"

// pixmap data
#include "application_icon.xpm" /* 19x19 icon presented to the OS for alt-tab etc. */
#include "icon_app.xpm"   /* application icon that's the same size as the others, 36x36 */
#include "textfiledoc.xpm"
#include "newdoc.xpm"
#include "clangdoc.xpm"
#include "makefiledoc.xpm"



static int MyWindowCallback(Window wID, XEvent *pEvent);

static int FileExitHandler(XClientMessageEvent *);
static int FileNewHandler(XClientMessageEvent *);
static int FileOpenHandler(XClientMessageEvent *);
static int FileSaveHandler(XClientMessageEvent *);
static int FileSaveUIHandler(WBMenu *, WBMenuItem *);
static int FileSaveAsHandler(XClientMessageEvent *);
static int FileSaveAsUIHandler(WBMenu *, WBMenuItem *);
static int FileSaveAllHandler(XClientMessageEvent *);
static int FileSaveAllUIHandler(WBMenu *, WBMenuItem *);
static int FileCloseHandler(XClientMessageEvent *);

static int HelpAboutHandler(XClientMessageEvent *);
static int HelpContentsHandler(XClientMessageEvent *);
static int HelpContextHandler(XClientMessageEvent *);

static int TabLeftHandler(XClientMessageEvent *);
static int TabRightHandler(XClientMessageEvent *);
static int TabMoveLeftHandler(XClientMessageEvent *);
static int TabMoveRightHandler(XClientMessageEvent *);
static int TabUIHandler(WBMenu *, WBMenuItem *);

static int ToolBoxHandler(XClientMessageEvent *pEvent);
static int ToolBoxUIHandler(WBMenu *pMenu, WBMenuItem *pMenuItem);



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   _____                          __        ___           _                 ____  _                   _         //
//  |  ___| __ __ _ _ __ ___   ___  \ \      / (_)_ __   __| | _____      __ / ___|| |_ _ __ _   _  ___| |_ ___   //
//  | |_ | '__/ _` | '_ ` _ \ / _ \  \ \ /\ / /| | '_ \ / _` |/ _ \ \ /\ / / \___ \| __| '__| | | |/ __| __/ __|  //
//  |  _|| | | (_| | | | | | |  __/   \ V  V / | | | | | (_| | (_) \ V  V /   ___) | |_| |  | |_| | (__| |_\__ \  //
//  |_|  |_|  \__,_|_| |_| |_|\___|    \_/\_/  |_|_| |_|\__,_|\___/ \_/\_/   |____/ \__|_|   \__,_|\___|\__|___/  //
//                                                                                                                //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define FW_FILE_NEW_PROJECT_MENU   "IDM_NEW_PROJECT"
#define FW_FILE_NEW_PROJECT_ACCEL  ""
#define FW_FILE_NEW_SOURCE_MENU    "IDM_NEW_SOURCE"
#define FW_FILE_NEW_SOURCE_ACCEL   ""
#define FW_FILE_NEW_HEADER_MENU    "IDM_NEW_HEADER"
#define FW_FILE_NEW_HEADER_ACCEL   ""
#define FW_FILE_NEW_DIALOG_MENU    "IDM_NEW_DIALOG"
#define FW_FILE_NEW_DLG_ACCEL      ""
#define FW_FILE_NEW_MENU_MENU      "IDM_NEW_MENU"
#define FW_FILE_NEW_MENU_ACCEL     ""
#define FW_FILE_NEW_MAKE_MENU      "IDM_NEW_MAKE"
#define FW_FILE_NEW_MAKE_ACCEL     ""
#define FW_FILE_NEW_AUTOCONF_MENU  "IDM_NEW_AUTOCONF"
#define FW_FILE_NEW_AUTOCONF_ACCEL ""
#define FW_FILE_NEW_AUTOMAKE_MENU  "IDM_NEW_AUTOMAKE"
#define FW_FILE_NEW_AUTOMAKE_ACCEL ""



// application menu and the application menu handler structure 'main_menu_handlers'

///////////////////////////////////////////////////////////////////////
// default application menu - what I see when there's no open document
///////////////////////////////////////////////////////////////////////
const char szAppMenu[]="1\n"
                       "_File\tpopup\t2\n"
                       "_Tools\tpopup\t5\n"
                       "\tseparator\n"
                       "_Help\tpopup\t3\n"
                       "\n"
                       "2\n"
//                       "_New File\tIDM_FILE_NEW\tNew File\tCtrl+N\n"
                       "_New File\tpopup\t7\n"
                       "_Open File\tIDM_FILE_OPEN\tOpen File\tCtrl+O\n"
                       "\tseparator\n"
                       "_Preferences\tIDM_PREFERENCES\tApplication Preferences\tCtrl+P\n"
                       "\tseparator\n"
                       "E_xit\tIDM_FILE_EXIT\tClose Application\tAlt+F4\n"
                       "\n"
                       "3\n"
                       "_Contents\tIDM_HELP_CONTENTS\tHelp Contents\tAlt+F1\n"
                       "Conte_xt\tIDM_HELP_CONTEXT\tContext Help\tF1\n"
                       "\tseparator\n"
                       "_About X11workbench\tIDM_HELP_ABOUT\tAbout X11workbench\tAlt+F1\n"
                       "\n"
                       "5\n"
                       "_Toolbox\tIDM_TOOLBOX\tDisplay (or hide) the Toolbox\n"
                       "_Options\tIDM_TOOLS_OPTIONS\tDisplay Options Editor\n"
                        "\n"
                        "7\n"
                        "_Text File\t" FW_FILE_NEW_MENU "\tNew Text File\t" FW_FILE_NEW_ACCEL "\n"
                        "_Project\t" FW_FILE_NEW_PROJECT_MENU "\tNew Project\t" FW_FILE_NEW_PROJECT_ACCEL "\n"
                        "_Source File\t" FW_FILE_NEW_SOURCE_MENU "\tNew Source File\t" FW_FILE_NEW_SOURCE_ACCEL "\n"
                        "_Header File\t" FW_FILE_NEW_HEADER_MENU "\tNew Source File\t" FW_FILE_NEW_HEADER_ACCEL "\n"
                        "_Dialog Box\t" FW_FILE_NEW_DIALOG_MENU "\tNew Project\t" FW_FILE_NEW_DLG_ACCEL "\n"
                        "Menu _Resource\t" FW_FILE_NEW_MENU_MENU "\tNew Menu\t" FW_FILE_NEW_MENU_ACCEL "\n"
                        "_Make File\t" FW_FILE_NEW_MAKE_MENU "\tNew Make File\t" FW_FILE_NEW_MAKE_ACCEL "\n"
                        "_Autoconf File\t" FW_FILE_NEW_AUTOCONF_MENU "\tNew Make File\t" FW_FILE_NEW_AUTOCONF_ACCEL "\n"
                        "Automa_ke File\t" FW_FILE_NEW_AUTOMAKE_MENU "\tNew Automake File\t" FW_FILE_NEW_AUTOMAKE_ACCEL "\n"
                       "\n";

////////////////////////////////////////////////////////////
// edit menu - what I see when there's a child frame active
////////////////////////////////////////////////////////////
const char szEditMenu[]="1\n"
                        "_File\tpopup\t2\n"
                        "_Edit\tpopup\t4\n"
                        "_Tools\tpopup\t5\n"
                        "\tseparator\n"
                        "_Window\tpopup\t6\n"
                        "\tseparator\n"
                        "_Help\tpopup\t3\n"
                        "\n"
                        "2\n"
//                        "_New File\t" FW_FILE_NEW_MENU "\tNew File\t" FW_FILE_NEW_ACCEL "\n"
                        "_New File\tpopup\t7\n"
                        "_Open File\t" FW_FILE_OPEN_MENU "\tOpen File\t" FW_FILE_OPEN_ACCEL "\n"
                        "_Save File\t" FW_FILE_SAVE_MENU "\tSave File\t" FW_FILE_SAVE_ACCEL "\n"
                        "Save _As\t" FW_FILE_SAVE_AS_MENU "\tSave As\t" FW_FILE_SAVE_AS_ACCEL "\n"
                        "Save A_ll\t" FW_FILE_SAVE_ALL_MENU "\tSave All\t" FW_FILE_SAVE_ALL_ACCEL "\n"
                        "\tseparator\n"
                        "_Close File\t" FW_FILE_CLOSE_MENU "\tClose File\tCtrl+F4\n"
                        "\tseparator\n"
                        "_Preferences\tIDM_PREFERENCES\tApplication Preferences\tCtrl+P\n"
                        "\tseparator\n"
                        "E_xit\tIDM_FILE_EXIT\tClose Application\tAlt+F4\n"
                        "\n"
                        "3\n"
                        "_Contents\tIDM_HELP_CONTENTS\tHelp Contents\tAlt+F1\n"
                        "Conte_xt\tIDM_HELP_CONTEXT\tContext Help\tF1\n"
                        "\tseparator\n"
                        "_About X11workbench\tIDM_HELP_ABOUT\tAbout X11workbench\tAlt+F1\n"
                        "\n"
                        "4\n"
                        "_Undo\t" FW_EDIT_UNDO_MENU "\tUn-do last action\t" FW_EDIT_UNDO_ACCEL "\n"
                        "_Redo\t" FW_EDIT_REDO_MENU "\tRe-do last action\t" FW_EDIT_REDO_ACCEL "\n"
                        "\tseparator\n"
                        "Cu_t\t" FW_EDIT_CUT_MENU "\tCut to Clipboard\t" FW_EDIT_CUT_ACCEL "\n"
                        "_Copy\t" FW_EDIT_COPY_MENU "\tCopy to Clipboard\t" FW_EDIT_COPY_ACCEL "\n"
                        "_Paste\t" FW_EDIT_PASTE_MENU "\tPaste from Clipboard\t" FW_EDIT_PASTE_ACCEL "\n"
                        "_Delete\t" FW_EDIT_DELETE_MENU "\tDelete selection\n"
                        "\tseparator\n"
                        "Select _All\t" FW_EDIT_SELECT_ALL_MENU "\tSelect All in context\t" FW_EDIT_SELECT_ALL_ACCEL "\n"
                        "Select _None\t" FW_EDIT_SELECT_NONE_MENU "\tSelect None in context\t" FW_EDIT_SELECT_NONE_ACCEL "\n"
                        "\tseparator\n"
                        "_Find\tIDM_EDIT_FIND\tFind within Document\tCtrl+F\n"
                        "Find Ne_xt\tIDM_EDIT_FIND_NEXT\tFind next occurence within Document\tCtrl+G\n"
                        "_Project Find\tIDM_EDIT_PROJ_FIND\tFind within entire project\tCtrl+Shift+F\n"
                        "Pro_ject Find Next\tIDM_EDIT_PROJ_FIND_NEXT\tFind next occurrence within entire project\tCtrl+Shift+G\n"
                        "\tseparator\n"
                        "Properties\t" FW_EDIT_PROPERTIES_MENU "\tFind next occurrence within entire project\t" FW_EDIT_PROPERTIES_ACCEL "\n"
                        "\n"
                        "5\n"
                        "_Toolbox\tIDM_TOOLBOX\tDisplay (or hide) the Toolbox\n"
                        "_Options\tIDM_TOOLS_OPTIONS\tDisplay Options Editor\n"
                        "\n"
                        "6\n"
                        "Tab _Left\tIDM_TAB_LEFT\tScroll Tab Left\tCtrl+Alt+PgUp\n"
                        "Tab _Right\tIDM_TAB_RIGHT\tScroll Tab Right\tCtrl+Alt+PgDown\n"
                        "\tseparator\n"
                        "Re-order Le_ft\tIDM_TAB_MOVE_LEFT\tScroll Tab Left\tCtrl+Alt+Shift+PgUp\n"
                        "Re-order R_ight\tIDM_TAB_MOVE_RIGHT\tScroll Tab Right\tCtrl+Alt+Shift+PgDown\n"
                        "\tseparator\n" // NOTE:  I can add a list of windows here, with hotkeys
                        "\n"
                        "7\n"
                        "_Text File\t" FW_FILE_NEW_MENU "\tNew Text File\t" FW_FILE_NEW_ACCEL "\n"
                        "_Project\t" FW_FILE_NEW_PROJECT_MENU "\tNew Project\t" FW_FILE_NEW_PROJECT_ACCEL "\n"
                        "_Source File\t" FW_FILE_NEW_SOURCE_MENU "\tNew Source File\t" FW_FILE_NEW_SOURCE_ACCEL "\n"
                        "_Header File\t" FW_FILE_NEW_HEADER_MENU "\tNew Source File\t" FW_FILE_NEW_HEADER_ACCEL "\n"
                        "_Dialog Box\t" FW_FILE_NEW_DIALOG_MENU "\tNew Project\t" FW_FILE_NEW_DLG_ACCEL "\n"
                        "Menu _Resource\t" FW_FILE_NEW_MENU_MENU "\tNew Menu\t" FW_FILE_NEW_MENU_ACCEL "\n"
                        "_Make File\t" FW_FILE_NEW_MAKE_MENU "\tNew Make File\t" FW_FILE_NEW_MAKE_ACCEL "\n"
                        "_Autoconf File\t" FW_FILE_NEW_AUTOCONF_MENU "\tNew Make File\t" FW_FILE_NEW_AUTOCONF_ACCEL "\n"
                        "Automa_ke File\t" FW_FILE_NEW_AUTOMAKE_MENU "\tNew Automake File\t" FW_FILE_NEW_AUTOMAKE_ACCEL "\n"
                        "\n";


// NOTES ON 'F' KEYS - these were compiled by someone else with respect to winders
// CTRL+ALT (and +SHIFT) any F key in X11 switches to a virtual desktop in console mode
//
// F1 - brings up a help window
//      Alt - system menu
// F2 - in winders, rename selected object.
//      Alt+Ctrl - in MS Orifice, opens documents library (don't do this in X11)
// F3 - in winders, open search box
// F4 - in winders XP, display address bar list (or similar)
//      Alt - close application/window
// F5 - window refresh/update (such as in Firefox)
// F6 - cycle through screen elements of a window
// F7 - turn on/off "caret mode" in Firefox; in MS Weird, spell/grammar checking
// F8 - extend selection (MS orifice)
// F9 - update fields (MS orifice, particularly ExHell)
// F10 - activates the menu
//       SHIFT - pops up a context menu (in X11 at least) like a right-click would
//       ALT - in X11, maximize/normal (retaining window decorations)
// F11 - toggle between "true full-screen" (no window decorations) and normal [works in gnome/mate as well]
// F12 - opens 'save as' dialog (in MS Orifice)




// menu handler, similar to what MFC does
// in theory I can swap in a new menu handler when the window focus changes
// this is the DEFAULT handler, when no 'child frame' has the focus.  It also handles
// all of the OTHER menu stuff, out of convenience

FW_MENU_HANDLER_BEGIN(main_menu_handlers)
  FW_MENU_HANDLER_ENTRY("IDM_FILE_EXIT",FileExitHandler,NULL)
  FW_MENU_HANDLER_ENTRY("IDM_FILE_NEW",FileNewHandler,NULL)
  FW_MENU_HANDLER_ENTRY("IDM_FILE_OPEN",FileOpenHandler,NULL)
  FW_MENU_HANDLER_ENTRY("IDM_FILE_SAVE",FileSaveHandler,FileSaveUIHandler)
  FW_MENU_HANDLER_ENTRY("IDM_FILE_SAVE_AS",FileSaveAsHandler,FileSaveAsUIHandler)
  FW_MENU_HANDLER_ENTRY("IDM_FILE_SAVE_ALL",FileSaveAllHandler,FileSaveAllUIHandler)

  FW_MENU_HANDLER_ENTRY("IDM_FILE_CLOSE",FileCloseHandler,NULL) // TODO:  do a UI handler?

  FW_MENU_HANDLER_ENTRY("IDM_PREFERENCES", ApplicationPreferences,NULL)

  FW_MENU_HANDLER_ENTRY("IDM_TOOLBOX",ToolBoxHandler,ToolBoxUIHandler)

  FW_MENU_HANDLER_ENTRY("IDM_TAB_LEFT",TabLeftHandler, TabUIHandler)
  FW_MENU_HANDLER_ENTRY("IDM_TAB_RIGHT",TabRightHandler, TabUIHandler)
  FW_MENU_HANDLER_ENTRY("IDM_TAB_MOVE_LEFT",TabMoveLeftHandler, TabUIHandler)
  FW_MENU_HANDLER_ENTRY("IDM_TAB_MOVE_RIGHT",TabMoveRightHandler, TabUIHandler)

  FW_MENU_HANDLER_ENTRY("IDM_HELP_ABOUT",HelpAboutHandler,NULL)
  FW_MENU_HANDLER_ENTRY("IDM_HELP_CONTEXT",HelpContextHandler,NULL)
  FW_MENU_HANDLER_ENTRY("IDM_HELP_CONTENTS",HelpContentsHandler,NULL)

#if 0
  // additional EDIT MENU HANDLERS

  FW_MENU_HANDLER_ENTRY("IDM_EDIT_FIND",EditFindHandler,EditFindUIHandler)
  FW_MENU_HANDLER_ENTRY("IDM_EDIT_FIND_NEXT",EditFindNextHandler,EditFindNextUIHandler)
  FW_MENU_HANDLER_ENTRY("IDM_EDIT_PROJ_FIND",EditProjFindHandler,EditProjFindUIHandler)
  FW_MENU_HANDLER_ENTRY("IDM_EDIT_PROJ_FIND_NEXT",EditProjFindNextHandler,EditProjFindNextUIHandler)
#endif // 0

FW_MENU_HANDLER_END



// end of global variables



void get_min_window_height_width(int *piMinHeight, int *piMinWidth)
{
#if 0 // old code, consider removing this

  // DEMO CODE - using the 'STRING' size, calculate the minimum width/height and pass that
  //             as parameters to WBInitSizeHints

  // calculating the actual font height for the default font

  unsigned long fth = WBGetDefaultFont()->max_bounds.ascent  // default font height
                    + WBGetDefaultFont()->max_bounds.descent;

  // the pad and border are part of the 'hello world' text and green border displayed for the demo
  unsigned long pad = 32 /*BORDER*/; // font padding (only used here) [TODO:  fix this better]
  unsigned long bw = 1;              // border width (only used here)

  *piMinHeight = fth + pad * 2 + bw * 2;
  *piMinWidth = XTextWidth(WBGetDefaultFont(),     // minimum width (based on text width)
                           STRING,
                           strlen(STRING)) + pad * 2 + bw * 2;
#endif // 0

  if(*piMinHeight)
  {
    *piMinHeight = 360; // for now just hard-code it
  }

  if(*piMinWidth)
  {
    *piMinWidth = 512; // for now just hard-code it
  }
}

WBFrameWindow *DoCreateMainFrameWindow(const char *szTitle, int idIcon,
                                       const char *pszMenu, unsigned long ulFlags)
{
int iMinHeight, iMinWidth;
XSizeHints  xsh;            /* Size hints for window manager */
WBFrameWindow *pRval;

  get_min_window_height_width(&iMinHeight, &iMinWidth);

  // I will need to center the new window, so figure out how to do that

  WBInitSizeHints(&xsh,        // pointer to XSizeHints
                  GetX11Display(), // Display pointer
                  iMinHeight,  // minimum height (based on font height)
                  iMinWidth);  // minimum width

//  // init window attributes
//
//  WBInitWindowAttributes(&xswa,           // attribute structure
//                         bd,              // border pixel color
//                         bg,              // background window color
//                         colormap,        // colormap
//                         CenterGravity);  // gravity

  // create frame window object (always uses default display)

  pRval = FWCreateFrameWindow(szTitle,                  // title
                              idIcon,                   // icon
                              pszMenu,                  // menu
                              xsh.x, xsh.y,             // position
                              xsh.width, xsh.height,    // size
                              MyWindowCallback,         // callback
                              ulFlags);                 // flags and attributes

  if(pRval)
  {
    // assign menu handlers to the frame window (this does the callback functions for me)
    // this is part of the frame window functionality for the DEFAULT menu

    FWSetMenuHandlers(pRval, main_menu_handlers);
    FWRegisterFrameWindowContextHelp(pRval, DoContextSensitiveHelp);
  }

  return pRval;
}

WBEditWindow * DoCreateEditChildFrame(WBFrameWindow *pFrame)
{
WBEditWindow *pRval;

  pRval = WBCreateEditWindow(pFrame,
                             NULL, // default font
                             szEditMenu, main_menu_handlers, 0);

  // TODO:  anything else??

  return pRval;
}



///////////////////////
// CALLBACK FUNCTIONS
///////////////////////

extern void TestFunc(Display *pDisplay, WBGC gc, Window wID, int iX, int iY);

static int MyWindowCallback(Window wID, XEvent *pEvent)
{
WBFrameWindow *pMainFrame = GetFrameWindow();
XWindowAttributes xwa;      /* Temp Get Window Attribute struct */
int iRval = 0;

  /*
    * On the last of each group of Expose events,  repaint the entire
    * window.  See Section 8.4.5.1.
    */

  if(pEvent->type == DestroyNotify &&
     pEvent->xdestroywindow.window == wID)
  {
    WB_DEBUG_PRINT(DebugLevel_Heavy | DebugSubSystem_Application,
                   "%s - DestroyNotify\n", __FUNCTION__);

    if(pMainFrame && pMainFrame->wID == wID)
    {
      pMainFrame = NULL;  // because I'm destroying it
    }

    return 0;  // let remaining default processing happen
  }

  if(pEvent->type == ClientMessage &&
     pEvent->xclient.message_type == aQUERY_CLOSE)
  {
    WB_DEBUG_PRINT(DebugLevel_Heavy | DebugSubSystem_Application,
                   "%s - QUERY_CLOSE\n", __FUNCTION__);

    if(pEvent->xclient.data.l[0]) // close is imminent if I return 0
    {
      if(pMainFrame && pMainFrame->wID == wID)
      {
        pMainFrame = NULL;  // because I'm destroying it
      }
    }

    return 0; // "ok to close" (let frame window handle anything else)
  }

  if (pEvent->type == Expose && pEvent->xexpose.count == 0)
  {
    WBGC gc;
    WB_GEOM geom;

// NOTE:  this is managed by the toolkit
//    /*
//      * Remove any other pending Expose events from the queue to
//      * avoid multiple repaints. See Section 8.7.
//      */
//    while(!bQuitFlag && XCheckTypedWindowEvent(GetX11Display(), wID, Expose, pEvent))
//      ;
//
//    if(bQuitFlag)
//    {
//      iRval = 1;
//      WB_DEBUG_PRINT(DebugLevel_Light | DebugSubSystem_Application,
//                     "%s - Quit flag set - returning %d\n", __FUNCTION__, iRval);
//
//      return iRval;  // let default processing happen
//    }

    /*
      * Find out how big the window is now,  so that we can center
      * the text in it.
      */
    if(XGetWindowAttributes(GetX11Display(), wID, &xwa) == 0)
    {
      WB_ERROR_PRINT("%s - Cannot get correct attributes for window!  Returning %d\n",
                    __FUNCTION__, iRval);
      return iRval;
    }

    gc = WBBeginPaint(wID, &(pEvent->xexpose), &geom);  // begin paint, get a gc for it

    if(gc == None)
    {
      WB_DEBUG_PRINT(DebugLevel_Medium | DebugSubSystem_Expose,
                     "%s.%d - WBBeginPaint returns 'None'\n",
                    __FUNCTION__, __LINE__);

      return 1; // handled (this is because a None gc means I didn't have anything to update)
    }

    WBClearWindow(wID, gc);  // does the erase background intelligently

#ifdef DISPLAY_HELLO_WORLD
    {
      XFontStruct *pFont;
      int x, y, x0, y0, x1, y1;

      pFont = WBGetWindowFontStruct(wID);
      if(!pFont)
      {
        WB_ERROR_PRINT("%s - No font - returning %d\n", __FUNCTION__, iRval);
        return iRval;  // let default processing happen
      }

      x0 = XTextWidth(pFont, STRING, strlen(STRING));
      y0 = pFont->max_bounds.ascent
          - pFont->max_bounds.descent;

      x = (xwa.width - x0) / 2;
      y = (xwa.height + pFont->max_bounds.ascent
        - pFont->max_bounds.descent) / 2;

      // adjust new values for X0 and y0 to surround x and y

      x1 = x + x0 + 20;
      x0 = x - 20;

      y1 = y + pFont->max_bounds.descent + 20;
      y0 = y - y0 - 20;

      /*
        * Fill the window with the background color,  and then paint
        * the centered string.
        */

      XSetForeground(GetX11Display(), gc, WBGetWindowFGColor(wID));
      XDrawString(GetX11Display(), wID, gc, x, y, STRING, strlen(STRING));

      XSetForeground(GetX11Display(), gc, clrGreen.pixel);

      // draw my green rectangle

      XDrawRectangle(GetX11Display(), wID, gc, x0, y0, x1 - x0, y1 - y0);
      XSetForeground(GetX11Display(), gc, WBGetWindowFGColor(wID));  // restore it at the end
    }
#endif // DISPLAY_HELLO_WORLD

//    TestFunc(GetX11Display(), gc, wID, x0 - 32, y0 - 32); // TEMPORARY

    WBEndPaint(wID, gc);  // done now [free resources]

    iRval = 1;  // processed

    WB_DEBUG_PRINT(DebugLevel_Excessive | DebugSubSystem_Window,
                   "%s - Expose returning %d\n", __FUNCTION__, iRval);

    return iRval;  // let default processing happen
  }

  // menu events
  if(pEvent->type == ClientMessage && pEvent->xclient.message_type == aMENU_COMMAND)
  {
    iRval = 1;

    WB_DEBUG_PRINT(DebugLevel_Light | DebugSubSystem_Window | DebugSubSystem_Menu | DebugSubSystem_Event,
                   "%s - detecting main window menu event %ld\n", __FUNCTION__,
                   pEvent->xclient.data.l[0]);
  }

  WB_DEBUG_PRINT(DebugLevel_Excessive | DebugSubSystem_Window,
                 "%s - Returning %d\n", __FUNCTION__, iRval);

  return iRval;  // let default processing happen if zero, else 'processed'
}


////////////////////////////////////////////////////////////////////////////
//   __  __                     ____      _ _ _                _          //
//  |  \/  | ___ _ __  _   _   / ___|__ _| | | |__   __ _  ___| | _____   //
//  | |\/| |/ _ \ '_ \| | | | | |   / _` | | | '_ \ / _` |/ __| |/ / __|  //
//  | |  | |  __/ | | | |_| | | |__| (_| | | | |_) | (_| | (__|   <\__ \  //
//  |_|  |_|\___|_| |_|\__,_|  \____\__,_|_|_|_.__/ \__,_|\___|_|\_\___/  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

static int FileExitHandler(XClientMessageEvent *pEvent)
{
  // TODO:  'OnExit' processing for open files (files to be saved, etc.)

  bQuitFlag = 1;  // time to die

  return 1; // handled
}

static int FileNewHandler(XClientMessageEvent *pEvent)
{
WBFrameWindow *pMainFrame = GetFrameWindow();
WBEditWindow *pEW;

  if(!pMainFrame)
  {
    DLGMessageBox(MessageBox_OK | MessageBox_Error, None,
                  "File New", "'File _New' and no container window");
    return 1;
  }

  // TODO:  ask for document type first???  Create window based on THAT ???

  // create a new child frame within the main frame
  // this should be pretty straightforward as I implement it properly

  // 1st, create a new 'WBEditWindow', attaching it to the frame

  pEW = WBCreateEditWindow(pMainFrame, NULL, szEditMenu, main_menu_handlers, 0);

  if(!pEW)
  {
    DLGMessageBox(MessageBox_OK | MessageBox_Error, None,
                  "File New", "'File _New' unable to create edit window");
  }

  return 1; // handled
}


static int FileOpenHandler(XClientMessageEvent *pEvent)
{
WBFrameWindow *pMainFrame = GetFrameWindow();
Window wIDOwner = pMainFrame ? pMainFrame->wID : None;
char *pFile;

  pFile = DLGFileDialog(FileDialog_Open, wIDOwner, ".", "",
                        GetKnownFileTypes());

  if(pFile)
  {
    DoFileOpen(pMainFrame, pFile);

    WBFree(pFile);  // required resource cleanup
  }

  return 1; // handled
}

static int FileSaveHandler(XClientMessageEvent *pEvent)
{
WBFrameWindow *pMainFrame = GetFrameWindow();
WBChildFrame *pC;
Window wIDOwner = pMainFrame ? pMainFrame->wID : None;


  pC = FWGetFocusWindow(pMainFrame); // current active child frame

  if(!pC || !pC->pUI || !pC->pUI->save)
    return 0;

  if(!pC->pUI->get_file_name || !pC->pUI->get_file_name(pC) || !pC->pUI->get_file_name(pC)[0])
    return 0; // no file name, can't use 'File Save'

  return DoFileSave(pC);
}

static int FileSaveUIHandler(WBMenu *pMenu, WBMenuItem *pItem)
{
WBFrameWindow *pMainFrame = GetFrameWindow();
WBChildFrame *pC;
WBEditWindow *pEditWindow;

  pC = FWGetFocusWindow(pMainFrame); // current active child frame

  if(!pC || !pC->pUI || !pC->pUI->save)
    return 0;

  if(!pC->pUI->get_file_name || !pC->pUI->get_file_name(pC) || !pC->pUI->get_file_name(pC)[0])
    return 0; // no file name, can't use 'File Save'

  if(!pC->pUI->has_changed(pC))
    return 0; // file has not changed, so no 'file save'

  return 1; // enabled (for now just always do this)
}

static int FileSaveAsHandler(XClientMessageEvent *pEvent)
{
WBFrameWindow *pMainFrame = GetFrameWindow();
Window wIDOwner = pMainFrame ? pMainFrame->wID : None;
WBChildFrame *pC;
char *pFile;

  pC = FWGetFocusWindow(pMainFrame);

  if(!pC || !pC->pUI || !pC->pUI->save)
    return 0;

  pFile = DLGFileDialog(FileDialog_Save, wIDOwner, ".", "",
                        GetKnownFileTypes());

  if(pFile)
  {
    DoFileSaveAs(pC, pFile);

    WBFree(pFile);  // required resource cleanup

    return pC ? 1 : 0; // handled
  }

  return -1; // error
}

static int FileSaveAsUIHandler(WBMenu *pMenu, WBMenuItem *pItem)
{
WBFrameWindow *pMainFrame = GetFrameWindow();
WBChildFrame *pC;

  // allosw whenever there's a document visiblde

  pC = FWGetFocusWindow(pMainFrame);

  if(!pC || !pC->pUI || !pC->pUI->save)
    return 0;

  return 1; // allow it
}

static int FileSaveAllHandler(XClientMessageEvent *pEvent)
{
WBFrameWindow *pMainFrame = GetFrameWindow();
Window wIDOwner = pMainFrame ? pMainFrame->wID : -1;

  DLGMessageBox(MessageBox_OK | MessageBox_Warning, wIDOwner,
                "File Save All",
                "'File Save A_ll' not currently implemented");

  return 1; // handled
}

static int FileSaveAllUIHandler(WBMenu *pMenu, WBMenuItem *pItem)
{
  return -1; // disabled
}

static int HelpAboutHandler(XClientMessageEvent *pEvent)
{
WBFrameWindow *pMainFrame = GetFrameWindow();
Window wIDOwner = pMainFrame ? pMainFrame->wID : None;


  DLGMessageBox(MessageBox_OK | /*MessageBox_Info*/ MessageBox_App, wIDOwner,
                "About X11workbench",
                "X11workbench - BBB's answer to a proper development environment for X11");

  return 1; // handled
}

static int FileCloseHandler(XClientMessageEvent *pEvent)
{
WBFrameWindow *pMainFrame = GetFrameWindow();
WBChildFrame *pC;

  if(!pMainFrame)
  {
    return 0;
  }

  pC = FWGetFocusWindow(pMainFrame);

  if(pC)
  {
    // TODO:  check for 'safe to close it' ?

    FWRemoveContainedWindow(pMainFrame, pC);

    FWDestroyChildFrame(pC); // this will destroy the super-class as well
  }

  return 1;
}

static int TabLeftHandler(XClientMessageEvent *pEvent)
{
WBFrameWindow *pMainFrame = GetFrameWindow();
int iIndex;

  if(!pMainFrame)
  {
    return 0;
  }

  iIndex = FWGetChildFrameIndex(pMainFrame, NULL);

  WB_ERROR_PRINT("TEMPORARY:  %s - move left %d\n", __FUNCTION__, iIndex);

  if(iIndex > 0)
  {
    FWSetFocusWindowIndex(pMainFrame, iIndex - 1);
  }

  return 1;
}

static int TabRightHandler(XClientMessageEvent *pEvent)
{
WBFrameWindow *pMainFrame = GetFrameWindow();
int iIndex;

  if(!pMainFrame)
  {
    return 0;
  }

  iIndex = FWGetChildFrameIndex(pMainFrame, NULL);

  WB_ERROR_PRINT("TEMPORARY:  %s - move right %d\n", __FUNCTION__, iIndex);

  if(iIndex >= 0)
  {
    FWSetFocusWindowIndex(pMainFrame, iIndex + 1);
  }

  return 1;
}

static int TabMoveLeftHandler(XClientMessageEvent *pEvent)
{
WBFrameWindow *pMainFrame = GetFrameWindow();
WBChildFrame *pC;

  if(!pMainFrame)
  {
    return 0;
  }

  pC = FWGetFocusWindow(pMainFrame);

  if(pC)
  {
    FWMoveChildFrameTabIndex(pMainFrame, pC, MOVE_CHILD_FRAME_TAB_INDEX_LEFT);
  }

  return 1;
}

static int TabMoveRightHandler(XClientMessageEvent *pEvent)
{
WBFrameWindow *pMainFrame = GetFrameWindow();
WBChildFrame *pC;

  if(!pMainFrame)
  {
    return 0;
  }

  pC = FWGetFocusWindow(pMainFrame);

  if(pC)
  {
    FWMoveChildFrameTabIndex(pMainFrame, pC, MOVE_CHILD_FRAME_TAB_INDEX_RIGHT);
  }

  return 1;
}

static int TabUIHandler(WBMenu *pMenu, WBMenuItem *pMenuItem)
{
WBFrameWindow *pMainFrame = GetFrameWindow();

  if(FWGetNumContWindows(pMainFrame) > 1)
  {
    return 1; // enabled (more than one tab)
  }

  return -1; // disabled (zero or one child frames)
}

static int ToolBoxHandler(XClientMessageEvent *pEvent)
{
  DLGMessageBox(MessageBox_WTF | MessageBox_No, None, "X11workbench - Unimplemented",
                "ToolBox Handler not (yet) implemented");

  return 1; // "handled"
}

static int ToolBoxUIHandler(WBMenu *pMenu, WBMenuItem *pMenuItem)
{
  return 1; // enabled  (TODO:  check # of tabs, etc.)
}



static int HelpContentsHandler(XClientMessageEvent *pEvent)
{
WBFrameWindow *pMainFrame = GetFrameWindow();
Window wIDOwner = pMainFrame ? pMainFrame->wID : None;

  char *pTemp = DLGInputBox(wIDOwner, "Help Contents", "Enter a search term for the documentation", "WBInit", -1, -1);

  if(pTemp)
  {
    DoContextSensitiveHelp(pTemp);

    WBFree(pTemp);
  }
//  else
//  {
//    DLGMessageBox(MessageBox_OK | MessageBox_Bang, wIDOwner,
//                  "Something Bad Happened", "Unexpected 'NULL' return from DLGInputBox");
//  }

  return 1; // handled
}

static int HelpContextHandler(XClientMessageEvent *pEvent)
{
WBFrameWindow *pMainFrame = GetFrameWindow();
WBChildFrame *pCH = pMainFrame ? FWGetFocusWindow(pMainFrame) : NULL;
Window wIDOwner = pMainFrame ? pMainFrame->wID : None;

  // see if there's an active child frame, and if it is
  // possible to see its selection, and it's text, and it's
  // a single word, activate context-sensitive help

  if(pCH)
  {
    if(pCH->pUI && pCH->pUI->help)
    {
      pCH->pUI->help(pCH, 0);
      return 1; // handled
    }

    // TODO:  check to see if it's actually handled?

//  DoContextSensitiveHelp(szWhateverWord);

  }

  DLGMessageBox(MessageBox_OK | MessageBox_Bang, wIDOwner,
                "Context Help", "No Context Available");

  return 1; // handled
}


