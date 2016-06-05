/////////////////////////////////////////////////////////////////////////////////////////////
//          _                                     _            _                           //
//   _ __  (_)__  __ _ __ ___    __ _  _ __      | |__    ___ | | _ __    ___  _ __  ___   //
//  | '_ \ | |\ \/ /| '_ ` _ \  / _` || '_ \     | '_ \  / _ \| || '_ \  / _ \| '__|/ __|  //
//  | |_) || | >  < | | | | | || (_| || |_) |    | | | ||  __/| || |_) ||  __/| | _| (__   //
//  | .__/ |_|/_/\_\|_| |_| |_| \__,_|| .__/_____|_| |_| \___||_|| .__/  \___||_|(_)\___|  //
//  |_|                               |_|  |_____|               |_|                       //
//                                                                                         //
//             pixmap and icon helpers (ultimately providing universal support)            //
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


#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <strings.h>

// application header file
#include "pixmap_helper.h" // this will include platform_helper.h and window_helper.h

// include pixmap data

#include "icon_ok.xpm"
#include "icon_stop.xpm"
#include "icon_warn.xpm"
#include "icon_what.xpm"
#include "icon_splat.xpm"

// other pixmaps (some not entirely work safe)
#include "icon_bang.xpm"
#include "icon_barney.xpm"
#include "icon_bear.xpm"
#include "icon_death.xpm"
#include "icon_finger.xpm"
#include "icon_skull.xpm"
#include "icon_triangle.xpm"
#include "icon_what_bold.xpm"
#include "icon_wtf.xpm"
#include "icon_thumbup.xpm"
#include "icon_thumbdown.xpm"



#ifdef NODEBUG
#define DEBUG_DUMP_XPM_ATTRIBUTES(X) 
#else
static void DebugDumpXpmAttributes(const char *szFunction, int nLine, XPM_ATTRIBUTES *pAttr);
#define DEBUG_DUMP_XPM_ATTRIBUTES(X) DebugDumpXpmAttributes(__FUNCTION__, __LINE__, X)
#endif // NODEBUG


#define MINIMUM_ATOM_RESOURCE_LIST_SIZE 256

typedef struct __INTERNAL_ATOM_RESOURCE_LIST__
{
  Atom aAtom;
  char **ppResource;
} INTERNAL_ATOM_RESOURCE_LIST;

static INTERNAL_ATOM_RESOURCE_LIST *pAtomResourceList = NULL;
static int nAtomResourceList = 0, nAtomResourceListMax = 0;

static char **ppRegAppLarge_Internal = NULL;
static char **ppRegAppSmall_Internal = NULL;


void PXM_RegisterAppIcons(char *ppRegAppLarge[], char *ppRegAppSmall[])
{
  ppRegAppLarge_Internal = ppRegAppLarge;
  ppRegAppSmall_Internal = ppRegAppSmall;
}

static char **GetPreDefinedIconResource(int idIcon)
{
char **pData = NULL;

  switch(idIcon)
  {
    case ID_APPLICATION:
      if(ppRegAppSmall_Internal)
      {
        pData = ppRegAppSmall_Internal;
      }
      else
      {
        pData = icon_ok_xpm; // TODO:  make a 19x19 version
      }
      break;
    case ID_ICON_APP:
      if(ppRegAppLarge_Internal)
      {
        pData = ppRegAppLarge_Internal;
      }
      else
      {
        pData = icon_ok_xpm;
      }
      break;
    case ID_ICON_OK:
      pData = icon_ok_xpm;
      break;
    case ID_ICON_STOP:
      pData = icon_stop_xpm;
      break;
    case ID_ICON_WARN:
      pData = icon_warn_xpm;
      break;
    case ID_ICON_WHAT:
      pData = icon_what_xpm;
      break;
    case ID_ICON_SPLAT:
      pData = icon_splat_xpm;
      break;
    case ID_ICON_BANG:
      pData = icon_bang_xpm;
      break;
    case ID_ICON_TRIANGLE:
      pData = icon_triangle_xpm;
      break;
    case ID_ICON_WHAT_BOLD:
      pData = icon_what_bold_xpm;
      break;
    case ID_ICON_WTF:
      pData = icon_wtf_xpm;
      break;
    case ID_ICON_DEATH:
      pData = icon_death_xpm;
      break;
    case ID_ICON_FINGER:
      pData = icon_finger_xpm;
      break;
    case ID_ICON_SKULL:
      pData = icon_skull_xpm;
      break;
    case ID_ICON_THUMBUP:
      pData = icon_thumbup_xpm;
      break;
    case ID_ICON_THUMBDOWN:
      pData = icon_thumbdown_xpm;
      break;
    case ID_ICON_BEAR:
      pData = icon_bear_xpm;
      break;
    case ID_ICON_BARNEY:  // this is a joke.  really.
      pData = icon_barney_xpm;
      break;
  }

  return pData;
}

static void RegisterIconResource(Atom aIcon, char **ppResource)
{
int i1;

  if(!pAtomResourceList)
  {
    pAtomResourceList = (INTERNAL_ATOM_RESOURCE_LIST *)
      malloc(MINIMUM_ATOM_RESOURCE_LIST_SIZE * sizeof(*pAtomResourceList));

    if(!pAtomResourceList)
    {
      WB_ERROR_PRINT("%s - not enough memoory for atom resource list\n", __FUNCTION__);
      return;
    }

    nAtomResourceList = 0;
    nAtomResourceListMax = MINIMUM_ATOM_RESOURCE_LIST_SIZE;
  }

  for(i1=0; i1 < nAtomResourceList; i1++)
  {
    if(pAtomResourceList[i1].aAtom == aIcon) // already there?
    {
      WB_ERROR_PRINT("%s - matching atom already in the list (ignoring)\n", __FUNCTION__);
      return;  // fow now I just leave.  later I might allow editing
    }
  }

  if((nAtomResourceList + 1) >= nAtomResourceListMax)
  {
    int iNewSize = MINIMUM_ATOM_RESOURCE_LIST_SIZE / 2 + nAtomResourceListMax;

    void *pTemp = realloc(pAtomResourceList, iNewSize);
    if(!pTemp)
    {
      WB_ERROR_PRINT("%s - not enough memoory for atom resource list re-alloc\n", __FUNCTION__);
      return;
    }

    pAtomResourceList = (INTERNAL_ATOM_RESOURCE_LIST *)pTemp;
    nAtomResourceListMax += MINIMUM_ATOM_RESOURCE_LIST_SIZE / 2;
  }

  pAtomResourceList[nAtomResourceList].aAtom = aIcon;
  pAtomResourceList[nAtomResourceList].ppResource = ppResource;
  nAtomResourceList++;
}

static const char * const szPreDefinedIconResources[] =
{
  "ID_APPLICATION",
  "ID_ICON_OK",
  "ID_ICON_STOP",
  "ID_ICON_WARN",
  "ID_ICON_WHAT",
  "ID_ICON_QUESTION",
  "ID_ICON_SPLAT",
  "ID_ICON_ASTERISK",
  "ID_ICON_BANG",
  "ID_ICON_TRIANGLE",
  "ID_ICON_WHAT_BOLD",
  "ID_ICON_WTF",
  "ID_ICON_DEATH",
  "ID_ICON_FINGER",
  "ID_ICON_SKULL",
  "ID_ICON_THUMBUP",
  "ID_ICON_THUMBDOWN",
  "ID_ICON_BEAR",
  "ID_ICON_BARNEY",
  "ID_ICON_APP"
};

static char **GetRegisteredIconResource(Atom aIcon)
{
static int iHasBeenRegistered = 0;
int i1, i2;

  // if the pre-defined atoms have not yet been registered, do it NOW

  if(!iHasBeenRegistered)
  {
    for(i1=ID_ICON_FIRST, i2=0;
        i1 <= ID_ICON_LAST
         && i2 < sizeof(szPreDefinedIconResources)/sizeof(szPreDefinedIconResources[0]);
        i1++, i2++)
    {
      Atom aTemp = XInternAtom(WBGetDefaultDisplay(), szPreDefinedIconResources[i2], True); // create or lookup

      if(aTemp != None)
      {
        RegisterIconResource(aTemp, GetPreDefinedIconResource(i1));
      }
    }
  }

  for(i1=0; i1 < nAtomResourceList; i1++)
  {
    if(pAtomResourceList[i1].aAtom == aIcon) // already there?
    {
      return pAtomResourceList[i1].ppResource;
    }
  }

  return NULL;
}



Pixmap PXM_GetIconPixmap(int idIcon, XPM_ATTRIBUTES *pAttr, Pixmap *pMask)
{
char **pData;


  pData = GetPreDefinedIconResource(idIcon);

  if(!pData)
  {
    if(pAttr)
    {
      bzero(pAttr, sizeof(*pAttr));
    }

    if(pMask)
    {
      *pMask = None;
    }

    return None;
  }

  return PXM_LoadPixmap(pData, pAttr, pMask);
}


Pixmap PXM_GetIconPixmapFromAtom(Atom aIcon, XPM_ATTRIBUTES *pAttr, Pixmap *pMask /* = NULL*/)
{
char **pData;


  pData = GetRegisteredIconResource(aIcon);

  if(!pData)
  {
    if(pAttr)
    {
      bzero(pAttr, sizeof(*pAttr));
    }

    if(pMask)
    {
      *pMask = None;
    }

    return None;
  }

  return PXM_LoadPixmap(pData, pAttr, pMask);
}


Pixmap PXM_LoadPixmap(char *ppXPM[], XPM_ATTRIBUTES *pAttr, Pixmap *pMask /* = NULL*/)
{
Pixmap pixRval = None, pixRval2 = None;
XPM_ATTRIBUTES xattr;
//#ifndef NODEBUG
//WB_UINT64 ullTime = WBGetTimeIndex();
//#endif // NODEBUG


  if(!ppXPM)
  {
    if(pAttr)
    {
      bzero(pAttr, sizeof(*pAttr));
    }

    if(pMask)
    {
      *pMask = None;
    }

    return None;
  }

  bzero(&xattr, sizeof(xattr));

//  WB_ERROR_PRINT("TEMPORARY:  %s line %d  delta tick %lld\n", __FUNCTION__, __LINE__, (WBGetTimeIndex() - ullTime));

  BEGIN_XCALL_DEBUG_WRAPPER
  XPM_CREATE_PIXMAP_FROM_DATA(WBGetDefaultDisplay(), WBGetHiddenHelperWindow(),
                              ppXPM, &pixRval, &pixRval2, &xattr);
  END_XCALL_DEBUG_WRAPPER

//  WB_ERROR_PRINT("TEMPORARY:  %s line %d  delta tick %lld\n", __FUNCTION__, __LINE__, (WBGetTimeIndex() - ullTime));

  if(pAttr)
  {
    memcpy(pAttr, &xattr, sizeof(xattr));
  }
  else
  {
    WB_IF_DEBUG_LEVEL(DebugLevel_Light | DebugSubSystem_Pixmap)
    {
      DEBUG_DUMP_XPM_ATTRIBUTES(&xattr);
    }

    XPM_FREE_ATTRIBUTES(&xattr);
  }

  if(pMask)
  {
    *pMask = pixRval2;
  }
  else if(pixRval2 != None) // free pixRval2 if it was allocated (it's the 'mask')
  {
    BEGIN_XCALL_DEBUG_WRAPPER
    XFreePixmap(WBGetDefaultDisplay(), pixRval2);
    END_XCALL_DEBUG_WRAPPER
  }

  return(pixRval);
}

Pixmap PXM_ImageToPixmap(Display *pDisplay, Drawable dw, XImage *pImage,
                         unsigned long clrFGPixel, unsigned long clrBGPixel)
{
Pixmap pxRval;
GC gc;
XGCValues gcv;
int iW, iH;

  if(!pDisplay)
  {
    pDisplay = WBGetDefaultDisplay();
  }

  if(!pImage)
  {
    WB_ERROR_PRINT("%s - pImage is NULL\n", __FUNCTION__);
    return None;
  }

  iW = pImage->width;
  iH = pImage->height;

  if(!iW || !iH)
  {
    WB_ERROR_PRINT("%s - Image width/height not valid, iW=%d,iH=%d\n", __FUNCTION__, iW, iH);
    return None;
  }

  BEGIN_XCALL_DEBUG_WRAPPER
  pxRval = XCreatePixmap(pDisplay, dw, iW, iH, DefaultDepth(pDisplay, DefaultScreen(pDisplay)));
  END_XCALL_DEBUG_WRAPPER

  if(pxRval == None)
  {
    WB_ERROR_PRINT("%s - XCreatePixmap failed\n", __FUNCTION__);
    return None;
  }

  // I will need to create a GC.  Make it a simple one that only specifies FG and BG

  gcv.foreground = clrFGPixel;//BlackPixel(pDisplay, DefaultScreen(pDisplay));
  gcv.background = clrBGPixel;//WhitePixel(pDisplay, DefaultScreen(pDisplay));

  // NOTE:  for monochrome masks I'd likely use '1' for foreground, '0' for background

  BEGIN_XCALL_DEBUG_WRAPPER
  gc = XCreateGC(pDisplay, dw, (GCForeground | GCBackground), &gcv);
  END_XCALL_DEBUG_WRAPPER

  if(gc == None)
  {
    WB_ERROR_PRINT("%s - XCreateGC failed\n", __FUNCTION__);

    BEGIN_XCALL_DEBUG_WRAPPER
    XFreePixmap(pDisplay, pxRval);
    END_XCALL_DEBUG_WRAPPER
    return None;
  }
  else
  {
    BEGIN_XCALL_DEBUG_WRAPPER
    XPutImage(pDisplay, pxRval, gc, pImage, 0, 0, 0, 0, iW, iH); // and now I have a copy of it

    XFreeGC(pDisplay, gc);
    END_XCALL_DEBUG_WRAPPER
  }

  return pxRval;
}


Pixmap PXM_ImageToPixmap0(Display *pDisplay, Drawable dw, XImage *pImage)
{
  if(!pDisplay)
  {
    pDisplay = WBGetDefaultDisplay();
  }

  return PXM_ImageToPixmap(pDisplay, dw, pImage,
                           BlackPixel(pDisplay, DefaultScreen(pDisplay)),
                           WhitePixel(pDisplay, DefaultScreen(pDisplay)));
}


XImage *PXM_PixmapToImage(Display *pDisplay, Pixmap pxImage)
{
XImage *pRval;
int iX=0, iY=0;
Window winRoot; // not used, still needed?
unsigned int iWidth=0, iHeight=0, iBorder;
unsigned int uiDepth = 0;


  if(!pDisplay)
  {
    pDisplay = WBGetDefaultDisplay();
  }

  // TODO:  special handling for pxImage == None ???

  if(pxImage == None)
  {
    return NULL;
  }

  BEGIN_XCALL_DEBUG_WRAPPER

  // use XGetGeometry to obtain the characteristics of the pixmap.  iX and iY SHOULD be zero...
  XGetGeometry(pDisplay, pxImage, &winRoot, &iX, &iY, &iWidth, &iHeight, &iBorder, &uiDepth);

  if(!iWidth || !iHeight)
  {
    pRval = NULL;
  }
  else
  {
    // TODO:  do I still need iX and iY?
    pRval = XGetImage(pDisplay, pxImage, 0, 0, iWidth, iHeight, -1L, ZPixmap);
  }

  END_XCALL_DEBUG_WRAPPER

  return pRval;
}




void PXM_OnExit(void)
{
  if(pAtomResourceList)
  {
    free(pAtomResourceList);
    pAtomResourceList = NULL;
  }

  nAtomResourceList = 0;
  nAtomResourceListMax = 0;

  ppRegAppLarge_Internal = NULL;
  ppRegAppSmall_Internal = NULL;
}



#ifndef NODEBUG
static void DebugDumpXpmAttributes(const char *szFunction, int nLine, XPM_ATTRIBUTES *pAttr)
{
  WBDebugPrint("%s line %d XPM_ATTRIBUTES contain:\n", szFunction, nLine);
#if defined(HAVE_XPM)
  WBDebugPrint("  valuemask:          %ld\n", pAttr->valuemask);
  WBDebugPrint("  visual:             %p\n", pAttr->visual);
  WBDebugPrint("  colormap:           %p\n", (void *)pAttr->colormap);
#endif // defined(HAVE_XPM)

  WBDebugPrint("  depth:              %u\n", pAttr->depth);
  WBDebugPrint("  width:              %u\n", pAttr->width);
  WBDebugPrint("  height:             %u\n", pAttr->height);

#if defined(HAVE_XPM)
  WBDebugPrint("  x_hotspot:          %u\n", pAttr->x_hotspot);
  WBDebugPrint("  y_hotspot:          %u\n", pAttr->y_hotspot);
  WBDebugPrint("  cpp:                %u\n", pAttr->cpp);
  WBDebugPrint("  pixels:             %p\n", pAttr->pixels);
  WBDebugPrint("  npixels:            %u\n", pAttr->npixels);
  WBDebugPrint("  colorsymbols:       %p\n", pAttr->colorsymbols);
  WBDebugPrint("  numsymbols:         %u\n", pAttr->numsymbols);
  WBDebugPrint("  rgb_fname:          %s\n", pAttr->rgb_fname);
  WBDebugPrint("  nextensions:        %u\n", pAttr->nextensions);
  WBDebugPrint("  extensions:         %p\n", pAttr->extensions);
  WBDebugPrint("  ncolors:            %u\n", pAttr->ncolors);
  WBDebugPrint("  colorTable:         %p\n", pAttr->colorTable);
  WBDebugPrint("  mask_pixel:         %u\n", pAttr->mask_pixel);
  WBDebugPrint("  exactColors:        %c\n", pAttr->exactColors ? 'T' : 'F');
  WBDebugPrint("  closeness:          %u\n", pAttr->closeness);
  WBDebugPrint("  red_closeness:      %u\n", pAttr->red_closeness);
  WBDebugPrint("  green_closeness:    %u\n", pAttr->green_closeness);
  WBDebugPrint("  blue_closeness:     %u\n", pAttr->blue_closeness);
  WBDebugPrint("  color_key:          %d\n", pAttr->color_key);
  WBDebugPrint("  alloc_pixels:       %p\n", pAttr->alloc_pixels);
  WBDebugPrint("  nalloc_pixels:      %d\n", pAttr->nalloc_pixels);
  WBDebugPrint("  alloc_close_colors: %c\n", pAttr->alloc_close_colors ? 'T' : 'F');
  WBDebugPrint("  bitmap_format:      %d\n", pAttr->bitmap_format);
  WBDebugPrint("  alloc_color:        %p\n", pAttr->alloc_color);
  WBDebugPrint("  free_colors:        %p\n", pAttr->free_colors);
  WBDebugPrint("  color_closure:      %p\n", pAttr->color_closure);
#endif // defined(HAVE_XPM)
}
#endif // !NODEBUG

