/////////////////////////////////////////////////////////////////////////////////////////////////////////
//         _  _         _                                                            _       _         //
//      __| |(_)  __ _ | |  ___    __ _       ___  _   _  _ __   _ __    ___   _ __ | |_    | |__      //
//     / _` || | / _` || | / _ \  / _` |     / __|| | | || '_ \ | '_ \  / _ \ | '__|| __|   | '_ \     //
//    | (_| || || (_| || || (_) || (_| |     \__ \| |_| || |_) || |_) || (_) || |   | |_  _ | | | |    //
//     \__,_||_| \__,_||_| \___/  \__, |_____|___/ \__,_|| .__/ | .__/  \___/ |_|    \__|(_)|_| |_|    //
//                                |___/|_____|           |_|    |_|                                    //
//                                                                                                     //
//                                                                                                     //
//                                 additional dialog control support                                   //
// This file contains semi-private definitions that are needed to generically support dialog controls  //
//                                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////////////

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



// INTERNAL USE ONLY for dialog controls, etc.

#ifndef _DIALOG_SUPPORT_H_INCLUDED_
#define _DIALOG_SUPPORT_H_INCLUDED_

#include "dialog_controls.h" // make sure this is included first
#include "text_object.h" // needed for  _WB_EDIT_CONTROL_


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


/** \ingroup dlglist
  * \struct __LISTINFO__
  * \copydoc LISTINFO
**/
/** \ingroup dlglist
  * \typedef LISTINFO
  * \brief Structure containing data for list-related dialog controls
  *
  * Internally maintained structure containing list-related control data
  * including selection information and function callbacks for allocation,
  * de-allocation, display, and sorting.
  *
  * \code

  typedef struct __LISTINFO__
  {
    int nItems, nMaxItems;              // size/max size of aItems (must re-alloc to increase nMaxItems)
    int nPos, nTop;                     // scroll position and position of top item
    int nHeight, nItemHeight;           // height (in items) and height of item (in pixels)
    int nFlags;                         // flags (sorted, etc.)
    Window wOwner;                      // owning window [to be notified on change]
    void *(*pfnAllocator)(const void *, int); // copy constructor to call for each item that's added
                                        // typically this will call 'WBAlloc' followed by 'memcpy'
                                        // if NULL, the caller-supplied pointer is assigned to 'aItems' as-is
    void (*pfnDestructor)(void *);      // destructor to call for each item that's removed
                                        // typically this will point to 'WBFree'
                                        // if NULL, the caller-supplied pointer is ignored

    void (*pfnDisplay)(WBDialogControl *pControl, void *pData, int iSelected, GC gcPaint, WB_GEOM *pGeom, XFontSet fontSet);
                                        // generic function to display contents of item within 'pGeom' using GC
                                        // typically one of the listbox 'display item' functions

    int (*pfnSort)(const void *, const void *); // sort proc (NULL implies strcmp)

    void *aItems[1];                    // array of item data (remainder of struct)
  } LISTINFO;

  * \endcode
  *
  * \sa  DLGInitControlListInfo()
  *
**/
typedef struct __LISTINFO__
{
  int nItems,              ///< current number of valid entries in 'aItems'
      nMaxItems;           ///< max size of aItems (must re-alloc to increase nMaxItems)
  int nPos,                ///< current scroll position
      nTop;                ///< scroll position of the top item currently displayed
  int nHeight,             ///< height (in items) of display area, recalculated on resize/expose
      nItemHeight;         ///< height of a single item (in pixels)
  int nFlags;              ///< flags (sorted, etc.)
  Window wOwner;           ///< owning window [to be notified on change]
  /** \brief pointer to the copy constructor to call for each item that's added
    *
    * typically this will call 'WBAlloc' followed by 'memcpy' to construct a new pointer\n
    * if this is NULL, the caller-supplied pointer is assigned to 'aItems' as-is
  **/
  void *(*pfnAllocator)(const void *, int);
  /** \brief destructor to call for each item that's removed
    *
    * typically this will point to 'WBFree()'.  if it is NULL, the caller-supplied pointer is ignored on cleanup
  **/
  void (*pfnDestructor)(void *);
  /** \brief display callback function to paint the entry on the display surface
    *
    * generic callback function to display the contents of the item within 'pGeom' using
    * the provided GC.  The 'pControl' 'pData' and 'iSelected' parameters reference the
    * WBDialogControl, the list entry's data pointer, and a boolean 'selection' flag, respectively.\n
    * Typically this will be on of the listbox 'DisplayItem' API functions.
  **/
  void (*pfnDisplay)(WBDialogControl *pControl, void *pData, int iSelected, GC gcPaint, WB_GEOM *pGeom, XFontSet fontSet);

  /** \brief Optional sort comparison function.  NULL implies 'strcmp' */
  int (*pfnSort)(const void *, const void *); // sort proc (NULL implies strcmp)

  /** \brief The array of item data, integrated into the memory block containing this structure */
  void *aItems[1];                    // array of item data (remainder of struct)
} LISTINFO;



// specialized structures 'derived' from WBDialogControl (internal only)
/** \ingroup dlgctrl
  * \struct _WB_IMAGE_CONTROL_
  * \copydoc WBImageControl
*/
/** \ingroup dlgctrl
  * \typedef WBImageControl
  * \brief Static 'Image' control structure
  *
  * \code

  typedef struct _WB_IMAGE_CONTROL_
  {
    WBDialogControl wbDLGCtrl; // Standard dialog control members
    Pixmap pixmap;             // foreground pixmap
    Pixmap pixmap2;            // background (transparency) pixmap
  } WBImageControl;

  * \endcode
  *
  * Structure allocated for Image control
**/
typedef struct _WB_IMAGE_CONTROL_
{
  WBDialogControl wbDLGCtrl; ///< Standard dialog control members
  Pixmap pixmap;             ///< foreground pixmap
  Pixmap pixmap2;            ///< background (transparency) pixmap
} WBImageControl;

/** \ingroup dlgctrl
  * \struct _WB_PUSHBUTTON_CONTROL_
  * \copydoc WBPushButtonControl
**/
/** \ingroup dlgctrl
  * \typedef WBPushButtonControl
  * \brief Button 'pushbutton' control structure
  *
  * \code

  typedef struct _WB_PUSHBUTTON_CONTROL_
  {
    WBDialogControl wbDLGCtrl; // Standard dialog control members
    Pixmap pixmap;             // foreground pixmap
    Pixmap pixmap2;            // background (transparency) pixmap
  } WBPushButtonControl;

  * \endcode
  *
  * Structure allocated for pushbuttons.  allows assigning an image or icon
**/
typedef struct _WB_PUSHBUTTON_CONTROL_
{
  WBDialogControl wbDLGCtrl; ///< Standard dialog control members
  Pixmap pixmap;             ///< foreground pixmap
  Pixmap pixmap2;            ///< background (transparency) pixmap
} WBPushButtonControl;

/** \ingroup dlgctrl
  * \struct _WB_EDIT_CONTROL_
  * \copydoc WBEditControl
**/
/** \ingroup dlgctrl
  * \typedef WBEditControl
  * \brief Edit control structure
  *
  * \code

  typedef struct _WB_EDIT_CONTROL_
  {
    WBDialogControl wbDLGCtrl; // Standard dialog control members
    TEXT_OBJECT xTextObject;   // A Text Object associated with the edit text
  } WBEditControl;

  * \endcode
  *
  * Structure allocated for Edit control, allows for an edit state buffer
**/
typedef struct _WB_EDIT_CONTROL_
{
  WBDialogControl wbDLGCtrl; ///< Standard dialog control members
  TEXT_OBJECT xTextObject;   ///< A Text Object associated with the edit text
} WBEditControl;


/** \ingroup dlglist
  * \struct _WB_LIST_CURSEL_
  * \copydoc WBListCurSel
**/
/** \ingroup dlglist
  * \typedef WBListCurSel
  * \brief Structure containing information about the current selection in a list
  *
  * \code

  typedef struct _WB_LIST_CURSEL_
  {
    int iCurSel;      // current selection
    int iTopIndex;    // index of item at top of window
    int iHeight;      // calculated height of window in "entries" (see next member)
    int iEntryHeight; // cached display height of each entry (calculated by Expose handler)
  } WBListCurSel;

  * \endcode
  *
  * Typically used in a listbox or combo box, this structure specifies the current list
  * selection info for a listbox, combo box, or tree control.\n
  * NOTE:  this structure must immediately follow the WBDialogControl members in the control's structure
  * for a control that has the CONTROL_SupportListInfo flag.
**/
typedef struct _WB_LIST_CURSEL_
{
  int iCurSel;      ///< current selection
  int iTopIndex;    ///< index of item at top of window
  int iHeight;      ///< calculated height of window in "entries" (see next member)
  int iEntryHeight; ///< cached display height of each entry (calculated by Expose handler)
} WBListCurSel;


/** \ingroup dlgctrl
  * \struct _WB_LIST_CONTROL_
  * \copydoc  WBListControl
**/
/** \ingroup dlgctrl
  * \typedef  WBListControl
  * \brief List control structure
  *
  * \code

  typedef struct _WB_LIST_CONTROL_
  {
    WBDialogControl wbDLGCtrl; // Standard dialog control members
    WBListCurSel sel;          // selection state, must follow wbDLGCtrl
    int *pSelBitmap;           // bitmap of selections (when applicable) (use 'WBAlloc/WBFree')
    int cbBitmap;              // size of bitmap (in bytes, granular at sizeof(int))
    XFontSet fsBold;           // bold font set - assigned on the fly, struct creator must free if not 'None'
  } WBListControl;

  * \endcode
  *
  * Structure allocated for List control
**/
typedef struct _WB_LIST_CONTROL_
{
  WBDialogControl wbDLGCtrl; ///< Standard dialog control members
  WBListCurSel sel;          ///< selection state, must follow wbDLGCtrl
  int *pSelBitmap;           ///< bitmap of selections (when applicable) (use 'WBAlloc/WBFree')
  int cbBitmap;              ///< size of bitmap (in bytes, granular at sizeof(int))
  XFontSet fsBold;           ///< bold font set - assigned on the fly, struct creator must free if not 'None'
} WBListControl;


/** \ingroup dlgctrl
  * \struct _WB_COMBO_CONTROL_
  * \copydoc WBComboControl
*/
/** \ingroup dlgctrl
  * \typedef WBComboControl
  * \brief List control structure
  *
  * \code

  typedef struct _WB_COMBO_CONTROL_
  {
    WBDialogControl wbDLGCtrl; // Standard dialog control members
    WBListCurSel sel;          // selection state, must follow wbDLGCtrl
    TEXT_OBJECT xTextObject;   // A Text Object associated with the editable text
  } WBComboControl;

  * \endcode
  *
  * Structure allocated for List control
**/
typedef struct _WB_COMBO_CONTROL_
{
  WBDialogControl wbDLGCtrl; ///< Standard dialog control members
  WBListCurSel sel;          ///< selection state, must follow wbDLGCtrl
  TEXT_OBJECT xTextObject;   ///< A Text Object associated with the editable text
} WBComboControl;


/** \ingroup dlgctrl
  * \struct _WB_TREE_CONTROL_
  * \copydoc WBTreeControl
**/
/** \ingroup dlgctrl
  * \typedef WBTreeControl
  * \brief Static 'Image' control structure
  *
  * \code

  typedef struct _WB_TREE_CONTROL_
  {
    WBDialogControl wbDLGCtrl; // Standard dialog control members
    WBListCurSel sel;          // must follow wbDLGCtrl
    XFontSet fsBold;           // bold font set - assigned on the fly, struct creator must free if not 'None'
  } WBTreeControl;

  * \endcode
  *
  * Structure allocated for Tree control
**/
typedef struct _WB_TREE_CONTROL_
{
  WBDialogControl wbDLGCtrl; ///< Standard dialog control members
  WBListCurSel sel;          ///< must follow wbDLGCtrl
  XFontSet fsBold;           ///< bold font set - assigned on the fly, struct creator must free if not 'None'
} WBTreeControl;



/** \ingroup dlgctrl
  * \brief Destroy Properties for a dialog control
  *
  * \param pPropList A pointer to the property list for the dialog control
  * \returns void
  *
  * Call this function to destroy the property list for a dialog control
  *
  * Header File:  dialog_support.h
**/
void DLGCDestroyProperties(WBDialogPropList *pPropList);

/** \ingroup dlgctrl
  * \brief Create LISTINFO structure for a list type control
  *
  * \param wOwner the Window ID for the owner of the LISTINFO structure
  * \param nMax the initial maximum size of the list (can be increased, but needs re-allocation)
  * \param nFlags A binary flag indicating various properties, such as 'sorted'
  * \param pfnAllocator A pointer to a constructor callback for a new list item.  Use NULL for default
  * \param pfnDestructor A pointer to a destructor callback for the list item.  Use NULL for default
  * \param pfnDisplay A pointer to a 'display' callback
  * \param pfnSort A pointer to a 'sort' callback.  NULL uses the default
  * \returns An allocated pointer to a LISTINFO structure.  To free this pointer, call DLGCListINfoDestructor()
  *
  * Call this function to create a LISTINFO structure associated with a list type control
  *
  * \sa LISTINFO
  *
  * Header File:  dialog_support.h
**/
LISTINFO *DLGCListInfoConstructor(Window wOwner, int nMax, int nFlags,
                                  void *(*pfnAllocator)(const void *,int), void (*pfnDestructor)(void *),
                                  void (*pfnDisplay)(WBDialogControl *, void *, int, GC, WB_GEOM *, XFontSet),
                                  int (*pfnSort)(const void *, const void *));

/** \ingroup dlgctrl
  * \brief Destroy a LISTINFO structure
  *
  * \param pListInfo A pointer to a LISTINFO structure created by DLGCListInfoConstructor()
  * \returns void
  *
  * Call this function to destroy the LISTINFO structure created by DLGCListInfoConstructor()
  *
  * Header File:  dialog_support.h
**/
void DLGCListInfoDestructor(LISTINFO *pListInfo);


/** \ingroup dlgctrl
  * \brief The default 'display proc' for displaying a list item from a LISTINFO structure
  *
  * \param pList A pointer to a WBDialogControl structure for the list type control
  * \param pData A pointer to the data to display
  * \param iSelected A flag indicating whether the item has been selected (1 = selected, 0 = not selected)
  * \param gc The graphics context for displaying the data
  * \param pGeom a WB_GEOM representing the rectangular area to draw the item in.
  * \param fontSet An XFontSet to use when displaying the text. May be 'None', which will use the default font set for the display
  * \returns void
  *
  * Pass this function's address as 'pfnDisplay' when calling DLGCListInfoConstructor to get a default
  * display handler for your listbox items.
  *
  * Header File:  dialog_support.h
**/
void DLGCDefaultListControlDisplayProc(WBDialogControl *pList, void *pData, int iSelected, GC gc, WB_GEOM *pGeom, XFontSet fontSet);



#ifndef DIALOG_SUPPORT_C

extern const Atom aDLGC_TEXT;
extern const Atom aDLGC_CAPTION;
extern const Atom aDLGC_FONT;
extern const Atom aDLGC_SCROLLINFO;
extern const Atom aDLGC_LISTINFO;

#endif // DIALOG_SUPPORT_C


#ifdef NO_DEBUG
#define DEBUG_DUMP_LIST(X)
#else // !NO_DEBUG
void DEBUG_DUMP_LIST(WBDialogControl *pCtrl);
#endif // NO_DEBUG


#ifdef __cplusplus
};
#endif // __cplusplus

#endif // _DIALOG_SUPPORT_H_INCLUDED_


