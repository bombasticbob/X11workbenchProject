///////////////////////////////////////////////////////////////////////////////////////////
//                                                                                       //
//        _        _                         _            _                     _        //
//     __| |  ___ | |__   _   _   __ _      | |__    ___ | | _ __    ___  _ __ | |__     //
//    / _` | / _ \| '_ \ | | | | / _` |     | '_ \  / _ \| || '_ \  / _ \| '__|| '_ \    //
//   | (_| ||  __/| |_) || |_| || (_| |     | | | ||  __/| || |_) ||  __/| | _ | | | |   //
//    \__,_| \___||_.__/  \__,_| \__, |_____|_| |_| \___||_|| .__/  \___||_|(_)|_| |_|   //
//                               |___/|_____|               |_|                          //
//                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////
//                                                                                       //
//   Copyright (c) 2010-2017 by 'Big Bad Bombastic Bob' Frazier - all rights reserved.   //
//  You may use this file in any way you see fit provided that any copy or derived work  //
//  includes the above copyright notice.                                                 //
//                                                                                       //
//     This header file contains debug-related functionality, some of which will be      //
//     in the release version of the library, and expanded in the debug version.         //
//                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DEBUG_HELPER_H_INCLUDED_
#define _DEBUG_HELPER_H_INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


//////////////////////////////////////////////////////////////////////////////////////////////
//   ____   _____  ____   _   _   ____    _   _  _____  _      ____   _____  ____   ____    //
//  |  _ \ | ____|| __ ) | | | | / ___|  | | | || ____|| |    |  _ \ | ____||  _ \ / ___|   //
//  | | | ||  _|  |  _ \ | | | || |  _   | |_| ||  _|  | |    | |_) ||  _|  | |_) |\___ \   //
//  | |_| || |___ | |_) || |_| || |_| |  |  _  || |___ | |___ |  __/ | |___ |  _ <  ___) |  //
//  |____/ |_____||____/  \___/  \____|  |_| |_||_____||_____||_|    |_____||_| \_\|____/   //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////


/** \ingroup debug
  * \enum DebugLevel
  * \brief Debug level enumeration
  *
  * Debug Level enumeration and subsystem idetification enumeration
  *
  * Bits 0 to 3 identify the level (0 = none, 7 = excessive)\n
  * The remaining bits identify the subsystem filters (no bits set for ALL)
  *
  * \sa WBParseStandardArguments() in platform_helper.c, 'aszDebugSubSys[]' array
  *
**/
enum DebugLevel
{
  DebugLevel_None = 0,      //!< none (no debug output)
  DebugLevel_ERROR = 0,     //!< errors (output whenever debug cmopiled in)
  DebugLevel_WARN = 1,      //!< warnings (all debug levels)

                            // criteria for selecting debug output
  DebugLevel_Minimal = 1,   //!< minimal, implies warnings and important information
  DebugLevel_Light = 2,     //!< light, implies basic/summary process/flow information
  DebugLevel_Medium = 3,    //!< medium, implies process/flow tracing
  DebugLevel_Heavy = 4,     //!< heavy, implies detailed process/flow tracing
  DebugLevel_Chatty = 5,    //!< chatty, implies details about flow decisions
  DebugLevel_Verbose = 6,   //!< verbose, implies details regarding information used for decision making
  DebugLevel_Excessive = 7, //!< excessive, implies more information that you probably want
  DebugLevel_MASK = 7,      //!< mask for allowed 'level' values (none through Excessive)

  // next are subsystem masks for additional debugging.
  // Also, see 'aszDebugSubSys' in WBParseStandardArguments(), platform_helper.c
  DebugSubSystem_ALL         = 0,           //!< 'ALL' is the default unless masked bits are non-zero
  DebugSubSystem_RESTRICT    = 0x80000000,  //!< only show specific subsystems (prevents zero masked value)
  DebugSubSystem_BITSHIFT    = 3,           //!< bit # for 'lowest' subsystem bit
  DebugSubSystem_Init        = 0x00000008,  //!< initialization/termination          "init"
  DebugSubSystem_Application = 0x00000010,  //!< application-level                   "application"
  DebugSubSystem_Window      = 0x00000020,  //!< generic window processing           "window"
  DebugSubSystem_Menu        = 0x00000040,  //!< generic menu processing             "menu"
  DebugSubSystem_Event       = 0x00000080,  //!< generic event processing            "event"
  DebugSubSystem_Dialog      = 0x00000100,  //!< dialog box (container window)       "dialog"
  DebugSubSystem_DialogCtrl  = 0x00000200,  //!< dialog controls                     "dialogctrl"
  DebugSubSystem_Frame       = 0x00000400,  //!< Frame windows                       "frame"
  DebugSubSystem_Keyboard    = 0x00000800,  //!< generic keyboard processing         "keyboard"
  DebugSubSystem_Mouse       = 0x00001000,  //!< generic mouse event processing      "mouse"
  DebugSubSystem_Font        = 0x00002000,  //!< font manager                        "font"
  DebugSubSystem_Settings    = 0x00004000,  //!< settings manager                    "settings"
  DebugSubSystem_Selection   = 0x00008000,  //!< selection processing (clipboard)    "selection"
  DebugSubSystem_Pixmap      = 0x00010000,  //!< pixmap handling                     "pixmap"
  DebugSubSystem_Expose      = 0x00020000,  //!< expose/paint handling               "expose"
  DebugSubSystem_EditWindow  = 0x00040000,  //!< edit window callbacks               "editwindow"
  DebugSubSystem_ScrollBar   = 0x00080000,  //!< edit window callbacks               "scrollbar"

  DebugSubSystem_MASK = ~7L  //!< mask for allowed 'subsystem' bits
};



/** \ingroup debug
  * \brief set debug level
  *
  * This function should typically be called once, at the beginning of the program.
  * Any changes made to the debug level will be applied 'live'
  *
  * Pass the debug level as 'iLevel'
  *   0 for none, 1 for minimal, 2 for more, etc. up to 7
  *
  * Header File:  debug_helper.h
**/
void WBSetDebugLevel(unsigned int iLevel);


/** \ingroup debug
  * \brief Returns the current debug level assigned by WBSetDebugLevel
  *
  * \return The current debug level and flags
  *
  * Various 'debug print' macros need to know what the current debug level
  * is in order to filter their output.  This function provides a standard
  * way in which to obtain the debug level.  It is defined 'inline' to
  * minimize any performance impact and to allow optimization.
  *
  *
  * Header File:  debug_helper.h
**/

#if defined(__GNUC__) || defined(_MSVC_VER)
static __inline__ unsigned int WBGetDebugLevel(void)
{
extern unsigned int iWBDebugLevel;

  return iWBDebugLevel;
}
#else // !defined(__GNUC__) && !defined(_MSVC_VER)
unsigned int WBGetDebugLevel(void);
#endif // defined(__GNUC__) || defined(_MSVC_VER)

/** \ingroup debug
  * \brief conditional debug message output
  *
  * \param pFmt A pointer to a 'printf' compatible format string
  *
  * Additional parameters are those required by the format string
  *
  * Sends output to the 'debug device' which is (by default) stderr using
  * a function call convention similar to 'printf'.
  *
  * This function is enabled even for a non-debug build.  To have conditional
  * debug output ONLY for a debug build (or given debug level), use the
  * various 'WB_DEBUG_PRINT()' (and related) macros.
  *
  * Header File:  debug_helper.h
**/
#ifdef __GNUC__
void WBDebugPrint(const char *pFmt, ...) __attribute__ ((format(printf, 1, 2)));
#else // __GNUC__
void WBDebugPrint(const char *pFmt, ...);
#endif // __GNUC__

/** \ingroup debug
  * \brief conditionally dumps binary data to debug message output
  *
  * \param szTitle The title of the debug dump
  * \param pData A pointer to the data to be dumped
  * \param cbData the number of bytes to dump
  *
  * Allows you to generate a custom 'binary data dump' in an easily
  * readable hex/ASCII format, using the debug message output
  *
  * This function is enabled even for a non-debug build.  To have conditional
  * debug output ONLY for a debug build (or given debug level), use the
  * various 'WB_DEBUG_PRINT()' (and related) macros.
  *
  * Header File:  debug_helper.h
**/
void WBDebugDump(const char *szTitle, void *pData, int cbData); // dump binary data

/** \ingroup debug
  * \brief dumps a GC's settings
  *
  * \param pDisplay A pointer to the display
  * \param hGC The graphics context to dump
  *
  * Debug function that dumps out all of the settings for a GC that can be
  * retrieved using normal API functions.
  *
  * This function is enabled even for a non-debug build.  To have conditional
  * debug output ONLY for a debug build (or given debug level), use the
  * various 'WB_DEBUG_PRINT()' (and related) macros.
  *
  * Header File:  debug_helper.h
**/
void WBDebugDumpGC(Display *pDisplay, GC hGC);

/** \ingroup debug
  * \brief dumps contents of a region
  *
  * \param hRgn The region to dump
  * \param bRotate True if you want to rotate the output 90 degrees (useful for very WIDE regions)
  *
  * Debug function that dumps out the contents of a region
  *
  * This function is enabled even for a non-debug build.  To have conditional
  * debug output ONLY for a debug build (or given debug level), use the
  * various 'WB_DEBUG_PRINT()' (and related) macros.
  *
  * Header File:  debug_helper.h
**/
void WBDebugDumpRegion(Region hRgn, int bRotate);


/** \ingroup debug
  * \brief dumps the contents of an XEvent
  *
  * \param pEvent an Event structure obtained via 'XNextEvent' or similar
  *
  * Debug function that dumps out all of the members of the XNextEvent.  It assumes
  * that the event is genuine; that is, the 'display' 'window' etc. are valid.
  *
  * This function is enabled even for a non-debug build.  To have conditional
  * debug output ONLY for a debug build (or given debug level), use the
  * various 'WB_DEBUG_PRINT()' (and related) macros.
  *
  * Header File:  debug_helper.h
**/
void WBDebugDumpEvent(XEvent *pEvent);


/** \ingroup debug
  * \brief Check specified debug level against the value returned by WBGetDebugLevel() and return non-zero for a match
  *
  * \param dwLevel A combination of one or more 'DebugLevel_' and 'DebugSubSystem_' flags to check
  * \return A non-zero value if the specified debug level matches that returned by WBGetDebugLevel(), zero otherwise.
  *
  * Various 'debug print' macros need to know what the current debug level
  * is in order to filter their output.  This function provides a standard
  * way in which to obtain the debug level.  It is defined 'inline' to
  * minimize any performance impact and to allow optimization.
  *
  * Header File:  debug_helper.h
**/
#if defined(__GNUC__) || defined(_MSVC_VER)
static __inline__ int WBCheckDebugLevel(unsigned int dwLevel)
{
extern unsigned int iWBDebugLevel;

  if(WB_LIKELY((iWBDebugLevel & DebugLevel_MASK) < (dwLevel & DebugLevel_MASK)))
  {
    return 0;
  }

  if(!WB_UNLIKELY( WBGetDebugLevel() & DebugSubSystem_RESTRICT )) // RESTRICT not specified
  {
    if(!(dwLevel & DebugSubSystem_MASK) ) // no subsystem specified in debug output
    {
      return 1; // this is acceptable - since no subsystem specified, allow debug output if not 'RESTRICT'
    }
  }

  // at this point I have a debug subsystem 'RESTRICT' specified

  if(((dwLevel & DebugSubSystem_MASK) & (iWBDebugLevel & DebugSubSystem_MASK))
     != 0) // check to see that subsystem bits in 'dwLevel' match bits in 'iWBDebugLevel'
  {
    // at least one subsystem bit matches from 'dwLevel' and iWBDebugLevel
    return 1;
  }

  return 0;
}
#else // !defined(__GNUC__) && !defined(_MSVC_VER)
int WBGetDebugLevel(void);
#endif // defined(__GNUC__) || defined(_MSVC_VER)



// NOTE:  The debug code will be included when NO_DEBUG is *NOT* defined

#ifdef NO_DEBUG /* assign this to disable debugging - most likely a -D in Makefile */
#define WB_DEBUG_PRINT(...)
#define WB_DEBUG_DUMP(L,X,Y,Z)
#define WB_IF_DEBUG_LEVEL(L) if(0) /* TODO:  turn off the warning? */
#else // NO_DEBUG

// NOTE:  this macro is the preferred method of implementing debug output.
/** \ingroup debug
  * \brief Preferred method of implementing conditional debug output
  *
  * \param L Debug level (bitmask of \ref DebugLevel enumeration)
  *
  * The remaining parameters are passed as-is to \ref WBDebugPrint()
  *
  * This macro conditionally generates debug output via WBDebugPrint().
  * Bits that are set in the debug level enable you to determine which debug messages
  * you see and which ones remain 'hidden'.  This macro easily allows you to implement
  * filtered debug output.  The first paramater, 'L', is the debug level (0 to 7)
  * along with (optional) bits indicating which subsystems are applicable to this message.
  * Specifying NO subsystem bits assumes ALL subsystems.\n
  * The remaining parameters, format string (and a variable number of parameters), are
  * passed along to the WBDebugPrint function.\n
  * \sa \ref DebugLevel
**/
#define WB_DEBUG_PRINT(L, ...) \
    WB_IF_DEBUG_LEVEL(L) { WBDebugPrint(__VA_ARGS__); }

/** \ingroup debug
  * \brief Preferred method of implementing conditional debug 'dump' output
  *
  * \param L Debug level (bitmask of \ref DebugLevel enumeration)
  * \param X A const pointer to a 0-byte terminated string containing the 'title' string
  * \param Y A const void pointer to the data to be dumped
  * \param Z An unsigned integer indicating the number of bytes to display
  *
  * The remaining parameters are passed as-is to \ref WBDebugDump()
  *
  * Similar to \ref WB_DEBUG_PRINT, this macro will conditionally dump binary data
  * via the WBDebugDump() function.  The first parameter, 'L', is the debug level
  * (0 to 7) along with (optional) bits indicating which subsystems are applicable
  * to this debug output.  Specifying NO subsystem bits assumes ALL subsystems.\n
  * The remainng 3 parameters are passed to WBDebugDump as the title, data pointer,
  * and size of the data (in bytes).\n
  * \sa \ref DebugLevel
**/
#define WB_DEBUG_DUMP(L,X,Y,Z) \
    WB_IF_DEBUG_LEVEL(L) { WBDebugDump(X,Y,Z); }

/** \ingroup debug
  * \brief Preferred method of implementing conditional debug 'if block' code
  *
  * \param L Debug level (bitmask of \ref DebugLevel enumeration)
  *
  * Similar to \ref WB_DEBUG_PRINT, this macro will conditionally execute a statement
  * or block of code that follows.  Example:
  *
  * \code
  WB_IF_DEBUG_LEVEL(DebugLevel_WARN | DebugSubSystem_Keyboard)
  {
    // do something for warnings or higher debug level
  }
  * \endcode
  * \sa \ref DebugLevel
**/
#define WB_IF_DEBUG_LEVEL(L) if(WBCheckDebugLevel((L)))

#endif // NO_DEBUG


/** \ingroup debug
  * \brief Preferred method of implementing a 'warning level' debug message for all subsystems
  *
  * Preferred method of implementing a 'warning level' debug message for all subsystems
**/
#define WB_WARN_PRINT(...) WB_DEBUG_PRINT(DebugLevel_WARN, __VA_ARGS__)

/** \ingroup debug
  * \brief Preferred method of implementing an 'error level' debug message for all subsystems
  *
  * Preferred method of implementing an 'error level' debug message for all subsystems
**/
#define WB_ERROR_PRINT(...) WB_DEBUG_PRINT(DebugLevel_ERROR, __VA_ARGS__)

/** \ingroup debug
  * \brief Preferred method of implementing a 'warning level' binary dump for all subsystems
  *
  * \param X A const pointer to a 0-byte terminated string containing the 'title' string
  * \param Y A const void pointer to the data to be dumped
  * \param Z An unsigned integer indicating the number of bytes to display
  *
  * Preferred method of implementing a 'warning level' binary dump for all subsystems
**/
#define WB_WARN_DUMP(X,Y,Z) WB_DEBUG_DUMP(DebugLevel_WARN, X,Y,Z)

/** \ingroup debug
  * \brief Preferred method of implementing an 'error level' binary dump for all subsystems
  *
  * \param X A const pointer to a 0-byte terminated string containing the 'title' string
  * \param Y A const void pointer to the data to be dumped
  * \param Z An unsigned integer indicating the number of bytes to display
  *
  * Preferred method of implementing an 'error level' binary dump for all subsystems
**/
#define WB_ERROR_DUMP(X,Y,Z) WB_DEBUG_DUMP(DebugLevel_ERROR, X,Y,Z)


#ifdef __cplusplus
};
#endif // __cplusplus

#endif // _DEBUG_HELPER_H_INCLUDED_


