/////////////////////////////////////////////////////////////////////////////////////
//                                                                                 //
//                     _               __                                  _       //
//   _ __ ___    __ _ (_) _ __        / _| _ __  __ _  _ __ ___    ___    | |__    //
//  | '_ ` _ \  / _` || || '_ \      | |_ | '__|/ _` || '_ ` _ \  / _ \   | '_ \   //
//  | | | | | || (_| || || | | |     |  _|| |  | (_| || | | | | ||  __/ _ | | | |  //
//  |_| |_| |_| \__,_||_||_| |_|_____|_|  |_|   \__,_||_| |_| |_| \___|(_)|_| |_|  //
//                             |_____|                                             //
//                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////
//                                                                                 //
//             Copyright (c) 2020 by S.F.T. Inc. - All rights reserved             //
//      Use, copying, and distribution of this software are licensed according     //
//        to the GPLv2, LGPLv2, or BSD license, as appropriate (see COPYING)       //
//                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_FRAME_H_INCLUDED_
#define _MAIN_FRAME_H_INCLUDED_

// headers needed to avoid errors
#include "window_helper.h"
#include "frame_window.h"
#include "edit_window.h"

#ifdef __cplusplus__
extern "C"
{
#endif // __cplusplus__

extern const char szAppMenu[];
extern const char szEditMenu[];

void get_min_window_height_width(int *piMinHeight, int *piMinWidth);

WBFrameWindow *DoCreateMainFrameWindow(const char *szTitle, int idIcon,
                                       const char *pszMenu, unsigned long ulFlags);

WBEditWindow * DoCreateEditChildFrame(WBFrameWindow *pFrame);


#ifdef __cplusplus__
}
#endif // __cplusplus__

#endif // _MAIN_FRAME_H_INCLUDED_

