//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                                                                          //
//                 __ _  _ __  _ __  __ _  _   _  ___     ___               //
//                / _` || '__|| '__|/ _` || | | |/ __|   / __|              //
//               | (_| || |   | |  | (_| || |_| |\__ \ _| (__               //
//                \__,_||_|   |_|   \__,_| \__, ||___/(_)\___|              //
//                                         |___/                            //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//          Copyright (c) 2020 by S.F.T. Inc. - All rights reserved         //
//  Use, copying, and distribution of this software are licensed according  //
//    to the GPLv2, LGPLv2, or BSD license, as appropriate (see COPYING)    //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////


#include "arrays.h"

WBArrayObject * WBArray(int nObjectSize, int nInitialSize,
                        void *(* placement_new)(int cbSize, void *pMember),
                        void (* placement_delete)(void *pMember))
{
  return NULL; // for now
}

void WBArrayAddRef(WBArrayObject *pArrayObj);
{
}

void WBArrayRelease(WBArrayObject *pArrayObj);
{
}

uint32_t WBArrayGetSize(WBArrayObject *pArrayObj);
{
  return 0; // for now
}

const void * WBArrayGetAt(const WBArrayObject *pArrayObj, int nIndex);
{
  return NULL; // for now
}

int WBArrayInsert(WBArrayObject *pArrayObj, int nIndex, void *pObject);
{
  return -1; // for now
}

void * WBArrayRemove(WBArrayObject *pArrayObj, void *pObject, int nIndex);
{
  return NULL; // for now
}


