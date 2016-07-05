//////////////////////////////////////////////////////////////////////////////////////////
//                                                        _                             //
//                       _ __ ___    ___  _ __   _   _   | |__                          //
//                      | '_ ` _ \  / _ \| '_ \ | | | |  | '_ \                         //
//                      | | | | | ||  __/| | | || |_| | _| | | |                        //
//                      |_| |_| |_| \___||_| |_| \__,_|(_)_| |_|                        //
//                                                                                      //
//                        generic menu resource implementation                          //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////

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

/** \file menu.h
  * \brief Definition file for menus
  *
  * This is the definition file for menu functions and structures that handle
  * their interpretation, construction, manipulation, and destruction
**/

/** \ingroup frame
  * \defgroup menu Menu functions and structures
  *
  * Structures and API functions for managing and working with menus and menu items
**/


// a menu bar is a window that contains one or more text menu items, displaying a popup menu
// whenever a text menu item is activated.  The popup menu is displayed as needed.

#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


/** \ingroup menu
  @{
**/
#define WBMENU_RESERVE_DEFAULT (256 * sizeof(void *)) /**< default menu 'iReserveSpace' size **/

#define WBMENU_POPUP_HIGH_BIT 0x80000000    /**< popup 'high bit' (TODO may become platform dependent) **/
#define WBMENU_DYNAMIC_HIGH_BIT 0x40000000  /**< marks menu item as 'dynamic' for WBMenuItem::iAction (TODO may become platform dependent) **/
#define WBMENU_POPUP_MASK 0x3fffffff        /**< popup mask (TODO may become platform dependent) **/
#define WBMENU_SEPARATOR -1                 /**< marks menu item as a 'separator' for WBMenuItem::iAction **/
/**
  @}
**/


/** \ingroup menu
  * \struct __WBMenuItem__
  * \copydoc WBMenuItem
**/
/** \typedef WBMenuItem
  * \ingroup menu
  * \brief structure for managing menu items
  *
  * Definition for the structure that defines a single menu item.
  *
  * \code

  typedef struct __WBMenuItem__
  {
    unsigned int uiTag;    // a 'tag' identifying this as a WBMenuItem

    // the following data members are offsets from 'data' for each component of the menu item
    // '-1' generically indicates "none"
    int iMenuItemText;     // offset in 'data' to null-byte terminated strings (-1 if none)
    int iUnderscore;       // offset of (first) 'underscore' within menu text (-1 if none)
    int iTooltipText;      // offset in 'data' to null-byte terminated strings (-1 if none)
    int iHotKey;           // hotkey description (-1 if none)

    // the following data members are numeric properties (not offsets)
    int iAction;           // high bit set for popup; -1 for separator, -2 for dynamic; otherwise, it's a message
                           // if high bit set, corresponding lower bits are the popup menu ID
    int nHotKey;           // hotkey character translation (0 if none)
    int iTextWidth;        // width of menu text (in pixels; assign '-1' to calculate it)
    int iPosition;         // horizontal/vertical position of menu (in pixels; assign '-1' to calculate it)

    int nDataSize;         // total size of data
    char data[4];          // data follows

  } WBMenuItem;

  * \endcode
  *
  * \sa WBMenu
  *
**/
typedef struct __WBMenuItem__
{
  unsigned int uiTag;    ///< a 'tag' identifying this as a WBMenuItem

  // the following data members are offsets from 'data' for each component of the menu item
  // '-1' generically indicates "none"
  int iMenuItemText;     ///< offset in 'data' to null-byte terminated strings (-1 if none)
  int iUnderscore;       ///< offset of (first) 'underscore' within menu text (-1 if none)
  int iTooltipText;      ///< offset in 'data' to null-byte terminated strings (-1 if none)
  int iHotKey;           ///< hotkey description (-1 if none)

  // the following data members are numeric properties (not offsets)
  int iAction;           ///< high bit set for popup; -1 for separator, -2 for dynamic; otherwise, it's a message
                         ///< if hight bit set, corresponding lower bits are the popup menu ID
  int nHotKey;           ///< hotkey character translation (0 if none)
  int iTextWidth;        ///< width of menu text (in pixels; assign '-1' to calculate it)
  int iPosition;         ///< horizontal/vertical position of menu (in pixels; assign '-1' to calculate it)

  int nDataSize;         ///< total size of data
  char data[4];          ///< data follows

} WBMenuItem;


/** \ingroup menu
  * \struct __WBMenu
  * \copydoc WBMenu
**/
/** \typedef WBMenu
  * \ingroup menu
  * \brief structure for managing menu items
  *
  * Definition for the structure that defines a menu.  A menu is a single-level entity that can
  * contain other "sub menus" that display as 'popup' menus when the menu item is highlighted or selected.
  *
  * \code

  typedef struct __WBMenu
  {
    unsigned int uiTag;          // a 'tag' identifying this as a WBMenu

    int iMenuID;                 // menu identifier specified when menu was created (high bit set for popup)

    WBMenuItem **ppItems;        // An allocated array of menu items
    int nItems;                  // The number of menu item entries in the 'ppItems' array
    int nMaxItems;               // The maximum number of menu item entries that can be stored in 'ppItems'

    struct __WBMenu **ppPopups;  // An allocated array of 'popup' menus contained by this menu
    int nPopups;                 // The number of popup menu entries in the 'ppPopups' array
    int nMaxPopups;              // The maximum number of popup menu entries that can be stored in 'ppPopups'

  } WBMenu;

  * \endcode
  *
  * \sa WBMenuItem
  *
**/
typedef struct __WBMenu
{
    unsigned int uiTag;          ///< a 'tag' identifying this as a WBMenu

    int iMenuID;                 ///< menu identifier specified when menu was created (high bit set for popup)

    WBMenuItem **ppItems;        ///< An allocated array of menu items
    int nItems;                  ///< The number of menu item entries in the 'ppItems' array
    int nMaxItems;               ///< The maximum number of menu item entries that can be stored in 'ppItems'

    struct __WBMenu **ppPopups;  ///< An allocated array of 'popup' menus contained by this menu
    int nPopups;                 ///< The number of popup menu entries in the 'ppPopups' array
    int nMaxPopups;              ///< The maximum number of popup menu entries that can be stored in 'ppPopups'

} WBMenu;

#define WBMENU_TAG     (*((const unsigned int *)"WBMM"))
#define WBMENUITEM_TAG (*((const unsigned int *)"WBMI"))


//------------------------------
// construction and destruction
//------------------------------

/** \ingroup menu
  * \brief Create a WBMenu from a text menu resource
  *
  * \param iID The menu ID, or '-1' to read it from the resource
  * \param iPopup The 'popup' ID, or '-1' to read it from the resource
  * \param pszResource A pointer to a (const) character string, terminated by a 0 byte, defining the menu structure
  * \param iReserveSpace The amount of space (in menu items) to reserve for use by MBAddMenuItem() and MBAddPopupMenu()
  * \returns An allocated pointer to a WBMenu structure, or NULL on error.  Use MBDestroyMenu() to de-allocate non-NULL return values.
  *
  * Use this function to create a WBMenu structure from a text resource, optionally specifying the menu's ID and/or popup ID.
  *
  * Header File:  menu.h
**/
WBMenu *MBCreateMenu(int iID, int iPopup, const char *pszResource, int iReserveSpace);
  // pass in full menu resource 'pszResource' including any referenced popup menus. If a popup menu
  // isn't specified here it can be added later via 'MBAddPopupMenu'.  Specify '-1' for iID to
  // read ID (and popup specifier) from the menu resource.  'iPopup' is non-zero for popup, zero for bar

/** \ingroup menu
  * \brief Destroy a WBMenu created by MBCreateMenu(), freeing up its resources
  *
  * \param pMenu A pointer to a WBMenu structure, created by MBCreateMenu()
  * \returns void
  *
  * Use this function to destroy a WBMenu created by MBCreateMenu(), and free up its resources.
  *
  * Header File:  menu.h
**/
void MBDestroyMenu(WBMenu *pMenu);  // always destroy with this function

/** \ingroup menu
  * \brief Create a copy of a WBMenu from an existing WBMenu
  *
  * \param pMenu A pointer to a WBMenu structure, created by MBCreateMenu()
  * \param iReserveSpace The amount of space (in menu items) to reserve for use by MBAddMenuItem() and MBAddPopupMenu().  A negative value will retain the existing WBMenu's reserve space.
  * \returns An allocated pointer to a new WBMenu structure, or NULL on error.  Use MBDestroyMenu() to de-allocate non-NULL return values.
  *
  * Use this function to make a copy of an existing WBMenu structure.
  *
  * Header File:  menu.h
**/
WBMenu *MBCopyMenu(const WBMenu *pMenu, int iReserveSpace);

/** \ingroup menu
  * \brief Check whether a 'WBMenu' pointer is valid
  *
  * \param pMenu A (const) pointer to a WBMenu structure, created by MBCreateMenu()
  * \returns A non-zero 'True' value if valid; zero if NOT valid
  *
  * Use this function to validate a 'WBMenu' pointer before attempting to use it
  *
  * Header File:  menu.h
**/
int MBIsMenuValid(const WBMenu *pMenu);

//------------
// menu items
//------------

/** \ingroup menu
  * \brief Create a single allocated WBMenuItem structure from a text menu resource, advancing the source text pointer to the next menu item
  *
  * \param ppszResource A pointer to a pointer to a (const) character string, terminated by a 0 byte, which is updated by the function call
  * \returns An allocated pointer to a WBMenuItem structure, or NULL on error.  Use MBDestroyMenuItem() to de-allocate non-NULL return values
  *
  * Use this function to create a WBMenuItem structure from a text resource, automatically advancing the source
  * text pointer to the next menu item in the text resource.  Repeated calls to this function allow you to parse
  * a text menu resource and create all of the menu items.
  *
  * Header File:  menu.h
  *
  * \sa MBCreateMenu()
**/
WBMenuItem *MBCreateMenuItem(const char **ppszResource);  // create single menu item, and point *ppszResource to next item in resource

/** \ingroup menu
  * \brief Destroy a WBMenuItem created by MBCreateMenuItem(), freeing up its resources
  *
  * \param pMenuItem A pointer to a WBMenuItem structure, created by MBCreateMenuItem()
  * \returns void
  *
  * Use this function to destroy a WBMenuItem created by MBCreateMenuItem(), and free up its resources.
  *
  * Header File:  menu.h
**/
void MBDestroyMenuItem(WBMenuItem *pMenuItem);

/** \ingroup menu
  * \brief Check whether a 'WBMenuItem' pointer is valid
  *
  * \param pMenuItem A (const) pointer to a WBMenuItem structure, created by MBCreateMenuItem()
  * \returns A non-zero 'True' value if valid; zero if NOT valid
  *
  * Use this function to validate a 'WBMenuItem' pointer before attempting to use it
  *
  * Header File:  menu.h
**/
int MBIsMenuItemValid(const WBMenuItem *pMenuItem);

/** \ingroup menu
  * \brief Add a WBMenuItem menu item to an existing WBMenu
  *
  * \param pMenu A pointer to a WBMenu structure, created by MBCreateMenu()
  * \param pMenuItem A pointer to a (const) WBMenuItem structure, created by MBCreateMenuItem()
  * \param iPos The position to insert/add the menu item to, or -1 to add it to the end
  * \returns the index for the added menu item, or -1 on error.
  *
  * Use this function to add a WBMenuItem to the WBMenu.  This function makes a copy of the specified
  * WBMenuItem and inserts the copy (i.e. does not take ownership of the WBMenuItem).
  *
  * Header File:  menu.h
**/
int MBAddMenuItem(WBMenu *pMenu, const WBMenuItem *pMenuItem, int iPos);

/** \ingroup menu
  * \brief Remove a WBMenuItem from a menu created by MBCreateMenu(), freeing up its resources
  *
  * \param pMenu A pointer to a WBMenu structure, created by MBCreateMenu()
  * \param iPos A zero-based index for the menu item's position
  * \returns void
  *
  * Use this function to remove a WBMenuItem from a WBMenu created by MBCreateMenu()
  *
  * Header File:  menu.h
**/
void MBRemoveMenuItem(WBMenu *pMenu, int iPos);  // deletes menu item - 'iPos' has same semantics as above

//-------------
// popup menus
//-------------

/** \ingroup menu
  * \brief Add a WBMenuItem menu item to an existing WBMenu
  *
  * \param pMenu A pointer to a WBMenu structure, created by MBCreateMenu()
  * \param pPopupMenu A pointer to a (const) WBMenu structure, created by MBCreateMenu()
  * \returns the index for the added popup menu item, or -1 on error.
  *
  * Use this function to add a WBMenu 'popup' to the WBMenu.  This function makes a copy of the specified
  * WBMenu and inserts the copy (i.e. does not take ownership of the WBMenu).  The copy operation is recursive,
  * so that the added WBMenu 'popup' does not share memory with any other.
  *
  * Header File:  menu.h
**/
int MBAddPopupMenu(WBMenu *pMenu, const WBMenu *pPopupMenu); // use this when you add a popup menu item to add the actual popup menu

/** \ingroup menu
  * \brief Locate a WBMenu 'popup' within a menu created by MBCreateMenu()
  *
  * \param pMenu A pointer to a WBMenu structure, created by MBCreateMenu()
  * \param idPopup The 'popup menu' ID for the popup menu (this differs from WBRemoveMenuItem() which uses the index)
  * \returns a pointer to the WBMenu corresponding to the popup menu.
  *
  * Use this function to locate a 'popup' WBMenu within a WBMenu created by MBCreateMenu(), returning a
  * pointer to the WBMenu structure, using the popup menu's ID.  The returned WBMenu pointer can then
  * be used in the functions that maintain a WBMenu, so long as the actual pointer value is not modified.\n
  * If the pointer must be re-allocated, or a new popup menu is created to replace it, you can delete
  * the popup menu using MBRemovePopupMenu() and then re-add the new one with MBAddPopupMenu()
  *
  * Header File:  menu.h
**/
WBMenu *MBFindPopupMenu(WBMenu *pMenu, int idPopup);

/** \ingroup menu
  * \brief Remove a WBMenu 'popup' from a menu created by MBCreateMenu(), freeing up its resources
  *
  * \param pMenu A pointer to a WBMenu structure, created by MBCreateMenu()
  * \param idPopup The 'popup menu' ID for the popup menu (this differs from WBRemoveMenuItem() which uses the index)
  * \returns void
  *
  * Use this function to remove a popup WBMenu from a WBMenu created by MBCreateMenu()
  *
  * Header File:  menu.h
**/
void MBRemovePopupMenu(WBMenu *pMenu, int idPopup);


/** \ingroup menu
  * \brief Indicate whether a 'WBMenu' refers to a popup menu
  *
  * \param pMenu A pointer to a WBMenu structure, created by MBCreateMenuItem()
  * \returns a non-zero value if the WBMenu refers to a 'popup' menu, zero otherwise
  *
  * Use this function to indicate whether a 'WBMenu' refers to a popup menu
  *
  * Header File:  menu.h
**/
static __inline__ int MBMenuIsPopup(WBMenu *pMenu)
{
  return pMenu && (pMenu->iMenuID & WBMENU_POPUP_HIGH_BIT);
}

/** \ingroup menu
  * \brief Event handler for menu hotkeys
  *
  * \param pMenu A pointer to a WBMenu structure, created by MBCreateMenuItem()
  * \param pEvent A pointer to the XKeyEvent being processed
  * \returns a non-zero value if the key event was processed as a hot-key, zero otherwise
  *
  * Use this function to process keyboard 'XKeyEvent' events, determine if they correspond to menu hot-keys,
  * and to notify the menu owner with the hot-key action (if found).
  *
  * Header File:  menu.h
**/
int MBMenuProcessHotKey(WBMenu *pMenu, XKeyEvent *pEvent); // menu owner calls this to check for menu-based hotkeys

#ifdef __cplusplus
};
#endif // __cplusplus

#endif // MENU_H_INCLUDED

