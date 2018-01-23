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
    Copyright (c) 2010-2018 by Bob Frazier (aka 'Big Bad Bombastic Bob')
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


#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <strings.h>
#include <math.h> // and now I'll need -lm

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



#ifdef NO_DEBUG
#define DEBUG_DUMP_XPM_ATTRIBUTES(X)
#define DEBUG_DUMP_COLORMAP(X)
#else
static void DebugDumpXpmAttributes(const char *szFunction, int nLine, XPM_ATTRIBUTES *pAttr);
static void DebugDumpColormap(const XStandardColormap *pMap);
#define DEBUG_DUMP_XPM_ATTRIBUTES(X) DebugDumpXpmAttributes(__FUNCTION__, __LINE__, X)
#define DEBUG_DUMP_COLORMAP(X) DebugDumpColormap(X)
#endif // NO_DEBUG


#define _PI_ 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679 /* approximately */


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

XStandardColormap PXM_StandardColormapFromColormap_rval; // storage for static var for PXM_StandardColormapFromColormap()

//-------------------------
// RGB and YUV conversions
//-------------------------

static int clip255(int iIn)
{
  if(iIn < 0)
  {
    return 0;
  }
  else if(iIn > 255)
  {
    return 255;
  }

  return iIn;
}

void PXM_RGBToYUV(int iR, int iG, int iB, int *piY, int *piU, int *piV)
{
int iY, iU, iV;

  iR = clip255(iR);
  iG = clip255(iG);
  iB = clip255(iB);

  iY = clip255(((  66 * iR + 129 * iG +  25 * iB + 128) >> 8) + 16);
  iU = clip255((( -38 * iR -  74 * iG + 112 * iB + 128) >> 8) + 128);
  iV = clip255((( 112 * iR -  94 * iG -  18 * iB + 128) >> 8) + 128);

  if(piY)
  {
    *piY = iY;
  }

  if(piU)
  {
    *piU = iU;
  }

  if(piV)
  {
    *piV = iV;
  }
}

void PXM_YUVToRGB(int iY, int iU, int iV, int *piR, int *piG, int *piB)
{
int iR, iG, iB;
int iC = iY - 16;
int iD = iU - 128;
int iE = iV - 128;

  iR = clip255(( 298 * iC            + 409 * iE + 128) >> 8);
  iG = clip255(( 298 * iC - 100 * iD - 208 * iE + 128) >> 8);
  iB = clip255(( 298 * iC + 516 * iD            + 128) >> 8);

  if(piR)
  {
    *piR = iR;
  }

  if(piG)
  {
    *piG = iG;
  }

  if(piB)
  {
    *piB = iB;
  }
}

void PXM_HSVToRGB(int iH, int iS, int iV, int *piR, int *piG, int *piB)
{
int iR, iG, iB;
double dH = iH * _PI_ * 2.0 / 256;

  iR = clip255((int)floor(iV * (1.0 + iS * (cos(dH) - 1.0))));
  iG = clip255((int)floor(iV * (1.0 + iS * (cos(dH - (_PI_ * 2.0 / 3.0)) - 1.0))));
  iB = clip255((int)floor(iV * (1.0 + iS * (cos(dH + (_PI_ * 2.0 / 3.0)) - 1.0))));

//    function hsv.torgb(h,s,v)
//        local r=v*(1+s*(cos(h)-1))
//        local g=v*(1+s*(cos(h-2.09439)-1))
//        local b=v*(1+s*(cos(h+2.09439)-1))
//        return r,g,b
//    end

  if(piR)
  {
    *piR = iR;
  }

  if(piG)
  {
    *piG = iG;
  }

  if(piB)
  {
    *piB = iB;
  }
}

void PXM_RGBToHSV(int iR, int iG, int iB, int *piH, int *piS, int *piV)
{
int iC = iR + iG + iB;
int iH=0, iS=0, iV=0;

  if(iC == 0) // black
  {
    iH = 0;
    iS = 512 / 3; // red?
    iV = 0;
  }
  else
  {
    int iP = 2*(iB * iB + iG * iG + iR * iR - iG * iR - iB * iG - iB * iR);

    if(iP < 0)
    {
      iP = 0; // added by bob, to prevent dividing by zero (evar)
    }
    iH = ((int)floor(256 * atan2(iB - iG, sqrt(fabs((2.0 * iR - iB - iG)/3.0))) / (_PI_ * 2.0)))
          & 255; // this is calculating an angle, 0 to 2PI, which should rotate 0-255
    iS = clip255(iP / (iC + iP));  // iC is never zero
    iV = clip255((iC + iP) / 3);

//    function hsv.fromrgb(r,b,g)
//        local c=r+g+b
//        if c<1e-4 then
//            return 0,2/3,0
//        else
//            local p=2*(b*b+g*g+r*r-g*r-b*g-b*r)^0.5
//            local h=atan2(b-g,(2*r-b-g)/3^0.5)
//            local s=p/(c+p)
//            local v=(c+p)/3
//            return h,s,v
//        end
//    end
  }

  if(piH)
  {
    *piH = iH;
  }

  if(piS)
  {
    *piS = iS;
  }

  if(piV)
  {
    *piV = iV;
  }
}


void PXM_PixelToRGB(XStandardColormap *pMap, XColor *pColor)
{
unsigned long long lColor, l2;
XStandardColormap map;


  if(!pColor)
  {
    return;
  }

  if(!pMap)
  {
    WBDefaultStandardColormap(WBGetDefaultDisplay(), &map);
    pMap = &map;

//    DEBUG_DUMP_COLORMAP(pMap);
  }

  // 'nuking' this one out is a bit difficult.  I have to sort the values properly

  lColor = pColor->pixel - pMap->base_pixel;

  if(!pMap->red_mult && !pMap->green_mult && !pMap->blue_mult)
  {
    return;
  }

  pColor->flags = DoRed | DoGreen | DoBlue; // pre-assign this, re-assign as needed

  if(pMap->red_mult >= pMap->green_mult &&
     pMap->red_mult >= pMap->blue_mult) // this includes monochrome I think
  {
    if(!pMap->green_mult && !pMap->blue_mult)
    {
      // it's monochrome, so I only use red

      if(pMap->red_mult == 1)
      {
        l2 = ((unsigned long long)65535 * lColor);
      }
      else
      {
        l2 = ((unsigned long long)65535 * lColor) / pMap->red_mult;
      }

      if(l2 >= 65536 * pMap->red_max)
      {
        l2 = 65535;
      }
//      else if(lColor < 0)
//      {
//        l2 = 0;
//      }
      else
      {
        l2 /= pMap->red_max;
      }

      pColor->red = l2;
      pColor->green = l2;
      pColor->blue = l2; // since it's monochrome, though only 'DoRed' is assigned

      pColor->flags = DoRed; // usually indicates 'monochrome'
    }
    else if(pMap->green_mult > pMap->blue_mult)
    {
      // red first

      l2 = ((unsigned long long)65535 * lColor) / pMap->red_mult;
      lColor %= pMap->red_mult;


      if(l2 >= 65536 * pMap->red_max)
      {
        l2 = 65535 * pMap->red_max;
      }
//      else if(lColor < 0) // won't happen
//      {
//        l2 = 0;
//      }
      else
      {
        l2 /= pMap->red_max;
      }

      pColor->red = l2;

      // then green

      l2 = ((unsigned long long)65535 * lColor) / pMap->green_mult;
      lColor %= pMap->green_mult;

      if(l2 >= 65536 * pMap->green_max)
      {
        l2 = 65535 * pMap->green_max;
      }
//      else if(lColor < 0) // won't happen
//      {
//        l2 = 0;
//      }
      else
      {
        l2 /= pMap->green_max;
      }

      pColor->green = l2;

      // and then, blue

      if(!pMap->blue_mult)
      {
        l2 = 0;
      }
      else
      {
        l2 = ((unsigned long long)65535 * lColor) / pMap->blue_mult;

        if(l2 >= 65536 * pMap->blue_max)
        {
          l2 = 65535 * pMap->blue_max;
        }
//        else if(lColor < 0)
//        {
//          l2 = 0;
//        }
        else
        {
          l2 /= pMap->blue_max;
        }
      }
      pColor->blue = l2;
    }
    else // if(pMap->blue_mult > pMap->green_mult)
    {
      // red first

      l2 = ((unsigned long long)65535 * lColor) / pMap->red_mult;
      lColor %= pMap->red_mult;

      if(l2 >= 65536 * pMap->red_max)
      {
        l2 = 65535 * pMap->red_max;
      }
//      else if(lColor < 0)
//      {
//        l2 = 0;
//      }
      else
      {
        l2 /= pMap->red_max;
      }

      pColor->red = l2;

      // then blue

      l2 = ((unsigned long long)65535 * lColor) / pMap->blue_mult;
      lColor %= pMap->blue_mult;

      if(l2 >= 65536 * pMap->blue_max)
      {
        l2 = 65535 * pMap->blue_max;
      }
//      else if(lColor < 0)
//      {
//        l2 = 0;
//      }
      else
      {
        l2 /= pMap->blue_max;
      }

      pColor->blue = l2;

      // and then, green

      if(!pMap->green_mult)
      {
        l2 = 0;
      }
      else
      {
        l2 = ((unsigned long long)65535 * lColor) / pMap->green_mult;

        if(l2 >= 65536 * pMap->green_max)
        {
          l2 = 65535 * pMap->green_max;
        }
//        else if(lColor < 0)
//        {
//          l2 = 0;
//        }
        else
        {
          l2 /= pMap->green_max;
        }
      }

      pColor->green = l2;
    }
  }
  else if(pMap->green_mult >= pMap->red_mult &&
          pMap->green_mult >= pMap->blue_mult)
  {
    if(pMap->red_mult > pMap->blue_mult)
    {
      // green first

      l2 = ((unsigned long long)65535 * lColor) / pMap->green_mult;
      lColor %= pMap->green_mult;

      if(l2 >= 65536 * pMap->green_max)
      {
        l2 = 65535 * pMap->green_max;
      }
//      else if(lColor < 0)
//      {
//        l2 = 0;
//      }
      else
      {
        l2 /= pMap->green_max;
      }

      pColor->green = l2;

      // then red

      l2 = ((unsigned long long)65535 * lColor) / pMap->red_mult;
      lColor %= pMap->red_mult;

      if(l2 >= 65536 * pMap->red_max)
      {
        l2 = 65535 * pMap->red_max;
      }
//      else if(lColor < 0)
//      {
//        l2 = 0;
//      }
      else
      {
        l2 /= pMap->red_max;
      }

      pColor->red = l2;

      // and then, blue

      if(!pMap->blue_mult)
      {
        l2 = 0;
      }
      else
      {
        l2 = ((unsigned long long)65535 * lColor) / pMap->blue_mult;

        if(l2 >= 65536 * pMap->blue_max)
        {
          l2 = 65535 * pMap->blue_max;
        }
//        else if(lColor < 0)
//        {
//          l2 = 0;
//        }
        else
        {
          l2 /= pMap->blue_max;
        }
      }
      pColor->blue = l2;
    }
    else // if(pMap->blue_mult > pMap->green_mult)
    {
      // green first

      l2 = ((unsigned long long)65535 * lColor) / pMap->green_mult;
      lColor %= pMap->green_mult;

      if(l2 >= 65536 * pMap->green_max)
      {
        l2 = 65535 * pMap->green_max;
      }
//      else if(lColor < 0)
//      {
//        l2 = 0;
//      }
      else
      {
        l2 /= pMap->green_max;
      }

      pColor->green = l2;

      // then blue

      l2 = ((unsigned long long)65535 * lColor) / pMap->blue_mult;
      lColor %= pMap->blue_mult;

      if(l2 >= 65536 * pMap->blue_max)
      {
        l2 = 65535 * pMap->blue_max;
      }
//      else if(lColor < 0)
//      {
//        l2 = 0;
//      }
      else
      {
        l2 /= pMap->blue_max;
      }

      pColor->blue = l2;

      // and then, red

      if(!pMap->red_mult)
      {
        l2 = 0;
      }
      else
      {
        l2 = ((unsigned long long)65535 * lColor) / pMap->red_mult;

        if(l2 >= 65536 * pMap->red_max)
        {
          l2 = 65535 * pMap->red_max;
        }
//        else if(lColor < 0)
//        {
//          l2 = 0;
//        }
        else
        {
          l2 /= pMap->red_max;
        }
      }

      pColor->red = l2;
    }
  }
  else //if(pMap->blue_mult >= pMap->red_mult &&
       //   pMap->blue_mult >= pMap->green_mult)
  {
    if(pMap->red_mult > pMap->green_mult)
    {
      // blue first

      l2 = ((unsigned long long)65535 * lColor) / pMap->blue_mult;
      lColor %= pMap->blue_mult;

      if(l2 >= 65536 * pMap->blue_max)
      {
        l2 = 65535 * pMap->blue_max;
      }
//      else if(lColor < 0)
//      {
//        l2 = 0;
//      }
      else
      {
        l2 /= pMap->blue_max;
      }

      pColor->blue = l2;

      // then red

      l2 = ((unsigned long long)65535 * lColor) / pMap->red_mult;
      lColor %= pMap->red_mult;

      if(l2 >= 65536 * pMap->red_max)
      {
        l2 = 65535 * pMap->red_max;
      }
//      else if(lColor < 0)
//      {
//        l2 = 0;
//      }
      else
      {
        l2 /= pMap->red_max;
      }

      pColor->red = l2;

      // and then, green

      if(!pMap->green_mult)
      {
        l2 = 0;
      }
      else
      {
        l2 = ((unsigned long long)65535 * lColor) / pMap->green_mult;

        if(l2 >= 65536 * pMap->green_max)
        {
          l2 = 65535 * pMap->green_max;
        }
//        else if(lColor < 0)
//        {
//          l2 = 0;
//        }
        else
        {
          l2 /= pMap->green_max;
        }
      }
      pColor->green = l2;
    }
    else // if(pMap->blue_mult > pMap->green_mult)
    {
      // blue first

      l2 = ((unsigned long long)65535 * lColor) / pMap->blue_mult;
      lColor %= pMap->blue_mult;

      if(l2 >= 65536 * pMap->blue_max)
      {
        l2 = 65535 * pMap->blue_max;
      }
//      else if(lColor < 0)
//      {
//        l2 = 0;
//      }
      else
      {
        l2 /= pMap->blue_max;
      }

      pColor->blue = l2;

      // then green

      l2 = ((unsigned long long)65535 * lColor) / pMap->green_mult;
      lColor %= pMap->green_mult;

      if(l2 >= 65536 * pMap->green_max)
      {
        l2 = 65535 * pMap->green_max;
      }
//      else if(lColor < 0)
//      {
//        l2 = 0;
//      }
      else
      {
        l2 /= pMap->green_max;
      }

      pColor->green = l2;


      // and then, red

      if(!pMap->red_mult)
      {
        l2 = 0;
      }
      else
      {
        l2 = ((unsigned long long)65535 * lColor) / pMap->red_mult;

        if(l2 >= 65536 * pMap->red_max)
        {
          l2 = 65535 * pMap->red_max;
        }
//        else if(lColor < 0)
//        {
//          l2 = 0;
//        }
        else
        {
          l2 /= pMap->red_max;
        }
      }

      pColor->red = l2;
    }
  }
}

void PXM_RGBToPixel(XStandardColormap *pMap, XColor *pColor)
{
unsigned long lR, lG, lB;
XStandardColormap map;


  if(!pColor)
  {
    return;
  }

  if(!pMap)
  {
    WBDefaultStandardColormap(WBGetDefaultDisplay(), &map);
    pMap = &map;

//    DEBUG_DUMP_COLORMAP(pMap);
  }

  // this one is straightforward, right out of the docs for the XStandardColormap structure

  lR = lG = lB = 0; // pre-assign

  if(!pColor->flags) // assume all 3 primaries, assign accordingly
  {
    pColor->flags = DoRed | DoGreen | DoBlue;
  }

  if(pColor->flags & DoRed)
  {
    lR = (unsigned long)(pColor->red * pMap->red_max) / 65535;
  }

//  if(lR < 0)
//  {
//    lR = 0;
//  }
//  else
   if(lR > pMap->red_max)
  {
    lR = pMap->red_max;
  }

  if(pColor->flags & DoGreen)
  {
    lG = (unsigned long)(pColor->green * pMap->green_max) / 65535;
  }

//  if(lG < 0)
//  {
//    lG = 0;
//  }
//  else
   if(lG > pMap->green_max)
  {
    lG = pMap->green_max;
  }

  if(pColor->flags & DoBlue)
  {
    lB = (unsigned long)(pColor->blue * pMap->blue_max) / 65535;
  }

//  if(lB < 0)
//  {
//    lB = 0;
//  }
//  else
   if(lB > pMap->blue_max)
  {
    lB = pMap->blue_max;
  }

  pColor->pixel = (pMap->base_pixel
                   + lR * pMap->red_mult
                   + lG * pMap->green_mult
                   + lB * pMap->blue_mult)
                & 0xffffffffL;
}


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
                        WBAlloc(MINIMUM_ATOM_RESOURCE_LIST_SIZE * sizeof(*pAtomResourceList));

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

    void *pTemp = WBReAlloc(pAtomResourceList, iNewSize);
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
      Atom aTemp = WBGetAtom(WBGetDefaultDisplay(), szPreDefinedIconResources[i2]);

      if(aTemp != None)
      {
        RegisterIconResource(aTemp, GetPreDefinedIconResource(i1));
      }
    }

    iHasBeenRegistered = 1;
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
//#ifndef NO_DEBUG
//WB_UINT64 ullTime = WBGetTimeIndex();
//#endif // NO_DEBUG


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

#ifdef X11WORKBENCH_TOOLKIT_HAVE_XPM
  BEGIN_XCALL_DEBUG_WRAPPER
#endif // X11WORKBENCH_TOOLKIT_HAVE_XPM
  XPM_CREATE_PIXMAP_FROM_DATA(WBGetDefaultDisplay(), WBGetHiddenHelperWindow(),
                              ppXPM, &pixRval, &pixRval2, &xattr);
#ifdef X11WORKBENCH_TOOLKIT_HAVE_XPM
  END_XCALL_DEBUG_WRAPPER
#endif // X11WORKBENCH_TOOLKIT_HAVE_XPM

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
    WBFree(pAtomResourceList);
    pAtomResourceList = NULL;
  }

  nAtomResourceList = 0;
  nAtomResourceListMax = 0;

  ppRegAppLarge_Internal = NULL;
  ppRegAppSmall_Internal = NULL;
}



#ifndef NO_DEBUG
static void DebugDumpXpmAttributes(const char *szFunction, int nLine, XPM_ATTRIBUTES *pAttr)
{
  WBDebugPrint("%s line %d XPM_ATTRIBUTES contain:\n", szFunction, nLine);
#if defined(X11WORKBENCH_TOOLKIT_HAVE_XPM)
  WBDebugPrint("  valuemask:          %ld\n", pAttr->valuemask);
  WBDebugPrint("  visual:             %p\n", pAttr->visual);
  WBDebugPrint("  colormap:           %p\n", (void *)pAttr->colormap);
#endif // defined(X11WORKBENCH_TOOLKIT_HAVE_XPM)

  WBDebugPrint("  depth:              %u\n", pAttr->depth);
  WBDebugPrint("  width:              %u\n", pAttr->width);
  WBDebugPrint("  height:             %u\n", pAttr->height);

#if defined(X11WORKBENCH_TOOLKIT_HAVE_XPM)
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
#endif // defined(X11WORKBENCH_TOOLKIT_HAVE_XPM)
}

static void DebugDumpColormap(const XStandardColormap *pMap)
{
  WBDebugPrint("XStandardColormap:  %p\n", pMap);
  WBDebugPrint("  colormap   = %lld\n", (WB_UINT64)pMap->colormap);
  WBDebugPrint("  red_max    = %ld\n", pMap->red_max);
  WBDebugPrint("  red_mult   = %ld\n", pMap->red_mult);
  WBDebugPrint("  green_max  = %ld\n", pMap->green_max);
  WBDebugPrint("  green_mult = %ld\n", pMap->green_mult);
  WBDebugPrint("  blue_max   = %ld\n", pMap->blue_max);
  WBDebugPrint("  blue_mult  = %ld\n", pMap->blue_mult);
  WBDebugPrint("  base_pixel = %ld (%08lxH)\n", pMap->base_pixel, pMap->base_pixel);
}


#endif // !NO_DEBUG

