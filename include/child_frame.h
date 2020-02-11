///////////////////////////////////////////////////////////////////////
//                                                                   //
//        _     _ _     _      __                            _       //
//    ___| |__ (_) | __| |    / _|_ __ __ _ _ __ ___   ___  | |__    //
//   / __| '_ \| | |/ _` |   | |_| '__/ _` | '_ ` _ \ / _ \ | '_ \   //
//  | (__| | | | | | (_| |   |  _| | | (_| | | | | | |  __/_| | | |  //
//   \___|_| |_|_|_|\__,_|___|_| |_|  \__,_|_| |_| |_|\___(_)_| |_|  //
//                      |_____|                                      //
//                                                                   //
//         a window that exists as a tab in a frame window           //
//                                                                   //
//           (part of the 'frame window' support API)                //
//                                                                   //
///////////////////////////////////////////////////////////////////////

/*****************************************************************************

    X11workbench - X11 programmer's 'work bench' application and toolkit
    Copyright (c) 2010-2019 by Bob Frazier (aka 'Big Bad Bombastic Bob')


  DISCLAIMER:  The X11workbench application and toolkit software are supplied
               'as-is', with no warranties, either implied or explicit.
               Any claims to alleged functionality or features should be
               considered 'preliminary', and might not function as advertised.

  MIT-like license:

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
  MIT-like license.  See COPYING and README files for more information.


  Additional information at http://sourceforge.net/projects/X11workbench

******************************************************************************/


#ifndef _CHILD_FRAME_H_INCLUDED_
#define _CHILD_FRAME_H_INCLUDED_

#include "text_object.h"
#include "frame_window.h"


/** \file child_frame.h
  * \brief Child Frame API functions
  *
  * Child Frames are child windows that display a tab in the Frame window, and have
  * specialized menus and menu handlers associated with them.\n
  * The Child Frame also provides for horizontal and vertical scrolling of the display
  * surface, as needed, varying the display region.\n
  * A derived window will need to register the appropriate callbacks, and have the
  * 'WBChildFrame' structure as the first member of its own structure.  When the derived
  * window creates the structure, it must initialize its own structure first, and then
  * call FWInitChildFrame() to complete the process.  And when the drived window is destroyed,
  * it must call FWDestroyChildFrame() to complete the 'destruction' process.\n
  * The behavior of a Child Frame is like a 'base class' in C++, with a kind of polymorphism
  * and abstraction used by the containing WBFrameWindow to manage focus, tabs, and layout changes.\n
  * The Child Frame then handles scrolling and 'menu change on focus change', so that each type of
  * Child Frame can have its own menu.  The Frame Window will change the displayed menu according to
  * the Child Frame that has the current focus, and display a default menu when there is no Child Frame.\n
  * A derived Child Frame maintains its own data as well as handling events.  So, rather than abstracting
  * the data from the Child Frame, this toolkit combines them (as it is simpler to do so).  If you need
  * extra levels of abstraction, just create more 'derived' versions from your own designs, and abstract
  * the 'next level up'.\n
**/


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


// SEE frame_window.h for WBChildFrame definition

/** \ingroup child_frame
  * \brief TAG for the WBChildFrame structure
**/
#define CHILD_FRAME_TAG (*((const unsigned int * const)"FWCF"))

/** \ingroup child_frame
  * \brief TAG for the WBChildFrameUI structure
**/
#define CHILD_FRAME_UI_TAG ((WB_UINT32)'F' | ((WB_UINT32)'W' << 8) | ((WB_UINT32)'C' << 16) | ((WB_UINT32)'U' << 24)) /* FWCU */

/** \ingroup child_frame
  * @{
**/

#define FW_FILE_NEW_MENU          "IDM_FILE_NEW"         /**< standard menu item identifier for 'File New' functionality **/
#define FW_FILE_CLOSE_MENU        "IDM_FILE_CLOSE"       /**< standard menu item identifier for 'File Close' functionality **/
#define FW_FILE_OPEN_MENU         "IDM_FILE_OPEN"        /**< standard menu item identifier for 'File Open' functionality **/
#define FW_FILE_SAVE_MENU         "IDM_FILE_SAVE"        /**< standard menu item identifier for 'File Save' functionality **/
#define FW_FILE_SAVE_AS_MENU      "IDM_FILE_SAVE_AS"     /**< standard menu item identifier for 'File Save As' functionality **/
#define FW_FILE_SAVE_ALL_MENU     "IDM_FILE_SAVE_ALL"    /**< standard menu item identifier for 'File Save All' functionality **/

#define FW_EDIT_CUT_MENU          "IDM_EDIT_CUT"         /**< standard menu item identifier for 'Edit Cut' functionality **/
#define FW_EDIT_COPY_MENU         "IDM_EDIT_COPY"        /**< standard menu item identifier for 'Edit Copy' functionality **/
#define FW_EDIT_PASTE_MENU        "IDM_EDIT_PASTE"       /**< standard menu item identifier for 'Edit Paste' functionality **/
#define FW_EDIT_DELETE_MENU       "IDM_EDIT_DELETE"      /**< standard menu item identifier for 'Edit Delete' functionality **/
#define FW_EDIT_SELECT_ALL_MENU   "IDM_EDIT_SELECT_ALL"  /**< standard menu item identifier for 'Edit Select All' functionality **/
#define FW_EDIT_SELECT_NONE_MENU  "IDM_EDIT_SELECT_NONE" /**< standard menu item identifier for 'Edit Select None' functionality **/
#define FW_EDIT_UNDO_MENU         "IDM_EDIT_UNDO"        /**< standard menu item identifier for 'Edit Undo' functionality **/
#define FW_EDIT_REDO_MENU         "IDM_EDIT_REDO"        /**< standard menu item identifier for 'Edit Redo' functionality **/
#define FW_EDIT_PROPERTIES_MENU   "IDM_EDIT_PROPERTIES"  /**< standard menu item identifier for 'Edit Properties' functionality **/

#define FW_FILE_NEW_ACCEL         "Ctrl+N"               /**< standard menu accelerator key for 'File New' functionality **/
#define FW_FILE_CLOSE_ACCEL       "Ctrl+F4"              /**< standard menu accelerator key for 'File Close' functionality **/
#define FW_FILE_OPEN_ACCEL        "Ctrl+O"               /**< standard menu accelerator key for 'File Open' functionality **/
#define FW_FILE_SAVE_ACCEL        "Ctrl+S"               /**< standard menu accelerator key for 'File Save' functionality **/
#define FW_FILE_SAVE_AS_ACCEL     "Ctrl+Shift+S"         /**< standard menu accelerator key for 'File Save As' functionality **/
#define FW_FILE_SAVE_ALL_ACCEL    "Ctrl+Shift+L"         /**< standard menu accelerator key for 'File Save All' functionality **/

#define FW_EDIT_CUT_ACCEL         "Ctrl+X"               /**< standard menu accelerator key for 'Edit Cut' functionality **/
#define FW_EDIT_COPY_ACCEL        "Ctrl+C"               /**< standard menu accelerator key for 'Edit Copy' functionality **/
#define FW_EDIT_PASTE_ACCEL       "Ctrl+V"               /**< standard menu accelerator key for 'Edit Paste' functionality **/
#define FW_EDIT_SELECT_ALL_ACCEL  "Ctrl+A"               /**< standard menu accelerator key for 'Edit Select All' functionality **/
#define FW_EDIT_SELECT_NONE_ACCEL "Ctrl+Shift+A"         /**< standard menu accelerator key for 'Edit Select None' functionality **/
#define FW_EDIT_UNDO_ACCEL        "Ctrl+Z"               /**< standard menu accelerator key for 'Edit Undo' functionality **/
#define FW_EDIT_REDO_ACCEL        "Ctrl+Shift+Z"         /**< standard menu accelerator key for 'Edit Redo' functionality **/
#define FW_EDIT_PROPERTIES_ACCEL  "Alt+Enter"            /**< standard menu accelerator key for 'Edit Properties' functionality **/

/**
  * @}
**/



/** \ingroup child_frame
  * \brief Initialize a child frame using a frame window parent (assumed to be a base 'class' for the window)
  *
  * \param pChildFrame a pointer to the WBChildFrame associated with the window.  Can not be NULL.
  * \param pOwner A pointer to the owning WBFrameWindow.  Can not be NULL.
  * \param pFont The desired WB_FONTC font, or 'NULL' to use the default
  * \param szFocusMenu A const pointer to a text-based menu resource describing the menu that should appear when the Child Frame's tab has the focus.  Can be NULL if none.
  * \param pHandlerArray A const pointer to an array of WBFWMenuHandler structures for the 'focus' menu handler.  Can be NULL if none.
  * \param pUserCallback A pointer to the callback function that handles messages (Return 0 for default handling, != 0 if handled).  Can be NULL.
  * \param fFlags A bitwise 'or' of the desired flags associated with this Child Frame.  See 'WBChildFrame_FLAGS'
  * \returns an integer value, 0 on success, anything else being an error code
  *
  * This function creates the actual window for a 'derived' Child Frame window, and correctly initializes
  * that portion of the internal data structure corresponding to a Child Frame.  The 'WBChildFrame' should
  * normally be the first member of the data structure representing the 'derived' Child Frame window, so you
  * can do a direct type cast of the pointer, similar to the way abstraction and polymorphism works in C++.\n
  * A child frame is not created directly.  Instead, create the 'derived' version's structure, and initialize
  * the structure for the derived behavior, with the WBChildFrame structure as the first element.  Then initialize
  * the WBChildFrame structure with this function, which will create the child frame window ID and register it with
  * the specified WBFrameWindow.\n
  * Failure to call this function will prevent the 'Child Frame' functionality from working correctly, and will not
  * register the relationship between the Window ID and the WBChildFrame structure.
  *
  * NOTE:  To create a stand-alone 'derived' Child Frame, first create a minimal frame window, with disabled 'tabs'.
  * Then, create the Child Frame 'derived' window using WBInitChildFrame().  Only one will be allowed.\n
  * NOTE 2:  If WBInitChildFrame() returns a non-zero value, do NOT call WBDestroyChildFrame() on that WBChildFrame .
  *
  * \sa \ref frame "Frame Windows"
  *
  * Header File:  child_frame.h
**/
int FWInitChildFrame(WBChildFrame *pChildFrame, WBFrameWindow *pOwner, WB_FONTC pFont,
                     const char *szFocusMenu, const WBFWMenuHandler *pHandlerArray,
                     WBWinEvent pUserCallback, int fFlags);

/** \ingroup child_frame
  * \brief Initialize a child frame using a child frame parent (assumed to be a base 'class' for the window)
  *
  * \param pChildFrame a pointer to the WBChildFrame associated with the window.  Can not be NULL.
  * \param pOwner A pointer to the owning WBChildFrame.  Can not be NULL.
  * \param pFont The desired WB_FONTC font, or 'NULL' to use the default
  * \param szFocusMenu A const pointer to a text-based menu resource describing the menu that should appear when the Child Frame's tab has the focus.  Can be NULL if none.
  * \param pHandlerArray A const pointer to an array of WBFWMenuHandler structures for the 'focus' menu handler.  Can be NULL if none.
  * \param pUserCallback A pointer to the callback function that handles messages (Return 0 for default handling, != 0 if handled).  Can be NULL.
  * \param fFlags A bitwise 'or' of the desired flags associated with this Child Frame.  See 'WBChildFrame_FLAGS'
  * \returns an integer value, 0 on success, anything else being an error code
  *
  * This function creates the actual window for a 'derived' Child Frame window, as a direct child
  * of a containing Child Frame window, and correctly initializes that portion of the internal data
  * structure corresponding to a Child Frame.  It is similar to \ref FWInitChildFrame(), except that the
  * owner is a \ref WBChildFrame, and not a \ref WBFrameWindow.  The target 'WBChildFrame' structure should
  * normally be the first member of the data structure representing the 'derived' Child Frame window, so you
  * can do a direct type cast of the pointer, similar to the way abstraction and polymorphism works in C++.\n
  * A child frame is not created directly.  Instead, create the 'derived' version's structure, and initialize
  * the structure for the derived behavior, with the WBChildFrame structure as the first element.  Then initialize
  * the WBChildFrame structure with this function, which will create the child frame window ID and register it with
  * the specified WBFrameWindow.\n
  * Failure to call this function will prevent the 'Child Frame' functionality from working correctly, and will not
  * register the relationship between the Window ID and the WBChildFrame structure.
  *
  * NOTE:  To create a stand-alone 'derived' Child Frame, first create a minimal frame window, with disabled 'tabs'.
  * Then, create the Child Frame 'derived' window using WBInitChildFrame().  Only one will be allowed.\n
  * NOTE 2:  If WBInitChildFrame() returns a non-zero value, do NOT call WBDestroyChildFrame() on that WBChildFrame .
  *
  * \sa \ref frame "Frame Windows"
  *
  * Header File:  child_frame.h
**/
int FWInitChildFrame2(WBChildFrame *pChildFrame, WBChildFrame *pOwner, WB_FONTC pFont,
                      const char *szFocusMenu, const WBFWMenuHandler *pHandlerArray,
                      WBWinEvent pUserCallback, int fFlags);

/** \ingroup child_frame
  * \brief Destroy an Child Frame
  *
  * \param pChildFrame A pointer to the WBChildFrame structure for the Child Frame
  * \returns void
  *
  * Call this function to destroy the actual window and de-reference
  *
  * Header File:  child_frame.h
**/
void FWDestroyChildFrame(WBChildFrame *pChildFrame);

/** \ingroup child_frame
  * \brief Function to assign the menu resource to a Child Frame
  *
  * \param pChildFrame The pointer to the WBChildFrame structure for the desired Child Frame window
  * \param szFocusMenu A const pointer to a text-based menu resource describing the menu that should appear when the Child Frame's tab has the focus
  *
  * Assigns the (new) menu for the Child Frame.  The text will be copied, and the copy will be
  * used internally, and free'd as necessary.  Only valid when the owner is a Frame Window.
  *
  * Header File:  child_frame.h
**/
void FWSetChildFrameMenu(WBChildFrame *pChildFrame, const char *szFocusMenu);

/** \ingroup child_frame
  * \brief Function to assign the context menu ID (from the menu resource) to a Child Frame
  *
  * \param pChildFrame The pointer to the WBChildFrame structure for the desired Child Frame window
  * \param nID The popup menu ID from the active menu's resource; can be the owner frame's menu OR a menu assigned by FWSetChildFrameMenu().
  *        The default is -1, which does not display a context menu
  *
  * Assigns the (new) context menu ID for the Child Frame.  A value of '-1' (the default) does not display
  * any context menu with SHIFT+F10 or right-mouse-click.  Otherwise, the active menu resource is searched
  * for a popup menu with the appropriate ID value, and the popup menu is displayed as if it were activated
  * using an appropriate menu hotkey.
  *
  * Header File:  child_frame.h
**/
void FWSetChildFrameContextMenuID(WBChildFrame *pChildFrame, int nID);

/** \ingroup child_frame
  * \brief Function to assign menu handlers to a Child Frame
  *
  * \param pChildFrame The pointer to the WBChildFrame structure for the desired Child Frame window
  * \param pHandlerArray A pointer to an array of WBFWMenuHandler structures - \sa \ref FW_MENU_HANDLER_ENTRY
  *
  * Assigns the (new) menu handlers for the Child Frame.  The array will be copied, and the copy will be
  * used internally, and free'd as necessary.
  *
  * Header File:  child_frame.h
**/
void FWSetChildFrameMenuHandlers(WBChildFrame *pChildFrame, const WBFWMenuHandler *pHandlerArray);

/** \ingroup child_frame
  * \brief Assign the display name
  *
  * \param pChildFrame The pointer to the WBChildFrame structure for the desired Child Frame window
  * \param szDisplayName A pointer to a character array containing the new display name text, to appear in the tab and/or title bar
  *
  * Assigns the display name for the Child Frame, to appear in the tab and (optionally) in the frame window's title bar
  * whenever the Child Frame has the focus.
  *
  * Header File:  child_frame.h
**/
void FWSetChildFrameDisplayName(WBChildFrame *pChildFrame, const char *szDisplayName);

/** \ingroup child_frame
  * \brief Assign the image atom for the child frame.  the image appears in the tab associated with the child frame
  *
  * \param pChildFrame The pointer to the WBChildFrame structure for the desired Child Frame window
  * \param aImage An atom that identifies the registered image.  The default is no image.  For more information, see PXM_RegisterPixmapResource().
  *
  * Assigns the Atom for the image to be displayed in the tab.  The default is no image.  The image will appear
  * on the far left edge of the tab, before the tab text.  Typically it will be an icon-like image that represents
  * the contents of the document being viewed/edited in that particular tab.\n
  * the image should be approximately 16 pixels high, and approximately 12 pixels wide, in order to to properly fit.
  *
  * Header File:  child_frame.h
**/
void FWSetChildFrameImageAtom(WBChildFrame *pChildFrame, Atom aImage);

/** \ingroup child_frame
  * \brief Set the X,Y extent for the child frame (notifies everything)
  *
  * \param pChildFrame The pointer to the WBChildFrame structure for the desired Child Frame window
  * \param iXExtent the 'X' extent of the display surface
  * \param iYExtent the 'Y' extent of the display surface
  *
  * Use this function to assign the X and Y extents for the display surface on a Child Frame window.  Calling
  * this function will set up the scrolling behavior as well as notifying 'everything' of the changes, such
  * as re-painting the window or re-sizing something.
  *
  * A 'superclass' callback function handling aRESIZE_NOTIFY should not call this function.  Instead, it
  * should call FWSetChildFrameScrollInfo() instead, which will not cause recursion.
  *
  * Header File:  child_frame.h
**/
void FWSetChildFrameExtent(WBChildFrame *pChildFrame, int iXExtent, int iYExtent);


/** \ingroup child_frame
  * \brief Set the X,Y extent for the child frame (notifies everything)
  *
  * \param pChildFrame The pointer to the WBChildFrame structure for the desired Child Frame window
  * \param iRow The current row (0-based) for the cursor
  * \param iMaxRow The total number of rows in the current object being displayed or edited
  * \param iCol The current column (0-based) for the cursor
  * \param iMaxCol The maximum number of columns in the current object being displayed or edited
  * \param iRowHeight The height of a single row (in pixels).  For text, this would be the font height plus row spacing.
  * \param iColWidth The width of a single column (in pixels).  For text, this would be the average character width.
  *
  * Use this function to assign the current scroll information and update the 'extent' information
  * (similar to 'FWSetChildFrameExtent()').  It is safe to call this from within the 'superclass' callback
  * function that is handling an aRESIZE_NOTIFY Client Message event.
  *
  * Header File:  child_frame.h
**/
void FWSetChildFrameScrollInfo(WBChildFrame *pChildFrame, int iRow, int iMaxRow, int iCol, int iMaxCol,
                               int iRowHeight, int iColWidth);


/** \ingroup child_frame
  * \brief Child frame notification callback (called by frame window)
  *
  * \param pChildFrame The pointer to the WBChildFrame structure for the desired Child Frame window
  *
  * The frame window will call this function whenever the layout changes, including at the point the
  * child frame is added to the frame window.  Internally, it will call the user-defined event handler
  * function with an event type of 'ClientMessage' and message type \ref aRESIZE_NOTIFY "RESIZE_NOTIFY"
  *
  * The 'superclass' user-defined event handler MUST update scroll bar info when it receives this
  * message, in order for scrolling to work correctly, and should NOT call FWChildFrameRecalcLayout()
  * when processing the aRESIZE_NOTIFY message.
  *
  * Header File:  child_frame.h
**/
void FWChildFrameRecalcLayout(WBChildFrame *pChildFrame);


/** \ingroup child_frame
  * \brief Notify Child Frame to update status text in Frame Window
  *
  * \param pChildFrame The pointer to the WBChildFrame structure for the desired Child Frame window
  *
  * This function notifies the Child Frame that the status has changed, and it needs to update the
  * status text in the associated Frame Window.  The status text is NOT generated by the child frame
  * object itself; rather, the derived 'superclass' window updates it internally.  This function is
  * simply a notification to update the frame window display.  It may be called internally, or
  * at any time whenever the status might have changed.  This includes 'tab switch'.
  *
  * Header File:  child_frame.h
**/
void FWChildFrameStatusChanged(WBChildFrame *pChildFrame);

/** \ingroup child_frame
  * \brief Returns if it's safe to close the child frame; prompts user as needed
  *
  * \param pChildFrame The pointer to the WBChildFrame structure for the desired Child Frame window
  * \returns An integer value, 0 for 'ok to close', 1 for 'not ok to close', -1 on error
  *
  * Use this function to safely determine whether it's ok to close a WBChildFrame window.  The user may be prompted.
  *
  * Internally, this will send a QUERY_CLOSE Client Message Event to the handler function.  the default is to return
  * a zero value, which is 'ok to close'.  Your own function needs to handle this to prompt to save unsaved data and
  * allow the user to cancel (thus returning a '1').  The message is invoked via an 'internal' Send Event which will
  * directly call the handler functions.
  *
  * Header File:  child_frame.h
**/
int FWChildFrameQueryClose(WBChildFrame *pChildFrame);


/** \ingroup child_frame
  * \brief Default event handler for Child Frame window
  *
  * \param wID A valid Window ID for a Child Frame window
  * \param pEvent A pointer to the XEvent structure
  * \returns An integer value, in which zero indicates "not handled", and non-zero indicates "handled"
  *
  * Use this function to safely obtain the correct WBChildFrame structure for a given Window ID.
  *
  * Header File:  child_frame.h
**/
int FWChildFrameEvent(Window wID, XEvent *pEvent);


/** \ingroup child_frame
  * \brief Obtain the associated WBChildFrame structure pointer for a Window ID
  *
  * \param wID A valid Window ID
  * \returns A pointer to the associated WBChildFrame structure (if it is a WBChildFrame), or NULL on error
  *
  * Use this function to safely obtain the correct WBChildFrame structure for a given Window ID.
  *
  * Header File:  child_frame.h
**/
static __inline__ WBChildFrame *FWGetChildFrameStruct(Window wID)
{
  WBChildFrame *pRval = (WBChildFrame *)WBGetWindowData(wID, 0);  // offset 0 for window-specific structs
  if(pRval && pRval->ulTag == CHILD_FRAME_TAG)
  {
    return(pRval);
  }

  return(NULL);
}



#ifdef __cplusplus
};
#endif // __cplusplus

#endif // _CHILD_FRAME_H_INCLUDED_

