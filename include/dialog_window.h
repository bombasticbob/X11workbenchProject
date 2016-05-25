/////////////////////////////////////////////////////////////////////////////////////////////
//      _  _         _                             _             _                  _      //
//   __| |(_)  __ _ | |  ___    __ _    __      __(_) _ __    __| |  ___ __      __| |__   //
//  / _` || | / _` || | / _ \  / _` |   \ \ /\ / /| || '_ \  / _` | / _ \\ \ /\ / /| '_ \  //
// | (_| || || (_| || || (_) || (_| |    \ V  V / | || | | || (_| || (_) |\ V  V /_| | | | //
//  \__,_||_| \__,_||_| \___/  \__, |_____\_/\_/  |_||_| |_| \__,_| \___/  \_/\_/(_)_| |_| //
//                             |___/|_____|                                                //
//                                                                                         //
//                  modal or modeless window based on a dialog template                    //
//                                                                                         //
/////////////////////////////////////////////////////////////////////////////////////////////

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


#ifndef DIALOG_WINDOW_H_INCLUDED
#define DIALOG_WINDOW_H_INCLUDED

#include "window_helper.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


/** \file dialog_window.h Definitions for dialog (frame) window and intercommunication structures and APIs
*/

#define DIALOG_WINDOW_TAG (*((const unsigned int *)"DLGW"))
#define DIALOG_ENTRY_TAG (*((const unsigned int *)"DLGE"))

/** \ingroup dialog
  * \defgroup dlgwindow Dialog (Frame) Windows
  *
  * Dialog (Frame) Windows are specialized frame windows that normally execute
  * as MODAL windows ( see WBShowModal() and WBEndModal() ).\n
  * A lot of specialized processing needs to be done for a proper dialog window, including
  * all of the hot keys necessary to change focus and activate 'default' buttons and controls.
  * Additionally, it is often desirable to create a dialog window 'on the fly' from a resource
  * that can be language-dependent or content-dependent (or both).  For this reason the Dialog
  * portion of the X11 Work Bench Toolkit API supports dialog window creation from a text resource,
  * using registered identifier names within the resource text.
*/
/** \ingroup dialog
  * \defgroup dlgctrl Dialog Box Controls
  *
  * Structures and API functions used for dialog controls
*/

/** \ingroup dialog
  * \defgroup dlglist List Support
  *
  * Designed specifically for dialog controls, 'List Support' APIs and
  * structures contain lists of objects or strings that are 'self-maintained'
  * via special callback functions assigned to the 'List Info' structure.
*/

/** \ingroup dialog
  * \defgroup dlgatom Dialog Window ClientMessage Atoms
  *
  * The following atoms are used by dialog windows in ClientMessage events
  * for intercommunication between controls and the dialog frame window, or
  * to notify the dialog box callback function of certain events (like initialization).
*/

/** \ingroup dlgwindow
  * \typedef WBDialogEntry
  * \brief Structure identifying one of the controls that appears on a dialog window
  *
  * Public members of the structure that identifies a dialog control
\code
  typedef struct __WB_DIALOG_ENTRY__
  {
    unsigned int ulTag;  // tag word DIALOG_ENTRY_TAG
    Window wID;          // window ID of control
    Atom aClass;         // control class identifier
    int iID;             // control unique identifier (number or atom, as appropriate, unique to dialog window)
    int iFlags;          // visibility, focus, etc.
    int iX,              // horizontal position of upper left corner of control (client-relative)
        iY,              // vertical position of upper left corner of control (client-relative)
        iWidth,          // width of control
        iHeight;         // height of control
  } WBDialogEntry;
\endcode
  * see also \ref __WB_DIALOG_ENTRY__
*/
/** \ingroup dlgwindow
  * \struct __WB_DIALOG_ENTRY__
  * \brief Structure identifying one of the controls that appears on a dialog window
  *
  * This structure identifies one of the controls that appears on a dialog window.  Each control will
  * uniquely map to one of these structures.
*/



typedef struct __WB_DIALOG_ENTRY__
{
  unsigned int ulTag; ///< tag word, always assigned to DIALOG_ENTRY_TAG
  Window wID;         ///< window ID of control
  Atom aClass;        ///< control class identifier
  int iID;            ///< control unique identifier (number or atom, as appropriate, unique to dialog window)
  int iFlags;         ///< visibility, focus, etc.
  int iX,             ///< horizontal position of upper left corner of control (client-relative)
      iY,             ///< vertical position of upper left corner of control (client-relative)
      iWidth,         ///< width of control
      iHeight;        ///< height of control

} WBDialogEntry;

/** \ingroup dlgwindow
  * \typedef WBDialogWindow
  * \brief Structure identifying a dialog (frame) window
  *
  * Public members of the structure that identifies a dialog (frame) window.
\code
  typedef struct __WB_DIALOG_WINDOW__
  {
    unsigned int ulTag;    // tag word, always assigned to DIALOG_WINDOW_TAG
    Window wID;            // window ID of the dialog (frame) window
    Window wIDOwner;       // window ID of the dialog's OWNER (can be 'None', should be assigned for Modal dialogs)
    void *pUserData;       // user data that's assignable via API
    int iFlags;            // bitmask of attribute flags (see 'WBDialogWindowFlags' enumeration)
    int iClientX,          // horizontal position of upper left corner of client area, relative to the window
        iClientY,          // vertical position of upper left corner of client area, relative to the window
        iClientWidth,      // width of the dialog window's client area
        iClientHeight;     // height of the dialog window's client area
    XColor clrFG,          // foreground pixel color
           clrBG,          // background pixel color
           clrBD;          // border pixel color
  } WBDialogWindow;
\endcode
  * see also \ref __WB_DIALOG_WINDOW__
*/
/** \ingroup dlgwindow
  * \struct __WB_DIALOG_WINDOW__
  * \brief Structure identifying a dialog (frame) window
  *
  * This structure identifies the dialog (frame) window.  The public members are
  * documented in this file.  Private members store information that is only accessible
  * using the API functions (such as an array of \ref WBDialogEntry structures).
*/

typedef struct __WB_DIALOG_WINDOW__
{
  unsigned int ulTag;    ///< tag word, always assigned to DIALOG_WINDOW_TAG
  Window wID;            ///< window ID of the dialog (frame) window
  Window wIDOwner;       ///< window ID of the dialog's OWNER (can be 'None', should be assigned for Modal dialogs)
  void *pUserData;       ///< user data that's assignable via API
  int iFlags;            ///< bitmask of attribute flags (see 'WBDialogWindowFlags' enumeration)
  int iClientX,          ///< horizontal position of upper left corner of client area, relative to the window
      iClientY,          ///< vertical position of upper left corner of client area, relative to the window
      iClientWidth,      ///< width of the dialog window's client area
      iClientHeight;     ///< height of the dialog window's client area
  XColor clrFG,          ///< foreground pixel color
         clrBG,          ///< background pixel color
         clrBD;          ///< border pixel color
} WBDialogWindow;

/** \ingroup dlgwindow
  * \hideinitializer
  * \brief enumeration for the iFlags member of WBDialogWindow */
enum WBDialogWindowFlags
{
  /** \brief Application Window flag
    *
    * set this flag for an application top-level dialog window so that whenver it is destroyed the application will exit
  **/
  WBDialogWindow_APP_WINDOW  = 1,
  /** \brief Visibility flag
    *
    * set this to make the window immediately visible.\n
    * \sa \ref WBDialogWindow_CENTERMOUSE, \ref WBDialogWindow_DOMODAL
  **/
  WBDialogWindow_VISIBLE     = 2,
  /** \brief Modality flag
    *
    * When this flag is set, the act of creating the dialog window will also call WBShowModal() automatically.
    * Also implies \ref WB_DialogWindow_VISIBLE, \ref WB_DialogWindow_CENTERMOUSE
  **/
  WBDialogWindow_DOMODAL     = 4,
  /** \brief Auto-center mouse pointer
    *
    * When this flag is set, the act of creating the dialog window will center the mouse over the dialog box.\n
    * NOTE: This only works properly when WBDialogWindow_VISIBLE is ALSO set.
  **/
  WBDialogWindow_CENTERMOUSE = 8,
  /** \brief Horizontal Resize flag
    *
    * When this flag is set, the dialog can be re-sized horizontally with the UI
  **/
  WBDialogWindow_HRESIZE     = 0x100,
  /** \brief Vertical Resize flag
    *
    * When this flag is set, the dialog can be re-sized vertically with the UI
  **/
  WBDialogWindow_VRESIZE     = 0x200,

  /** \brief MAX flag (reserved)
    *
    * Identifies the maximum value of assignable bit flags
  **/
  WBDialogWindow_MAX = 0x80000000L
};

/** \ingroup dlgwindow
  * \hideinitializer
  * \brief enumeration for the iFlags member of WBDialogEntry */
enum WBDialogEntryFlags
{
  WBDialogEntry_VISIBLE = 0x1,           ///< visible item
  WBDialogEntry_EDIT = 0x2,              ///< user-editable text
  WBDialogEntry_MULTILINE = 0x4,         ///< for edit controls, prevents trapping &lt;ENTER&gt;
  WBDialogEntry_ALLCHARS = 0x8,          ///< prevents trapping &lt;ESC&gt; &lt;ENTER&gt; and anything else

// TODO:  other user-definable flags


// the remaining bits are reserved for internal use

  // TODO:  others

  // alignment flags - these match the 'CONTROL_' bit flags in dialgo_controls.h

  WBDialogEntry_HResize         = 0x00040000, ///< dialog box flag, can be resized horizontally
  WBDialogEntry_VResize         = 0x00080000, ///< dialog box flag, can be resized vertically
  WBDialogEntry_ResizeMask      = 0x000c0000, ///< resize mask for dialog box

  WBDialogEntry_VAlignFixed     = 0x00000000, ///< default position re-alignment with respect to center (as a percentage, no stretching)
  WBDialogEntry_VAlignTop       = 0x00100000, ///< resize maintains position/size with respect to top
  WBDialogEntry_VAlignBottom    = 0x00200000, ///< resize maintains position/size with respect to bottom
  WBDialogEntry_VAlignCenter    = 0x00300000, ///< re-alignment with respect to center (as a percentage, stretching allowed)
  WBDialogEntry_VAlignMask      = 0x00300000, ///< vertical alignment mask

  WBDialogEntry_HAlignFixed     = 0x00000000, ///< default position re-alignment with respect to center (as a percentage, no stretching)
  WBDialogEntry_HAlignLeft      = 0x00400000, ///< resize maintains position/size with respect to left
  WBDialogEntry_HAlignRight     = 0x00800000, ///< resize maintains position/size with respect to right
  WBDialogEntry_HAlignCenter    = 0x00c00000, ///< re-alignment with respect to center (as a percentage, stretching allowed)
  WBDialogEntry_HAlignMask      = 0x00c00000, ///< horizontal alignment mask
  WBDialogEntry_AlignMask       = 0x00f00000, ///< alignment mask (both horizontal and vertical)


  WBDialogEntry_Reserved24      = 0x01000000, ///< reserved bit 24
  WBDialogEntry_Reserved25      = 0x02000000, ///< reserved bit 25
  WBDialogEntry_Reserved26      = 0x04000000, ///< reserved bit 26
  WBDialogEntry_PRESSED         = 0x08000000, ///< for pushbuttons, causes it to be drawn 'pressed'

  // focus and 'default' flags

  WBDialogEntry_CAN_HAVE_FOCUS  = 0x10000000, ///< set when item CAN have focus.
  WBDialogEntry_HAS_FOCUS       = 0x20000000, ///< set when item gets focus.  cleared when no focus
  WBDialogEntry_PUSHBUTTON      = 0x40000000, ///< set for 'pushbuttons' (which override 'default' for &lt;ENTER&gt;)
  WBDialogEntry_DEFAULT         = 0x80000000  ///< set for "default" controls

};

/** \ingroup dialog
  * \brief create a dialog window using a text resource
  *
  * \returns Pointer to WBDialogWindow structure that identifies the dialog (frame) window
  * \param szTitle Pointer to a null-byte terminated string containing the title
  * \param szDialogResource Pointer to a null-byte terminated string containing the dialog resource
  * \param iX Screen X coordinate of upper left corner of dialog window (-1 for default)
  * \param iY Screen Y coordinate of upper left corner of dialog window (-1 for default)
  * \param iWidth desired width of dialog window (-1 for default)
  * \param iHeight desired height of dialog window (-1 for default)
  * \param pUserCallback The dialog window's callback function - see \ref WBWinEvent
  * \param iFlags Various creation flags, see \ref WBDialogWindowFlags
  * \param pUserData A 'user data' pointer that is to be associated with the dialog window
  *
  * Use this high-level function to create a dialog window with a text resource.  The text resource
  * contains information about constructing the dialog window.  Standard symbol names are supported,
  * along with numeric symbols.
  *
  * Within a specified object (control or dialog) custom properties are passed to the control (reserved)
  * upon creation.  All properties are specified using a single name, optionally followed by a value with
  * a ':' separating the name and value.  Properties and Values with embedded white space must be quoted.
  *
  * A typical dialog window resource might look as follows:\n
\code
  BEGIN_DIALOG FONT:Variable HEIGHT:50 WIDTH:200 TITLE:"Sample Message Box Dialog Window"
    CONTROL:Icon ID:1000 X:2 Y:2 HEIGHT:20 WIDTH:20 VISIBLE
    CONTROL:Text ID:1001 X:24 Y:2 HEIGHT:20 WIDTH:172 VISIBLE
    CONTROL:DefPushButton ID:IDOK TITLE:OK X:80 Y:28 WIDTH:40 HEIGHT:18 VISIBLE
  END_DIALOG
\endcode
  *
  * For more information, see \ref dialog_resource .
*/

WBDialogWindow *DLGCreateDialogWindow(const char *szTitle, const char *szDialogResource,
                                      int iX, int iY, int iWidth, int iHeight,
                                      WBWinEvent pUserCallback, int iFlags,
                                      void *pUserData);

/** \ingroup dlgwindow
  * \brief Assign owning window to dialog
  *
  * The 'owning' window defines the window that the dialog belongs to such that when
  * the dialog is destroyed (and currently has focus) this window will automatically
  * be raised and assigned the input focus.  This is NOT the same as the parent window!
*/
void DLGAssignOwner(WBDialogWindow *pDlg, Window wIDOwner);

/** \ingroup dlgwindow
  * \brief Force re-calculation of dialog window (and control) layout
  *
  * Forcing re-calculation of the layout implies that size, position, and content information
  * is to be re-evaluated and updated, as needed, in an implementation-specific manner.
*/
void DLGRecalcLayout(Window wID);  // recalculate layout information (propagates to children)

/** \ingroup dlgwindow
  * \brief Height of a single dialog unit (in pixels)
  *
  * Return the height of a dialog unit, in pixels.  Currently hard-coded to '1'
*/
int DLGPixelHeight(WBDialogWindow *pDlg, int iHeight);  // pixel height from dialog units

/** \ingroup dlgwindow
  * \brief Width of a single dialog unit (in pixels)
  *
  * Return the width of a dialog unit, in pixels.  Currently hard-coded to '1'
*/
int DLGPixelWidth(WBDialogWindow *pDlg, int iWidth);    // pixel width from dialog units


/** \ingroup dlgwindow
  * \brief Assign the callback function for a dialog window
  *
  * The dialog's callback function is similar to that of a regular window, except that any
  * events that are NOT handled (function returns zero) will be processed by a special
  * 'default handler' that deals with the specific nuances associated with dialog windows.
  * As a result it is important to be careful when processing certain kinds of events.  When
  * in doubt, return zero anyway.\n
  * NOTE:  not all events will pass through the callback.  Some are handled solely by the dialog window default processing
*/
void DLGSetUserCallback(WBDialogWindow *pDLG, WBWinEvent pCallBack);
  // assigns the user-defined callback function (one only).  Not all messages pass through the callback

/** \ingroup dlgwindow
  * \brief return non-zero if a hot-key was processed and no further processing should be done.  Otherwise returns zero.
  * \internal
  *
  * This call appears near the beginning of the dialog window's window callback to determine if a hot-key is present, and
  * blocks further processing if a hot-key is handled.\n
*/
int DLGProcessHotKey(WBDialogWindow *pDLG, XEvent *pEvent);
  // returns non-zero value if hot-key was processed and no further event processing
  // should be performed.  Otherwise returns zero.  Default dialog callback handles this
  // automatically (as does the default dialog control callback, when applicable).


/** \ingroup dlgwindow
  * \brief Returns a pointer to the dialog window's \ref WBDialogWindow structure
  *
  * Use this function to obtain a pointer to the WBDialogWindow structure for the dialog (frame) window.
*/
static __inline__ WBDialogWindow *DLGGetDialogWindowStruct(Window wID)  // for dialog windows, returns WBDialogWindow struct
{
  WBDialogWindow *pRval = (WBDialogWindow *)WBGetWindowData(wID, 0);  // offset 0 for window-specific structs

  if(pRval && DIALOG_WINDOW_TAG == pRval->ulTag)
  {
    return(pRval);
  }

//  if(pRval && pRval->ulTag != DIALOG_WINDOW_TAG)
//  {
//    WB_ERROR_PRINT("ERROR:  DLGGetDialogWindowStruct, bad tag: %08xH %08xH\n", DIALOG_WINDOW_TAG, pRval->ulTag);
//  }
//  else // if(!pRval)
//  {
//    WB_ERROR_PRINT("ERROR:  DLGGetDialogWindowStruct - pointer is NULL\n");
//  }

  return(NULL);
}

/** \ingroup dlgwindow
  * \brief Returns a pointer to the dialog window's 'user data' assigned at create time
  *
  * Use this function to obtain a pointer to the 'user data' for the dialog (frame) window.
  * This function returns NULL if there is no \ref WBDialogWindow structure associated
  * with the window, or if there is no user data assigned.
*/
void * DLGGetDialogWindowUserData(Window wID);

/** \ingroup dlgwindow
  * \brief Destroy a modeless dialog window via its window ID
  *
  * Use this function to destroy a modeless dialog window using the window ID of the dialog (frame) window.
  * This also causes the 'WBDialogWindow' structure to be destroyed.  Use caution if you retained a pointer to it.
*/
void DLGDestroyDialogWindow(Window wID);  // destroys frame window using the Window ID (frees the struct also)
/** \ingroup dlgwindow
  * \brief Destroy a modeless dialog window via a pointer to its WBDialogWindow structure
  *
  * Use this function to destroy a modeless dialog window using the WBDialogWindow for the dialog (frame) window.
  * This also causes the 'WBDialogWindow' structure to be destroyed.  Use caution if you retained a pointer to it.
*/
void DLGDestroyDialogWindow2(WBDialogWindow *pDialogWindow); // destroys it using the struct pointer
// after calling either 'Destroy' function the DIALOG_WINDOW struct is no longer valid


// control query and enumeration

/** \ingroup dlgwindow
  * \brief returns the Window ID of a member control of the dialog window using the 'control ID'
  *
  * To obtain the Window ID of a member control, using the control's ID, call this function.
*/
Window DLGGetDialogControl(WBDialogWindow *pDialog, int iControlID);  // returns first match
/** \ingroup dlgwindow
  * \brief returns a pointer to the WBDialogEntry for a member control using the control's Window ID
  *
  * This is usually the preferred method of obtaining information about a dialog control.  Call
  * this function to obtain a pointer to the WBDialogEntry for a control, passing the WBDialogWindow
  * pointer and the window ID.  The return value from DLGGetDialogControl() can be passed directly
  * as the control's Window ID.
*/
WBDialogEntry *DLGGetDialogControlEntry(WBDialogWindow *pDialog, Window idControl);  // returns matching WBDialogEntry

/** \ingroup dlgwindow
  * \brief Return the Window ID of the FIRST dialog control within the 'tab order'
  *
  * Each dialog window has what can be called a 'tab order' in which the FIRST control listed in
  * the resource is the FIRST control in the tab order, the next control listed in the resource
  * is the NEXT control, and so on.  Tab order has nothing to do with the control's position
  * within the dialog window.  It is solely dependent on its order in the resource.
*/
Window DLGGetFirstDialogControl(WBDialogWindow *pDialog);  // first dialog control in tab order
/** \ingroup dlgwindow
  * \brief Return the Window ID of the NEXT dialog control within the 'tab order', based on the specified control window ID
  *
  * Each dialog window has what can be called a 'tab order' in which the FIRST control listed in
  * the resource is the FIRST control in the tab order, the next control listed in the resource
  * is the NEXT control, and so on.  Tab order has nothing to do with the control's position
  * within the dialog window.  It is solely dependent on its order in the resource.\n
  * If the specified control ID is the LAST one, this function returns the FIRST dialog control's window ID
*/
Window DLGGetNextDialogControl(WBDialogWindow *pDialog, Window idControl);  // control following wIDControl
/** \ingroup dlgwindow
  * \brief Return the Window ID of the PREVIOUS dialog control within the 'tab order', based on the specified control window ID
  *
  * Each dialog window has what can be called a 'tab order' in which the FIRST control listed in
  * the resource is the FIRST control in the tab order, the next control listed in the resource
  * is the NEXT control, and so on.  Tab order has nothing to do with the control's position
  * within the dialog window.  It is solely dependent on its order in the resource.\n
  * If the specified control ID is the FIRST one, this function returns the LAST dialog control's window ID
*/
Window DLGGetPrevDialogControl(WBDialogWindow *pDialog, Window idControl);  // control prior to wIDControl
  // NOTE:  'GetNext' and 'GetPrev' wrap around and may return the same window ID


// POSTING AND SENDING NOTIFICATION MESSAGES

/** \ingroup dlgwindow
  * \brief Notify dialog window by calling the window's own callback function directly with an event
  *
  * Use this function when immediate notification is needed, and there is no possibility
  * of a blocking call, recursion, or invalid state as a result of the event processing.
  *
  * \sa  DLGNotifyDlgAsync()
*/
static __inline__ void DLGNotifyDlg(WBDialogWindow *pDLG, Atom aNotify,
                                    long lData0, long lData1, long lData2, long lData3, long lData4)
{
Display *pDisplay = WBGetWindowDisplay(pDLG->wID);

  XClientMessageEvent evt = {
                              .type=ClientMessage,
                              .serial=0,
                              .send_event=0,
                              .display=pDisplay,
                              .window=pDLG->wID,
                              .message_type=aNotify,
                              .format=32
                            };
  evt.data.l[0] = lData0;
  evt.data.l[1] = lData1;
  evt.data.l[2] = lData2;
  evt.data.l[3] = lData3;
  evt.data.l[4] = lData4;

  WBWindowDispatch(pDLG->wID, (XEvent *)&evt);
}


/** \ingroup dlgwindow
  * \brief Notify dialog window by posting an event that will ASYNCHRONOUSLY be sent to the dialog window's callback function
  *
  * Use this function whenever immediate notification is NOT needed, or if there is a possibility
  * of a blocking call, recursion, or invalid state as a result of a direct call to the window's
  * callback function.
  *
  * \sa  DLGNotifyDlg()
*/
static __inline__ void DLGNotifyDlgAsync(WBDialogWindow *pDLG, Atom aNotify,
                                         long lData0, long lData1, long lData2, long lData3, long lData4)
{
Display *pDisplay = WBGetWindowDisplay(pDLG->wID);

  XClientMessageEvent evt = {
                              .type=ClientMessage,
                              .serial=0,
                              .send_event=0,
                              .display=pDisplay,
                              .window=pDLG->wID,
                              .message_type=aNotify,
                              .format=32
                            };
  evt.data.l[0] = lData0;
  evt.data.l[1] = lData1;
  evt.data.l[2] = lData2;
  evt.data.l[3] = lData3;
  evt.data.l[4] = lData4;

  WBPostPriorityEvent(pDLG->wID, (XEvent *)&evt);
}



// special pre-defined dialog windows

/** \ingroup dialog
  * \brief Message Box flag enumeration
  *
  * Use these bit-flags and masks for calls to DLGMessageBox()\n
  * You should 'or' one or more of the 'Button Bits' constants with one of the 'Icon' constants
*/
enum MessageBoxEnum
{
    MessageBox_Error=0,              ///< Icon - red stop sign
    MessageBox_Warning=1,            ///< Icon - exclamation point on yellow triangle
    MessageBox_Info=2,               ///< Icon - green circle
    MessageBox_Asterisk=3,           ///< Icon - white asterisk on blue circle
    MessageBox_Question=4,           ///< Icon - white question mark in green triangle
    MessageBox_BoldQuestion=5,       ///< Icon - black question mark in yellow triangle
    MessageBox_WTF=6,                ///< Icon - multiple question marks in red triangle
    MessageBox_Bang=7,               ///< Icon - yellow circle with white exclamation point
    MessageBox_Triangle=8,           ///< Icon - yellow triangle (nothing else)
    MessageBox_FaceOfDeath=9,        ///< Icon - white fright mask on black circle
    MessageBox_JollyRoger=10,        ///< Icon - white skull and crossbones on black square
    MessageBox_ThumbsUp=11,          ///< Icon - white thumb up on green square
    MessageBox_ThumbsDown=12,        ///< Icon - white thumb down on red square
    MessageBox_MiddleFinger=13,      ///< Icon - white 'middle finger' on red triangle
    MessageBox_TeddyBear=14,         ///< Icon - brown and tan teddy bear on grey circle
    MessageBox_Barney=15,            ///< Icon - purple T-Rex head on white circle (more like mozilla)
    MessageBox_App=16,               ///< Icon - application icon (from icon_app.xpm)

    MessageBox_ICON_MASK = 0x1f,     ///< mask for icon bits

    // 'or' with one or more button types (none implies 'ok' by itself)
    // NOTE: left-right ordering by numeric value (lowest is left-most)

    MessageBox_OK          = 0x20,   ///< Button Bits - OK button
    MessageBox_Yes         = 0x40,   ///< Button Bits - Yes button
    MessageBox_No          = 0x80,   ///< Button Bits - No button
    MessageBox_Abort       = 0x100,  ///< Button Bits - Abort button
    MessageBox_Retry       = 0x200,  ///< Button Bits - Retry button
    MessageBox_Ignore      = 0x400,  ///< Button Bits - Ignore button
    MessageBox_Cancel      = 0x800,  ///< Button Bits - Cancel button

    MessageBox_BUTTON_MASK = 0xfe0   ///< mask for button bits
};

/** \ingroup dialog
  * \brief Display a modal 'message box' dialog window with a specific title, message, and button combination
  *
  * \return The Control ID of the button that was pressed, or -1 (equivalent to 'IDCANCEL') on error
  * \param iType A combination of bits from the \ref MessageBoxEnum enumeration
  * \param wIDOwner The Window ID of the owner (can be 'None')
  * \param szTitle A null-byte terminated string containing the title, to be displayed in the window manager's title area
  * \param szMessage A null-byte terminated string that is to be displayed in a message area on the dialog window
  *
  * Use this function to display a simple modal 'message box' dialog window, and return the button that was
  * pressed for further processing.
*/
int DLGMessageBox(int iType, Window wIDOwner, const char *szTitle, const char *szMessage);

/** \ingroup dialog
  * \brief Displays a special purpose dialog window that retrieves a character string as input
  *
  * \return A 'malloc'd memory block containing a null-byte terminated input string, or NULL on cancel/error
  * \param wIDOwner Owner Window ID (or None)
  * \param szTitle A null-byte terminated string containing the title, to be displayed in the window manager's title area
  * \param szPrompt A null-byte terminated string that is to be displayed in a message area on the dialog window
  * \param szDefault A null-byte terminated string containing the default value.  May be NULL
  * \param iWidth The specified width of the dialog window, or -1 for default
  * \param iMaxChar The maximum number of characters in the input string
  *
*/

char *DLGInputBox(Window wIDOwner, const char *szTitle, const char *szPrompt, const char *szDefault,
                  int iWidth, int iMaxChar);


/** \ingroup dialog
  * \brief FileDialog enumeration
  *
  * Enumeration of bit flags for DLGFileDialog()\n
  * Specify one of the 'Type Flag' bits, and any 'Modifier Bits' that apply
*/

enum FileDialogEnum
{
    FileDialog_Open = 0,    ///< Type Flag - File Open Dialog
    FileDialog_Save = 1,    ///< Type Flag - File Save Dialog
    FileDialog_SaveAs = 2,  ///< Type Flag - File Save As Dialog
    FileDialog_Folder = 3,  ///< Type Flag - Folder Dialog
    FileDialog_MASK = 0xf,  ///< bit mask for type flag

    // modifications
    FileDialog_ReadOnly = 0x10,           ///< Modifier Flag - Read Only
    FileDialog_NoOverwrite = 0x20,        ///< Modifier Flag - No Overwrite
    FileDialog_NoOverwritePrompt = 0x40,  ///< Modifier Flag - No 'Overwrite Prompt' if overwriting
    FileDialog_NoFolderNav = 0x80,        ///< Modifier Flag - No 'Folder Navigation'
    FileDialog_Simple = 0x100,            ///< Modifier Flag - simple 'file name input'

    FileDialog_MODIFIER_MASK = 0xff0      ///< bit mask for modifier flag
};

/** \ingroup dialog
  * \brief Display a modal File Dialog window, returning a malloc'd pointer to a null-byte terminated string containing a fully qualified file or path name
  *
  * \return Returns either a NULL on cancel/error, or a malloc'd pointer to a null-byte terminated string containing a fully qualified file or path name
  * \param iType See the \ref MessageBoxEnum enumeration for a list of possible values
  * \param wIDOwner The owner's Window ID (or None)
  * \param szDefPath A null-byte terminated string containing the default path name (may be NULL for current directory)
  * \param szDefName A null-byte terminated string contianing the default file name (may be NULL, no path specifiers allowed)
  * \param szExtAndDescList Information containing the known extensions and file types.  May be NULL.  See below for format.
  *
  * Use this to display an X11 Work Bench Toolkit custom File Dialog based on the specified parameters.  These file dialogs
  * have been intionally tweeked for better UI performance than other file dialogs.\n
  *
  * When specifying the 'szExtAndDescList' parameter, point it to a string similar to the following:\n
\code
  static const char szExtAndDescList[]=
    "*.txt\tText Files\n"
    "*.doc\tDocument Files\n"
    "*.sh\tShell Script Files\n"
    "*.*\tOther Files\n";
\endcode
  * This describes 3 file types with extensions '.txt', '.doc', and '.sh' with a 4th file type of 'other files' that
  * encompasses everything else.  Simple pattern matching is applied to each file name for the selected pattern.
*/
char *DLGFileDialog(int iType, Window wIDOwner, const char *szDefPath, const char *szDefName,
                    const char *szExtAndDescList);


/** \ingroup dialog
  * \brief Display a splash screen for 5 seconds (with animation and copyright string), and then return.
  *
  * \param aXPM a pointer to an XPM array (as created by utilities like 'gimp').
  * \param szCopyright a zero-byte terminated string containing the copyright information
  * \param clrText a pixel color value for the copyright text
  *
  * This function will create an animated splash screen based on the dimensions of the pixmap passed to it,
  * animating the pixmap with a 'diagonal flash' from upper left to lower right, and draw the copyright
  * string onto the lower 1/3 of the pixmap, vertically centered.
**/

void DLGSplashScreen(char * aXPM[], const char *szCopyright, unsigned long clrText);


#ifdef __cplusplus
};
#endif // __cplusplus


#endif // DIALOG_WINDOW_H_INCLUDED

