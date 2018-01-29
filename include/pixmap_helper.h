//////////////////////////////////////////////////////////////////////////////////////////////
//          _                                     _            _                     _      //
//   _ __  (_)__  __ _ __ ___    __ _  _ __      | |__    ___ | | _ __    ___  _ __ | |__   //
//  | '_ \ | |\ \/ /| '_ ` _ \  / _` || '_ \     | '_ \  / _ \| || '_ \  / _ \| '__|| '_ \  //
//  | |_) || | >  < | | | | | || (_| || |_) |    | | | ||  __/| || |_) ||  __/| | _ | | | | //
//  | .__/ |_|/_/\_\|_| |_| |_| \__,_|| .__/_____|_| |_| \___||_|| .__/  \___||_|(_)|_| |_| //
//  |_|                               |_|  |_____|               |_|                        //
//                                                                                          //
//             pixmap and icon helpers (ultimately providing universal support)             //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////

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


#ifndef _PIXMAP_HELPER_H_
#define _PIXMAP_HELPER_H_

#include "window_helper.h"  // if not already included, I must include this

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


#define ID_ICON_FIRST     101
#define ID_APPLICATION    101 /* 19x19 application icon */
#define ID_ICON_OK        102
#define ID_ICON_STOP      103
#define ID_ICON_WARN      104 /* yellow triangle, ! */
#define ID_ICON_WHAT      105
#define ID_ICON_QUESTION  105
#define ID_ICON_SPLAT     106
#define ID_ICON_ASTERISK  106
#define ID_ICON_BANG      107 /* yellow circle, ! */
#define ID_ICON_TRIANGLE  108
#define ID_ICON_WHAT_BOLD 109
#define ID_ICON_WTF       110
#define ID_ICON_DEATH     111
#define ID_ICON_FINGER    112
#define ID_ICON_SKULL     113
#define ID_ICON_THUMBUP   114
#define ID_ICON_THUMBDOWN 115
#define ID_ICON_BEAR      116
#define ID_ICON_BARNEY    117
#define ID_ICON_APP       118 /* 36x36 application icon */
#define ID_ICON_LAST      118 /* must alter this if new items added */


/** \ingroup pixmap
  * \brief Simple RGB assignment to pixel, 0-255 RGB
  *
  * \param R red color, 0-255
  * \param G green color, 0-255
  * \param B blue color, 0-255
  * \param X An XColor structure
  *
  * Assigns the appropriate RGB elements of an XColor structure based on 0-255 RGB.
  * This does NOT assign the 'pixel' element.  For that, use PXM_RGBToPixel()
**/
#define RGB255_TO_XCOLOR(R,G,B,X) { (X).red = ((unsigned int)(R) << 8) & 0xffff; \
                                    (X).green = ((unsigned int)(G) << 8) & 0xffff; \
                                    (X).blue = ((unsigned int)(B) << 8) & 0xffff; \
                                    (X).flags = DoRed | DoGreen | DoBlue; }

/** \ingroup pixmap
  * \brief Simple RGB assignment to pixel, 0-65535 RGB
  *
  * \param R red color, 0-65535
  * \param G green color, 0-65535
  * \param B blue color, 0-65535
  * \param X An XColor structure
  *
  * Assigns the appropriate elements of an XColor structure based on 0-65535 RGB
  * This does NOT assign the 'pixel' element.  For that, use PXM_RGBToPixel()
**/
#define RGB_TO_XCOLOR(R,G,B,X) { (X).red = ((unsigned int)(R)) & 0xffff; \
                                 (X).green = ((unsigned int)(G)) & 0xffff; \
                                 (X).blue = ((unsigned int)(B)) & 0xffff; \
                                 (X).flags = DoRed | DoGreen | DoBlue; }


/** \ingroup pixmap
  * \brief Simple RGB assignment from pixel, 0-255 RGB
  *
  * \param X An XColor structure with valid RGB elements
  * \param R returned red color, 0-255
  * \param G returned green color, 0-255
  * \param B returned blue color, 0-255
  *
  * Assigns the R, G, and B parameters based on an XColor structure with 0-255 RGB
  * This assumes the red, green, and blue elements of the XColor structure are correct.
  * To assign red, green, and blue elements from the 'pixel' element within the XColor
  * structure, use PXM_PixelToRGB()
**/
#define RGB255_FROM_XCOLOR(X,R,G,B) {register unsigned short __iR,__iG,__iB; RGB_FROM_XCOLOR(X,__iR, __iG, __iB); \
                                     if(__iR < 0xff80){ __iR += 0x80; } \
                                     if(__iG < 0xff80){ __iG += 0x80; } \
                                     if(__iB < 0xff80){ __iB += 0x80; } \
                                     __iR = __iR >> 8; __iG = __iG >> 8; __iB = __iB >> 8; \
                                     __iR &= 0xff; __iG &= 0xff; __iB &= 0xff; \
                                     (R) = __iR; (G) = __iG; (B) = __iB; }

/** \ingroup pixmap
  * \brief Simple RGB assignment from pixel, 0-65535 RGB
  *
  * \param X An XColor structure with valid RGB elements
  * \param R returned red color, 0-65535
  * \param G returned green color, 0-65535
  * \param B returned blue color, 0-65535
  *
  * Assigns the R, G, and B parameters based on an XColor structure with 0-65535 RGB
  * This assumes the red, green, and blue elements of the XColor structure are correct.
  * To assign red, green, and blue elements from the 'pixel' element within the XColor
  * structure, use PXM_PixelToRGB()
**/
#define RGB_FROM_XCOLOR(X,R,G,B) { (R) = (((X).flags) & DoRed) ? ((unsigned int)(X).red) & 0xffff : 0; \
                                   (G) = (((X).flags) & DoGreen) ? ((unsigned int)(X).green) & 0xffff : 0; \
                                   (B) = (((X).flags) & DoBlue) ? ((unsigned int)(X).blue) & 0xffff : 0; }


/** \ingroup pixmap
  * \brief create temporary XStandardColormap from a Colormap
  *
  * \param pDisplay The Display * associated with the color map (or NULL for default)
  * \param colormap The Colormap value (NOTE:  it must be of the same type as the default colormap for the associated Display)
  * \returns A pointer to an internal (static) XStandardColormap structure.  This value should not be cached.
  *
  * This function returns a pointer to a static XStandardColormap structure assigned to a specified Colormap,
  * using the specified Display (or default if NULL is specified for the 'D' parameter) for color-related
  * parameters that are part of the XStandardColormap structure.
  *
  * This is a convenience function for things like PXM_PixelToRGB() and PXM_RGBToPixel(), where you may
  * have a Colormap but you need an XStandardColormap.
**/
static __inline XStandardColormap *PXM_StandardColormapFromColormap(Display *pDisplay, Colormap colormap)
{
extern XStandardColormap PXM_StandardColormapFromColormap_rval; // declare extern rather than static, for single mem spot

  if(!pDisplay)
  {
    pDisplay = WBGetDefaultDisplay();
  }

  WBDefaultStandardColormap(pDisplay, &PXM_StandardColormapFromColormap_rval);

  PXM_StandardColormapFromColormap_rval.colormap = colormap;

  return &PXM_StandardColormapFromColormap_rval;
}

/** \ingroup pixmap
  * \brief Convert R, G, B values to Y, U, V with 0-255 range
  *
  * \param iR the Red value (0-255)
  * \param iG the Green value (0-255)
  * \param iB the Blue value (0-255)
  * \param piY the returned 'Y' value (0-255)
  * \param piU the returned 'U' value (0-255)
  * \param piV the returned 'V' value (0-255)
  *
  * Translate colors from RGB to YUV.  Often this is desirable when doing color conversions,
  * where you want to alter the brightness but leave the chroma information as-is.\n
  * The chrominance (U, V) information determines the color space, and the luminance (Y)
  * determines the brightness.  Altering the brightness 'Y' effectively leaves the chrominance
  * information intact, while affecting how 'bright' a color appears.  A color can also be 'faded'
  * or 'saturated' by equally increasing or decreasing the 'U' and 'V' levels.
  *
  * In YUV, for a white pixel, Y should be 255, U and V should both be 128\n
  * In RGB, a white pixel would have R, G, and B at 255.\n
  * Similarly, a black pixel should have R, G, B of 0, with Y at 255, and both U and V 128.\n
  *
  * However, the actual YUV calculations return 'iY' as 16 for 'black', and 235 for 'white'.
  * Anything outside of this Y range SHOULD result in 'faded' colors (towards black or white).
  * This algorithm has been tested for RGB values between 0 and 255, respectively, and the inverse
  * calculation results in a deviation of at most '1' for R, G, or B, from the original RGB value.
  *
  * Header File:  pixmap_helper.h
**/
void PXM_RGBToYUV(int iR, int iG, int iB, int *piY, int *piU, int *piV);


/** \ingroup pixmap
  * \brief Convert R, G, B values to H, S, V with 0-255 range
  *
  * \param iR the Red value (0-255)
  * \param iG the Green value (0-255)
  * \param iB the Blue value (0-255)
  * \param piH the returned 'H' value (0-255)
  * \param piS the returned 'S' value (0-255)
  * \param piV the returned 'V' value (0-255)
  *
  * Translate colors from RGB to HSV.  Often this is desirable when doing color conversions,
  * where you want to alter the brightness or saturation, but leave the color 'tone' as-is.\n
  * The 'hue' (H) information determines the color tone, the 'S' the saturation, and the value (V)
  * determines the brightness.
  *
  * Header File:  pixmap_helper.h
**/
void PXM_RGBToHSV(int iR, int iG, int iB, int *piH, int *piS, int *piV);


/** \ingroup pixmap
  * \brief Convert Y, U, V values to  R, G, B with 0-255 range
  *
  * \param iY the 'Y' value (0-255)
  * \param iU the 'U' value (0-255)
  * \param iV the 'V' value (0-255)
  * \param piR the returned Red value (0-255)
  * \param piG the returned Green value (0-255)
  * \param piB the returned Blue value (0-255)
  *
  * Translate colors from YUV to RGB.  Often this is desirable when doing color conversions,
  * where you want to alter the brightness but leave the chroma information as-is.\n
  * The chrominance (U, V) information determines the color space, and the luminance (Y)
  * determines the brightness.  Altering the brightness 'Y' effectively leaves the chrominance
  * information intact, while affecting how 'bright' a color appears.  A color can also be 'faded'
  * or 'saturated' by equally increasing or decreasing the 'U' and 'V' levels.\n
  *
  * In YUV, for a white pixel, Y should be 255, U and V should both be 128\n
  * In RGB, a white pixel would have R, G, and B at 255.\n
  * Similarly, a black pixel should have R, G, B of 0, with Y at 255, and both U and V 128.\n
  *
  * However, the actual YUV calculations return 'iY' as 16 for 'black', and 235 for 'white'.
  * Anything outside of this Y range SHOULD result in 'faded' colors (towards black or white).
  * This algorithm has been tested for RGB values between 0 and 255, respectively, and the inverse
  * calculation results in a deviation of at most '1' for R, G, or B, from the original RGB value.
  *
  * Header File:  pixmap_helper.h
**/
void PXM_YUVToRGB(int iY, int iU, int iV, int *piR, int *piG, int *piB);


/** \ingroup pixmap
  * \brief Convert H, S, V values to  R, G, B with 0-255 range
  *
  * \param iH the 'H' value (0-255)
  * \param iS the 'S' value (0-255)
  * \param iV the 'V' value (0-255)
  * \param piR the returned Red value (0-255)
  * \param piG the returned Green value (0-255)
  * \param piB the returned Blue value (0-255)
  *
  * Translate colors from HSV to RGB.  Often this is desirable when doing color conversions,
  * where you want to alter the brightness or saturation, but leave the color 'tone' as-is.\n
  * The 'hue' (H) information determines the color tone, the 'S' the saturatio, and the value (V)
  * determines the brightness.
  *
  * Header File:  pixmap_helper.h
**/
void PXM_HSVToRGB(int iH, int iS, int iV, int *piR, int *piG, int *piB);


/** \ingroup pixmap
  * \brief Convert the pixel menber of an XColor to RGB
  *
  * \param pMap A pointer to the XStandardColormap for conversion
  * \param pColor A pointer to the XColor structure.  The 'pixel' member must be a valid pixel value
  *
  * This function reads the pixel member from 'pColor' and calculates the RGB values between 0 and 65535,
  * assigning them to the red, green, and blue members of the XColor structure along with appropriate flags.
  *
  * Header File:  pixmap_helper.h
**/
void PXM_PixelToRGB(XStandardColormap *pMap, XColor *pColor);


/** \ingroup pixmap
  * \brief Icon Registration for application 'large' and 'small' icons
  *
  * \param pMap A pointer to the XStandardColormap for conversion
  * \param pColor A pointer to the XColor structure.  The 'red' 'green' and 'blue' members must be a valid RGB value
  *
  * This function reads the red, green, and blue members from 'pColor' and calculates the pixel value.  RGB
  * entries are assumed to be between 0 and 65535, for each of the red, green, and blue members.
  *
  * Header File:  pixmap_helper.h
**/
void PXM_RGBToPixel(XStandardColormap *pMap, XColor *pColor);


/** \ingroup pixmap
  * \brief Icon Registration for application 'large' and 'small' icons
  *
  * \param ppRegAppLarge XPM definition array for 'large' 36x36 icon, IDC_ICON_APP
  * \param ppRegAppSmall XPM definition array for 'small' 19x19 icon, ID_APPLICATION
  *
  * Use this function to register the application's icons for IDC_ICON_APP and ID_APPLICATION.  In this
  * way you can use both of them in dialog boxes and take advantage of PXM_GetIconPixmap() to create
  * a pair of icon pixmaps whenever they are needed.
  *
  * Header File:  pixmap_helper.h
**/
void PXM_RegisterAppIcons(char *ppRegAppLarge[], char *ppRegAppSmall[]);


/** \ingroup pixmap
  * \brief Register an icon (or pixmap) resource using an Atom
  *
  * \param aResource The Atom that will be used to identify the resource
  * \param ppResource The XPM data array that defines the pixmap or icon
  * \returns The function returns zero on success, non-zero on error.
  *
  * This function allows you to register a pixmap or icon resource using an Atom.  Typically
  * this will be used by dialog boxes or menus to conveniently identify pixmap or icon images.
  *
  * Header File:  pixmap_helper.h
**/
int PXM_RegisterPixmapResource(Atom aResource, char *ppResource[]);



/** \ingroup pixmap
  * \brief Create Icon pixmap pair using pre-defined resource ID
  *
  * \param idIcon The unique (numeric) identifier associated with the icon
  * \param pAttr A pointer to an XpmAttributes structure when libXpm is present, or an XPM_ATTRIBUTES compatibility structure otherwise.\n
  * For portability you should use the XPM_ATTRIBUTES definition, even when libXpm is guaranteed to be present.
  * \param pMask A pointer to the variable that receives the 'Transparency Mask' pixmap for the icon (may be 'None').  This parameter may be NULL.
  * \returns The pixmap identifier for the Icon image, or None on error.
  *
  * Use this function whenever you need to load an icon using a pre-defined resource ID.
  *
  * Header File:  pixmap_helper.h
**/
Pixmap PXM_GetIconPixmap(int idIcon, XPM_ATTRIBUTES *pAttr, Pixmap *pMask /* = NULL*/);


/** \ingroup pixmap
  * \brief Create Icon pixmap pair using a registered or pre-defined resource ID
  *
  * \param aIcon An atom that identifies a registered or pre-defined resource ID
  * \param pAttr A pointer to an XpmAttributes structure when libXpm is present, or an XPM_ATTRIBUTES compatibility structure otherwise.\n
  * For portability you should use the XPM_ATTRIBUTES definition, even when libXpm is guaranteed to be present.
  * \param pMask A pointer to the variable that receives the 'Transparency Mask' pixmap for the icon (may be 'None').  This parameter may be NULL.
  * \returns The pixmap identifier for the Icon image, or None on error.
  *
  * Use this function whenever you need to load an icon using a pre-defined or registered Atom
  *
  * Header File:  pixmap_helper.h
**/
Pixmap PXM_GetIconPixmapFromAtom(Atom aIcon, XPM_ATTRIBUTES *pAttr, Pixmap *pMask /* = NULL*/);



/** \ingroup pixmap
  * \brief Create pixmap or pixmap pair using an XPM array
  *
  * \param ppXPM XPM definition array for the associated graphic
  * \param pAttr A pointer to an XpmAttributes structure when libXpm is present, or an XPM_ATTRIBUTES compatibility structure otherwise.
  * Pass a NULL pointer if you do not need the attributes.\n
  * For portability you should use the XPM_ATTRIBUTES definition, even when libXpm is guaranteed to be present.
  * \param pMask A pointer to the variable that receives the 'Transparency Mask' pixmap for the image or icon (may be 'None').
  * Pass a NULL pointer if you do not need the mask.
  * \returns The pixmap identifier for the image, or None on error.
  *
  * This function conveniently wraps XpmCreatePixmapFromData()
  *
  * Header File:  pixmap_helper.h
**/
Pixmap PXM_LoadPixmap(char *ppXPM[], XPM_ATTRIBUTES *pAttr, Pixmap *pMask /* = NULL*/);



/** \ingroup pixmap
  * \brief Convert 'locally stored' XImage to 'server object' Pixmap
  *
  * \param pDisplay The display pointer.  NULL uses the default display.
  * \param dw The 'Drawable' for which the pixmap will be created.
  * \param pImage The image to convert.  This must be a valid image with non-zero height/width specified in the structure.
  * \param clrFGPixel Foreground color pixel to use for monochrome images, typically BlackPixel(pDisplay, DefaultScreen(pDisplay))
  * \param clrBGPixel Background color pixel to use for monochrome images, typically WhitePixel(pDisplay, DefaultScreen(pDisplay))
  * \returns The resultant pixmap
  *
  * This function is a convenient way to perform a standard operation that converts an Image to a Pixmap.  A pixmap is a
  * more efficient way of storing graphic data for subsequent display.  Typically a pixmap will be cached for a window's
  * Expose event handler, and is re-created when needed.  Pixmaps are typically stored by the X server, so an operation
  * that paints a window with a pixmap should perform more efficiently.  However, a pixmap cannot be queried nor
  * directly manipulated, while an XImage can.\n
  * The function returns a Pixmap identifier, or None.  To delete the Pixmap, use XFreePixmap().
  *
  * Header File:  pixmap_helper.h
**/
Pixmap PXM_ImageToPixmap(Display *pDisplay, Drawable dw, XImage *pImage,
                         unsigned long clrFGPixel, unsigned long clrBGPixel);



/** \ingroup pixmap
  * \brief Convert 'locally stored' XImage to 'server object' Pixmap using default FG/BG colors for monochrome
  *
  * \param pDisplay The display pointer.  NULL uses the default display.
  * \param dw The 'Drawable' for which the pixmap will be created.
  * \param pImage The image to convert.  This must be a valid image with non-zero height/width specified in the structure.
  * \returns The resultant pixmap
  *
  * This function is a convenient way to perform a standard operation that converts an Image to a Pixmap.  A pixmap is a
  * more efficient way of storing graphic data for subsequent display.  Typically a pixmap will be cached for a window's
  * Expose event handler, and is re-created when needed.  Pixmaps are typically stored by the X server, so an operation
  * that paints a window with a pixmap should perform more efficiently.  However, a pixmap cannot be queried nor
  * directly manipulated, while an XImage can.\n
  * For monochrome images, the foreground color is black, and the background color is white.\n
  * The function returns a Pixmap identifier, or None.  To delete the Pixmap, use XFreePixmap().
  *
  * Header File:  pixmap_helper.h
**/
Pixmap PXM_ImageToPixmap0(Display *pDisplay, Drawable dw, XImage *pImage);



/** \ingroup pixmap
  * \brief Convert pixmap to image (a wrapper for XGetImage on a pixmap)
  *
  * \param pDisplay The disply pointer.  NULL uses the default display.
  * \param pxImage The image pixmap
  * \returns An XImage pointer allocated by Xlib, with data in ZPixmap format.  Use XDestroyImage to dispose of it.
  *
  * This function wraps the functionality of XGetImage with a simpler interface, returning an XImage * to an object
  * allocated by Xlib.  A pixmap is typically stored on the X Server, whereas an XImage is stored locally.\n
  * XGetImage creates an XImage using the same depth as the pixmap.  The plane mask specified by this function includes
  * all image data, and the origin is always 0,0.  The XImage format is always ZPixmap.\n
  * The size of the pixmap is automatically determined using XGetGeometry().  As a side note, you could theoretically
  * use this function to create an image from a pixmap for the purpose of determining its characteristics.\n
  * The function returns NULL on error.  Use XDestroyImage to dispose of the XImage object.
  *
  * Header File:  pixmap_helper.h
**/
XImage *PXM_PixmapToImage(Display *pDisplay, Pixmap pxImage);




/** \ingroup startup
  * \brief Frees resources allocated by \ref pixmap
  *
  * Call this to free resources allocated by the PXM functions.  Called by \ref WBExit()
  *
  * Header File:  pixmap_helper.h
**/
void PXM_OnExit(void);


//-------------------
// HELPERS FOR XColor
//-------------------

/** \ingroup debug
  * \brief Dump XColor members for debugging
  *
  * \param szTitle A const pointer to a string, to be displayed as the title for debug output
  * \param pColor A const pointer to an XColor structure.
  *
  * Call this to do a debug dump of XColor information
  *
  * Header File:  pixmap_helper.h
**/
void WBDebugDumpXColor(const char *szTitle, const XColor *pColor);

/** \ingroup debug
  * \brief Dump XStandardColormap members for debugging
  *
  * \param szTitle A const pointer to a string, to be displayed as the title for debug output
  * \param pMap A const pointer to an XStandardColormap structure.
  *
  * Call this to do a debug dump of XStandardColormap information
  *
  * Header File:  pixmap_helper.h
**/
void WBDebugDumpColormap(const char *szTitle, const XStandardColormap *pMap);


//--------------------
// HELPERS FOR XImage
//--------------------


/** \ingroup pixmap
  * \brief Returns pointer to XImage data
  *
  * \param pImage A pointer to an XImage (must not be NULL)
  * \returns The data pointer element from the XImage
  *
  * Call this to get the pointer to the data stored in an XImage.  This data can be copied
  * and stored elsewhere, as needed, or restored from a stored copy.
  *
  * Header File:  pixmap_helper.h
**/
static __inline void *PXM_GetImageDataPtr(XImage *pImage)
{
  return pImage->data;
}

/** \ingroup pixmap
  * \brief Returns the length of XImage data
  *
  * \param pImage A pointer to an XImage (must not be NULL)
  * \returns The length of the data from the XImage
  *
  * Call this to get the length of the data stored in an XImage.  This data can be copied
  * and stored elsewhere, as needed, or restored from a stored copy.
  *
  * Header File:  pixmap_helper.h
**/
static __inline unsigned long PXM_GetImageDataLength(XImage *pImage)
{
  if(pImage->depth <= 1)
  {
    return (unsigned long)pImage->bytes_per_line * (unsigned long)pImage->height; // TODO:  is this right?
  }

  return (unsigned long)pImage->bytes_per_line * (unsigned long)pImage->height
         * (unsigned long)pImage->depth;
}


#ifdef __cplusplus
};
#endif // __cplusplus

#endif // _PIXMAP_HELPER_H_

