//////////////////////////////////////////////////////////////////////////////////////////
//                 _                          _               _       _                 //
//              __| | _ __  __ _ __      __  | |_  ___ __  __| |_    | |__              //
//             / _` || '__|/ _` |\ \ /\ / /  | __|/ _ \\ \/ /| __|   | '_ \             //
//            | (_| || |  | (_| | \ V  V /   | |_|  __/ >  < | |_  _ | | | |            //
//             \__,_||_|   \__,_|  \_/\_/_____\__|\___|/_/\_\ \__|(_)|_| |_|            //
//                                      |_____|                                         //
//                                                                                      //
//                        text draw, multi-line and tabbed text                         //
//                       Also, additional text-related utilities                        //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////

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


/** \file draw_text.h
  * \brief Utilities for copying and drawing text, determining text extents, and so on
  *
  * X11 Work Bench Toolkit Toolkit API
**/


#ifndef _DRAW_TEXT_H_INCLUDED_
#define _DRAW_TEXT_H_INCLUDED_

#ifndef WINDOW_HELPER_H_INCLUDED
#include "window_helper.h"
#endif // WINDOW_HELPER_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


/** \ingroup draw_text
  * \enum DTAlignment
  * \hideinitializer
  * \brief Alignment flags for various 'draw text' API functions.
**/
enum DTAlignment
{
    DTAlignment_HLEFT = 1,               ///< left-justified text
    DTAlignment_HCENTER = 2,             ///< horizontally centered text.  tabs are treated as white space
    DTAlignment_HRIGHT = 3,              ///< right-justified text
    DTAlignment_HJUSTIFY = 4,            ///< right/left 'justified' text - add or remove text kerning to 'justify' it
                                         /// As with DTAlignment_HCENTER, tabs are treated as regular white space.
    DTAlignment_HMASK = 0xf,             ///< horizontal alignment mask

    DTAlignment_VTOP = 0x10,             ///< top-aligned text (using highest vertical ascent)
    DTAlignment_VBOTTOM = 0x20,          ///< bottom-aligned text (using lowest vertical descent)
    DTAlignment_VCENTER = 0x30,          ///< center using entire text height (ascent + descent for single line)
    DTAlignment_VCENTERASCENT = 0x40,    ///< center halfway up the 'ascent' portion (single line only)
    DTAlignment_VCENTERBASELINE = 0x50,  ///< center along the font baseline (single line only)
    DTAlignment_VTOPHALF = 0x60,         ///< center within the top half of the rectangle (single line only)
    DTAlignment_VTBOTTOMHALF = 0x70,     ///< center within the bottom half of the rectangle (single line only)
    DTAlignment_VMASK = 0xf0,

    // these next flags are primarily for DTCalcIdealBounds, and some for DTCalcIdealFont

    DTAlignment_V_BEFORE_H = 0x100,      ///< re-size vertically before horizontally (affects text wrapping).
                                         /// if not specified, resize horizontally before vertically.  In other words,
                                         /// the default would increase the width of the rectangle BEFORE wrapping the
                                         /// text and adding another line.  The general rule attempts to keep the ratio
                                         /// of height to width similar to that of the source rectangle [when specified].
    DTAlignment_NO_VRESIZE = 0x200,      ///< do not increase size vertically
    DTAlignment_NO_HRESIZE = 0x400,      ///< do not increase size horizontally
    DTAlignment_NO_RESIZE  = 0x600,      ///< if specified, does not increase rect size (combination of above 2).
                                         /// If re-sizing is required, and this flag is specified, 'DTCalcIdealBounds'
                                         /// will simply return -1 indicating that resizing is needed, but not calculate
                                         /// the new size.

    DTAlignment_RESIZE_PIXEL = 0x800,    ///< resize by adding an equal number of pixels, both to horizontal
                                         /// and vertical, or as close as possible to the same number.
                                         /// if not specified, try to maintain the same ratio of H : V
    DTAlignment_NO_SHRINK = 0x1000,      ///< Do not shrink the bounding rectangle for ideal text dimension

    DTAlignment_ALLOW_TRAILING = 0x2000, ///< allow trailing white space (normally it is trimmed/ignored for alignment).
                                         /// This only applies with horizontal centering, or when trailing text affects a
                                         /// line split or precedes a line feed
    DTAlignment_NO_WORD_WRAP = 0x4000,   ///< do not perform word wrapping in order to 'fit' the text.  Normally the
                                         /// DTCalcIdealBounds and DTCalcIdealFont functions will attempt to restructure
                                         /// the text with word-wrapping to fit within horizontal bounds.

    DTAlignment_SINGLELINE = 0x20000000, ///< internal flag, 'single line only' (no line breaks).  Implies DTAlignment_NO_WORD_WRAP
    DTAlignment_UNDERSCORE = 0x40000000, ///< internal flag, indicates that an underscore underlines the next character
    DTAlignment_PRINTING = 0x80000000    ///< internal flag, indicates that I'm printing, so don't go beyond word wrap
                                         /// in order to make the text fit within the display rectangle.
};


/** \ingroup draw_text
  * \struct __DT_WORD__
  * \copydoc DT_WORD
**/
/** \ingroup draw_text
  * \typedef DT_WORD
  * \brief Structure defining a 'word' for rendering purposes.
  *
  * A 'word' is defined as one or more characters as a group that is to be rendered in
  * as single-line or multi-line text.  Special case is white space, line feeds, and tabs.
  * The width and height of the 'word' element can be used to line up and do 'word breaks'
  * on text for line wrapping.  Multiple white space blocks can also be eliminated from the
  * output when it is at the end of a line, particularly when wrapping text.
  *
  * \code

  typedef struct __DT_WORD__
  {
    const char *pText;      // pointer to start of word (most likely in the supplied buffer)
                            // for tabs and white space, it will be NULL
                            // for OTHER special characters, it points to the character
    int nLength;            // length of text element (in bytes)
    int iWidth;             // width of text in pixels (based on font size) (NA for tabs, LF, valid for white space)
    int iHeight;            // height of text in pixels (based on font size) (NA for tabs, LF, valid for white space)
    int iIsTab;             // indicates that the text consists of tabs (nLength for number of tabs, pText is NULL)
    int iIsLineFeed;        // indicates a line feed (CR, CRLF, LF, or LFCR) (nLength for number of line feeds, pText is NULL)
    int iIsWhiteSpace;      // indicates that it is 'white space' (regular or special character) (nLength, width, height applies)
    int iX;                 // Relative 'X' pixel position for the beginning of this element (when pre-rendered, else -1)
    int iY;                 // Relative 'Y' pixel position for the beginning of this element (when pre-rendered, else -1)
  } DT_WORD;

  * \endcode
  *
**/
typedef struct __DT_WORD__
{
  const char *pText;      ///< pointer to start of word (most likely in the supplied buffer)
                          /// for tabs and white space, it will be NULL
                          ///< for OTHER special characters, it points to the character
  int nLength;            ///< length of text element (in bytes)
  int iWidth;             ///< width of text in pixels (based on font size) (NA for tabs, LF, valid for white space)
  int iHeight;            ///< height of text in pixels (based on font size) (NA for tabs, LF, valid for white space)
  int iIsTab;             ///< indicates that the text consists of tabs (nLength for number of tabs, pText is NULL)
  int iIsLineFeed;        ///< indicates a line feed (CR, CRLF, LF, or LFCR) (nLength for number of line feeds, pText is NULL)
  int iIsWhiteSpace;      ///< indicates that it is 'white space' (regular or special character) (nLength, width, height applies)
  int iX;                 ///< Relative 'X' pixel position for the beginning of this element (when pre-rendered, else -1)
  int iY;                 ///< Relative 'Y' pixel position for the beginning of this element (when pre-rendered, else -1)
} DT_WORD;


/** \ingroup draw_text
  * \struct __DT_WORDS__
  * \copydoc DT_WORDS
**/
/** \ingroup draw_text
  * \typedef DT_WORDS
  * \brief A collection of DT_WORD structures along with a pointer to the original text
  *
  * A collection of DT_WORD structures, along with a pointer to the original text.  This structure is
  * typically used to render text and calculate bounding rectangles for the purpose of window sizing
  * and viewport scrolling.
  *
  * \code

  typedef struct __DT_WORDS__
  {
    struct __DT_WORDS__ *pPrev; // pointer to previous structure in linked list (reserved)
    struct __DT_WORDS__ *pNext; // pointer to next structure in linked list (reserved)
    int nCount;           // number of items in aWords
    int nMax;             // max number of items in aWords (for memory allocation purposes)
    int iMaxWidth;        // maximum width of contained text, or -1 if not known
    int iMaxHeight;       // maximum height of contained text, or -1 if not known
    const char *szText;   // original text pointer
    DT_WORD aWords[1];    // The actual data (self-contained within the memory block)
  } DT_WORDS;

  * \endcode
  *
**/
typedef struct __DT_WORDS__
{
  struct __DT_WORDS__ *pPrev; ///< pointer to previous structure in linked list (reserved)
  struct __DT_WORDS__ *pNext; ///< pointer to next structure in linked list (reserved)
  int nCount;           ///< number of items in aWords
  int nMax;             ///< max number of items in aWords (for memory allocation purposes)
  int iMaxWidth;        ///< maximum width of contained text, or -1 if not known
  int iMaxHeight;       ///< maximum height of contained text, or -1 if not known
  const char *szText;   ///< original text pointer
  DT_WORD aWords[1];    ///< The actual data (self-contained within the memory block)
} DT_WORDS;



/** \ingroup draw_text
  * \brief Draw text in a platform-independent manner for UTF-8 or multi-byte text
  *
  * \param pDisplay A pointer to the display to use for rendering the text
  * \param drawable The 'Drawable' object upon which to render the text
  * \param fontSet The Font Set for which to query metrics
  * \param gc The graphics context 'GC' for rendering the text
  * \param x The 'x' coordinate for the text alignment
  * \param y The 'y' coordinate for the text alignment
  * \param pString A const pointer to a UTF-8 or multi-byte string
  * \param nLength The BYTE LENGTH of the UTF-8 or mult-byte string (not character length)
  *
  * Use this function to 'Draw Text' in a platform-independent manner, using the
  * specified font set, upon the specified 'Drawable'.  This function is the equivalent
  * of the X11 library's XmbDrawString() and Xutf8DrawString().
  *
  * As an extra added bonus, this function will apply anti-aliasing features, as appropriate
  *
  * Header File:  draw_text.h
**/
void DTDrawString(Display *pDisplay, Drawable drawable, XFontSet fontSet,
                  GC gc, int x, int y, const char *pString, int nLength);


/** \ingroup draw_text
  * \brief XTextWidth equivalent for MBCS and UTF-8 strings
  *
  * \param fontSet The Font Set for which to query metrics
  * \param szUTF8 A const pointer to a UTF-8 string
  * \param nLength The BYTE LENGTH of the UTF-8 string (not character length)
  * \returns the text width, in pixels
  *
  * Use this function in lieu of XTextWidth for MBCS and UTF-8 strings.
  *
  * Header File:  draw_text.h
**/
int DTGetTextWidth(XFontSet fontSet, const char *szUTF8, int nLength);


/** \ingroup draw_text
  * \brief Calculate the ideal font based on the text and rectangle
  *
  * \param pRefFont A pointer to an XFontStruct (NULL implies system default font)
  * \param szText A pointer to a (0-byte terminated) ASCII string that may span multiple lines
  * \param geomBounds A pointer to a \ref WB_GEOM structure that defines a bounding rectangle
  *
  * sometimes you want to adjust text size to fit within a particular rectangle.  This function
  * determines how to do that by adjusting the font size.
  *
  * Header File:  draw_text.h
**/
XFontStruct *DTCalcIdealFont(XFontStruct *pRefFont, const char *szText, WB_GEOM *geomBounds);


/** \ingroup draw_text
  * \brief Calculate the ideal font based on the text and rectangle
  *
  * \param pDisplay A pointer to the Display associated with the font set
  * \param fontSet The referense XFontSet (None implies system default font set)
  * \param szText A pointer to a (0-byte terminated) ASCII string that may span multiple lines
  * \param geomBounds A pointer to a \ref WB_GEOM structure that defines a bounding rectangle
  *
  * sometimes you want to adjust text size to fit within a particular rectangle.  This function
  * determines how to do that by adjusting the font size.  It is similar to 'DTCalcIdealFont'.
  *
  * Header File:  draw_text.h
**/
XFontSet DTCalcIdealFontSet(Display *pDisplay, XFontSet fontSet, const char *szText, WB_GEOM *geomBounds);


/** \ingroup draw_text
  * \brief Calculate the ideal bounding rectangle for the specified text and font
  *
  * \param pFont A pointer to an XFontStruct (NULL implies system default font)
  * \param szText A pointer to a (0-byte terminated) ASCII string that may span multiple lines
  * \param iTabWidth A positive integer in 'characters', or negative integer in pixels, indicating tab width
  * \param iTabOrigin An unsigned integer indicating the tab origin, using the same units as iTabWidth, corresponding to the first character.
  * \param prcSource A pointer to the 'source' bounding rectangle in which the text is intended to fit. (If NULL, the function will ALWAYS return non-zero and return the raw text extents without trying to fit it into a particular bounding rectangle.)
  * \param prcDest A pointer to the 'destination' bounding rectangle, based on the actual text size, and fit according to iAlignment flags.
  * \param iAlignment The desired text alignment, one or more of the DTAlignment bit flags
  * \returns zero if the text will fit within prcSource, -1 error, or 1 to indicate that prcDest is larger than prcSource
  *
  * This function will calculate the ideal bounding rectangle for the specified text, if
  * possible, within the limitations of the source rectangle.  If the text cannot be
  * rendered within the source rectangle, the function will attempt to calculate
  * an appropriately sized rectangle in which to render it.
  * You can use this function to re-size windows to accomodate the size of the text that
  * must be displayed within the window.
  *
  * NOTE:  tab width is negative for pixels, positive for avg char width, beginning at
  *        the left most position of the specified rectangle.  Tab Origin is always in
  *        pixels using the absolute position (relative to the same origin as prcSource).
  *
  * Header File:  draw_text.h
**/
int DTCalcIdealBounds0(XFontStruct *pFont, const char *szText, int iTabWidth, unsigned int iTabOrigin,
                       const WB_RECT *prcSource, WB_RECT *prcDest, int iAlignment);

/** \ingroup draw_text
  * \brief Calculate the ideal bounding rectangle for the specified text and font
  *
  * \param pDisplay The Display associated with the XFontSet (NULL implies default display)
  * \param fontSet An XFontSet for which to measure the bounding rectangle (None implies system default font set)
  * \param szText A pointer to a (0-byte terminated) ASCII string that may span multiple lines
  * \param iTabWidth A positive integer in 'characters', or negative integer in pixels, indicating tab width
  * \param iTabOrigin An unsigned integer indicating the tab origin, using the same units as iTabWidth, corresponding to the first character.
  * \param prcSource A pointer to the 'source' bounding rectangle in which the text is intended to fit. (If NULL, the function will ALWAYS return non-zero and return the raw text extents without trying to fit it into a particular bounding rectangle.)
  * \param prcDest A pointer to the 'destination' bounding rectangle, based on the actual text size, and fit according to iAlignment flags.
  * \param iAlignment The desired text alignment, one or more of the DTAlignment bit flags
  * \returns zero if the text will fit within prcSource, -1 error, or 1 to indicate that prcDest is larger than prcSource
  *
  * This function will calculate the ideal bounding rectangle for the specified text, if
  * possible, within the limitations of the source rectangle.  If the text cannot be
  * rendered within the source rectangle, the function will attempt to calculate
  * an appropriately sized rectangle in which to render it.
  * You can use this function to re-size windows to accomodate the size of the text that
  * must be displayed within the window.
  *
  * NOTE:  tab width is negative for pixels, positive for avg char width, beginning at
  *        the left most position of the specified rectangle.  Tab Origin is always in
  *        pixels using the absolute position (relative to the same origin as prcSource).
  *
  * Header File:  draw_text.h
**/
int DTCalcIdealBounds(Display *pDisplay, XFontSet fontSet, const char *szText, int iTabWidth, unsigned int iTabOrigin,
                      const WB_RECT *prcSource, WB_RECT *prcDest, int iAlignment);

/** \ingroup draw_text
  * \brief draw single-line text
  *
  * \param pFont A pointer to the font to use for drawing text (NULL implies system default font)
  * \param szText A pointer to a (0-byte terminated) ASCII string that may span multiple lines
  * \param pDisplay the display associated with the GC and Drawable.
  * \param gc The graphics context for the Drawable
  * \param dw The Drawable (Window or Pixmap, typically)
  * \param iTabWidth The width of a tab, negative for pixels, positive for avg char width
  * \param iTabOrigin The 'origin point' for tabulation corresponding to the first character in the string
  * \param prcBounds A pointer to the bounding rectangle
  * \param iAlignment The desired text alignment, one or more of the DTAlignment bit flags
  *
  * This function draws single-line text within the specified bounds rectangle on the specified
  * drawable using the specified GC, adjusting for tab width and text alignment.\n
  * If the text cannot fit within the bounding rectangle, it will be truncated.\n
  * If you want the text to wrap, use DTDrawMultiLineText()
  *
  * NOTE:  tabs begin at the 'left most' text position, adjusted by 'iTabOrigin'.
  *
  * Header File:  draw_text.h
**/
void DTDrawSingleLineText0(XFontStruct *pFont, const char *szText, Display *pDisplay, GC gc, Drawable dw,
                           int iTabWidth, int iTabOrigin, const WB_RECT *prcBounds, int iAlignment);

/** \ingroup draw_text
  * \brief draw single-line text
  *
  * \param fontSet An XFontSet to use for drawing text (None implies system default font set)
  * \param szText A pointer to a (0-byte terminated) ASCII string that may span multiple lines
  * \param pDisplay the display associated with the GC and Drawable.
  * \param gc The graphics context for the Drawable
  * \param dw The Drawable (Window or Pixmap, typically)
  * \param iTabWidth The width of a tab, negative for pixels, positive for avg char width
  * \param iTabOrigin The 'origin point' for tabulation corresponding to the first character in the string
  * \param prcBounds A pointer to the bounding rectangle
  * \param iAlignment The desired text alignment, one or more of the DTAlignment bit flags
  *
  * This function draws single-line text within the specified bounds rectangle on the specified
  * drawable using the specified GC, adjusting for tab width and text alignment.\n
  * If the text cannot fit within the bounding rectangle, it will be truncated.\n
  * If you want the text to wrap, use DTDrawMultiLineText()
  *
  * NOTE:  tabs begin at the 'left most' text position, adjusted by 'iTabOrigin'.
  *
  * Header File:  draw_text.h
**/
void DTDrawSingleLineText(XFontSet fontSet, const char *szText, Display *pDisplay, GC gc, Drawable dw,
                          int iTabWidth, int iTabOrigin, const WB_RECT *prcBounds, int iAlignment);

/** \ingroup draw_text
  * \brief draw multi-line text
  *
  * \param pFont A pointer to the font to use for drawing text (NULL implies system default font)
  * \param szText A pointer to a (0-byte terminated) ASCII string that may span multiple lines
  * \param pDisplay the display associated with the GC and Drawable.
  * \param gc The graphics context for the drawable
  * \param dw The Drawable (Window or Pixmap, typically)
  * \param iTabWidth The width of a tab, negative for pixels, positive for avg char width
  * \param iTabOrigin The 'origin point' for tabulation corresponding to the first character in the string
  * \param prcBounds A pointer to the bounding rectangle
  * \param iAlignment The desired text alignment, one or more of the DTAlignment bit flags
  *
  * This function draws multi-line text within the specified bounds rectangle on the specified
  * drawable using the specified GC, adjusting for tab width and text alignment.
  *
  * NOTE:  tabs begin at the 'left most' position, adjusted by 'iTabOrigin'.
  *
  * Header File:  draw_text.h
**/
void DTDrawMultiLineText0(XFontStruct *pFont, const char *szText, Display *pDisplay, GC gc, Drawable dw,
                          int iTabWidth, int iTabOrigin, const WB_RECT *prcBounds, int iAlignment);

/** \ingroup draw_text
  * \brief draw multi-line text
  *
  * \param fontSet An XFontSet to use for drawing text (None implies system default font set)
  * \param szText A pointer to a (0-byte terminated) ASCII string that may span multiple lines
  * \param pDisplay the display associated with the GC and Drawable.
  * \param gc The graphics context for the drawable
  * \param dw The Drawable (Window or Pixmap, typically)
  * \param iTabWidth The width of a tab, negative for pixels, positive for avg char width
  * \param iTabOrigin The 'origin point' for tabulation corresponding to the first character in the string
  * \param prcBounds A pointer to the bounding rectangle
  * \param iAlignment The desired text alignment, one or more of the DTAlignment bit flags
  *
  * This function draws multi-line text within the specified bounds rectangle on the specified
  * drawable using the specified GC, adjusting for tab width and text alignment.
  *
  * NOTE:  tabs begin at the 'left most' position, adjusted by 'iTabOrigin'.
  *
  * Header File:  draw_text.h
**/
void DTDrawMultiLineText(XFontSet fontSet, const char *szText, Display *pDisplay, GC gc, Drawable dw,
                         int iTabWidth, int iTabOrigin, const WB_RECT *prcBounds, int iAlignment);


/** \ingroup draw_text
  * \brief Parse 'DT_WORDS' structure from single-line or multi-line text
  *
  * \param pFont A pointer to an XFontStruct to be used, or NULL for the system default font
  * \param szText A 0-byte terminated UFT8 string containing the text to be rendered
  * \param iAlignment Alignment flags (typically zero).  The only flag that really matters is DTAlignment_UNDERSCORE
  * \return A 'WBAlloc'd pointer to a DT_WORDS structure (variable length).  use 'WBFree()' to de-allocate
  * the memory block.
  *
  * The 'DT_WORDS' structure is intended to be used to cache rendering information, particularly for
  * a large block of text that may be calculation-expensive to re-render.  Call 'DTPreRender()' and
  * 'DTRender()' to manage rendering with the DT_WORDS structure.
  *
  *
  * Header File:  draw_text.h
**/
DT_WORDS * DTGetWordsFromText0(XFontStruct *pFont, const char *szText, int iAlignment);

/** \ingroup draw_text
  * \brief Parse 'DT_WORDS' structure from single-line or multi-line text
  *
  * \param pDisplay A pointer to the display associated with the XFontSet
  * \param fontSet The XFontSet to be used, or None for the system default font set
  * \param szText A 0-byte terminated UFT8 string containing the text to be rendered
  * \param iAlignment Alignment flags (typically zero).  The only flag that really matters is DTAlignment_UNDERSCORE
  * \return A 'WBAlloc'd pointer to a DT_WORDS structure (variable length).  use 'WBFree()' to de-allocate
  * the memory block.
  *
  * The 'DT_WORDS' structure is intended to be used to cache rendering information, particularly for
  * a large block of text that may be calculation-expensive to re-render.  Call 'DTPreRender()' and
  * 'DTRender()' to manage rendering with the DT_WORDS structure.
  *
  *
  * Header File:  draw_text.h
**/
DT_WORDS * DTGetWordsFromText(Display *pDisplay, XFontSet fontSet, const char *szText, int iAlignment);

/** \ingroup draw_text
  * \brief Pre-render a 'DT_WORDS' structure for subsequent display
  *
  * \param pDisplay the display that you intend to render on with DTRender().  NULL uses the default display.
  * \param fontSet An XFontSet to use for drawing text (None implies system default font set)
  * \param pWords A pointer to the DT_WORDS structure.  This structure can be part of a linked list.
  * \param iTabWidth A positive integer in 'characters', or negative integer in pixels, indicating tab width
  * \param iTabOrigin An unsigned integer indicating the tab origin, using the same units as iTabWidth, corresponding to the first character.
  * \param prcBounds A pointer to the bounding rectangle (advisory with respect to available width and height, based on iAlignment).
  *  On return it contains the actual bounding rectangle, calculated from the passed-in value with alignment flags applied.
  *  It is important to pass the updated rectangle values when calling DTRender().
  * \param iAlignment The desired text alignment, one or more of the DTAlignment bit flags
  * \param iStartLine The starting line to begin pre-rendering the pWords array (0 is the first line)
  * \param iEndLine The ending line (inclusive) to end pre-rendering the pWords array.  (0 is the first line; -1 implies 'all')
  *
  * This function will scan the DT_WORDS structure 'pWords', and using the alignment flags and prcBounds,
  * modify the 'iX' and 'iY' members for each DT_WORD entry in 'pWords' according to their rendered
  * position within the display surface described by 'prcBounds'.  It will also calculate the iMaxWidth
  * and iMaxHeight members for 'pWords'.\n
  * Since the 'WB_RECT' elements are 32-bit integers, it is possible to create an 'infinite' display area
  * by specifying a value of 0x7fffffffL for the 'right' and 'bottom' members.  Then, you can use 'DTRender()'
  * and specify 'iHScrollBy' and 'iVScrollBy' to modify the location of the 'viewport' area using scroll bars.
  *
  * Header File:  draw_text.h
**/
void DTPreRender(Display *pDisplay, XFontSet fontSet, DT_WORDS *pWords, int iTabWidth, int iTabOrigin,
                 WB_RECT *prcBounds, int iAlignment, int iStartLine, int iEndLine);

/** \ingroup draw_text
  * \brief Using pre-rendered 'DT_WORDS' structure, display
  *
  * \param pDisplay the display associated with the GC and Drawable.
  * \param fontSet An XFontSet to use for drawing text (None implies system default font set)
  * \param pWords A pointer to the DT_WORDS structure.  This structure can be part of a linked list.
  * \param gc The graphics context for the Drawable
  * \param dw The Drawable (Window or Pixmap, typically)
  * \param iHScrollBy The amount to 'horizontally scroll by' in characters (positive values), or pixels (negative values)
  * \param iVScrollBy The amount to 'vertically scroll by' in characters (positive values), or pixels (negative values)
  * \param prcBounds A pointer to the bounding rectangle for pWords, the same rectangle calculated by DTPreRender()
  * \param prcViewport A pointer to the bounding rectangle for the viewport, for the Drawable 'dw'.
  * \param iAlignment The desired text alignment, one or more of the DTAlignment bit flags
  *
  * This function will scan the DT_WORDS structure 'pWords', and using the other parameters,
  * render the text within the 'prcBounds' rectangle on the specified Drawable.
  *
  * Header File:  draw_text.h
**/
void DTRender(Display *pDisplay, XFontSet fontSet, const DT_WORDS *pWords, GC gc, Drawable dw,
              int iHScrollBy, int iVScrollBy, const WB_RECT *prcBounds, const WB_RECT *prcViewport, int iAlignment);

#ifdef __cplusplus
};
#endif // __cplusplus


#endif // _DRAW_TEXT_H_INCLUDED_

