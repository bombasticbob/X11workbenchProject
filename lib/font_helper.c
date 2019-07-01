//////////////////////////////////////////////////////////////////////////////////////////
//          __                _         _            _                                  //
//         / _|  ___   _ __  | |_      | |__    ___ | | _ __    ___  _ __  ___          //
//        | |_  / _ \ | '_ \ | __|     | '_ \  / _ \| || '_ \  / _ \| '__|/ __|         //
//        |  _|| (_) || | | || |_      | | | ||  __/| || |_) ||  __/| | _| (__          //
//        |_|   \___/ |_| |_| \__|_____|_| |_| \___||_|| .__/  \___||_|(_)\___|         //
//                               |_____|               |_|                              //
//                       basic font enumeration and selection                           //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////

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

/** \file font_helper.c
  * \brief Implementation file for font helper functions
  *
  * This is the implementation file for font helper functions
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

//#include <locale.h>

#include "window_helper.h" // for debug output; also includes platform.h and font_helper.h
#include "draw_text.h"

#define FONT_DUMP_DEBUG_LEVEL DebugLevel_Heavy


// globals

static int bDisableAntiAlias = 0;
static int bEnableTrueTypeFonts = 0;

// libXft aka Freetype -
#ifdef X11WORKBENCH_TOOLKIT_HAVE_XFT
FT_Library __ftlib = NULL; /* located in font_helper.c */
static int bInitFtLibOnce = 0;
#endif // X11WORKBENCH_TOOLKIT_HAVE_XFT


void __internal_disable_antialias(void)
{
  bDisableAntiAlias = 1;
}

void __internal_enable_antialias(void)
{
  bDisableAntiAlias = 0;
}

void __internal_font_helper_init(void)
{
#ifdef X11WORKBENCH_TOOLKIT_HAVE_XFT

  // see https://www.freedesktop.org/wiki/Software/Xft/
  // and https://cgit.freedesktop.org/xorg/lib/libXft/
  // this software requires libXft2 and is not compatible
  // with any earlier version...

#if XFT_VERSION < 20000
#warning - You need libXft version 2 or greater - true type fonts disabled
  bInitFtLibOnce = 0;
  bEnableTrueTypeFonts = 0;
#else

  if(!bInitFtLibOnce)
  {
    // this must only be called once...
    if(XftInit(0) == FcTrue && XftInitFtLibrary() == FcTrue )
    {
      bInitFtLibOnce = 1;

      if(XftDefaultHasRender(WBGetDefaultDisplay()))
      {
        bEnableTrueTypeFonts = 1;
      }
      else
      {
        WB_ERROR_PRINT("Unable to render true type fonts, disabled\n");
      }

//      __ftlib = _XftFTlibrary; // the libXft cached value (this does not actually work)
      if(FT_Init_FreeType(&__ftlib))
      {
        __ftlib = NULL;
        WB_WARN_PRINT("Unable to get own copy of FreeType library, disabled\n");
      }
    }
    else
    {
      WB_ERROR_PRINT("Unable to initialize libXft - truetype fonts not enabled\n");
    }
  }
#endif // XFT_VERSION < 20000
#endif // X11WORKBENCH_TOOLKIT_HAVE_XFT

  // TODO:  other global initialization things for font_helper
}

void __internal_font_helper_exit(void)
{
#ifdef X11WORKBENCH_TOOLKIT_HAVE_XFT
  if(bInitFtLibOnce)
  {
    // TODO:  any uninitialization goes here.  for now there is none.

    bInitFtLibOnce = 0;
  }
#endif // X11WORKBENCH_TOOLKIT_HAVE_XFT
}

int WBFontEnableAntiAlias(void)
{
  return !bDisableAntiAlias; // for now, just do this
}

void WBFontSetEnableAntiAlias(int bEnable)
{
  bDisableAntiAlias = bEnable ? 1 : 0;
}


WB_FONT WBCopyFont(Display *pDisplay, WB_FONTC pOldFont)
{
WB_FONT pRval;

  if(!pDisplay)
  {
    pDisplay = WBGetDefaultDisplay();
  }

  if(!pOldFont)
  {
    pOldFont = WBGetDefaultFont();

    if(!pOldFont)
    {
      WB_WARN_PRINT("%s returns NULL (WBGetDefaultFont returns NULL)\n", __FUNCTION__);

      return NULL;
    }
  }

  pRval = (WB_FONT)WBAlloc(sizeof(*pRval));
  if(pRval)
  {
    memset(pRval, 0, sizeof(*pRval));

    pRval->pDisplay = pDisplay;
    pRval->iAscent = pRval->iDescent = pRval->iHeight = pRval->iAvgCharWidth = pRval->iMaxCharWidth = -1;

#ifdef X11WORKBENCH_TOOLKIT_HAVE_XFT
    if(pOldFont->pxftFont)
    {
      pRval->pxftFont = XftFontCopy(pDisplay, pOldFont->pxftFont);
      if(!pRval->pxftFont)
      {
        goto bad_font;
      }
//      pRval->pxftFontInfo = copy something from old one
    }
    else
#endif // X11WORKBENCH_TOOLKIT_HAVE_XFT
    {
      if(pOldFont->fsFont != None)
      {
        pRval->fsFont = WBCopyModifyFontSet(pDisplay, pOldFont->fsFont, 0, 0);

        if(pRval->fsFont == None)
        {
          WB_WARN_PRINT("%s returns NULL (unable to copy font set)\n", __FUNCTION__);

          goto bad_font;
        }
      }
      /*else*/ if(pOldFont->pFontStruct)
      {
        pRval->pFontStruct = WBCopyFontX(pOldFont->pFontStruct);
        if(!pRval->pFontStruct)
        {
          WB_WARN_PRINT("%s returns NULL (unable to copy font struct)\n", __FUNCTION__);

          goto bad_font;
        }
      }

      if(!pOldFont->pFontStruct && pOldFont->fsFont == None)
      {
        WB_WARN_PRINT("%s returns NULL (bad font)\n", __FUNCTION__);
bad_font:
        WBFreeFont(pDisplay, pRval);

        return NULL;
      }
    }
  }
  else
  {
    WB_WARN_PRINT("%s returns NULL (no memory for structure)\n", __FUNCTION__);
  }

  return pRval;
}

void WBFreeFont(Display *pDisplay, WB_FONT pFont)
{
  if(!pDisplay)
  {
    pDisplay = WBGetDefaultDisplay();
  }

  if(pFont)
  {
#ifdef X11WORKBENCH_TOOLKIT_HAVE_XFT
    if(pFont->pxftFont)
    {
      XftFontClose(pDisplay, pFont->pxftFont);
      pFont->pxftFont = NULL;
    }
    if(pFont->pxftFontInfo != None)
    {
      XftFontInfoDestroy(pDisplay, pFont->pxftFontInfo);
      pFont->pxftFontInfo = NULL;
    }
#endif // X11WORKBENCH_TOOLKIT_HAVE_XFT

    if(pFont->fsFont != None)
    {
      XFreeFontSet(pDisplay, pFont->fsFont);
      pFont->fsFont = None;
    }

    if(pFont->pFontStruct)
    {
      XFreeFont(pDisplay, pFont->pFontStruct);
      // TODO:  should I call XUnloadFont at all? how about XFreeFontInfo ?

      pFont->pFontStruct = NULL; // help eliminate potential re-use in possible buggy code
    }

    WBFree(pFont);
  }
}


WB_FONT WBLoadFont(Display *pDisplay, const char *szFontName,
                   int iFontSize, int iFlags)
{
WB_FONT pRval;

  if(!pDisplay)
  {
    pDisplay = WBGetDefaultDisplay();
  }

  pRval = (WB_FONT)WBAlloc(sizeof(*pRval));
  if(pRval)
  {
    memset(pRval, 0, sizeof(*pRval));

    pRval->pDisplay = pDisplay; // cache it
    pRval->iAscent = pRval->iDescent = pRval->iHeight = pRval->iAvgCharWidth = pRval->iMaxCharWidth = -1;

#ifdef X11WORKBENCH_TOOLKIT_HAVE_XFT

#warning TODO: IMPLEMENT THIS PART for Xft FONTS

//    if(!pRval->pxftFont)
#endif // X11WORKBENCH_TOOLKIT_HAVE_XFT
    {
      pRval->pFontStruct = WBLoadFontX(pDisplay, szFontName, iFontSize, iFlags);

      if(!pRval->pFontStruct)
      {
//bad_font:
        WBFree(pRval);

        WB_WARN_PRINT("%s returns NULL (WBLoadFontXl returns NULL)\n", __FUNCTION__);

        pRval = NULL;
      }
      else
      {
        pRval->fsFont = WBFontSetFromFont(pDisplay, pRval->pFontStruct);

        // TODO: check for error?
      }
    }
  }
  else
  {
    WB_WARN_PRINT("%s returns NULL (no memory for structure)\n", __FUNCTION__);
  }

  return pRval;
}

int WBFontAvgCharWidth(WB_FONTC pFont0)
{
int iRval = 0;
Display *pDisplay;
WB_FONT pFont = (WB_FONT)pFont0;


  if(!pFont || !pFont->pDisplay)
  {
    WB_WARN_PRINT("%s returns zero (bad param, %p, %p)\n",
                  __FUNCTION__, pFont, (void *)(pFont ? pFont->pDisplay : NULL));

    return 0;
  }

  pDisplay = pFont->pDisplay;

  if(pFont->iAvgCharWidth >= 0)
  {
    WB_DEBUG_PRINT(DebugLevel_Heavy | DebugSubSystem_Font,
                   "%s returns (cached) font avg char width %d\n", __FUNCTION__, pFont->iAvgCharWidth);
    return pFont->iAvgCharWidth;
  }

#ifdef X11WORKBENCH_TOOLKIT_HAVE_XFT
  if(pFont->pxftFont)
  {
    WB_ERROR_PRINT("ERROR:  %s font has 'pfxftFont' non-NULL\n", __FUNCTION__);
#warning TODO: IMPLEMENT THIS PART for Xft FONTS
  }
  else
#endif // X11WORKBENCH_TOOLKIT_HAVE_XFT
  {
    if(pFont->fsFont)
    {
      iRval = WBFontSetAvgCharWidth(pDisplay, pFont->fsFont);
    }
    else if(pFont->pFontStruct)
    {
      iRval = WBFontAvgCharWidthX(pDisplay, pFont->pFontStruct);
    }

    pFont->iAvgCharWidth = iRval;
  }

  WB_DEBUG_PRINT(DebugLevel_Heavy | DebugSubSystem_Font,
                 "%s returns font avg char width %d\n", __FUNCTION__, iRval);

  return iRval;
}

int WBFontMaxCharWidth(WB_FONTC pFont0)
{
int iRval = 0;
Display *pDisplay;
WB_FONT pFont = (WB_FONT)pFont0;


  if(!pFont || !pFont->pDisplay)
  {
    WB_WARN_PRINT("%s returns zero (bad param, %p, %p)\n",
                  __FUNCTION__, pFont, (void *)(pFont ? pFont->pDisplay : NULL));

    return 0;
  }

  pDisplay = pFont->pDisplay;

  if(pFont->iMaxCharWidth >= 0)
  {
    WB_DEBUG_PRINT(DebugLevel_Heavy | DebugSubSystem_Font,
                   "%s returns (cached) font max char width %d\n", __FUNCTION__, pFont->iMaxCharWidth);
    return pFont->iMaxCharWidth;
  }

#ifdef X11WORKBENCH_TOOLKIT_HAVE_XFT
  if(pFont->pxftFont)
  {
    WB_ERROR_PRINT("ERROR:  %s font has 'pfxftFont' non-NULL\n", __FUNCTION__);
#warning TODO: IMPLEMENT THIS PART for Xft FONTS
  }
  else
#endif // X11WORKBENCH_TOOLKIT_HAVE_XFT
  {
    XCharStruct xc = WBFontMaxBounds(pFont);

    iRval = xc.width;

    if(iRval > 0) // values > 0 assume it's right
    {
      pFont->iAvgCharWidth = iRval;
    }
  }

  WB_DEBUG_PRINT(DebugLevel_Heavy | DebugSubSystem_Font,
                 "%s returns font max char width %d\n", __FUNCTION__, iRval);

  return iRval;
}

int WBFontDescent(WB_FONTC pFont0)
{
int iRval = 0;
Display *pDisplay;
WB_FONT pFont = (WB_FONT)pFont0;


  if(!pFont || !pFont->pDisplay)
  {
    WB_WARN_PRINT("%s returns zero (bad param, %p, %p)\n",
                  __FUNCTION__, pFont, (void *)(pFont ? pFont->pDisplay : NULL));

    return 0;
  }

  pDisplay = pFont->pDisplay;

  if(pFont->iDescent >= 0)
  {
    WB_DEBUG_PRINT(DebugLevel_Heavy | DebugSubSystem_Font,
                   "%s returns (cached) font descent %d\n", __FUNCTION__, pFont->iDescent);
    return pFont->iDescent;
  }


#ifdef X11WORKBENCH_TOOLKIT_HAVE_XFT
  if(pFont->pxftFont)
  {
    WB_ERROR_PRINT("ERROR:  %s font has 'pfxftFont' non-NULL\n", __FUNCTION__);
#warning TODO: IMPLEMENT THIS PART for Xft FONTS
  }
  else
#endif // X11WORKBENCH_TOOLKIT_HAVE_XFT
  {
    if(pFont->fsFont)
    {
      iRval = WBFontSetDescent(pDisplay, pFont->fsFont);
    }
    else if(pFont->pFontStruct)
    {
      iRval = pFont->pFontStruct->descent;
    }

    pFont->iDescent = iRval; // cached for next time
  }

  WB_DEBUG_PRINT(DebugLevel_Heavy | DebugSubSystem_Font,
                 "%s returns font descent %d\n", __FUNCTION__, iRval);

  return iRval;
}

int WBFontAscent(WB_FONTC pFont0)
{
int iRval = 0;
Display *pDisplay;
WB_FONT pFont = (WB_FONT)pFont0;


  if(!pFont || !pFont->pDisplay)
  {
    WB_WARN_PRINT("%s returns zero (bad param, %p, %p)\n",
                  __FUNCTION__, pFont, (void *)(pFont ? pFont->pDisplay : NULL));

    return 0;
  }

  pDisplay = pFont->pDisplay;

  if(pFont->iAscent >= 0)
  {
    WB_DEBUG_PRINT(DebugLevel_Heavy | DebugSubSystem_Font,
                   "%s returns (cached) font ascent %d\n", __FUNCTION__, pFont->iAscent);
    return pFont->iAscent;
  }

#ifdef X11WORKBENCH_TOOLKIT_HAVE_XFT
  if(pFont->pxftFont)
  {
    WB_ERROR_PRINT("ERROR:  %s font has 'pfxftFont' non-NULL\n", __FUNCTION__);
#warning TODO: IMPLEMENT THIS PART for Xft FONTS
  }
  else
#endif // X11WORKBENCH_TOOLKIT_HAVE_XFT
  {
    if(pFont->fsFont)
    {
      iRval = WBFontSetAscent(pDisplay, pFont->fsFont);
    }
    else if(pFont->pFontStruct)
    {
      iRval = pFont->pFontStruct->ascent;
    }

    pFont->iAscent = iRval; // cached for next time
  }

  WB_DEBUG_PRINT(DebugLevel_Heavy | DebugSubSystem_Font,
                 "%s returns font ascent %d\n", __FUNCTION__, iRval);

  return iRval;
}

int WBFontHeight(WB_FONTC pFont0)
{
int iRval = 0;
Display *pDisplay;
WB_FONT pFont = (WB_FONT)pFont0;


  if(!pFont || !pFont->pDisplay)
  {
    WB_WARN_PRINT("%s returns zero (bad param, %p, %p)\n",
                  __FUNCTION__, pFont, (void *)(pFont ? pFont->pDisplay : NULL));

    return 0;
  }

  pDisplay = pFont->pDisplay;

  if(pFont->iHeight >= 0)
  {
    WB_DEBUG_PRINT(DebugLevel_Heavy | DebugSubSystem_Font,
                   "%s returns (cached) font height %d\n", __FUNCTION__, pFont->iHeight);
    return pFont->iHeight;
  }

#ifdef X11WORKBENCH_TOOLKIT_HAVE_XFT
  if(pFont->pxftFont)
  {
    WB_ERROR_PRINT("ERROR:  %s font has 'pfxftFont' non-NULL\n", __FUNCTION__);
#warning TODO: IMPLEMENT THIS PART for Xft FONTS
  }
  else
#endif // X11WORKBENCH_TOOLKIT_HAVE_XFT
  {
#warning review that this is the right way to handle this, or if there is a better way

    iRval = WBFontAscent(pFont0) + WBFontDescent(pFont0);

    pFont->iHeight = iRval; // cached for next time
  }

  WB_DEBUG_PRINT(DebugLevel_Heavy | DebugSubSystem_Font,
                 "%s returns font height %d\n", __FUNCTION__, iRval);

  return iRval;
}

XCharStruct WBFontMaxBounds(WB_FONTC pFont0)
{
XCharStruct rVal;
Display *pDisplay;
WB_FONT pFont = (WB_FONT)pFont0;


  memset(&rVal, 0, sizeof(rVal));

  if(!pFont || !pFont->pDisplay)
  {
    WB_WARN_PRINT("%s returns 'zero' struct (bad param, %p, %p)\n",
                  __FUNCTION__, pFont, (void *)(pFont ? pFont->pDisplay : NULL));

    return rVal; // return the zero'd structure if this happens
  }

  pDisplay = pFont->pDisplay;

  if(pFont->max_bounds.lbearing != 0 ||
     pFont->max_bounds.rbearing != 0 ||
     pFont->max_bounds.width != 0 ||
     pFont->max_bounds.ascent != 0 ||
     pFont->max_bounds.descent != 0)
  {
    WB_DEBUG_PRINT(DebugLevel_Heavy | DebugSubSystem_Font,
                   "%s returns (cached) font extent %d %d %d %d %d\n",
                   __FUNCTION__,
                   pFont->max_bounds.lbearing, pFont->max_bounds.rbearing,
                   pFont->max_bounds.width, pFont->max_bounds.ascent, pFont->max_bounds.descent);

    return pFont->max_bounds;
  }

#ifdef X11WORKBENCH_TOOLKIT_HAVE_XFT
  if(pFont->pxftFont)
  {
    WB_ERROR_PRINT("ERROR:  %s font has 'pfxftFont' non-NULL\n", __FUNCTION__);
#warning TODO: IMPLEMENT THIS PART for Xft FONTS
  }
  else
#endif // X11WORKBENCH_TOOLKIT_HAVE_XFT
  {
    if(pFont->fsFont)
    {
      rVal = WBFontSetMaxBounds(pDisplay, pFont->fsFont);
    }
    else if(pFont->pFontStruct)
    {
      rVal.lbearing = pFont->pFontStruct->max_bounds.lbearing;
      rVal.rbearing = pFont->pFontStruct->max_bounds.rbearing;
      rVal.width    = pFont->pFontStruct->max_bounds.width;
      rVal.ascent   = pFont->pFontStruct->max_bounds.ascent;
      rVal.descent  = pFont->pFontStruct->max_bounds.descent;

      memcpy(&rVal, &(pFont->pFontStruct->max_bounds), sizeof(rVal));
    }

    memcpy(&(pFont->max_bounds), &rVal, sizeof(pFont->max_bounds));
  }

  WB_DEBUG_PRINT(DebugLevel_Heavy | DebugSubSystem_Font,
                 "%s returns font extent %d %d %d %d %d\n",
                 __FUNCTION__, rVal.lbearing, rVal.rbearing, rVal.width, rVal.ascent, rVal.descent);

  return rVal;
}

WB_FONT WBCopyModifyFont(Display *pDisplay, WB_FONTC pOriginal,
                         int iFontSize, int iFlags)
{
WB_FONT pRval = NULL;


  if(!iFontSize && !iFlags)
  {
    return WBCopyFont(pDisplay, pOriginal);
  }

  if(!pDisplay)
  {
    pDisplay = WBGetDefaultDisplay();
  }

  if(!pOriginal)
  {
    pOriginal = WBGetDefaultFont();

    if(!pOriginal)
    {
      WB_WARN_PRINT("%s returns NULL (pOriginal is NULL)\n", __FUNCTION__);

      return NULL;
    }
  }

  pRval = (WB_FONT)WBAlloc(sizeof(*pRval));
  if(pRval)
  {
    memset(pRval, 0, sizeof(*pRval));

    pRval->pDisplay = pDisplay;
    pRval->iAscent = pRval->iDescent = pRval->iHeight = pRval->iAvgCharWidth = pRval->iMaxCharWidth = -1;

#ifdef X11WORKBENCH_TOOLKIT_HAVE_XFT

#warning TODO: IMPLEMENT THIS PART for Xft FONTS

//    if(!pRval->pxftFont)
#endif // X11WORKBENCH_TOOLKIT_HAVE_XFT
    {
      if(pOriginal->fsFont)
      {
        pRval->fsFont = WBCopyModifyFontSet(pDisplay, pOriginal->fsFont, iFontSize, iFlags);

        if(pRval->fsFont == None)
        {
          WB_WARN_PRINT("%s returns NULL (unable to copy font set)\n", __FUNCTION__);

          goto bad_font;
        }
      }
      /* else */ if(pOriginal->pFontStruct)
      {
        pRval->pFontStruct = WBLoadModifyFontX(pDisplay, pOriginal->pFontStruct, iFontSize, iFlags);

        // for now I make a copy of both of these when present
        // so only if both are NULL do I warn...
        if(pRval->fsFont == None && !pRval->pFontStruct)
        {
          WB_WARN_PRINT("%s returns NULL (unable to copy font struct)\n", __FUNCTION__);

          goto bad_font;
        }
      }

      if(!pOriginal->pFontStruct && pOriginal->fsFont == None)
      {
        WB_WARN_PRINT("%s returns NULL (bad font)\n", __FUNCTION__);
bad_font:
        WBFreeFont(pDisplay, pRval);

        return NULL;
      }
    }
  }
  else
  {
    WB_WARN_PRINT("%s returns NULL (not enough memory for structure)\n", __FUNCTION__);
  }

  return pRval;
}


int WBTextWidth(WB_FONTC pFont, const char *szText, int cbText)
{
int iLen, iRval = 0;


  if(!pFont || !cbText || !szText || (cbText < 0 && !*szText))
  {
    WB_ERROR_PRINT("TEMPORARY:  %s - returning zero (bad parameter %p %p %d)\n",
                    __FUNCTION__, pFont, szText, cbText);
    return 0;
  }
  else if(cbText < 0)
  {
    iLen = strlen(szText);
  }
  else
  {
    iLen = cbText;
  }

#ifdef X11WORKBENCH_TOOLKIT_HAVE_XFT
  if(pFont->pxftFont)
  {
    WB_ERROR_PRINT("ERROR:  %s font initialization has 'pfxftFont' non-NULL\n", __FUNCTION__);
#warning TODO: IMPLEMENT THIS PART for Xft FONTS
  }
  else
#endif // X11WORKBENCH_TOOLKIT_HAVE_XFT
  if(pFont->fsFont != None)
  {
    iRval = WB_TEXT_ESCAPEMENT(pFont->fsFont, szText, iLen);
  }
  else if(pFont->pFontStruct)
  {
    iRval = XTextWidth(pFont->pFontStruct, szText, iLen);
  }

  WB_DEBUG_PRINT(DebugLevel_Heavy | DebugSubSystem_Font,
                 "%s returns text width %d for \"%-.*s\"\n", __FUNCTION__, iRval, iLen, szText);

  return iRval;
}

void WBTextExtent(WB_FONTC pFont, const char *szText, int cbText, WB_EXTENT *pExtent)
{
int iLen;


  if(!pFont || !cbText || !szText || (cbText < 0 && !*szText))
  {
    WB_ERROR_PRINT("TEMPORARY:  %s - returning zero (bad parameter %p %p %d)\n",
                    __FUNCTION__, pFont, szText, cbText);
    return;
  }
  else if(cbText < 0)
  {
    iLen = strlen(szText);
  }
  else
  {
    iLen = cbText;
  }


#ifdef X11WORKBENCH_TOOLKIT_HAVE_XFT
  if(pFont->pxftFont)
  {
    WB_ERROR_PRINT("ERROR:  %s font initialization has 'pfxftFont' non-NULL\n", __FUNCTION__);
#warning TODO: IMPLEMENT THIS PART for Xft FONTS
  }
  else
#endif // X11WORKBENCH_TOOLKIT_HAVE_XFT
  if(pFont->fsFont != None)
  {
    XRectangle rct, rct2;

    memset(&rct2, 0, sizeof(rct2));

    BEGIN_XCALL_DEBUG_WRAPPER
    WB_TEXT_EXTENTS(pFont->fsFont, szText, iLen, &rct, &rct2);
    END_XCALL_DEBUG_WRAPPER

    pExtent->width = rct2.width;
    pExtent->height = rct2.height;
  }
  else if(pFont->pFontStruct)
  {
    int iDir, iAsc, iDesc;
    XCharStruct cs;

    memset(&cs, 0, sizeof(cs));

    BEGIN_XCALL_DEBUG_WRAPPER
    XTextExtents(pFont->pFontStruct, szText, iLen, &iDir, &iAsc, &iDesc, &cs);
    END_XCALL_DEBUG_WRAPPER

    pExtent->width = cs.width;
    pExtent->height = cs.ascent + cs.descent;
  }
}




