//////////////////////////////////////////////////////////////////////////////////////////
//                                                                             _        //
//    _ __ ___    ___  _ __   _   _        _ __    ___   _ __   _   _  _ __   | |__     //
//   | '_ ` _ \  / _ \| '_ \ | | | |      | '_ \  / _ \ | '_ \ | | | || '_ \  | '_ \    //
//   | | | | | ||  __/| | | || |_| |      | |_) || (_) || |_) || |_| || |_) |_| | | |   //
//   |_| |_| |_| \___||_| |_| \__,_|_____ | .__/  \___/ | .__/  \__,_|| .__/(_)_| |_|   //
//                                 |_____||_|           |_|           |_|               //
//                                                                                      //
//                        generic popup menu implementation                             //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////

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

/** \file menu_bar.h
  * \brief Definition file for menu popup window structures and API functions
  *
  * This is the definition file for menu popup window functions and structures
*/

/** \ingroup frame
  * \defgroup menu_popup Popup Menus
  * \brief Popup Menu windows and their associate structures and API functions
**/


#ifndef MENU_POPUP_H_INCLUDED
#define MENU_POPUP_H_INCLUDED

#include "menu_bar.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


#define MENU_POPUP_WINDOW_TAG (*((const unsigned int *)"MWMP"))

/** \typedef WBMenuPopupWindow
  * \struct  __WBMenuPopupWindow__
  * \ingroup menu_popup
  * \brief structure for managing a popup menu window
  *
  * Definition for the structure that defines a popup menu window
  *
  * \code

  typedef struct __WBMenuPopupWindow__
  {
    unsigned int ulTag; // tag indicating I'm a 'Menu Popup' window
    Window wSelf;       // window ID for the Menu Popup window
    Window wBar;        // window ID for the associated Menu Bar window
    Window wOwner;      // window ID for the owner window

    WBMenu *pMenu;      // a pointer to the associated WBMenu structure

    int iX;             // popup menu 'X' position within the owner's client area
    int iY;             // popup menu 'Y' position within the owner's client area
    int iWidth;         // popup menu width
    int iHeight;        // popup menu height

    int iColumns;       // total number of columns (currently 1)
    int iVisible;       // total number of visible menu items (including separators)
    int iTop;           // index of the 'top' visible menu item (zero-based) for scrolling menus
    int iSelected;      // currently selected menu (for display purposes)
    int iFlags;         // flags that determine behavior (reserved)

  } WBMenuPopupWindow;  // located at offset 0 in window data

  * \endcode
  *
  * \sa \ref \_\_WBMenu "WBMenu"
**/
typedef struct __WBMenuPopupWindow__
{
  unsigned int ulTag; ///< tag indicating I'm a 'Menu Popup' window
  Window wSelf;       ///< window ID for the Menu Popup window
  Window wBar;        ///< window ID for the associated Menu Bar window
  Window wOwner;      ///< window ID for the owner window

  WBMenu *pMenu;      ///< a pointer to the associated WBMenu structure

  int iX;             ///< popup menu 'X' position within the owner's client area
  int iY;             ///< popup menu 'Y' position within the owner's client area
  int iWidth;         ///< popup menu width
  int iHeight;        ///< popup menu height

  int iColumns;       ///< total number of columns (currently 1)
  int iVisible;       ///< total number of visible menu items (including separators)
  int iTop;           ///< index of the 'top' visible menu item (zero-based) for scrolling menus
  int iSelected;      ///< currently selected menu (for display purposes)
  int iFlags;         ///< flags that determine behavior (reserved)

} WBMenuPopupWindow;


/** \ingroup menu_popup
  * \brief Create a WBMenuPopupWindow object and associated window
  *
  * \param wIDBar The Window ID of the associated menu bar
  * \param wIDOwner The Window ID of the 'owner' window.  All coordinates are with respect to this window's client area
  * \param pMenu A pointer to the associated 'WBMenu'
  * \param iX The X coordinate for the upper left corner of the menu popup with respect to the client area of wIDOwner
  * \param iY The Y coordinate for the upper left corner of the menu popup with respect to the client area of wIDOwner
  * \param iFlags Various bit-flags associated with the popup menu
  * \returns A pointer to an allocated WBMenuPopupWindow, or NULL on error.  This function also creates the actual window.
  *
  * Use this function to create a WBMenuPopupWindow structure and an associated window at the specified location with
  * respect to the client area of 'wIDOwner'.  Use of this function assumes that you will immediately call MBMenuDoModal()
  * using the returned pointer.  The call to MBMenuDoModal() will automatically free up the allocated structure and resources.
  *
  * Header File:  menu_popup.h
**/
WBMenuPopupWindow *MBCreateMenuPopupWindow(Window wIDBar, Window wIDOwner, WBMenu *pMenu,
                                           int iX, int iY, int iFlags);
  // pass 'iX and 'iY' for the menu's origin location.  'pMenu' points to the popup menu and
  // it must contain a list of popup menus that it makes use of.

/** \ingroup menu_popup
  * \brief display a Menu Popup window in a 'modal' loop
  *
  * \param pMenuPopupWindow A pointer to a WBMenuPopupWindow that was created by MBCreateMenuPopupWindow()
  * \returns zero for 'ok', non-zero on error
  *
  * Use this function to display a Menu Popup window created by MBCreateMenuPopupWindow() in a modal loop.
  * On return, the WBMenuPopupWindow pointer will no longer be valid, and its resources will have been released.
  *
  * Header File:  menu_popup.h
**/
int MBMenuDoModal(WBMenuPopupWindow *pMenuPopupWindow);

/** \ingroup menu_popup
  * \brief Destroy a WBMenuPopupWindow structure
  *
  * \param pMenuPopupWindow A pointer to a WBMenuPopupWindow that was created by MBCreateMenuPopupWindow()
  * \returns void
  *
  * Use this function to destroy a WBMenuPopupWindow structure (and resources) created by MBCreateMenuPopupWindow()
  *
  * Header File:  menu_popup.h
**/
void MBDestroyMenuPopupWindow(WBMenuPopupWindow *pMenuPopupWindow);

/** \ingroup menu_popup
  * \brief Find the first WBMenuPopupWindow that references a WBMenu
  *
  * \param pSubMenu A pointer to a WBMenu structure that represents a 'sub menu' for a Menu Popup window
  * \returns A pointer to a WBMenuPopupWindow structure, or NULL on error
  *
  * Use this function to find the first WBMenuPopupWindow that references a WBMenu
  *
  * Header File:  menu_popup.h
**/
WBMenuPopupWindow *MBFindMenuPopupWindow(WBMenu *pSubMenu);  // find first (active) window that uses sub-menu 'pMenu'


/** \ingroup menu_popup
  * \brief Get the associated WBMenuPopupWindow structure from a Menu Popup window's window ID
  *
  * \param wID A Window ID for a Menu Popup window
  * \returns A pointer to the associated WBMenuPopupWindow structure, or NULL on error
  *
  * Use this function to obtain the associated WBMenuPopupWindow structure from a Menu Popup window's window ID
  *
  * Header File:  menu_popup.h
**/
static __inline__ WBMenuPopupWindow *MBGetMenuPopupWindowStruct(Window wID)
{
  WBMenuPopupWindow *pRval = (WBMenuPopupWindow *)WBGetWindowData(wID, 0);  // offset 0 for window-specific structs

  if(pRval && pRval->ulTag == MENU_POPUP_WINDOW_TAG)
    return(pRval);

  return(NULL);
}


#ifdef __cplusplus
};
#endif // __cplusplus


#endif // MENU_POPUP_H_INCLUDED

