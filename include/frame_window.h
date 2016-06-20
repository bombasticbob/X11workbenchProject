//////////////////////////////////////////////////////////////////////////////////////////////
//    __                                            _             _                  _      //
//   / _| _ __  __ _  _ __ ___    ___    __      __(_) _ __    __| |  ___ __      __| |__   //
//  | |_ | '__|/ _` || '_ ` _ \  / _ \   \ \ /\ / /| || '_ \  / _` | / _ \\ \ /\ / /| '_ \  //
//  |  _|| |  | (_| || | | | | ||  __/    \ V  V / | || | | || (_| || (_) |\ V  V /_| | | | //
//  |_|  |_|   \__,_||_| |_| |_| \___|_____\_/\_/  |_||_| |_| \__,_| \___/  \_/\_/(_)_| |_| //
//                                   |_____|                                                //
//                                                                                          //
//    a top-level window that can have menus, toolbars, a status bar, and a client area     //
//               (multiple-document versions have tabs and child frames)                    //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////

/*****************************************************************************

    X11workbench - X11 programmer's 'work bench' application and toolkit
    Copyright (c) 2010-2016 by Bob Frazier (aka 'Big Bad Bombastic Bob')
                             all rights reserved

  DISCLAIMER:  The X11workbench application and toolkit software are supplied
               'as-is', with no waranties, either implied or explicit.

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


#ifndef FRAME_WINDOW_H_INCLUDED
#define FRAME_WINDOW_H_INCLUDED

//#include "window_helper.h"
#include "window_dressing.h"
#include "menu_bar.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus



/** \file frame_window.h
  * \brief Frame Window API functions and definitions
  *
  * Frame Windows are top-level windows that have a border, title bar, and (optionally)
  * a menu or other means of user interaction (like toolbars).  This file contains the
  * API functions generally needed to create, modify, and destroy Frame Windows.
**/

/** \ingroup frame
  * \defgroup frame_window Frame Window APIs and Structures
  *
  * To create a frame window, you will need to call the \ref FWCreateFrameWindow() API function,
  * which returns a WBFrameWindow structure pointer.  Until the window has been destroyed this
  * pointer will remain valid, though you should not manipulate its contents directly.\n
  * Typical code for creating a frame window follows:
  *
  * \code

  XSizeHints  xsh;            // Size hints for window manager
  WBFrameWindow *pFrame;      // pointer to WBFrameWindow for my new frame window

  ...

    // use 'size hints' along with display characteristics to pre-size window

    WBInitSizeHints(&xsh,                  // pointer to XSizeHints
                    WBGetDefaultDisplay(), // current (default) display
                    300,                   // minimum height (300 pixels in this case)
                    600);                  // minimum width (600 pixels in this case)

    // create frame window object

    pFrame = FWCreateFrameWindow("Window Title",           // title
                                 ID_APPLICATION,           // 'well known' icon identifier
                                                           // see PXM_GetIconPixmap()
                                 szAppMenu,                // default app menu (as text)
                                 xsh.x, xsh.y,             // position
                                 xsh.width, xsh.height,    // size
                                 MyWindowCallback,         // callback
                                 WBFrameWindow_APP_WINDOW  // flags and attributes
                                 | WBFrameWindow_VISIBLE);

  * \endcode
  *
  * Once the window has been created, you can manage its contents using FWAddContainedWindow()
  * and related functions.\n
  *
  * Contained windows need no special interface handling unless you want to control the
  * context of the menu, or overload the frame window's default menu event handling.  A
  * set of messages are used by the frame window for its 'contents' windows in order to
  * request that a new menu be assigned to the containing frame window.  Menu commands are
  * passed directly to the contained window's callback functions as they are received,
  * before being handled by the frame window itself.  In this way the frame window can
  * handle anything NOT already handled by the contained window.\n
  * Menu and UI events for which there is no handler should always return '-1'.\n
  *
  * In order to 'fit' a contained window within the frame window's client area, it will always
  * be re-sized when added, and whenever it gains the focus.  A re-size will also be performed
  * for the 'focus' window whenever the frame window is re-sized.  The focus window will
  * need to respond to re-size events and re-draw itself as needed.
  *
**/

/** \ingroup frame
  * \defgroup frame_menu Frame Window Menu Handling
  * \brief Macros and functions needed to set up a frame window's menu
  *
  * Setting up a menu event handler for a frame window using the WBFWMenuHandler structures involves
  * the use of set of macros to create the structure, and a call to \ref FWSetMenuHandlers() to
  * assign them to the Frame window.
  *
  * Typical sample code follows:
  *
  * \code

  static int FileExitHandler(XClientMessageEvent *);
  static int FileOpenHandler(XClientMessageEvent *);
  static int FileSaveHandler(XClientMessageEvent *);

  static char szMyMenu[]="1\n"
                         "_File\tpopup\t2\n"
                         "2\n"
                         "_Open File\tIDM_FILE_OPEN\tOpen File\tCtrl+O\n"
                         "_Save File\tIDM_FILE_SAVE\tSave File\tCtrl+S\n"
                         "\tseparator\n"
                         "E_xit\tIDM_FILE_EXIT\tClose Application\tAlt+F4\n"
                         "\n";

  ...

  FW_MENU_HANDLER_BEGIN(main_menu_handler)
    FW_MENU_HANDLER_ENTRY("IDM_FILE_EXIT",FileExitHandler,NULL)
    FW_MENU_HANDLER_ENTRY("IDM_FILE_OPEN",FileOpenHandler,NULL)
    FW_MENU_HANDLER_ENTRY("IDM_FILE_SAVE",FileSaveHandler,NULL)
  FW_MENU_HANDLER_END

  ---

    // after creating the frame window 'pFrameWindow' with menu 'szMyMenu'

    FWSetMenuHandlers(pFrameWindow, main_menu_handler);

  * \endcode
  *
  * \sa \ref frame_window "Frame Window APIs and Structures"
  *
**/



/** \ingroup frame_window
  * \brief TAG for the WBFrameWindow structure
**/
#define FRAME_WINDOW_TAG (*((const unsigned int *)"FWFW"))


/** \struct __WB_FW_MENU_HANDLER__
  * \ingroup frame_menu
  * \copydoc WBFWMenuHandler
**/
/** \typedef WBFWMenuHandler
  * \ingroup frame_menu  
  * \brief structure for managing menu callbacks
  *
  * The WBFWMenuHandler structure is designed to be initialized via macros, so
  * that a set of callback functions can then be easily used to handle menu events.
  * If no menu handler is present for a menu item, or if the menu UI handler is
  * NOT NULL and returns a non-zero value, the menu item will be disabled and
  * displayed accordingly.  It will not be possible to use its hotkey nor select it.
  * Otherwise, the menu will be displayed normally and be selectable, and its hotkey
  * will be able to activate it.
  *
  * \code

  typedef struct __WB_FW_MENU_HANDLER__
  {
    unsigned long lMenuID;                       // menu ID (< 0x10000L) or const pointer to string

    int (* callback)(XClientMessageEvent *);     // menu callback (gets pointer to the 'XClientMessageEvent')
    int (* UIcallback)(WBMenu *, WBMenuItem *);  // menu 'UI' callback to handle displaying menu states
                                                 // A return value of '0' displays normally, -1 disables
                                                 // Other return values are reserved
  } WBFWMenuHandler;

  * \endcode
  *
  * \sa \ref FW_MENU_HANDLER_ENTRY "FW_MENU_HANDLER_ENTRY", \ref FW_MENU_HANDLER_BEGIN "FW_MENU_HANDLER_BEGIN"
**/
typedef struct __WB_FW_MENU_HANDLER__
{
  unsigned long lMenuID;                       ///< menu ID (< 0x10000L) or const pointer to string

  int (* callback)(XClientMessageEvent *);     ///< menu callback (gets pointer to the 'XClientMessageEvent').

  /** \brief menu 'UI' callback to handle displaying menu states.
    *
    * This callback function handles the menu states.  A return value of '0' displays normaly, -1 disables.\n
    * Other return values are reserved.
  **/
  int (* UIcallback)(WBMenu *, WBMenuItem *);
} WBFWMenuHandler;



/** \struct __WB_FRAME_WINDOW__
  * \ingroup frame_window
  * \copydoc WBFrameWindow
**/
/** \typedef WBFrameWindow
  * \ingroup frame_window
  * \brief main controlling structure for frame windows
  *
  * By default a frame window can handle multiple documents.  Documents are selected via tabs.  At least
  * one tab will always be visible unless there are no 'contents'.  The contained windows will be re-sized
  * automatically as needed.  Contained windows may also register their own menus and/or menu handlers.
  *
  * The WBFrameWindow structure maintains the basic window state information.
  * You should not manipulate this structure directly.  Use the appropriate API functions.
  *
  * \code

  typedef struct __WB_FRAME_WINDOW__
  {
    unsigned int ulTag;                  // tag indicating I'm a frame window
    Window wID;                          // Window id for the frame window
    int iFlags;                          // bitmask of attribute flags (see \ref WBFrameWindow_FLAGS enumeration)

    int iClientX;                        // The current X position of the frame window's client area
    int iClientY;                        // The current Y position of the frame window's client area
    int iClientWidth;                    // The current width of the frame window's client area
    int iClientHeight;                   // The current height of the frame window's client area

    int iFirstTab;                       // The first visible tab index (0 if none)
    int iTabWidth;                       // The width of each tab (in pixels)
    int iNumTabs;                        // The total number of visible tabs
  } WBFrameWindow;

  * \endcode
  *
**/
typedef struct __WB_FRAME_WINDOW__
{
  unsigned int ulTag;                  ///< tag indicating I'm a frame window
  Window wID;                          ///< Window id for the frame window
  int iFlags;                          ///< bitmask of attribute flags (see \ref WBFrameWindow_FLAGS enumeration)

  int iClientX;                        ///< The current X position of the frame window's client area (relative to the window)
  int iClientY;                        ///< The current Y position of the frame window's client area (relative to the window)
  int iClientWidth;                    ///< The current width of the frame window's client area
  int iClientHeight;                   ///< The current height of the frame window's client area

  int iFirstTab;                       ///< The first visible tab index (0 if none)
  int iTabWidth;                       ///< The width of each tab (in pixels)
  int iNumTabs;                        ///< The total number of visible tabs
} WBFrameWindow;



/** \ingroup frame_window
  * \enum WBFrameWindow_FLAGS
  * \brief Frame Window type and status flags
  *
  * Assign zero or more of these flags to the 'iFlags' parameter in the call to FWCreateFrameWindow()
**/
enum WBFrameWindow_FLAGS
{

  WBFrameWindow_APP_WINDOW = 1,   ///< set this flag for application top-level window and whenever it is destroyed the application will exit
  WBFrameWindow_VISIBLE    = 2,   ///< set this to make window immediately visible
  WBFrameWindow_NO_TABS    = 4,   ///< set this to disable tabs (single child frame only)
  WBFrameWindow_STATUS_BAR = 8,   ///< set this to enable a 'status bar' at the bottom

  WBFrameWindow_MAX = 0x80000000L ///< maximum flag value (for reference only)
};

/** \enum WBStatusTabInfo_FLAGS
  * \ingroup frame_window
  * \brief enumeration for bit flags that are 'or'd with the tab stop index
  *
**/
enum WBStatusTabInfo_FLAGS
{
  WBStatusTabInfo_MASK          = 0x0fffffff, ///< mask for the actual tab value
  WBStatusTabInfo_BREAK         = 0x10000000, ///< represents a 'break' (marks end of a column)
  WBStatusTabInfo_JUSTIFY_MASK  = 0x6fffffff, ///< mask for the 'justification' value
  WBStatusTabInfo_JUSTIFY_LEFT  = 0x00000000, ///< left-justify text within the column (default)  
  WBStatusTabInfo_JUSTIFY_CENTER= 0x20000000, ///< center text within the column
  WBStatusTabInfo_JUSTIFY_RIGHT = 0x40000000, ///< right justify text within the column
  WBStatusTabInfo_JUSTIFY_res6  = 0x60000000, ///< reserved: unspecified new justification method
  WBStatusTabInfo_RTL_COLUMN    = 0x80000000, ///< bit flag to 'right justify' the column location
};


// WBChildFrame is defined HERE to avoid circular header file dependencies

/** \struct __WBChildFrame__
  * \ingroup child_frame
  * \copydoc WBChildFrame
**/
/** \typedef WBChildFrame
  * \ingroup child_frame
  * \brief Structure that defines a Child Frame within a Frame Window
  *
  * The WBFWMenuHandler structure is designed to be initialized via macros, so
  * that a set of callback functions can then be easily used to handle menu events.
  * If no menu handler is present for a menu item, or if the menu UI handler is
  * NOT NULL and returns a non-zero value, the menu item will be disabled and
  * displayed accordingly.  It will not be possible to use its hotkey nor select it.
  * Otherwise, the menu will be displayed normally and be selectable, and its hotkey
  * will be able to activate it.
  *
  * \code

  typedef struct __WBChildFrame__
  {
    unsigned int ulTag;               // tag indicating I'm a 'Child Frame' window
    Window wID;                       // window identifier for the 'Child Frame' window
    WBFrameWindow *pOwner;            // a pointer to the WBFrameWindow owner
    XFontStruct *pFont;               // default font for the window

    int iTop;                         // 0-based position of the top of the current viewport (in lines or pixels)
    int iHeight;                      // 0-based height of the current viewport (in lines or pixels)
    int iLeft;                        // 0-based position of the left of the current viewport (in characters or pixels)
    int iWidth;                       // 0-based width of the current viewport (in characters or pixels)

    int iXExtent;                     // X extent for the display surface (determines scrolling behavior)
    int iYExtent;                     // Y extent for the display surface (determines scrolling behavior)

    WB_SCROLLINFO scroll;             // 'scroll info' (horizontal and vertical min/max/pos and other details)

    int iSplit;                       // reserved - position for 'split'

    WB_SCROLLINFO scrollSplit;        // 'scroll info' for 'split' area (vertical only)

    int iTabIndex;                    // Current tab index (for tabbed versions; -1 for "no tabs") - set by owner
    int fFlags;                       // various bitflags defining features.

    char *szDisplayName;              // display name shown in tab and title bar.  You should not alter this member directly.
    Atom aImageAtom;                  // 'image' atom for display in tabs.  default is 'None'.  You should not alter this member directly.

    WBWinEvent pUserCallback;         // message callback function pointer (can be NULL)

    struct __WBChildFrame__ *pNext;   // 'Next Object' pointer in an internally stored linked list
  } WBChildFrame;

  * \endcode
  *
  * \sa \ref frame "Frame Windows"
  *
  * NOTE:  you should not attempt to modify any of these structure members directly.  Use the appropriate
  *        API functions to modify their values.  If this were C++, these members would be marked 'protected'.
**/
typedef struct __WBChildFrame__
{
  unsigned int ulTag;               ///< tag indicating I'm a 'Child Frame' window
  Window wID;                       ///< window identifier for the 'Child Frame' window
  WBFrameWindow *pOwner;            ///< a pointer to the WBFrameWindow owner
  XFontStruct *pFont;               ///< default font for the window

  int iTop;                         ///< 0-based position of the top of the current viewport (in lines or pixels)
  int iHeight;                      ///< 0-based height of the current viewport (in lines or pixels)
  int iLeft;                        ///< 0-based position of the left of the current viewport (in characters or pixels)
  int iWidth;                       ///< 0-based width of the current viewport (in characters or pixels)

  int iXExtent;                     ///< X extent for the display surface (determines scrolling behavior)
  int iYExtent;                     ///< Y extent for the display surface (determines scrolling behavior)

  WB_SCROLLINFO scroll;             ///< 'scroll info' (horizontal and vertical min/max/pos and other details)

  int iSplit;                       ///< reserved - position for 'split'

  WB_SCROLLINFO scrollSplit;        ///< 'scroll info' for 'split' area (vertical only)

  int iTabIndex;                    ///< Current tab index (for tabbed versions; -1 for "no tabs") - set by owner
  int fFlags;                       ///< various bitflags defining features.  See WBChildFrame_FLAGS enum.

  char *szDisplayName;              ///< display name shown in tab and title bar.  You should not alter this member directly.
  Atom aImageAtom;                  ///< 'image' atom for display in tabs.  default is 'None'.  You should not alter this member directly.

  WBWinEvent pUserCallback;         ///< message callback function pointer (can be NULL)

  struct __WBChildFrame__ *pNext;   ///< 'Next Object' pointer in an internally stored linked list
} WBChildFrame;





/** \enum WBChildFrame_FLAGS
  * \ingroup child_frame
  * \brief enumeration for 'fFlags' member of WBChildFrame
  *
**/
enum WBChildFrame_FLAGS 
{
  WBChildFrame_NO_TAB = 1,        ///< does not use MDI tabs [intended for SDI interface]
  WBChildFrame_PIXELS = 2,        ///< use PIXELS instead of characters and lines to define the viewport
  WBChildFrame_VSPLIT = 4,        ///< RESERVED - 'splitter' window with vertical sizeable 'split'
  WBChildFrame_HSPLIT = 8,        ///< RESERVED - 'splitter' window with horizontal sizeable 'split'
  WBChildFrame_SPLIT_MASK = 12,   ///< RESERVED - bit mask for 'splitter' flags
  WBChildFrame_SPLIT_NOSIZE = 16, ///< RESERVED - bit set if split cannot be sized with the mouse
};


// FRAME WINDOW ATOMS

#if !defined(_FRAME_WINDOW_C_) && !defined(_CLIPBOARD_HELPER_C)
     /* this declares the atoms 'const' outside of frame_window.c, and does NOT declare them in clipboard_helper.c */
     /* These atoms are GLOBAL variables, assigned by the DEFAULT Display, and may not work for other threads.     */

extern const Atom aNEW_TAB;      // command sent by Client Message to create a new tab (also ctrl+N)
extern const Atom aNEXT_TAB;     // command sent by Client Message to switch to the next tab (also ctrl+alt+pgdn)
extern const Atom aPREV_TAB;     // command sent by Client Message to switch to the previous tab (also ctrl+alt+pgup)
extern const Atom aSET_TAB;      // command sent by Client Message to switch to the specified tab
extern const Atom aCLOSE_TAB;    // command sent by Client Message to close the specified tab
extern const Atom aREORDER_TAB;  // command sent by Client Message to re-order the specified tab (activate with mouse-drag)

#endif // !defined(_FRAME_WINDOW_C_) && !defined(_CLIPBOARD_HELPER_C)





/** \ingroup frame_window
  * \brief Create a frame window
  *
  * \param szTitle A zero-byte terminated ASCII string that defines the window title
  * \param idIcon An integer identifier coresponding to a registered icon resource
  * \param szMenuResource A zero-byte terminated ASCII definition of the menu for this window.  Pass 'NULL' for this parameter if you do not want a menu.
  * \param iX An integer identifying the x-coordinate for the window, or -1 for default
  * \param iY An integer identifying the y-coordinate for the window, or -1 for default
  * \param iWidth An integer identifying the width of the window, or -1 for default
  * \param iHeight An integer identifying the height of the window, or -1 for default
  * \param pUserCallback A WBWinEvent callback function that receives event notifications for the window
  * \param iFlags An integer with one or more bit flags that alters the parameters for the frame window.  See \ref WBFrameWindow_FLAGS
  * \return A pointer to a WBFrameWindow object.  The caller should not destroy this object, nor directly
  *         reference it after the window has been destroyed.
  *
  * Use this function to create a basic frame window.  A frame window has a title, icon, menu, and
  * can contain one or more 'Child Frames'.  The menu is optional, and support for multiple 'tab'
  * child frames is also optional, depending on the bit flags set in 'iFlags'.
  *
  * NOTE:  scroll up to the 'Detailed Description' section for an example of how to use this function
  *
  * Header File:  frame_window.h
  *
  * \sa \ref frame_menu "Frame Window Menu Handling"
**/
WBFrameWindow *FWCreateFrameWindow(const char *szTitle, int idIcon, const char *szMenuResource,
                                   int iX, int iY, int iWidth, int iHeight,
                                   WBWinEvent pUserCallback, int iFlags);

/** \ingroup frame_window
  * \brief Force a frame window to recalculate its layout, which may involve resizing multiple contained windows
  *
  * \param wID The Window ID of the WBFrameWindow (typically will be called within a message handler, such as a window re-size)
  *
  * Forces the frame window to recalculate its layout (i.e. size of menus, tabs, and 'child frame' windows) and propogate
  * that information to all 'owned' windows and objects it contains.
  *
  * Header File:  frame_window.h
**/
void FWRecalcLayout(Window wID);  // recalculate layout information (propagates to contained windows)

/** \ingroup frame_window
  * \brief assign a new WBWinEvent callback function for a frame window
  *
  * \param pFW A pointer to the WBFrameWindow structure
  * \param pCallBack A function pointer of type \ref WBWinEvent for the user-defined event handler callback.  Can be NULL.
  *
  * Use this function to assign the event handler callback for the frame window.
  *
  * Header File:  frame_window.h
**/
void FWSetUserCallback(WBFrameWindow *pFW, WBWinEvent pCallBack);
  // assigns the user-defined callback function (one only).  Not all messages pass through the callback


/** \ingroup frame_window
  * \brief Obtain the associated WBFrameWindow structure pointer for a frame window's Window ID
  *
  * \param wID A valid Window ID
  * \returns A pointer to the associated WBFrameWindow structure (if it is a WBFrameWindow), or NULL on error
  *
  * Use this function to safely obtain the correct WBFrameWindow structure for a given Window ID.
  *
  * Header File:  frame_window.h
**/
static __inline__ WBFrameWindow *FWGetFrameWindowStruct(Window wID)  // for frame windows, returns the frame window struct
{
  WBFrameWindow *pRval = (WBFrameWindow *)WBGetWindowData(wID, 0);  // offset 0 for window-specific structs

  if(pRval && pRval->ulTag == FRAME_WINDOW_TAG)
  {
    return(pRval);
  }

  return(NULL);
}

/** \ingroup frame_window
  * \brief Function to destroy a frame window based on the Window id
  *
  * \param wID The Window ID associated with the WBFrameWindow to destroy
  *
  * Use this function to destroy a WBFrameWindow based on it's ID.  After calling this function
  * the WBFrameWindow struct is no longer valid
  *
  * Header File:  frame_window.h
**/
void FWDestroyFrameWindow(Window wID);  // destroys frame window using the Window ID (frees the struct also)

/** \ingroup frame_window
  * \brief Function to destroy a frame window based on the WBFrameWindow structure
  *
  * \param pFrameWindow The pointer to the WBFrameWindow structure for the desired frame window
  *
  * Use this function to destroy a WBFrameWindow based on the WBFrameWindow struct pointer value.
  * After calling this function the WBFrameWindow struct is no longer valid
  *
  * Header File:  frame_window.h
**/
void FWDestroyFrameWindow2(WBFrameWindow *pFrameWindow); // destroys it using the struct pointer

/** \ingroup frame_window
  * \brief Function to assign the default menu handler to a frame window
  *
  * \param pFrameWindow The pointer to the WBFrameWindow structure for the desired frame window
  * \param pHandlerArray A pointer to an array of WBFWMenuHandler structures - see \ref FW_MENU_HANDLER_ENTRY
  *
  * Assigns the menu handlers for the frame window.  The array will be copied, and the copy will be
  * used internally.  These handlers are checked for matching entries after checking the 'contained' window's
  * handlers for the WBChildFrame that has the current focus.
  *
  * \sa \ref frame_menu "Frame Menus"
  *
  * Header File:  frame_window.h
**/
void FWSetMenuHandlers(WBFrameWindow *pFrameWindow, const WBFWMenuHandler *pHandlerArray);

/** \ingroup frame_menu
  * \def FW_MENU_HANDLER_BEGIN
  * \brief Use this macro to begin definition of a WBFWMenuHandler array
  *
  * \param X the name of the menu handler array variable
**/
#define FW_MENU_HANDLER_BEGIN(X) static const WBFWMenuHandler X[] = {
/** \ingroup frame_menu
  * \def FW_MENU_HANDLER_ENTRY
  * \brief Use this macro to define an entry for a WBFWMenuHandler array
  *
  * \param X A menu identifier expressed as an ASCII string or ID
  * \param Y A callback function pointer, or NULL - see WBFWMenuHandler::callback
  * \param Z A UI callback functino pointer, or NULL - see WBFWMenuHandler::UIcallback
**/
#define FW_MENU_HANDLER_ENTRY(X,Y,Z) { (unsigned long)X, Y, Z },
/** \ingroup frame_menu
  * \def FW_MENU_HANDLER_END
  * \brief Use this macro to complete the definition of a WBFWMenuHandler array
**/
#define FW_MENU_HANDLER_END {0, 0, 0} };


// frame window 'contents' functions

/** \ingroup frame_window
  * \brief Returns the total number of 'contained' windows
  *
  * \param pFrameWindow A const pointer to a WBFrameWindow structure for the frame window
  * \returns The total number of 'contained' windows, or -1 on error
  *
  * Header File:  frame_window.h
**/
int FWGetNumContWindows(const WBFrameWindow *pFrameWindow);

/** \ingroup frame_window
  * \brief Returns the Window ID for the specified 'contained' window.  The index follows the tab order.
  *
  * \param pFrameWindow A const pointer to a WBFrameWindow structure for the frame window
  * \param iIndex The 'tab order' index of the 'contained' window.  The first window is '0'.  A negative value implies the 'current focus' window
  * \returns A pointer to the WBChildFrame for the 'contained' window corresponding to the index, or 'NULL' on error
  *
  * Use this function to get the Window ID of a 'contained' (child frame) window.  To get more information
  * about contained 'Child Frame' window, see \ref child_frame "Child Frames"
  *
  * Header File:  frame_window.h
**/
WBChildFrame * FWGetContainedWindowByIndex(const WBFrameWindow *pFrameWindow, int iIndex);

/** \ingroup frame_menu
  * \def FWGetFocusWindow
  * \brief A macro to aid code readability, returning the Window ID of the contained window that has the focus
  *
  * \param pFW The WBFrameWindow structure pointer
**/
#define FWGetFocusWindow(pFW) FWGetContainedWindowByIndex(pFW, -1)

/** \ingroup frame_window
  * \brief Adds a 'contained' window and returns the tab order index
  *
  * \param pFrameWindow A pointer to a WBFrameWindow structure for the frame window
  * \param pNew A pointer to the WBChildFrame for the window that is to become part of the 'contents' for the frame window
  * \returns The tab order index of the window, or -1 on error
  *
  * Header File:  frame_window.h
**/
int FWAddContainedWindow(WBFrameWindow *pFrameWindow, WBChildFrame *pNew);

/** \ingroup frame_window
  * \brief Removes a 'contained' window from a frame window (does not destroy the window)
  *
  * \param pFrameWindow A pointer to a WBFrameWindow structure for the frame window
  * \param pCont A pointe to the WBChildFrame for the window that is to be removed from the 'contents' for the frame window
  * \returns void
  *
  * Header File:  frame_window.h
**/
void FWRemoveContainedWindow(WBFrameWindow *pFrameWindow, WBChildFrame *pCont);

/** \ingroup frame_window
  * \brief Sets the focus to a specific contained window using the Window ID
  *
  * \param pFrameWindow A pointer to a WBFrameWindow structure for the frame window
  * \param pCont A pointer to the WBChildFrame for the window that is to be assigned the focus.
  * \returns void
  *
  * Header File:  frame_window.h
**/
void FWSetFocusWindow(WBFrameWindow *pFrameWindow, WBChildFrame *pCont);

/** \ingroup frame_window
  * \brief Sets the focus to a specific contained window using its tab order index
  *
  * \param pFrameWindow A pointer to a WBFrameWindow structure for the frame window
  * \param iIndex The 'tab order' index of the 'contained' window that is to receive the focus
  * \returns void
  *
  * Header File:  frame_window.h
**/
void FWSetFocusWindowIndex(WBFrameWindow *pFrameWindow, int iIndex);

/** \ingroup frame_window
  * \brief Sets the 'status' text for a Frame Window with a status bar, forcing a re-paint
  *
  * \param pFrameWindow A pointer to a WBFrameWindow structure for the frame window
  * \param szText A tab-delimited character string containing UTF8 or ASCII status text. A copy of this will be stored internally.
  * \returns void
  *
  * Use this function to update the status text.  Status text can contain multiple 'columns' that
  * are displayed using the tab stop information assigned by FWSetStatusTabInfo().  Each column's
  * text data will be 'appropriately justified' and displayed in the status bar according to the tab info.\n
  * This function forces a re-paint of the status bar.  If you also want to change the tabs, call
  * FWSetStatusTabInfo() first, followed by this function.
  *
  * Header File:  frame_window.h
**/
void FWSetStatusText(WBFrameWindow *pFrameWindow, const char *szText);

/** \ingroup frame_window
  * \brief Sets the 'status' tab info for a Frame Window with a status bar
  *
  * \param pFrameWindow A pointer to a WBFrameWindow structure for the frame window
  * \param nTabs The total number of tabs in 'pTabs'.  If 'pTabs' is NULL, it is the default (even) tab spacing.
  * \param pTabs A pointer to an array of integers containing tab values.  See below on how they are interpreted.
  * \returns void
  *
  * Use this function to assign the tab spacing and/or columns for the status bar.  Tabs are bit-wise
  * 'or'd with flags from the \ref WBStatusTabInfo_FLAGS enumeration.\n
  * The physical tab value starts at the left side unless WBStatusTabInfo_RTL_COLUMN has been 'or'd with
  * its value.  Columns width is determined by both the next tab value and the current one.  Use the
  * 'WBStatusTabInfo_BREAK' flag to create a 'blank' area that defines the width of the previous column
  * but does not have anything displayed within it.\n
  * To left, right, or center text within a column, use one of the WBStatusTabInfo_JUSTIFY_ constants.  The
  * default (zero) is left-justification (even for RTL columns).\n
  * This function does NOT force a re-paint of the status bar.  It does invalidate the status bar rectangle,
  * however. To force a re-paint after updating the tabs, call WBUpdateWindow() for the frame window.
  *
  * Header File:  frame_window.h
**/
void FWSetStatusTabInfo(WBFrameWindow *pFrameWindow, int nTabs, const int *pTabs);

// DEFAULT COLORS

/** \ingroup frame_window
  * \brief Get the default foreground color
  *
  * \returns XColor representing default foreground color
  *
  * Header File:  frame_window.h
**/
XColor FWGetDefaultFG(void);

/** \ingroup frame_window
  * \brief Get the default background color
  *
  * \returns XColor representing default background color
  *
  * Header File:  frame_window.h
**/
XColor FWGetDefaultBG(void);

/** \ingroup frame_window
  * \brief Get the default border color
  *
  * \returns XColor representing default border color
  *
  * Header File:  frame_window.h
**/
XColor FWGetDefaultBD(void);


#if 0
/** \ingroup frame_window
  * \brief Handles 'selection' events for frame windows
  *
  * \param pFrameWindow A pointer to a WBFrameWindow structure for the frame window
  * \param wID The ID of the frame window
  * \param wIDMenu The Window ID of the frame window's currently active menu
  * \param pEvent The 'selection' event
  * \returns Non-zero if 'handled', zero if further (or default) processing is needed
  *
  * This function performs basic 'selection event' processing.  It is normally called
  * internally for default handling of selection events whenever the specified callback
  * function returns 0 for such events.
  *
  * Header File:  frame_window.h
**/
int FWDoSelectionEvents(WBFrameWindow *pFrameWindow, Window wID, Window wIDMenu, XEvent *pEvent);
#endif // 0


#ifdef __cplusplus
};
#endif // __cplusplus


#endif // FRAME_WINDOW_H_INCLUDED

