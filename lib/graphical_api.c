////////////////////////////////////////////////////////////////////////////////////////
//                                                                                    //
//                             _      _              _                    _           //
//    __ _  _ __  __ _  _ __  | |__  (_)  ___  __ _ | |      __ _  _ __  (_)    ___   //
//   / _` || '__|/ _` || '_ \ | '_ \ | | / __|/ _` || |     / _` || '_ \ | |   / __|  //
//  | (_| || |  | (_| || |_) || | | || || (__| (_| || |    | (_| || |_) || | _| (__   //
//   \__, ||_|   \__,_|| .__/ |_| |_||_| \___|\__,_||_|_____\__,_|| .__/ |_|(_)\___|  //
//   |___/             |_|                            |_____|     |_|                 //
//                                                                                    //
//                                                                                    //
//      Graphical API 'wrapper' functions, an X11-style API for cross-platform        //
//                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////

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


/** \file graphical_api.c
  * \brief 'graphical api' implementation file for the X11 Work Bench Toolkit API
  *
  * X11 Work Bench Toolkit Graphical API
  * This file implements an X11-like API for low-level graphical operations
  *
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <unistd.h>
#include <memory.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h> // for 'gettimeofday'

// TODO:  determine if pthread is available, optionally use for timers
#include <pthread.h> /* currently required */

#include <X11/cursorfont.h>
#ifndef XK_Delete /* moslty for interix */
#define XK_MISCELLANY /* mostly for interix */
#include <X11/keysymdef.h> // some platforms don't automatically include this with X headers
#endif // XK_Delete

#include "window_helper.h" /* contains definitions for functions implemented here */
#include "pixmap_helper.h"
#include "conf_help.h"


static XImage *__internalGetClipImage(WBGC hGC)
{
XImage *pRval = NULL;
Display *display = hGC->display;
const XGCValues *pGCValues = &(hGC->values);


  if(pGCValues->clip_mask != None)
  {
    Window winRoot = None;
    int iX0=0, iY0=0;
    unsigned int iWidth0=0, iHeight0=0, iBorder;
    unsigned int uiDepth = 0;

    // create a pixmap from the clip mask
    // TODO: cache all of this someplace?

    BEGIN_XCALL_DEBUG_WRAPPER
    XGetGeometry(display, pGCValues->clip_mask, &winRoot, &iX0, &iY0, &iWidth0, &iHeight0, &iBorder, &uiDepth);
    END_XCALL_DEBUG_WRAPPER

    // clip region will translate into 1-plane XYPixmap, from a 1-plane Pixmap
    // hopefully this is a REALLY FAST EFFICIENT process.  Otherwise, this could suck for performance.

    // TODO:  wrap the 'GC' as a WB_GC and cache a few things *LIKE* the clip region as an XImage
    //        This might actually work out better since I'd be avoiding a few arcane things and focusing
    //        on providing a way to do graphics operations compatible with WIN32

    pRval = WBXGetImage(display, pGCValues->clip_mask, 0, 0, iWidth0, iHeight0, 1, XYPixmap);
  }

  return pRval;
}



int WBDrawPoint(Display *display, Drawable d, WBGC gc, int x, int y)
{
int iRval;

#ifdef WIN32
#error not ready for WIN32 yet
#else // WIN32

XImage *pImage;

  if(!display)
  {
    display = WBGetWindowDisplay((Window)d);
  }

  // if it's a window, I'll get its XImage.  If NULL, do it the old way

  pImage = WBGetWindowImage(display, d);

  if(!pImage)
  {
    BEGIN_XCALL_DEBUG_WRAPPER
    iRval = XDrawPoint(display, d, gc->gc, x, y);
    END_XCALL_DEBUG_WRAPPER
  }
  else
  {
    iRval = WBXDrawPoint(pImage, gc, x, y);
  }

#endif // WIN32

  return iRval;
}


int WBDrawPoints(Display *display, Drawable d, WBGC gc, XPoint *points,
                 int npoints, int mode)
{
int iRval;

#ifdef WIN32
#error not ready for WIN32 yet
#else // WIN32

XImage *pImage;

  if(!display)
  {
    display = WBGetWindowDisplay((Window)d);
  }

  // if it's a window, I'll get its XImage.  If NULL, do it the old way

  pImage = WBGetWindowImage(display, d);

  if(!pImage)
  {
    BEGIN_XCALL_DEBUG_WRAPPER
    iRval = XDrawPoints(display, d, gc->gc, points, npoints, mode);
    END_XCALL_DEBUG_WRAPPER
  }
  else
  {
    iRval = WBXDrawPoints(pImage, gc, points, npoints, mode);
  }

#endif // WIN32

  return iRval;
}


int WBDrawLine(Display *display, Drawable d, WBGC gc,
               int x1, int y1, int x2, int y2)
{
int iRval;

#ifdef WIN32
#error not ready for WIN32 yet
#else // WIN32

XImage *pImage;

  if(!display)
  {
    display = WBGetWindowDisplay((Window)d);
  }

  // if it's a window, I'll get its XImage.  If NULL, do it the old way

  pImage = WBGetWindowImage(display, d);

  if(!pImage)
  {
    BEGIN_XCALL_DEBUG_WRAPPER
    iRval = XDrawLine(display, d, gc->gc, x1, y1, x2, y2);
    END_XCALL_DEBUG_WRAPPER
  }
  else
  {
    iRval = WBXDrawLine(pImage, gc, x1, y1, x2, y2);
  }

#endif // WIN32

  return iRval;
}


int WBDrawLines(Display *display, Drawable d, WBGC gc, XPoint *points,
                int npoints, int mode)
{
int iRval;

#ifdef WIN32
#error not ready for WIN32 yet
#else // WIN32

XImage *pImage;

  if(!display)
  {
    display = WBGetWindowDisplay((Window)d);
  }

  // if it's a window, I'll get its XImage.  If NULL, do it the old way

  pImage = WBGetWindowImage(display, d);

  if(!pImage)
  {
    BEGIN_XCALL_DEBUG_WRAPPER
    iRval = XDrawLines(display, d, gc->gc, points, npoints, mode);
    END_XCALL_DEBUG_WRAPPER
  }
  else
  {
    iRval = WBXDrawLines(pImage, gc, points, npoints, mode);
  }

#endif // WIN32

  return iRval;
}


int WBDrawRectangle(Display *display, Drawable d, WBGC gc, int x, int y,
                    unsigned int width, unsigned int height)
{
int iRval;

#ifdef WIN32
#error not ready for WIN32 yet
#else // WIN32

XImage *pImage;

  if(!display)
  {
    display = WBGetWindowDisplay((Window)d);
  }

  // if it's a window, I'll get its XImage.  If NULL, do it the old way

  pImage = WBGetWindowImage(display, d);

  if(!pImage)
  {
    BEGIN_XCALL_DEBUG_WRAPPER
    iRval = XDrawRectangle(display, d, gc->gc, x, y, width, height);
    END_XCALL_DEBUG_WRAPPER
  }
  else
  {
    iRval = WBXDrawRectangle(pImage, gc, x, y, width, height);
  }

#endif // WIN32

  return iRval;
}


int WBFillRectangle(Display *display, Drawable d, WBGC gc, int x, int y,
                    unsigned int width, unsigned int height)
{
int iRval;

#ifdef WIN32
#error not ready for WIN32 yet
#else // WIN32

XImage *pImage;

  if(!display)
  {
    display = WBGetWindowDisplay((Window)d);
  }

  // if it's a window, I'll get its XImage.  If NULL, do it the old way

  pImage = WBGetWindowImage(display, d);

  if(!pImage)
  {
    BEGIN_XCALL_DEBUG_WRAPPER
    iRval = XFillRectangle(display, d, gc->gc, x, y, width, height);
    END_XCALL_DEBUG_WRAPPER
  }
  else
  {
    iRval = WBXFillRectangle(pImage, gc, x, y, width, height);
  }

#endif // WIN32

  return iRval;
}


int WBDrawArc(Display *display, Drawable d, WBGC gc, int x, int y,
              unsigned int width, unsigned int height,
              int angle1, int angle2)
{
int iRval;

#ifdef WIN32
#error not ready for WIN32 yet
#else // WIN32

XImage *pImage;

  if(!display)
  {
    display = WBGetWindowDisplay((Window)d);
  }

  // if it's a window, I'll get its XImage.  If NULL, do it the old way

  pImage = WBGetWindowImage(display, d);

  if(!pImage)
  {
    BEGIN_XCALL_DEBUG_WRAPPER
    iRval = XDrawArc(display, d, gc->gc, x, y, width, height, angle1, angle2);
    END_XCALL_DEBUG_WRAPPER
  }
  else
  {
    iRval = WBXDrawArc(pImage, gc, x, y, width, height, angle1, angle2);
  }

#endif // WIN32

  return iRval;
}


int WBFillArc(Display *display, Drawable d, WBGC gc, int x, int y,
              unsigned int width, unsigned int height,
              int angle1, int angle2)
{
int iRval;

#ifdef WIN32
#error not ready for WIN32 yet
#else // WIN32

XImage *pImage;

  if(!display)
  {
    display = WBGetWindowDisplay((Window)d);
  }

  // if it's a window, I'll get its XImage.  If NULL, do it the old way

  pImage = WBGetWindowImage(display, d);

  if(!pImage)
  {
    BEGIN_XCALL_DEBUG_WRAPPER
    iRval = XFillArc(display, d, gc->gc, x, y, width, height, angle1, angle2);
    END_XCALL_DEBUG_WRAPPER
  }
  else
  {
    iRval = WBXFillArc(pImage, gc, x, y, width, height, angle1, angle2);
  }

#endif // WIN32

  return iRval;
}


int WBFillPolygon(Display *display, Drawable d, WBGC gc, XPoint *points,
                  int npoints, int shape, int mode)
{
int iRval;

#ifdef WIN32
#error not ready for WIN32 yet
#else // WIN32

XImage *pImage;

  if(!display)
  {
    display = WBGetWindowDisplay((Window)d);
  }

  // if it's a window, I'll get its XImage.  If NULL, do it the old way

  pImage = WBGetWindowImage(display, d);

  if(!pImage)
  {
    BEGIN_XCALL_DEBUG_WRAPPER
    iRval = XFillPolygon(display, d, gc->gc, points, npoints, shape, mode);
    END_XCALL_DEBUG_WRAPPER
  }
  else
  {
    iRval = WBXFillPolygon(pImage, gc, points, npoints, shape, mode);
  }

#endif // WIN32

  return iRval;
}


int WBDrawString(Display *display, Drawable d, WBGC gc, int x, int y,
                 const char *string, int length)
{
int iRval = 0;

#ifdef WIN32
#error not ready for WIN32 yet
#else // WIN32

XImage *pImage;

  // if it's a window, I'll get its XImage.  If NULL, do it the old way

  pImage = WBGetWindowImage(display, d);

  if(!display)
  {
    display = WBGetWindowDisplay((Window)d);
  }

  if(!pImage)
  {
    char *pTemp;

    pTemp = WBAlloc(length + 1);
    if(!pTemp)
    {
      return -1;
    }

    memcpy(pTemp, string, length);
    pTemp[length] = 0;

    // for now do this - but I should make it a call to DTDrawString instead
#ifdef X11WORKBENCH_TOOLKIT_HAVE_XFT
    if(gc->pFont && gc->pFont->pxftFont)
    {
      iRval = 0; // for now
#warning TODO: IMPLEMENT THIS PART for Xft FONTS
    }
    else
#endif // X11WORKBENCH_TOOLKIT_HAVE_XFT
    if(gc->pFont && gc->pFont->fsFont)
    {
      WB_DEBUG_PRINT(DebugLevel_Verbose | DebugSubSystem_DrawText,
                     "%s.%d using WB_DRAW_STRING for \"%-.*s\" color=#%08lxH bkgnd=#%08lxH\n",
                     __FUNCTION__, __LINE__, length, pTemp,
                     WBGetForeground(gc), WBGetBackground(gc));

      BEGIN_XCALL_DEBUG_WRAPPER
      WB_DRAW_STRING(display, d, gc->pFont->fsFont, gc->gc, x, y, pTemp, length);
      END_XCALL_DEBUG_WRAPPER
      iRval = length; // assume it worked
    }
    else // using legacy X11 fonts
    {
      if(gc->pFont && gc->pFont->pFontStruct && gc->pFont->pFontStruct->fid != None)
      {
        // if GC font is not this one, assign it to this one
        if(!gc->pFont || gc->values.font != gc->pFont->pFontStruct->fid)
        {
          BEGIN_XCALL_DEBUG_WRAPPER
          XChangeGC(gc->display, gc->gc, GCFont, &(gc->values));
          XGetGCValues(gc->display, gc->gc,
                       (GCAll & ~(GCClipMask | GCDashList)), // clip mask and dash mask can't be requested
                       &(gc->values));                      // query them all to snapshot them
          END_XCALL_DEBUG_WRAPPER
        }
      }

      WB_DEBUG_PRINT(DebugLevel_Verbose | DebugSubSystem_DrawText,
                     "%s.%d using XDrawString for \"%-.*s\" color=#%08lxH bkgnd=#%08lxH\n",
                     __FUNCTION__, __LINE__, length, pTemp,
                     WBGetForeground(gc), WBGetBackground(gc));

      BEGIN_XCALL_DEBUG_WRAPPER
      iRval = XDrawString(display, d, gc->gc, x, y, pTemp, length);
      END_XCALL_DEBUG_WRAPPER
    }

    WBFree(pTemp);
  }
  else
  {
    iRval = WBXDrawString(pImage, gc->pFont, gc, x, y, string, length);
    // NOTE:  does not update immediately, but after you 'end paint'
  }

#endif // WIN32

  return iRval;
}





/////////////////////////////////////////////////////////////////////////////////////////////////
//    ____                     _      _               ____               _               _     //
//   / ___| _ __  __ _  _ __  | |__  (_)  ___  ___   / ___| ___   _ __  | |_  ___ __  __| |_   //
//  | |  _ | '__|/ _` || '_ \ | '_ \ | | / __|/ __| | |    / _ \ | '_ \ | __|/ _ \\ \/ /| __|  //
//  | |_| || |  | (_| || |_) || | | || || (__ \__ \ | |___| (_) || | | || |_|  __/ >  < | |_   //
//   \____||_|   \__,_|| .__/ |_| |_||_| \___||___/  \____|\___/ |_| |_| \__|\___|/_/\_\ \__|  //
//                     |_|                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////////////


WBGC WBCreateGC(Display *pDisplay, Drawable dw, unsigned long valuemask,
                const XGCValues *values)
{
WBGC pRval;


  pRval = (WBGC)WBAlloc(sizeof(*pRval)); // WBGC is a pointer, don't use 'sizeof' on that!!!
  if(!pRval)
  {
    return NULL;
  }

  if(!pDisplay)
  {
    pDisplay = WBGetDefaultDisplay();
  }

  if(dw == None)
  {
    dw = WBGetHiddenHelperWindow(); // basically I need to have something valid for a Drawable
  }

  memset(pRval, 0, sizeof(*pRval));

  // TEMPORARILY, use 'pRval->values' to cache what I have in 'values' becauswe it's read-only
  if(valuemask && values)
  {
    memcpy(&(pRval->values), values, sizeof(pRval->values));
  }

  pRval->display = pDisplay;
  pRval->dw = dw;

  BEGIN_XCALL_DEBUG_WRAPPER
  // if 'values' is NULL, pass NULL; otherwise, address to pRval->values
  pRval->gc = XCreateGC(pDisplay, dw, valuemask, valuemask && values ? &(pRval->values) : NULL);
  END_XCALL_DEBUG_WRAPPER

  if(pRval->gc == None)
  {
    WBFree(pRval);
    return NULL;
  }

  // now re-get 'values' structure based on what was assigned...
  memset(&(pRval->values), 0, sizeof(pRval->values)); // must do this first

  BEGIN_XCALL_DEBUG_WRAPPER
  XGetGCValues(pDisplay, pRval->gc,
               (GCAll & ~(GCClipMask | GCDashList)), // clip mask and dash mask can't be requested
               &(pRval->values)); // query them all to snapshot them
  END_XCALL_DEBUG_WRAPPER

  // From the 'man' page:  an invalid resource ID (with one or more of the three most significant
  //                       bits set to 1) will be returned for GCFont, GCTile, and GCStipple if the
  //                       component has never been explicitly set by the client.

  if((CARD32)pRval->values.font & 0xe0000000) // not valid?
  {
    pRval->values.font = None;
  }

  if((CARD32)pRval->values.tile & 0xe0000000) // not valid?
  {
    pRval->values.tile = None;
  }

  if((CARD32)pRval->values.stipple & 0xe0000000) // not valid?
  {
    pRval->values.stipple = None;
  }

  // NOTE:  this really shouldn't be affected, but...
  if((CARD32)pRval->values.clip_mask & 0xe0000000) // not valid?
  {
    WB_ERROR_PRINT("ERROR - %s - hGCDest->values.clip_mask not valid, %d (%08xH)\n",
                   __FUNCTION__, (int)pRval->values.clip_mask, (int)pRval->values.clip_mask);

    pRval->values.clip_mask = None;
  }


  // if the original 'values' had a valid clip mask, make a copy of it

  if(values && (valuemask & GCClipMask) && values->clip_mask != None)
  {
    if(values->clip_mask & 0xe0000000) // not valid?
    {
      WB_ERROR_PRINT("ERROR - %s - values 'clip_mask' not valid, %d (%08xH)\n",
                     __FUNCTION__, (int)values->clip_mask, (int)values->clip_mask);

      pRval->values.clip_mask = None;
    }
    else
    {
      // TODO:  is this really the right way to do it???  should I make a copy instead?
      pRval->values.clip_mask = values->clip_mask; // make a copy

      // I am setting a clip mask, so cache it as an XImage

      pRval->clip_image = __internalGetClipImage(pRval);

      pRval->values.clip_mask = None; // don't share the old clip mask between WBGC's

      // TODO:  should I create a region from it?
    }
  }

  return pRval;
}

int WBChangeGC(WBGC hGC, unsigned long valuemask,
               const XGCValues *values)
{
int iRval;
XGCValues valtemp;

  // if I'm changing something that has a cache, destroy the old one
  // or the same thing if I can't actually read the value like GCDash, GC

  if(valuemask & GCDashList)
  {
    hGC->values.dashes = values->dashes;
  }

  if(valuemask & GCFont)
  {
//    if(hGC->pFont && (!values || values->font != hGC->values.font))
//    {
//      BEGIN_XCALL_DEBUG_WRAPPER
//      XFreeFontInfo(NULL, hGC->pFont, 1);
//      END_XCALL_DEBUG_WRAPPER
//
//      hGC->pFont = NULL;
//    }
//    if(hGC->fontset != None && (!values || values->font != hGC->values.font))
//    {
//      BEGIN_XCALL_DEBUG_WRAPPER
//      XFreeFontSet(hGC->display, hGC->fontset);
//      END_XCALL_DEBUG_WRAPPER
//
//      hGC->fontset = None;
//    }
  }

  if(valuemask & GCTile)
  {
    if(hGC->tile_image && (!values || values->tile != hGC->values.tile))
    {
      BEGIN_XCALL_DEBUG_WRAPPER
      XDestroyImage(hGC->tile_image);
      END_XCALL_DEBUG_WRAPPER

      hGC->tile_image = NULL;
    }
  }

  if(valuemask & GCStipple)
  {
    if(hGC->stip_image && (!values || values->stipple != hGC->values.stipple))
    {
      BEGIN_XCALL_DEBUG_WRAPPER
      XDestroyImage(hGC->stip_image);
      END_XCALL_DEBUG_WRAPPER

      hGC->stip_image = NULL;
    }
  }

  // NOTE:  the clip mask in this case is related to the clip region, and so if I
  //        change the clip region I should update the clip mask, since that's what
  //        the X server will be doing.  SO 'vice versa' if using this function call
  //        meaning I may have to 'nuke out' some kind of clip region???  Yeah that
  //        wouldn't be good, so maybe just free the thing...

  if(valuemask & GCClipMask)
  {
    // NOTE:  GCClipMask and GCDashList can't be requested but can be changed with this call
    //        when using the XChangeGC call...

#warning if I am NOT making a copy and assign anyway, will the ref count be messed up???

    if(hGC->values.clip_mask != None)
    {
      BEGIN_XCALL_DEBUG_WRAPPER
      XFreePixmap(hGC->display, hGC->values.clip_mask);
      END_XCALL_DEBUG_WRAPPER

      hGC->values.clip_mask = None;
    }

    if(hGC->clip_image && (!values || values->clip_mask != hGC->values.clip_mask))
    {
      BEGIN_XCALL_DEBUG_WRAPPER
      XDestroyImage(hGC->clip_image);
      END_XCALL_DEBUG_WRAPPER

      hGC->clip_image = NULL;
    }

    if(hGC->clip_rgn != None)
    {
      BEGIN_XCALL_DEBUG_WRAPPER
      XDestroyRegion(hGC->clip_rgn);
      END_XCALL_DEBUG_WRAPPER

      hGC->clip_rgn = None;
    }
  }

  // make the changes, then query the results [this is the simpler way]
  // later, do I want to re-cache important things at this time, or wait
  // until I actually NEED them?  For now, I wait.

  memcpy(&valtemp, values, sizeof(valtemp)); // because 'values' is a const pointer

  BEGIN_XCALL_DEBUG_WRAPPER
  iRval = XChangeGC(hGC->display, hGC->gc, valuemask, &valtemp);
  XGetGCValues(hGC->display, hGC->gc,
               (GCAll & ~(GCClipMask | GCDashList)), // clip mask and dash mask can't be requested
               &(hGC->values));                      // query them all to snapshot them
  END_XCALL_DEBUG_WRAPPER

  // From the 'man' page:  an invalid resource ID (with one or more of the three most significant
  //                       bits set to 1) will be returned for GCFont, GCTile, and GCStipple if the
  //                       component has never been explicitly set by the client.

  if((CARD32)hGC->values.font & 0xe0000000) // not valid?
  {
    hGC->values.font = None;
  }

  if((CARD32)hGC->values.tile & 0xe0000000) // not valid?
  {
    hGC->values.tile = None;
  }

  if((CARD32)hGC->values.stipple & 0xe0000000) // not valid?
  {
    hGC->values.stipple = None;
  }

  // NOTE:  this really shouldn't be affected, but...
  if((CARD32)hGC->values.clip_mask & 0xe0000000) // not valid?
  {
    WB_ERROR_PRINT("ERROR - %s - hGCDest->values.clip_mask not valid, %d (%08xH)\n",
                   __FUNCTION__, (int)hGC->values.clip_mask, (int)hGC->values.clip_mask);

    hGC->values.clip_mask = None;
  }

  if(valuemask & GCClipMask) // ONLY if this flag is assigned
  {
//    if(values->clip_mask != None)// && hGC->values.clip_mask != values->clip_mask)
//    {
//      WB_WARN_PRINT("WARNING - %s - calling PXM_CopyPixmap on %d (%08xH), %d (%08xH)\n", __FUNCTION__, (int)hGC->dw, (int)hGC->dw, (int)values->clip_mask, (int)values->clip_mask);
//      // TODO:  should I really be doing this ???
//      hGC->values.clip_mask = PXM_CopyPixmap(hGC->display, hGC->dw, values->clip_mask);
//    }

    // I am setting a clip mask, so cache it as an XImage

    hGC->clip_image = __internalGetClipImage(hGC);
  }

  return iRval;
}

Status WBGetGCValues(WBGC hGC, unsigned long valuemask,
                     XGCValues *values)
{
Status rval;

  // for now, just do this.  later, use my cached copy as much as I can


  // clean up the cached stuff

  if(valuemask & GCClipMask)
  {
    if(values->clip_mask != None)
    {
      BEGIN_XCALL_DEBUG_WRAPPER
      XFreePixmap(hGC->display, values->clip_mask);
      END_XCALL_DEBUG_WRAPPER

      values->clip_mask = None;
    }
  }

  if(valuemask & GCDashList)
  {
    // TODO:  cache dash ?
  }

  BEGIN_XCALL_DEBUG_WRAPPER
  rval = XGetGCValues(hGC->display, hGC->gc,         // a safe way to do this
                      valuemask, values);
  END_XCALL_DEBUG_WRAPPER

  if((CARD32)values->font & 0xe0000000) // not valid?
  {
    values->font = None;
  }

  if((CARD32)values->tile & 0xe0000000) // not valid?
  {
    values->tile = None;
  }

  if((CARD32)values->stipple & 0xe0000000) // not valid?
  {
    values->stipple = None;
  }

  // NOTE:  this really shouldn't be affected, but...
  if((CARD32)values->clip_mask & 0xe0000000) // not valid?
  {
    WB_ERROR_PRINT("ERROR - %s - values->clip_mask not valid, %d (%08xH)\n",
                   __FUNCTION__, (int)values->clip_mask, (int)values->clip_mask);

    values->clip_mask = None;
  }
  else if(values->clip_mask & 0xffffffffe0000000LL)
  {
    WB_ERROR_PRINT("ERROR ERROR !!! - %s - values->clip_mask not valid, %d (%08xH)\n",
                   __FUNCTION__, (int)values->clip_mask, (int)values->clip_mask);

    values->clip_mask = None;
  }

  if(valuemask & GCDashList)
  {
    // TODO:  cache dash ?
    values->dashes = hGC->values.dashes; // for now, assuming it's valid
  }

  return rval;
}

WBGC WBCopyGC(WBGC hGCOrig)
{
WBGC pRval;
int iRet;

  // this will make a complete copy of the original GC, including
  // any cached elements.  By doing it this way I can do whatever I
  // want to with the copy and not have leaks nor use-after-free,
  // while preserving as much performance as I can

  if(!hGCOrig || hGCOrig->gc == None)
  {
    WB_ERROR_PRINT("ERROR - %s - invalid source WBGC\n", __FUNCTION__);
    return NULL; // not valid
  }

  if(hGCOrig->dw == None)
  {
    WB_ERROR_PRINT("ERROR - %s - invalid (NULL) drawable in source WBGC\n", __FUNCTION__);
    return NULL;
  }

  pRval = (WBGC)WBAlloc(sizeof(*pRval)); // WBGC is a pointer, don't use 'sizeof' on that!!!
  if(!pRval)
  {
    return NULL;
  }

  memset(pRval, 0, sizeof(*pRval));

  pRval->display = hGCOrig->display;
  pRval->dw = hGCOrig->dw;


  BEGIN_XCALL_DEBUG_WRAPPER
  pRval->gc = XCreateGC(pRval->display, pRval->dw, 0, NULL);
  END_XCALL_DEBUG_WRAPPER

  if(pRval->gc == None)
  {
    WBFree(pRval);
    return NULL;
  }


//  WB_WARN_PRINT("TEMPORARY:  %s - calling WBCopyGC2()\n", __FUNCTION__);
  iRet = WBCopyGC2(hGCOrig, GCAll, pRval);

  // TODO:  test iRet for success

  return pRval;
}

int WBCopyGC2(WBGC hGCOrig, unsigned long valuemask, WBGC hGCDest)
{
int iRet;

  if(!hGCOrig || !hGCDest || hGCOrig->gc == None || hGCDest->gc == None)
  {
    return -1;
  }

  BEGIN_XCALL_DEBUG_WRAPPER
  iRet = XCopyGC(hGCOrig->display, hGCOrig->gc, valuemask, hGCDest->gc);
  END_XCALL_DEBUG_WRAPPER

  // TODO:  check 'iRet' value which is poorly documented in the 'man' page


  // clean up the cached stuff

  if(valuemask & GCFont)
  {
    if(hGCDest->pFont)
    {
      WBFreeFont(hGCDest->display, hGCDest->pFont);
//      BEGIN_XCALL_DEBUG_WRAPPER
//      XFreeFontInfo(NULL, hGCDest->pFont, 1); // just the one; does not unload the font
//      END_XCALL_DEBUG_WRAPPER

      hGCDest->pFont = NULL;
    }
//    if(hGCDest->fontset != None)
//    {
//      BEGIN_XCALL_DEBUG_WRAPPER
//      XFreeFontSet(hGCDest->display, hGCDest->fontset);
//      END_XCALL_DEBUG_WRAPPER
//
//      hGCDest->fontset = None;
//    }
  }

  if(valuemask & GCTile)
  {
    if(hGCDest->tile_image)
    {
      WBXDestroyImage(hGCDest->tile_image);
      hGCDest->tile_image = NULL;
    }
  }

  if(valuemask & GCStipple)
  {
    if(hGCDest->stip_image)
    {
      WBXDestroyImage(hGCDest->stip_image);
      hGCDest->stip_image = NULL;
    }
  }

  if(valuemask & GCClipMask)
  {
    if(hGCDest->values.clip_mask != None)
    {
      // TODO:  do I need to do this??
//      BEGIN_XCALL_DEBUG_WRAPPER
//      XFreePixmap(hGCDest->display, hGCDest->values.clip_mask);
//      END_XCALL_DEBUG_WRAPPER
//
//      hGCDest->values.clip_mask = None;
    }

    if(hGCDest->clip_image)
    {
      WBXDestroyImage(hGCDest->clip_image);
      hGCDest->clip_image = NULL;
    }

    if(hGCDest->clip_rgn)
    {
      BEGIN_XCALL_DEBUG_WRAPPER
      XDestroyRegion(hGCDest->clip_rgn);
      END_XCALL_DEBUG_WRAPPER

      hGCDest->clip_rgn = None;
    }
  }

  if(valuemask & GCDashList)
  {
    // TODO:  cache dash ?
  }


  BEGIN_XCALL_DEBUG_WRAPPER
  XGetGCValues(hGCDest->display, hGCDest->gc,         // a safe way to do this
               (GCAll & ~(GCClipMask | GCDashList)),   // clip mask and dash mask can't be requested
               &(hGCDest->values));                    // query them all to snapshot them
  END_XCALL_DEBUG_WRAPPER

  // From the 'man' page:  an invalid resource ID (with one or more of the three most significant
  //                       bits set to 1) will be returned for GCFont, GCTile, and GCStipple if the
  //                       component has never been explicitly set by the client.

//  if((CARD32)hGCDest->values.font & 0xe0000000) // not valid?
//  {
//    hGCDest->values.font = None;
//  }

  if((CARD32)hGCDest->values.tile & 0xe0000000) // not valid?
  {
    hGCDest->values.tile = None;
  }

  if((CARD32)hGCDest->values.stipple & 0xe0000000) // not valid?
  {
    hGCDest->values.stipple = None;
  }

  // NOTE:  this really shouldn't be affected, but...
  if((CARD32)hGCDest->values.clip_mask & 0xe0000000) // not valid?
  {
    WB_ERROR_PRINT("ERROR - %s - hGCDest->values.clip_mask not valid, %d (%08xH)\n",
                   __FUNCTION__, (int)hGCDest->values.clip_mask, (int)hGCDest->values.clip_mask);
  }
  else if(hGCDest->values.clip_mask & 0xffffffffe0000000LL)
  {
    WB_ERROR_PRINT("ERROR ERROR !!! - %s - hGCDest->values.clip_mask not valid, %d (%08xH)\n",
                   __FUNCTION__, (int)hGCDest->values.clip_mask, (int)hGCDest->values.clip_mask);
  }


  // make copies of all of the cached things


  // tile, stipple, clip_mask, dashes

  if(hGCOrig->values.dashes != 0)
  {
    // TODO:  anything more??
    hGCDest->values.dashes = hGCOrig->values.dashes;
  }

  // NOTE:  these are being 'fudged' a bit and it's really a cached version.
  if(valuemask & GCClipMask)
  {
    // TODO:  do I need to do this???
//    if(hGCOrig->values.clip_mask != None)
//    {
//      WB_WARN_PRINT("WARNING - %s - calling PXM_CopyPixmap on %d (%08xH), %d (%08xH)\n", __FUNCTION__, (int)hGCOrig->dw, (int)hGCOrig->dw, (int)hGCOrig->values.clip_mask, (int)hGCOrig->values.clip_mask);
//      hGCDest->values.clip_mask = PXM_CopyPixmap(hGCDest->display, hGCOrig->dw,
//                                                 hGCOrig->values.clip_mask);
//    }

    if(hGCOrig->clip_rgn)
    {
      hGCDest->clip_rgn = WBCopyRegion(hGCOrig->clip_rgn);
    }

    if(hGCOrig->clip_image)
    {
      hGCDest->clip_image = WBXCopyImage(hGCDest->display, hGCOrig->clip_image);
    }
  }

  if(valuemask & GCTile)
  {
//    if(hGCOrig->values.tile != None)
//    {
//      hGCDest->values.tile = PXM_CopyPixmap(hGCOrig->values.tile);
//    }

    if(hGCOrig->tile_image)
    {
      hGCDest->tile_image = WBXCopyImage(hGCDest->display, hGCOrig->tile_image);
    }
  }

  if(valuemask & GCStipple)
  {
//    if(hGCOrig->values.stipple != None)
//    {
//      hGCDest->values.stipple = PXM_CopyPixmap(hGCOrig->values.stipple);
//    }

    if(hGCOrig->stip_image)
    {
      hGCDest->stip_image = WBXCopyImage(hGCDest->display, hGCOrig->stip_image);
    }
  }

  if(valuemask & GCFont)
  {
    if(hGCOrig->pFont)
    {
      hGCDest->pFont = WBCopyFont(hGCDest->display, hGCOrig->pFont);
    }

//    if(hGCOrig->fontset != None)
//    {
//      hGCDest->fontset = WBCopyModifyFontSet(hGCDest->display, hGCOrig->fontset, 0, 0);
//    }
  }

  return iRet;
}

WBGC WBCopyDrawableGC(Display *pDisplay, Drawable dw, WBGC hGCOrig)
{
WBGC pRval;
int iRet;

  // this will make a complete copy of the original GC, including
  // any cached elements.  By doing it this way I can do whatever I
  // want to with the copy and not have leaks nor use-after-free,
  // while preserving as much performance as I can

  if(!hGCOrig || hGCOrig->gc == None)
  {
    return NULL; // not valid
  }

  if(pDisplay == NULL)
  {
    pDisplay = hGCOrig->display;
  }

  if(dw == None)
  {
    dw = hGCOrig->dw;
  }

  pRval = (WBGC)WBAlloc(sizeof(*pRval)); // WBGC is a pointer, don't use 'sizeof' on that!!!
  if(!pRval)
  {
    return NULL;
  }

  memset(pRval, 0, sizeof(*pRval));

  pRval->display = pDisplay;
  pRval->dw = dw;

  // TODO:  handle different 'pDisplay' etc.

  BEGIN_XCALL_DEBUG_WRAPPER
  pRval->gc = XCreateGC(pDisplay, dw, 0, NULL);
  END_XCALL_DEBUG_WRAPPER

  if(pRval->gc == None)
  {
    WBFree(pRval);
    return NULL;
  }

//  WB_WARN_PRINT("TEMPORARY:  %s - calling WBCopyGC2()\n", __FUNCTION__);
  iRet = WBCopyGC2(hGCOrig, GCAll, pRval);

  // TODO:  test iRval for success

  return pRval;
}

void WBFreeGC(WBGC hGC)
{
  if(!hGC) // TODO: other validity tests??
  {
    return;
  }

  // delete all of the cached things, and 'clip_mask' (if any)

  if(hGC->values.clip_mask != None)
  {
    BEGIN_XCALL_DEBUG_WRAPPER
    // TODO:  do I need to do this ???
    XFreePixmap(hGC->display, hGC->values.clip_mask);
    END_XCALL_DEBUG_WRAPPER

    hGC->values.clip_mask = None;
  }

  if(hGC->clip_rgn)
  {
    BEGIN_XCALL_DEBUG_WRAPPER
    XDestroyRegion(hGC->clip_rgn);
    END_XCALL_DEBUG_WRAPPER

    hGC->clip_rgn = None;
  }

  if(hGC->pFont)
  {
//    BEGIN_XCALL_DEBUG_WRAPPER
//    XFreeFontInfo(NULL, hGC->pFont, 1); // just the one; does not unload the font
//    END_XCALL_DEBUG_WRAPPER

    WBFreeFont(hGC->display, hGC->pFont);
    hGC->pFont = NULL;
  }

//  if(hGC->fontset != None)
//  {
//    BEGIN_XCALL_DEBUG_WRAPPER
//    XFreeFontSet(hGC->display, hGC->fontset);
//    END_XCALL_DEBUG_WRAPPER
//
//    hGC->fontset = None;
//  }

  if(hGC->clip_image)
  {
    WBXDestroyImage(hGC->clip_image);
    hGC->clip_image = NULL;
  }

  if(hGC->tile_image)
  {
    WBXDestroyImage(hGC->tile_image);
    hGC->tile_image = NULL;
  }

  if(hGC->stip_image)
  {
    WBXDestroyImage(hGC->stip_image);
    hGC->stip_image = NULL;
  }

  // free the actual GC and the allocated memory

  XFreeGC(hGC->display, hGC->gc);
  hGC->gc = None;

  WBFree(hGC); // free the allocated RAM for the struct and I'm done
}

GContext WBGContextFromGC(WBGC hGC)
{
GContext rval;

  // for now, just do this.  later, use my cached copy

  BEGIN_XCALL_DEBUG_WRAPPER
  rval = XGContextFromGC(hGC->gc);
  END_XCALL_DEBUG_WRAPPER

  return rval;
}

WB_FONTC WBQueryGCFont(WBGC gc)
{
  if(!gc)
  {
    return NULL;
  }

  return gc->pFont; // just return the value as-is (as WB_FONTC)
}

WB_FONT WBGetGCFont(Display *pDisplay, WBGC gc)
{
//XFontStruct *pF, *pRval;


  if(!pDisplay)
  {
    pDisplay = WBGetDefaultDisplay();
  }

  if(!gc)
  {
    return NULL;
  }

  if(gc->pFont)
  {
    return WBCopyFont(gc->display, gc->pFont); // return copy of it
  }

  return NULL;
#if 0
  pRval = NULL;

  BEGIN_XCALL_DEBUG_WRAPPER
  pF = XQueryFont(pDisplay, gc->values.font);

  if(pF)
  {
    unsigned long lName = 0;
    if(XGetFontProperty(pF, XA_FONT, &lName)) // grab font name, which I'll use to load it
    {
      char *pName = WBGetAtomName(WBGetDefaultDisplay(), (Atom)lName);
      if(pName)
      {
        gc->pFont = XLoadQueryFont(pDisplay, pName);  // saved cached value, now owned by the WBGC
        if(gc->pFont) // font was loaded properly
        {
          pRval = WBCopyFont(gc->pFont);
        }

        WBFree(pName);
      }
    }

    XFreeFontInfo(NULL, pF, 1); // to JUST free "the info" (required for XQueryFont)
    // NOTE:  do not assign NULL to pF here, it's a flag below
  }
  END_XCALL_DEBUG_WRAPPER

  if(!pRval)
  {
    pRval = WBCopyFont(WBGetDefaultFont());
  }

  return pRval; // caller must delete this using XFreeFont()
#endif // 0
}

//XFontSet WBGetGCFontSet(Display *pDisplay, WBGC gc)
//{
//#warning implement this
//
//  return None; // for now...
//}



int WBSetRegion(WBGC hGC, Region rgnClip)
{
int iRet;


  BEGIN_XCALL_DEBUG_WRAPPER
  iRet = XSetRegion(hGC->display, hGC->gc, rgnClip);
  END_XCALL_DEBUG_WRAPPER

  if(hGC->clip_image)
  {
    WBXDestroyImage(hGC->clip_image);
    hGC->clip_image = NULL;
  }

  if(hGC->values.clip_mask != None)
  {
    BEGIN_XCALL_DEBUG_WRAPPER
    // TODO:  do I need to do this ???
    XFreePixmap(hGC->display, hGC->values.clip_mask);
    END_XCALL_DEBUG_WRAPPER

    hGC->values.clip_mask = None;
  }

  if(hGC->clip_rgn)
  {
    BEGIN_XCALL_DEBUG_WRAPPER
    XDestroyRegion(hGC->clip_rgn);
    END_XCALL_DEBUG_WRAPPER
  }

  if(rgnClip != None)
  {
    hGC->clip_rgn = WBCopyRegion(rgnClip); // cache a copy of it
  }

  return iRet;
}

int WBSetClipOrigin(WBGC hGC, int clip_x_origin, int clip_y_origin)
{
int iRet;

  BEGIN_XCALL_DEBUG_WRAPPER
  iRet = XSetClipOrigin(hGC->display, hGC->gc, clip_x_origin, clip_y_origin);
  END_XCALL_DEBUG_WRAPPER

  hGC->values.clip_x_origin = clip_x_origin;
  hGC->values.clip_y_origin = clip_y_origin;

  return iRet;
}

int WBSetClipMask(WBGC hGC, Pixmap pixmap)
{
int iRet;

  BEGIN_XCALL_DEBUG_WRAPPER
  iRet = XSetClipMask(hGC->display, hGC->gc, pixmap);
  END_XCALL_DEBUG_WRAPPER

  hGC->values.clip_mask = PXM_CopyPixmap(hGC->display, hGC->dw, pixmap); // TODO:  do I make a copy or just use this as-is ?

  if(hGC->clip_rgn)
  {
    BEGIN_XCALL_DEBUG_WRAPPER
    XDestroyRegion(hGC->clip_rgn);
    END_XCALL_DEBUG_WRAPPER

    hGC->clip_rgn = None;
  }

  if(hGC->clip_image)
  {
    WBXDestroyImage(hGC->clip_image);
    hGC->clip_image = NULL;
  }

  return iRet;
}

int WBSetFunction(WBGC hGC, int function)
{
int iRet;

  BEGIN_XCALL_DEBUG_WRAPPER
  iRet = XSetFunction(hGC->display, hGC->gc, function);
  END_XCALL_DEBUG_WRAPPER

  hGC->values.function = function;

  return iRet;
}

int WBSetForeground(WBGC hGC, unsigned long foreground)
{
int iRet;

  BEGIN_XCALL_DEBUG_WRAPPER
  iRet = XSetForeground(hGC->display, hGC->gc, foreground);
  END_XCALL_DEBUG_WRAPPER

  hGC->values.foreground = foreground;

  return iRet;
}

int WBSetBackground(WBGC hGC, unsigned long background)
{
int iRet;

  BEGIN_XCALL_DEBUG_WRAPPER
  iRet = XSetBackground(hGC->display, hGC->gc, background);
  END_XCALL_DEBUG_WRAPPER

  hGC->values.background = background;

  return iRet;
}

int WBSetFont(WBGC hGC, WB_FONTC pFont)
{
int iRet = 0;
WB_FONT pNewFont;


  if(pFont)
  {
    pNewFont = WBCopyFont(hGC->display, pFont);

    if(!pNewFont) // copy failed
    {
      return -1;
    }
  }
  else
  {
    pNewFont = NULL;
  }

  iRet = WBSetFontNoCopy(hGC, pNewFont);
  if(iRet && pNewFont) // an error, no assignment made
  {
    WBFreeFont(hGC->display, pNewFont); // free the copy
  }

  return iRet;
}

int WBSetFontNoCopy(WBGC hGC, WB_FONT pFont)
{
int iRet = 0;


  if(pFont && pFont->pFontStruct &&
     pFont->pFontStruct->fid != hGC->values.font)
  {
    BEGIN_XCALL_DEBUG_WRAPPER
    iRet = XSetFont(hGC->display, hGC->gc, pFont->pFontStruct->fid);
    END_XCALL_DEBUG_WRAPPER

    hGC->values.font = pFont->pFontStruct->fid; // updated (TODO set mask as well?)
  }

  if(hGC->pFont)
  {
    WBFreeFont(hGC->display, hGC->pFont);
    hGC->pFont = NULL;
  }

#warning verify that XSetFont returns a zero on success and then enable this check
  if(/*!iRet &&*/ pFont)
  {
    hGC->pFont = pFont;

#warning verify that XSetFont returns a zero on success and then remove this next line
    iRet = 0;
  }

  return iRet;
}

int WBSetLineAttributes(WBGC hGC, unsigned int line_width,
                        int line_style, int cap_style, int join_style)
{
int iRet;

  BEGIN_XCALL_DEBUG_WRAPPER
  iRet = XSetLineAttributes(hGC->display, hGC->gc, line_width, line_style, cap_style, join_style);
  END_XCALL_DEBUG_WRAPPER

  hGC->values.line_width = line_width;
  hGC->values.line_style = line_style;
  hGC->values.cap_style  = cap_style;
  hGC->values.join_style = join_style;

  return iRet;
}

int WBSetDashes(WBGC hGC, int dash_offset, const char dash_list[], int n)
{
int iRet;
char *pDashList;
char c1 = 0;

  if(n > 0)
  {
    pDashList = WBAlloc(n + 1);

    if(!pDashList)
    {
      return -1;
    }

    memcpy(pDashList, dash_list, n);
  }
  else
  {
    pDashList = &c1;
  }

  BEGIN_XCALL_DEBUG_WRAPPER
  iRet = XSetDashes(hGC->display, hGC->gc, dash_offset, pDashList, n);
  END_XCALL_DEBUG_WRAPPER

  if(pDashList != &c1)
  {
    WBFree(pDashList);
  }

  // TODO:  cache the value, remove previous cached values

  return iRet;
}

