//////////////////////////////////////////////////////////////////////////////////////////
//                  _  _  _                 _             _                  _          //
//         ___   __| |(_)| |_    __      __(_) _ __    __| |  ___ __      __| |__       //
//        / _ \ / _` || || __|   \ \ /\ / /| || '_ \  / _` | / _ \\ \ /\ / /| '_ \      //
//       |  __/| (_| || || |_     \ V  V / | || | | || (_| || (_) |\ V  V /_| | | |     //
//        \___| \__,_||_| \__|_____\_/\_/  |_||_| |_| \__,_| \___/  \_/\_/(_)_| |_|     //
//                           |_____|                                                    //
//                                                                                      //
//                  a window into which you can type (and edit) text                    //
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


#ifndef _EDIT_WINDOW_H_INCLUDED_
#define _EDIT_WINDOW_H_INCLUDED_

#include "text_object.h"
#include "frame_window.h"
#include "child_frame.h"


/** \file edit_window.h
  * \brief Edit Window API functions
  *
  * Edit Windows are child windows in which the contained text may be edited, optionally
  * supporting multiple tabs.  'View Only' windows cannot be edited, but support all of
  * the other features of an edit window.\n
  * Each edited text object uses the TEXT_OBJECT structure to manage the data and the
  * user interface.  The currently active text object will receive all of the appropriate
  * events.  The Edit Window also provides for horizontal and vertical scrolling, as needed.
*/


/** \ingroup child_frame
  * \brief TAG for the WBEditWindow structure
**/
#define EDIT_WINDOW_TAG (*((const unsigned int * const)"FWEW"))


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// OBSOLETE 'IDEA' CODE, LEFT FOR REFERENCE
//
//typedef struct __WBLineBuffer
//{
//  int iLength;  // length of buffer (not including the structure size)
//  int iMaxPos;  // maximum position into which any text has been entered (usually end of line)
//  int iEndOfLine;  // the actual end of line (not counting trailing white space)
//  struct __WBLineBuffer *pPrev, *pNext;  // linked list of buffers
//  char data[4];  // the actual data for the line (typically ASCII)
//  // variable length following this
//} WBLineBuffer;
//
//typedef struct
//{
//  Window wSelf, wOwner;             // window identifiers for self, owner
//  XFontStruct *pFont;               // default font for the window
//  WBLineBuffer *pStart, *pEnd;      // pointers to start and end of linked buffer list
//  WBLineBuffer *pBufArray;          // array of buffers for visible lines
//  int nBufArray;                    // maximum size of buffer array
//  int iTop, iHeight, iLeft, iWidth; // line/char position and size of client area
//                                    // iTop corresponds to pBufArray[0]
//  int iMaxWidth, iMaxHeight;        // size of document in lines and chars
//  int iFlags;                       // various flags associated with the window
//  char *pszFileName;                // file name associated with edit window
//
//} WBEditWindow;
//
//enum
//{
//  WBEditWindowFlags_EncodingMask    = 0x7,
//  WBEditWindowFlags_EncodingASCII   = 0x0,
//  WBEditWindowFlags_EncodingUTF8    = 0x1,
//  WBEditWindowFlags_EncodingUTF16   = 0x2,
//  WBEditWindowFlags_EncodingOther   = 0x3, // 'other' implies locales, etc. (reserved)
//
//};


/** \ingroup edit_window
  * \struct __WBEditWindow__
  * \copydoc WBEditWindow
**/
/** \typedef WBEditWindow
  * \ingroup edit_window
  * \brief Structure that defines an Edit Window
  *
  * The WBEditWindow structure defines an 'Edit Window' that is a 'superclass' of
  * a Child Frame window.  The first element MUST be the 'WBChildFrame'.  Elements
  * that follow it are specific to the Edit Window.
  *
  * This uses a TEXT_OBJECT structure to store the actual text data, and to manage the
  * UI functionality.
  *
  * \code

  typedef struct __WBEditWindow__
  {
    WBChildFrame childframe;          // elements common to a 'child frame' (derived object)

    unsigned int ulTag;               // 'Tag' identifying this structure as a WBEditWindow

    char *szFileName;                 // malloc'd name of file associated with this edit window (NULL if none)

    WBWinEvent pUserCallback;         // user callback function to receive notifications and unhandled messages

    TEXT_OBJECT xTextObject;          // the 'TEXT_OBJECT' member, that does MOST of the work

  } WBEditWindow;

  * \endcode
  *
  * Additional 'Child Frame' API functions can be called directly by using a type cast from 'WBEditWindow *'
  * to 'WBChildFrame *', or by using the 'WBChildWindowFromWindowID()' function.
  *
  * \sa \ref TEXT_OBJECT
**/
typedef struct __WBEditWindow__
{
  WBChildFrame childframe;          ///< elements common to a 'child frame' (derived object)

  unsigned int ulTag;               ///< 'Tag' identifying this structure as a WBEditWindow

  char *szFileName;                 ///< malloc'd name of file associated with this edit window (NULL if none)

  WBWinEvent pUserCallback;         ///< user callback function to receive notifications and unhandled messages

  TEXT_OBJECT xTextObject;          ///< the 'TEXT_OBJECT' member, that does MOST of the work

} WBEditWindow;


// external atoms associated with edit windows, documented elsewhere

#ifndef _EDIT_WINDOW_C_IMPLEMENTED_
extern Atom aEW_HOVER_NOTIFY;
extern Atom aEW_EDIT_CHANGE;
#endif // _EDIT_WINDOW_C_IMPLEMENTED_

/** \ingroup edit_window
  * \brief Create an Edit Window
  *
  * \param pOwner A pointer to the owning WBFrameWindow.  Can not be NULL.
  * \param pFont The desired font, or NULL to use the default
  * \param szFocusMenu A const pointer to a text-based menu resource describing the menu that should appear when the Child Frame's tab has the focus.  Can be NULL if none.
  * \param pHandlerArray A const pointer to an array of WBFWMenuHandler structures for the 'focus' menu handler.  Can be NULL if none.
  * \param fFlags A bitwise 'or' of the desired flags associated with this Edit Window.  See 'WBChildFrame_FLAGS'
  * \returns a pointer to a WBEditWindow object describing the new edit window, or NULL on error
  *
  * This function allows you to create an Edit Window that is a regular child of a frame window or
  * dialog box, as a 'single document' handler.  To create a window that uses the maximum available
  * space within the client area of the parent, specify '-1' for iTop, iLeft, iWidth, and iHeight.
  *
  * Header File:  edit_window.h
**/
WBEditWindow *WBCreateEditWindow(WBFrameWindow *pOwner, XFontStruct *pFont,
                                 const char *szFocusMenu, const WBFWMenuHandler *pHandlerArray,
                                 int fFlags);


/** \ingroup edit_window
  * \brief Destroy an Edit Window
  *
  * \param pEditWindow A pointer to the WBEditWindow structure for the edit window
  * \returns void
  *
  * Call this function on any pointer returned by WBCreateEditWindow to destroy it and free its resources.
  *
  * Header File:  edit_window.h
**/
void WBDestroyEditWindow(WBEditWindow *pEditWindow);


/** \ingroup edit_window
  * \brief Obtain the associated WBEditWindow structure pointer for a Window ID
  *
  * \param wID A valid Window ID
  * \returns A pointer to the associated WBEditWindow structure (if it is a WBEditWindow), or NULL on error
  *
  * Use this function to safely obtain the correct WBEditWindow structure for a given Window ID.
  *
  * Header File:  edit_window.h
**/
WBEditWindow *WBEditWindowFromWindowID(Window wID);


/** \ingroup edit_window
  * \brief Open an existing file and read its contents into the Edit Window, storing the file name for later reference
  *
  * \param pEditWindow A pointer to the WBEditWindow structure
  * \param pszFileName A (const) pointer to a character string containing the file name.  PATH rules will be used to locate the actual file.
  * \returns A value of zero if successful, non-zero on error.
  *
  * Use this function to read a file into the Edit Window, replacing any existing contents.
  *
  * Header File:  edit_window.h
**/
int WBEditWindowLoadFile(WBEditWindow *pEditWindow, const char *pszFileName);


/** \ingroup edit_window
  * \brief Save the contents from the Edit Window to a file, overwriting the file if it already exists
  *
  * \param pEditWindow A pointer to the WBEditWindow structure
  * \param pszFileName A (const) pointer to a character string containing the file name.  PATH rules will be used to locate the actual file.  A value of NULL uses the stored file name.
  * \returns A value of zero if successful, non-zero on error.
  *
  * Use this function to write the contents of an Edit Window to a file, overwriting any existing file of the same name.
  *
  * Header File:  edit_window.h
**/
int WBEditWindowSaveFile(WBEditWindow *pEditWindow, const char *pszFileName);


/** \ingroup edit_window
  * \brief Clear the contents in the Edit Window, and NULL the stored file name
  *
  * \param pEditWindow A pointer to the WBEditWindow structure
  *
  * Use this function to clear an Edit Window, setting the file name to NULL
  *
  * Header File:  edit_window.h
**/
void WBEditWindowClear(WBEditWindow *pEditWindow);


/** \ingroup edit_window
  * \brief Check for valid WBEditWindow pointer
  *
  * \param pEditWindow A pointer to a WBEditWindow structure
  * \returns non-zero if valid, zero if NOT valid
  *
  * Header File:  edit_window.h
**/
static __inline int WBIsValidEditWindow(WBEditWindow *pEditWindow)
{
  if(!pEditWindow ||                                     // invalid pointer (TODO:  check range?)
     pEditWindow->childframe.ulTag != CHILD_FRAME_TAG || // child frame tag not valid
     pEditWindow->ulTag != EDIT_WINDOW_TAG)              // edit window tag not valid
  {
    return 0; // NOT valid.
  }

  return 1;
}


// CALLBACK REGISTRATION


/** \ingroup edit_window
  * \brief Clear the contents in the Edit Window, and NULL the stored file name
  *
  * \param pEditWindow A pointer to the WBEditWindow structure
  * \param pUserCallback A pointer to the callback function that handles messages (Return 0 for default handling, != 0 if handled).  Can be NULL.
  *
  * Use this function to specify a user callback for an Edit Window. By using this function to specify the callback,
  * you ensure that proper 'Edit Window' event handling will still occur.
  *
  * As with other callback functions that use WBWinEvent as their data type, you nee to return a value of zero
  * to indicate 'perform default handling', non-zero if you handle the event and do NOT want 'default' handling.
  *
  * Edit Windows can generate a number of custom Client Message events that are specific to Edit Windows.
  *
  * Header File:  edit_window.h
**/
void WBEditWindowRegisterCallback(WBEditWindow *pEditWindow, WBWinEvent pUserCallback);




#ifdef __cplusplus
};
#endif // __cplusplus

#endif // _EDIT_WINDOW_H_INCLUDED_

