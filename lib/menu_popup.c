//////////////////////////////////////////////////////////////////////////////////////////
//    _ __ ___    ___  _ __   _   _        _ __    ___   _ __   _   _  _ __     ___     //
//   | '_ ` _ \  / _ \| '_ \ | | | |      | '_ \  / _ \ | '_ \ | | | || '_ \   / __|    //
//   | | | | | ||  __/| | | || |_| |      | |_) || (_) || |_) || |_| || |_) |_| (__     //
//   |_| |_| |_| \___||_| |_| \__,_|_____ | .__/  \___/ | .__/  \__,_|| .__/(_)\___|    //
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

/** \file menu_popup.c
  * \brief Implementation file for menu popup windows
  *
  * This is the implementation file for menu popup windows, which are actually
  * top-level windows that act as if they are child windows of the menu bar.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <time.h>
#include <X11/cursorfont.h>

#ifndef XK_Delete /* moslty for interix */
#define XK_MISCELLANY /* mostly for interix */
#include <X11/keysymdef.h> // some platforms don't automatically include this with X headers
#endif // XK_Delete

#include "window_helper.h"
#include "pixmap_helper.h"  // pixmap helpers, including pre-defined icons
#include "frame_window.h"
#include "menu_popup.h"
#include "conf_help.h"


static int MBMenuPopupEvent(Window wID, XEvent *pEvent);

#define HEIGHT_SPACING 4    /* pixels between menu items */
#define SEPARATOR_HEIGHT 5  /* height of menu item separator */
#define SEPARATOR_POS 2 /* position of separator from 'iPosition' */

#define WB_MOUSE_FAR 24 /* 24 pixels, about 2 'W' characters on hi-res display */

static void __SetFirstSelection(WBMenuPopupWindow *pSelf, WBMenu *pMenu)
{
int i1;

  if(!pMenu)
  {
    return;
  }

  for(i1=0; pMenu->ppItems && i1 < pMenu->nItems; i1++)
  {
    WBMenuItem *pItem = pMenu->ppItems[i1];

    if(pItem && // invalid
       pItem->iAction != WBMENU_SEPARATOR) // separator
    {
      pSelf->iSelected = i1;
      break;
    }
  }
}

static void __SetNextSelection(WBMenuPopupWindow *pSelf, WBMenu *pMenu)
{
int i1;

  if(!pMenu)
  {
    return;
  }

  i1 = pSelf->iSelected;

  if(i1 < 0 || i1 >= pMenu->nItems)
  {
    i1 = 0;
  }

  for(i1++; pMenu->ppItems && i1 < pMenu->nItems; i1++)
  {
    WBMenuItem *pItem = pMenu->ppItems[i1];

    if(pItem && // invalid
       pItem->iAction != WBMENU_SEPARATOR) // separator
    {
      pSelf->iSelected = i1;
      return;
    }
  }
}

static void __SetPrevSelection(WBMenuPopupWindow *pSelf, WBMenu *pMenu)
{
int i1;

  if(!pMenu)
  {
    return;
  }

  i1 = pSelf->iSelected;

  if(i1 < 0 || i1 >= pMenu->nItems)
  {
    i1 = 0;
  }

  for(i1--; pMenu->ppItems && i1 >= 0; i1--)
  {
    WBMenuItem *pItem = pMenu->ppItems[i1];

    if(pItem && // invalid
       pItem->iAction != WBMENU_SEPARATOR) // separator
    {
      pSelf->iSelected = i1;
      return;
    }
  }
}

static void __SetLastSelection(WBMenuPopupWindow *pSelf, WBMenu *pMenu)
{
int i1;

  if(!pMenu)
  {
    return;
  }

  for(i1 = pMenu->nItems - 1; pMenu->ppItems && i1 >= 0; i1--)
  {
    WBMenuItem *pItem = pMenu->ppItems[i1];

    if(pItem && // invalid
       pItem->iAction != WBMENU_SEPARATOR) // separator
    {
      pSelf->iSelected = i1;
      return;
    }
  }
}

static WBMenuItem * __GetCurrentSelection(WBMenuPopupWindow *pSelf, WBMenu *pMenu)
{
  if(pSelf->iSelected >= 0 && pSelf->iSelected < pMenu->nItems)
  {
    return pMenu->ppItems[pSelf->iSelected];
  }

  return NULL;
}


WBMenuPopupWindow *MBCreateMenuPopupWindow(Window wIDBar, Window wIDOwner, WBMenu *pMenu,
                                           int iX, int iY, int iFlags)
{
  Display *pDisplay = WBGetWindowDisplay(wIDBar);
  XFontStruct *pDefaultMenuFont = MBGetDefaultMenuFont();
  WBMenuPopupWindow *pRval = NULL;
  unsigned long fg, bg, bd;   /* Pixel values */
  XSetWindowAttributes xswa;  /* Temporary Set Window Attribute struct */
  XSizeHints  xsh;            /* Size hints for window manager - width of owner + 2 * height of font */
  XWMHints xwmh;
  WB_GEOM geom;
  XFontStruct *pFS;
  int i1, iHPos, iVPos, iHBorder, iSelected = -1;
  Atom a1;
  unsigned long ul1;
  char tbuf[256];


  // check/initialize global menu objects
  if(!MBInitGlobal())
  {
    return NULL;
  }

  // step 1:  create the window

  bd = clrMenuBorder1.pixel; // BlackPixel(pDisplay, DefaultScreen(pDisplay));
  fg = clrMenuFG.pixel;// BlackPixel(pDisplay, DefaultScreen(pDisplay));  // black foreground for menus (always)
  bg = clrMenuBG.pixel;// WhitePixel(pDisplay, DefaultScreen(pDisplay));  // white background for menus (for now)

  // menu orientation and size depend upon the size of the owner menu's parent window
  // and the size of the popup menu itself.  For now assume orientation top left at iX iY

  pFS = WBGetWindowFontStruct(wIDBar);

  if(!pDefaultMenuFont && !pFS)
  {
    WB_WARN_PRINT("%s - * BUG * no font!\n", __FUNCTION__);
    return 0;
  }
  else if(pDefaultMenuFont)
  {
    pFS = pDefaultMenuFont;
  }

  // get absolute position of "parent"
  WBGetWindowGeom2(wIDBar, &geom);

  // set size hints to match client area, upper left corner (always)
  // as translated from the position of the "parent"

  xsh.flags = (PPosition | PSize);
  xsh.x = iX + geom.x;
  xsh.y = iY + geom.y;

  // get the dimensions of each menu item.  separators are 3 pixels.

  iVPos = 4;  // 2 pixels for border + 2 pixels for spacing
  iHPos = iHBorder = XTextWidth(pFS, "  ", 2);  // width of 2 spaces

  for(i1=0; pMenu && pMenu->ppItems && i1 < pMenu->nItems; i1++)
  {
    WBMenuItem *pItem = pMenu->ppItems[i1];
    const char *szText;
    int iU1=0, iU2=0;

    if(!pItem)
    {
      continue;
    }

    if(pItem->iAction == -1) // separator
    {
      pItem->iPosition = iVPos;
      pItem->iTextWidth = 0;
      iVPos += SEPARATOR_HEIGHT;
      continue;
    }

    if(iSelected < 0 || iSelected >= pMenu->nItems)
    {
      // set focus to first non-separator item in menu
      iSelected = i1;  // force first item to select
    }

    szText = pItem->data + pItem->iMenuItemText;

    if(strchr(szText, '_'))
    {
      char *p1;
      strncpy(tbuf, szText, sizeof(tbuf)/2);
      p1 = tbuf;
      while(*p1)
      {
        if(*p1 == '_')
        {
          *p1 = 0;

          if(p1 == tbuf)
            iU1 = 0;
          else
            iU1 = XTextWidth(pFS, tbuf, strlen(tbuf));

          if(p1[1])
          {
            iU2 = XTextWidth(pFS, p1, 1);
            strcpy(p1, p1 + 1);
          }
          else
            iU2 = iU1;  // shouldn't happen
        }
        p1++;
      }
    }
    else
    {
      strncpy(tbuf, szText, sizeof(tbuf)/2);
    }

    strcat(tbuf, "    ");
    if(pItem->iHotKey >= 0)
        strcat(tbuf, pItem->data + pItem->iHotKey);

    pItem->iPosition = iVPos;  // also needed for mousie/clickie
    pItem->iTextWidth = XTextWidth(pFS, tbuf, strlen(tbuf));

    iVPos += pFS->max_bounds.ascent + pFS->max_bounds.descent + HEIGHT_SPACING;

    if(iHPos < 2 * iHBorder + pItem->iTextWidth)
      iHPos = 2 * iHBorder + pItem->iTextWidth;
  }

  xsh.width = iHPos;
  xsh.height = iVPos + 2;  // 2 extra pixels for the bottom border

  memset(&xswa, 0, sizeof(xswa));

  xswa.border_pixel = bd;
  xswa.background_pixel = bg;
  xswa.colormap = DefaultColormap(pDisplay, DefaultScreen(pDisplay));
  xswa.bit_gravity = CenterGravity;
  xswa.override_redirect = True;  // so window manager won't mess with it

  pRval = (WBMenuPopupWindow *)malloc(sizeof(*pRval));

  if(!pRval)
  {
    WB_ERROR_PRINT("%s - not enough memory to allocate structure\n", __FUNCTION__);
    return NULL;
  }

  pRval->ulTag = MENU_POPUP_WINDOW_TAG;

  pRval->pMenu = pMenu;

  if(!pRval->pMenu)
  {
    WB_WARN_PRINT("%s - * BUG * pMenu is NULL!\n", __FUNCTION__);
  }

  // this window will be owned by the root, so it will require mouse and keyboard grabs
  // in order to prevent changing the focus.

//  pRval->wSelf = XCreateWindow(pDisplay, DefaultRootWindow(pDisplay),
//                               xsh.x, xsh.y, xsh.width, xsh.height,
//                               0, // no border
//                               DefaultDepth(pDisplay, DefaultScreen(pDisplay)),
//                               InputOutput,
//                               DefaultVisual(pDisplay, DefaultScreen(pDisplay)),
//                               CWBorderPixel | CWBackPixel | CWColormap | CWBitGravity | CWOverrideRedirect,
//                               &xswa);

  pRval->wSelf = WBCreateWindow(pDisplay, DefaultRootWindow(pDisplay), MBMenuPopupEvent, "MenuPopup",
                                xsh.x, xsh.y, xsh.width, xsh.height, 0,
                                InputOutput,
                                CWBorderPixel | CWBackPixel | CWColormap | CWBitGravity | CWOverrideRedirect,
                                &xswa);
  if(pRval->wSelf == -1)
  {
    WB_WARN_PRINT("%s - WARNING:  unable to create window for popup menu\n", __FUNCTION__);

    free(pRval);
    return NULL;
  }

  pRval->wBar = wIDBar;  // for now do it this way
  pRval->wOwner = wIDOwner;
  pRval->iSelected = iSelected;
  pRval->iFlags = iFlags;  // make a copy of them (for now)

  // calculate the initial position and height of the menu bar within the window
  pRval->iX = /*xsh.x + */2;
  pRval->iY = /*xsh.y + */2;
  pRval->iWidth = xsh.width - 4;
  pRval->iHeight = xsh.height - 4;

//  WBRegisterWindowCallback(pRval->wSelf, MBMenuPopupEvent); // this must happen first
//  WBSetWindowClassName(pRval->wSelf, "MenuPopup");

  // establish this window as NEVER getting the input focus
  bzero(&xwmh, sizeof(xwmh));
  xwmh.flags = InputHint;
  xwmh.input = 0;  // this represents 'None' (or 'Globally Active' if WM_TAKE_FOCUS is present)

  WBSetWMProperties(pRval->wSelf, NULL, NULL, &xwmh, NULL);

  WBSetWindowDefaultCursor(pRval->wSelf, XC_hand2); //XC_top_left_arrow);
  WBSetWindowData(pRval->wSelf, 0, pRval);  // a pointer back to me

  WBCreateWindowDefaultGC(pRval->wSelf, fg, bg);
  WBSetWindowFontStruct(pRval->wSelf, WBCopyFont(pFS));

  XSelectInput(pDisplay, pRval->wSelf,
               WB_STANDARD_INPUT_MASK | WB_KEYBOARD_INPUT_MASK | WB_MOUSE_INPUT_MASK);

  // before mapping the window, set some properties
  a1 = XInternAtom(pDisplay, "_NET_WM_WINDOW_TYPE", False);
  ul1 = XInternAtom(pDisplay, "_NET_WM_WINDOW_TYPE_MENU", False);
  XChangeProperty(pDisplay, pRval->wSelf, a1, XA_ATOM, 32, PropModeReplace, (unsigned char *)&ul1, 1);

  a1 = XInternAtom(pDisplay, "_NET_WM_STATE", False);
  ul1 = XInternAtom(pDisplay, "_NET_WM_STATE_MODAL", False);
  XChangeProperty(pDisplay, pRval->wSelf, a1, XA_ATOM, 32, PropModeReplace, (unsigned char *)&ul1, 1);

  a1 = XInternAtom(pDisplay, "WM_TRANSIENT_FOR", False);
  XChangeProperty(pDisplay, pRval->wSelf, a1, XA_WINDOW, 32, PropModeReplace, (unsigned char *)&wIDOwner, 1);//&wIDBar, 1);


  // TODO:  other properties... ?

  WB_DEBUG_PRINT(DebugLevel_Heavy | DebugSubSystem_Menu,
                 "%s - mapping popup menu window\n", __FUNCTION__);

  WBMapWindow(pDisplay, pRval->wSelf);  // make window visible

  WBGetParentWindow(pRval->wSelf);  // this syncs everything up (return value not needed)

  // TODO: set up font, GS, callback, etc.

  return(pRval);
}

static int __FindMenuPopupWindowCallback(Window wID, void *pData)
{
  WBMenuPopupWindow *pMP = MBGetMenuPopupWindowStruct(wID);

  if(pMP && (void *)(pMP->pMenu) == pData)
  {
    return 1;
  }

  return 0;
}

WBMenuPopupWindow *MBFindMenuPopupWindow(WBMenu *pMenu)  // find first (active) window that uses 'pMenu'
{
  Window wID = WBLocateWindow(__FindMenuPopupWindowCallback, (void *)pMenu);

  if(wID)
  {
    return MBGetMenuPopupWindowStruct(wID);
  }

  return NULL;
}

int MBMenuDoModal(WBMenuPopupWindow *pPopup)
{
  if(!pPopup)
    return -1;

  return WBShowModal(pPopup->wSelf, 1);  // will only return when the window is destroyed
}

void MBDestroyMenuPopupWindow(WBMenuPopupWindow *pMenuPopupWindow)
{
  if(!pMenuPopupWindow || pMenuPopupWindow->ulTag != MENU_POPUP_WINDOW_TAG)
  {
    return;
  }

  WBDestroyWindow(pMenuPopupWindow->wSelf); // destroy the window (this should clean the rest up automatically)
}


static void MBMenuPopupHandleMenuItem(Display *pDisplay, Window wID, WBMenuPopupWindow *pSelf, WBMenu *pMenu, WBMenuItem *pItem)
{
  if(pItem->iAction & WBMENU_POPUP_HIGH_BIT)
  {
    XClientMessageEvent evt;

    // post a high-priority message to myself to display the menu

    bzero(&evt, sizeof(evt));
    evt.type = ClientMessage;
    evt.display = pDisplay;
    evt.window = wID;
    evt.message_type = aMENU_DISPLAY_POPUP;
    evt.format = 32;
    evt.data.l[0] = pItem->iAction & WBMENU_POPUP_MASK;
    evt.data.l[1] = pItem->iPosition;

    WBPostPriorityEvent(wID, (XEvent *)&evt);
  }
  else
  {
    XClientMessageEvent evt;

    bzero(&evt, sizeof(evt));
    evt.type = ClientMessage;

    evt.display = pDisplay;
    evt.window = pSelf->wOwner;
    evt.message_type = aMENU_COMMAND;
    evt.format = 32;  // always
    evt.data.l[0] = pItem->iAction;  // menu command message ID
    evt.data.l[1] = (long)pMenu;     // pointer to menu object
    evt.data.l[2] = wID;             // window ID of menu bar

    WBPostEvent(pSelf->wOwner, (XEvent *)&evt);

    WB_DEBUG_PRINT(DebugLevel_Heavy | DebugSubSystem_Menu,
                    "%s - Post Event: %08xH %08xH %pH %08xH\n",
                    __FUNCTION__, (int)aMENU_COMMAND, (int)pItem->iAction,
                    pMenu, (int)wID);

    WBEndModal(wID, pItem->iAction);
  }
}

static int MBMenuPopupHandleMenuItemUI(Display *pDisplay, WBMenuPopupWindow *pSelf, WBMenu *pMenu, WBMenuItem *pItem)
{
  XClientMessageEvent evt;

  bzero(&evt, sizeof(evt));
  evt.type = ClientMessage;

  evt.display = pDisplay;
  evt.window = pSelf->wOwner;
  evt.message_type = aMENU_UI_COMMAND;
  evt.format = 32;  // always

  // sending pointer data - since this is internally 'sent' I don't have to
  // worry about async causing pointers to become invalid

  evt.data.l[0] = pItem->iAction;  // menu command message ID (needed to identify menu)

#if !defined(__SIZEOF_POINTER__) // TODO find a better way
#define __SIZEOF_POINTER_ 0
#endif
#if __SIZEOF_POINTER__ == 4 /* to avoid warnings in 32-bit linux */
  evt.data.l[1] = (long)pMenu;
  evt.data.l[2] = 0;
  evt.data.l[3] = (long)pItem;
  evt.data.l[4] = 0;
#else
  evt.data.l[1] = (long)((unsigned long long)pMenu & 0xffffffffL);
  evt.data.l[2] = (long)(((unsigned long long)pMenu >> 32) & 0xffffffffL);
  evt.data.l[3] = (long)((unsigned long long)pItem & 0xffffffffL);
  evt.data.l[4] = (long)(((unsigned long long)pItem >> 32) & 0xffffffffL);
#endif

  return WBWindowDispatch(pSelf->wOwner, (XEvent *)&evt); // 'send event'
}

static int MenuPopupDoExposeEvent(XExposeEvent *pEvent, WBMenu *pMenu,
                                  Display *pDisplay, Window wID,
                                  WBMenuPopupWindow *pSelf)
{
  int i1, /* i2, */ iHPos, iVPos, iHeight;
  XWindowAttributes xwa;      /* Temp Get Window Attribute struct */
  XFontStruct *pOldFont, *pFont, *pDefaultMenuFont;
  XPoint xpt[3];
  GC gc; // = WBGetWindowDefaultGC(wID);
  XGCValues xgc;
  WB_GEOM geomPaint;
  char tbuf[128];

  if (XGetWindowAttributes(pDisplay, wID, &xwa) == 0)
  {
    WB_WARN_PRINT("%s - * BUG * unable to get window attributes!\n", __FUNCTION__);
    return 0;
  }

  pFont = pOldFont = WBGetWindowFontStruct(wID);
  pDefaultMenuFont = MBGetDefaultMenuFont();

  if(!pDefaultMenuFont && !pOldFont)
  {
    WB_WARN_PRINT("%s - * BUG * no font!\n", __FUNCTION__);
    return 0;
  }
  else if(pDefaultMenuFont)
    pFont = pDefaultMenuFont;

  // get graphics context copy and begin painting
  gc = WBBeginPaint(wID, pEvent, &geomPaint);
  if(!gc)
  {
    WB_WARN_PRINT("%s - * BUG * no graphics context!\n", __FUNCTION__);
    return 0;
  }

  xgc.font = pFont->fid;
  BEGIN_XCALL_DEBUG_WRAPPER
  XChangeGC(pDisplay, gc, GCFont, &xgc);
  END_XCALL_DEBUG_WRAPPER

////  XClearWindow(pDisplay, wID);  // TODO:  rather than erase background, see if I need to
//  XClearArea(pDisplay, wID, geomPaint.x, geomPaint.y, geomPaint.width, geomPaint.height, 0);
  WBClearWindow(wID, gc);

  // paint a 3D-looking border
  XSetForeground(pDisplay, gc, clrMenuBorder2.pixel);
  xpt[0].x=xwa.border_width;
  xpt[0].y=xwa.height-1-2*xwa.border_width - 1;  // exclude first point
  xpt[1].x=xwa.border_width;
  xpt[1].y=xwa.border_width;
  xpt[2].x=xwa.width-1-2*xwa.border_width - 1;   // exclude last point
  xpt[2].y=xwa.border_width;

  XDrawLines(pDisplay, wID, gc, xpt, 3, CoordModeOrigin);

  XSetForeground(pDisplay, gc, clrMenuBorder3.pixel);
  xpt[0].x=xwa.width-1-2*xwa.border_width;
  xpt[0].y=xwa.border_width + 1;              // exclude first point
  xpt[1].x=xwa.width-1-2*xwa.border_width;
  xpt[1].y=xwa.height-1-2*xwa.border_width;
  xpt[2].x=xwa.border_width + 1;              // exclude final point
  xpt[2].y=xwa.height-1-2*xwa.border_width;

  XDrawLines(pDisplay, wID, gc, xpt, 3, CoordModeOrigin);

  // painting the menu items

  iHeight = pFont->max_bounds.ascent + pFont->max_bounds.descent;

  iVPos = 4;  // 2 pixels for border + 2 pixels
  iHPos = XTextWidth(pFont, "  ", 2);  // width of 2 spaces

  XSetForeground(pDisplay, gc, clrMenuFG.pixel);

  for(i1=0; pMenu && pMenu->ppItems && i1 < pMenu->nItems; i1++)
  {
    WBMenuItem *pItem = pMenu->ppItems[i1];
    const char *szText;
    int iU1=0, iU2=0;

    int iUIState = 0;

    if(!pItem)
      continue;

    if(pItem->iPosition < 0)
      pItem->iPosition = iVPos;  // also needed for mousie/clickie

    if(pItem->iAction == -1) // separator
    {
      xpt[0].x=xwa.border_width + 1;
      xpt[0].y=pItem->iPosition + SEPARATOR_POS - 1;
      xpt[1].x=xwa.width-1-2*xwa.border_width;
      xpt[1].y=xpt[0].y;

      XDrawLines(pDisplay, wID, gc, xpt, 2, CoordModeOrigin);

      XSetForeground(pDisplay, gc, clrMenuBorder2.pixel);
          xpt[1].y=++(xpt[0].y);
      XDrawLines(pDisplay, wID, gc, xpt, 2, CoordModeOrigin);

      iVPos += SEPARATOR_HEIGHT;
      XSetForeground(pDisplay, gc, clrMenuFG.pixel);
      continue;
    }

    iUIState = MBMenuPopupHandleMenuItemUI(pDisplay, pSelf, pMenu, pItem);
      // this will return 0 to handle normally, or something OTHER than zero to modify the menu
      // in this case, -1 disables the menu item

    if(i1 == pSelf->iSelected)  // selected item
    {
      int iItemHeight = pFont->max_bounds.ascent + pFont->max_bounds.descent + HEIGHT_SPACING;

      XSetForeground(pDisplay, gc, clrMenuActiveBG.pixel);
      XSetBackground(pDisplay, gc, clrMenuActiveBG.pixel);

      XFillRectangle(pDisplay, wID, gc,
                     xwa.border_width + 2, pItem->iPosition - 1,
                     xwa.width-4-2*xwa.border_width, iItemHeight - 1);

      if(!iUIState)
      {
        XSetForeground(pDisplay, gc, clrMenuActiveFG.pixel);
      }
      else
      {
        XSetForeground(pDisplay, gc, clrMenuActiveDisabledFG.pixel);
      }
    }
    else
    {
      if(!iUIState)
      {
        XSetForeground(pDisplay, gc, clrMenuFG.pixel);
      }
      else
      {
        XSetForeground(pDisplay, gc, clrMenuDisabledFG.pixel);
      }
    }

    szText = pItem->data + pItem->iMenuItemText;

    if(pItem->iUnderscore >= 0) //strchr(szText, '_'))
    {
      // locate the first (only the first) underscore
      char *p1;
      strcpy(tbuf, szText);
      p1 = tbuf + pItem->iUnderscore - pItem->iMenuItemText; // position of underscore
      if(*p1 == '_') // TODO:  allow multiple underscores?  Not much value in it (could loop)
      {
        *p1 = 0;

        if(p1 == tbuf)
          iU1 = 0;
        else
          iU1 = XTextWidth(pFont, tbuf, p1 - tbuf);

        if(p1[1]) // character just past underscore
        {
          iU2 = XTextWidth(pFont, p1 + 1, 1);
          strcpy(p1, p1 + 1); // adjust actual text so there is no underscore
        }
        else
          iU2 = iU1;  // shouldn't happen
      }
      else
      {
          WB_ERROR_PRINT("%s - ERROR:  cannot locate underscore\n", __FUNCTION__);
      }

      szText = tbuf; // modified text without '_' in it
    }

    if(pItem->iTextWidth < 0)
      pItem->iTextWidth = XTextWidth(pFont, szText, strlen(szText));

    // TODO:  change string into a series of XTextItem structures and
    //        then call XDrawText to draw the array of 'XTextItem's
    if(*szText)
      XDrawString(pDisplay, wID, gc, iHPos,
                  pItem->iPosition + pFont->max_bounds.ascent,
                  szText, strlen(szText));

    // next I want to indicate what the hotkey is.  This text must be right-justified
    if(pItem->iHotKey >= 0)
    {
      const char *p2 = pItem->data + pItem->iHotKey;
      if(*p2)
      {
        int iLen = strlen(p2);
        int iWidth = XTextWidth(pFont, p2, iLen)
                   + XTextWidth(pFont, " ", 1) * 2; // white space on right side
        XDrawString(pDisplay, wID, gc,
                    xwa.width + xwa.border_width - 2 - iWidth,
                    pItem->iPosition + pFont->max_bounds.ascent,
                    p2, iLen);
      }
    }

    if(pItem->iUnderscore >= 0)//strlen(szText) < strlen(pItem->data + pItem->iMenuItemText))
    {
      xpt[0].x=iHPos + iU1 - 1;
      xpt[0].y=pItem->iPosition + iHeight - 1;
      xpt[1].x=iHPos + iU1 + iU2;
      xpt[1].y=xpt[0].y;

      WB_DEBUG_PRINT(DebugLevel_Chatty | DebugSubSystem_Menu,
                     "%s - drawing underscore at %d,%d to %d,%d\n",
                     __FUNCTION__, xpt[0].x, xpt[0].y, xpt[1].x, xpt[1].y);

      XDrawLines(pDisplay, wID, gc, xpt, 2, CoordModeOrigin);
    }

    if(i1 == pSelf->iSelected)  // selected item
    {
      XSetForeground(pDisplay, gc, clrMenuFG.pixel);
      XSetBackground(pDisplay, gc, clrMenuBG.pixel);
    }

    iVPos += iHeight + HEIGHT_SPACING;
  }

  // by convention, restore original objects/state

  xgc.font = pOldFont->fid;
  BEGIN_XCALL_DEBUG_WRAPPER
  XChangeGC(pDisplay, gc, GCFont, &xgc);

  XSetForeground(pDisplay, gc, WBGetWindowFGColor(wID));  // restore it at the end
  END_XCALL_DEBUG_WRAPPER

  WBEndPaint(wID, gc);

  return 1;
}

static WBMenuItem * __SelectMenuItemFromMousePos(Window wID, WBMenuPopupWindow *pSelf, WBMenu *pMenu, int iX, int  iY)
{
int i1;
int iMaxY;
WB_GEOM geom;


  // first I must invalidate the currently selected item

  for(i1=0; pMenu->ppItems && i1 < pMenu->nItems; i1++)
  {
    WBMenuItem *pItem = pMenu->ppItems[i1];

    if(!pItem)
      continue;

    if((i1 + 1) < pMenu->nItems)
      iMaxY = pMenu->ppItems[i1 + 1]->iPosition - 1;
    else
      iMaxY = pSelf->iY + pSelf->iHeight;

    if(pItem->iPosition <= iY && iMaxY >= iY)  // between them
    {
      XFontStruct *pFont, *pOldFont, *pDefaultMenuFont;
      int iHPos;

      if(pSelf->iSelected == i1) // already selected?
      {
        return pItem;  // just return (nothing else to do)
      }

      if(pItem->iAction == WBMENU_SEPARATOR ||
         0 != MBMenuPopupHandleMenuItemUI(WBGetWindowDisplay(wID), pSelf, pMenu, pItem))
      {
        if(pSelf->iSelected >= 0 && pSelf->iSelected < pMenu->nItems)
        {
          return pMenu->ppItems[pSelf->iSelected];
        }

        return NULL;
      }

      pFont = pOldFont = WBGetWindowFontStruct(wID);
      pDefaultMenuFont = MBGetDefaultMenuFont();

      if(!pDefaultMenuFont && !pOldFont)
      {
        WB_WARN_PRINT("%s - * BUG * no font!\n", __FUNCTION__);
        return 0;
      }
      else if(pDefaultMenuFont)
      {
        pFont = pDefaultMenuFont;
      }

      iHPos = 1; // XTextWidth(pFont, "  ", 2);  // width of 2 spaces

      // if somethign is selected it will NOT be the same one I'm trying
      // to select now, so invalidate it so that it's re-painted
      if(pSelf->iSelected >= 0 && pSelf->iSelected < pMenu->nItems)
      {
        WBMenuItem *pItem0 = pMenu->ppItems[pSelf->iSelected];

        if(pItem0)
        {
          geom.x = iHPos;
          geom.y = pItem0->iPosition - 1; // actual practice suggests going outside the box a bit
          geom.width = pSelf->iWidth + 1;
          geom.height = iMaxY + 2;
          geom.border = 0;
          WBInvalidateGeom(wID, &geom, TRUE);  // force a re-paint (at some point in time)
        }
      }

      pSelf->iSelected = i1;  // indicate the item that's NOW selected

      geom.x = iHPos;
      geom.y = pItem->iPosition - 1; // actual practice suggests going outside the box a bit
      geom.width = pSelf->iWidth + 1; //pItem->iTextWidth;
      geom.height = iMaxY + 2;
      geom.border = 0;

      WBInvalidateGeom(wID, &geom, TRUE);  // force a re-paint (at some point in time)

      return pItem;
    }
  }

  return NULL;
}

static void __PostActivatePrevNextEvent(WBMenuPopupWindow *pSelf, int iPrevNext)
{
XClientMessageEvent evt;

  bzero(&evt, sizeof(evt));
  evt.type = ClientMessage;
  evt.display = WBGetWindowDisplay(pSelf->wBar);
  evt.window = pSelf->wBar;
  evt.message_type = aMENU_ACTIVATE;
  evt.format = 32;
  evt.data.l[0] = 0;
  evt.data.l[1] = iPrevNext;  // previous/next indicator

  WBPostPriorityEvent(pSelf->wBar, (XEvent *)&evt);

  WB_DEBUG_PRINT(DebugLevel_Chatty | DebugSubSystem_Menu | DebugSubSystem_Keyboard,
                  "%s - posting Prev/Next %d to menu bar %d (%08xH)\n",
                  __FUNCTION__, iPrevNext, (int)pSelf->wBar, (int)pSelf->wBar);
}

// this next callback is assigned via WBRegisterWindowCallback
static int MBMenuPopupEvent(Window wID, XEvent *pEvent)
{
  Display *pDisplay = WBGetWindowDisplay(wID);
  WBMenuPopupWindow *pSelf = MBGetMenuPopupWindowStruct(wID);
  WBMenuItem *pItem;
  WBMenu *pMenu = pSelf ? pSelf->pMenu : NULL;
  int i1, iPrevSel;
  WB_GEOM geom;



  if(!pSelf)
  {
    WB_WARN_PRINT("%s - pSelf is NULL\n", __FUNCTION__);
  }
  else
  {
    // process 'destroy' events
    if(pEvent->type == DestroyNotify)
    {
      if(pEvent->xdestroywindow.window == wID) // destroying myself?
      {
        WB_DEBUG_PRINT(DebugLevel_Heavy | DebugSubSystem_Menu,
                       "%s - DestroyNotify\n", __FUNCTION__);

        WBSetWindowData(pSelf->wSelf, 0, 0);  // clear the 'back pointer' now

        free(pSelf);  // on destroy I always do this (no owned objects so it's easy)

        return 1;  // processed
      }
    }

    // process 'FocusOut' events (this will destroy the window)

    if(pEvent->type == FocusOut)
    {
      WB_DEBUG_PRINT(DebugLevel_Heavy | DebugSubSystem_Menu,
                     "%s - FocusOut\n", __FUNCTION__);

      WBEndModal(wID, -1);  // return '-1' meaning "canceled"
      return 1;  // processed
    }

    // process 'expose' events

    if(pEvent->type == Expose)
    {
      return MenuPopupDoExposeEvent((XExposeEvent *)pEvent, pMenu, pDisplay, wID, pSelf);
    }

    // process keyboard events

    if((pEvent->type == KeyPress ||
        pEvent->type == KeyRelease))
    {
      int iACS=0;
      int iKey = WBKeyEventProcessKey((XKeyEvent *)pEvent, NULL, NULL, &iACS);

      WB_DEBUG_PRINT(DebugLevel_Heavy | DebugSubSystem_Menu | DebugSubSystem_Keyboard | DebugSubSystem_Event,
                     "%s - key press/release %x (%d) iACS=%x\n", __FUNCTION__, iKey, iKey, iACS);

      if(pEvent->type == KeyPress)
      {
        if(iACS & WB_KEYEVENT_KEYSYM)
        {
          if((iACS & WB_KEYEVENT_ALT)
             && (iKey >= XK_F1 && iKey <= XK_F35))
          {
            XKeyEvent kevt;

            // re-post message to owner as soon as the popup menu goes away

            memcpy(&kevt, &(pEvent->xkey), sizeof(kevt));
            kevt.window = kevt.subwindow = pSelf->wOwner;

            WBEndModal(wID, -1); // canceled menu

            WBSetInputFocus(pSelf->wOwner);
            XPutBackEvent(pDisplay, (XEvent *)&kevt);  // I want this processed correctly

            return 1; // processed
          }
          else if(iKey == XK_Left)
          {
            // post an activate message to the menu bar to move to next left and open popup
            // and then close this one.

            __PostActivatePrevNextEvent(pSelf, -1);

            WBEndModal(wID, -1); // canceled menu
            return 1; // processed
          }
          else if(iKey == XK_Right)
          {
            // post an activate message to the menu bar to move to next left and open popup
            // and then close this one.

            __PostActivatePrevNextEvent(pSelf, 1);

            WBEndModal(wID, -1); // canceled menu
            return 1; // processed
          }
          else if(iKey == XK_Up)
          {
            i1 = pSelf->iSelected;

            __SetPrevSelection(pSelf, pMenu);

            if(i1 != pSelf->iSelected)
            {
              WBInvalidateGeom(wID, NULL, TRUE); // TODO:  optimize
            }
            return 1; // handled
          }
          else if(iKey == XK_Down)
          {
            i1 = pSelf->iSelected;

            __SetNextSelection(pSelf, pMenu);

            if(i1 != pSelf->iSelected)
            {
              WBInvalidateGeom(wID, NULL, TRUE); // TODO:  optimize
            }
            return 1; // handled
          }
          else if(iKey == XK_Prior)
          {
            i1 = pSelf->iSelected;

            __SetFirstSelection(pSelf, pMenu);

            if(i1 != pSelf->iSelected)
            {
              WBInvalidateGeom(wID, NULL, TRUE); // TODO:  optimize
            }
            return 1; // handled
          }
          else if(iKey == XK_Next)
          {
            i1 = pSelf->iSelected;

            __SetLastSelection(pSelf, pMenu);

            if(i1 != pSelf->iSelected)
            {
              WBInvalidateGeom(wID, NULL, TRUE); // TODO:  optimize
            }
            return 1; // handled
          }
        }
        else if(iKey == '\x1b' ||
                (((iACS & WB_KEYEVENT_ALT) || (iACS & WB_KEYEVENT_CTRL))// checking for ctrl+TAB or ALT+TAB
                 && iKey=='\t'))
        {
          XKeyEvent kevt;

          memcpy(&kevt, &(pEvent->xkey), sizeof(kevt));
          kevt.window = kevt.subwindow = pSelf->wOwner;

          WBEndModal(wID, -1); // canceled menu

          if(iKey == '\t')  // for the ALT+TAB or CTRL+TAB
          {
            WBSetInputFocus(pSelf->wOwner);
            XPutBackEvent(pDisplay, (XEvent *)&kevt);  // I want this processed correctly
          }

          return 1; // processed
        }

        if(MBMenuProcessHotKey(pMenu, (XKeyEvent *)pEvent) > 0)
        {
          return 1;  // activated
        }
      }
      else if(pEvent->type == KeyRelease)
      {
        // up/down arrow keys, home, end, pgup, pgdown - choose active item

        // <ENTER>, space bar - activate current item

        if(!iACS && (iKey == '\n' || iKey == '\r' || iKey == ' '))
        {
          pItem = __GetCurrentSelection(pSelf, pMenu);

          MBMenuPopupHandleMenuItem(pDisplay, wID, pSelf, pMenu, pItem);
          return 1; // handled
        }
      }
    }

    // process mouse events

    if((pEvent->type == ButtonPress ||
        pEvent->type == ButtonRelease ||
        pEvent->type == MotionNotify))
    {
      int iX, iY; // mousie coordinates (when needed)

      // mousie clickie - yay!

      if(pEvent->type == MotionNotify)
      {
        WBXlatCoordPoint(pEvent->xmotion.window, pEvent->xmotion.x, pEvent->xmotion.y,
                         wID, &iX, &iY);
        if(pEvent->xmotion.state & Button1Mask) // left drag?
        {
          // todo - highlight selections by simulating button press, drop into a popup, etc.
        }
        else if(!(pEvent->xmotion.state & (Button1Mask | Button2Mask | Button3Mask)))
        {
          // "hover select" - hover far left or far far right, close popup
          //                  vertical hover over, select (immediate)
          //                  hover over far right, open popup (as needed)
          //                  hover below, scroll (as needed)
          //                  hover above, scroll (as needed)

          // hover far left or far far right

          WB_WARN_PRINT("TEMPORARY:  %d %d %d\n",
                        iX, pSelf->iX, pSelf->iX + pSelf->iWidth);
          if(iX < pSelf->iX - WB_MOUSE_FAR || iX > pSelf->iX + pSelf->iWidth + 2 * WB_MOUSE_FAR)
          {
            WBEndModal(wID, -1); // canceled menu

            WBSetInputFocus(pSelf->wOwner);
            return 1;  // no further processing on this one
          }

          // vertical 'hover over'
          iPrevSel = pSelf->iSelected;
          __SelectMenuItemFromMousePos(wID, pSelf, pMenu, iX, iY);

          if(iPrevSel != pSelf->iSelected)
          {
            // re-set timers for hover
          }

          return 0;  // this is just advisory
        }
      }
      else if(pEvent->type == ButtonPress && pMenu)
      {
        WBXlatCoordPoint(pEvent->xbutton.window, pEvent->xbutton.x, pEvent->xbutton.y,
                         wID, &iX, &iY);

        if(pEvent->xbutton.state & (Button2Mask | Button3Mask | Button4Mask | Button5Mask
                                    | ShiftMask | LockMask | ControlMask))
        {
          // this click I ignore (for now)
        }
        else if(iY >= pSelf->iY && iY <= (pSelf->iY + pSelf->iHeight))
        {
          // I'm within the dimensions of the 'menu area' so I must now
          // which menu item I'm pointing at

          if(__SelectMenuItemFromMousePos(wID, pSelf, pMenu, iX, iY))
          {
            return 1;  // item was found
          }

          WB_WARN_PRINT("%s - couldn't find the menu - %d, %d\n", __FUNCTION__, iX, iY);
        }
        else
        {
          WB_DEBUG_PRINT(DebugLevel_WARN | DebugSubSystem_Menu | DebugSubSystem_Mouse,
                         "%s - Mouse is out of range - %d, %d, %d, %d, %d, %d\n",
                         __FUNCTION__, iX, iY, pSelf->iX, pSelf->iY, pSelf->iWidth, pSelf->iHeight);
        }
      }
      else // if(pEvent->type == ButtonRelease)
      {
        i1 = pSelf->iSelected;
        pSelf->iSelected = -1;

        if(i1 < 0 || i1 >= pMenu->nItems)
          return 0;  // nothing selected

        pItem = pMenu->ppItems[i1];
        if(!pItem)
          return 0;

        geom.x = pItem->iPosition;
        geom.y = pSelf->iY;
        geom.width = pItem->iTextWidth;
        geom.height = pSelf->iHeight;
        geom.border = 0;

        WBInvalidateGeom(wID, &geom, TRUE);  // force a re-paint (at some point in time)

        // determine what the correct action for this menu item is...
        if(pItem->iAction == -1)
          return 0;  // "not handled"

        MBMenuPopupHandleMenuItem(pDisplay, wID, pSelf, pMenu, pItem);

        return 1;
      }
    }
  }

  if(pEvent && pEvent->type == ClientMessage)
  {
    if(pMenu &&
       ((XClientMessageEvent *)pEvent)->message_type == aMENU_ACTIVATE)
    {
      int iMenuItemIndex = ((XClientMessageEvent *)pEvent)->data.l[1];
      WBMenuItem *pItem = NULL;

      if(iMenuItemIndex >= 0 && iMenuItemIndex < pMenu->nItems)
      {
        pItem = pMenu->ppItems[iMenuItemIndex];
        if((unsigned long)pItem == (unsigned long)(((XClientMessageEvent *)pEvent)->data.l[0]))
        {
          MBMenuPopupHandleMenuItem(pDisplay, wID, pSelf, pMenu, pItem);

          return 1;  // handled
        }
      }

      WB_WARN_PRINT("%s - MENU_ACTIVATE event, invalid menu information, %d %d %p %p\n",
                    __FUNCTION__, iMenuItemIndex, pMenu->nItems,
                    (void *)pItem, (void *)(((XClientMessageEvent *)pEvent)->data.l[0]));
    }
    else if(((XClientMessageEvent *)pEvent)->message_type == aMENU_DISPLAY_POPUP)
    {
      WBMenuPopupWindow *pPopup;
      int iRval = -1;
      int iMenuItem = ((XClientMessageEvent *)pEvent)->data.l[0];
      int iPosition = ((XClientMessageEvent *)pEvent)->data.l[1];

      for(i1=0; i1 < pMenu->nPopups; i1++)
      {
        WB_DEBUG_PRINT(DebugLevel_Excessive | DebugSubSystem_Menu,
                       "%s - popup menu id = %d\n",
                       __FUNCTION__, (pMenu->ppPopups[i1]->iMenuID & WBMENU_POPUP_MASK));

        if(pMenu->ppPopups[i1] &&
            (pMenu->ppPopups[i1]->iMenuID & WBMENU_POPUP_MASK) == iMenuItem)
        {
          pPopup = MBCreateMenuPopupWindow(pSelf->wSelf, pSelf->wOwner, pMenu->ppPopups[i1],
                                           iPosition, pSelf->iY + pSelf->iHeight, 0);

          if(pPopup)
          {
            WB_DEBUG_PRINT(DebugLevel_Heavy | DebugSubSystem_Menu,
                           "%s - Displaying popup menu %d\n", __FUNCTION__, iMenuItem);

            iRval = MBMenuDoModal(pPopup);

            WB_DEBUG_PRINT(DebugLevel_Heavy | DebugSubSystem_Menu,
                           "%s - done with popup menu %d, return value %d\n", __FUNCTION__, iMenuItem, iRval);
          }
          else
          {
            WB_ERROR_PRINT("%s - Unable to create popup menu %d\n", __FUNCTION__, iMenuItem);
          }

          WBInvalidateGeom(pSelf->wSelf, NULL, 1);  // re-paint (sort of a bug fix)
          WBEndModal(wID, iRval);
        }
      }
      if(i1 >= pMenu->nPopups)
      {
        WB_ERROR_PRINT("%s - Unable to locate popup menu %d\n", __FUNCTION__, iMenuItem);
      }

      // in all cases, exit anyway
      WBEndModal(wID, -1);
    }
  }

  return 0;  // temporary
}

