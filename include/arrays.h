//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                                                       _                  //
//                __ _  _ __  _ __  __ _  _   _  ___    | |__               //
//               / _` || '__|| '__|/ _` || | | |/ __|   | '_ \              //
//              | (_| || |   | |  | (_| || |_| |\__ \ _ | | | |             //
//               \__,_||_|   |_|   \__,_| \__, ||___/(_)|_| |_|             //
//                                        |___/                             //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//          Copyright (c) 2020 by S.F.T. Inc. - All rights reserved         //
//  Use, copying, and distribution of this software are licensed according  //
//    to the GPLv2, LGPLv2, or BSD license, as appropriate (see COPYING)    //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////


#ifndef _ARRAYS_H_INCLUDED_
#define _ARRAYS_H_INCLUDED_

#include "platform_helper.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/** \struct tagARRAY_OBJECT
  * \ingroup arrays
  * \copydoc WBArrayObject
**/
/** \typedef WBArrayObject
  * \ingroup arrays
  * \brief Internal structure for Dynamic Array Object
  *
  * \code

  typedef struct tagARRAY_OBJECT
  {
    uint64_t llRefCount;       // thread-safe reference counter

    uint32_t nSize,            // current array size
             nPreAlloc;        // pre-allocated size

    void *(* placement_new)(size_t cbSize, void *pMember);      // 'placement new' (like C++)
      // 'pMember' must be a pointer to the memory block, cbSize is the size of that block
    void (* placement_delete)(void *pMember, void *pObjData);   // 'placement delete' (like C++)
      // like with C++, first param is pointer to mem block, 2nd is pointer to 'object of this type'

    uint8_t aData[sizeof(uint32_t)];   // actual data follows
  } WBArrayObject;

  * \endcode
**/
typedef struct tagARRAY_OBJECT
{
  uint64_t llRefCount;       ///< thread-safe reference counter

  uint32_t nSize,            ///< current array size
           nPreAlloc;        ///< pre-allocated size

  void *(* placement_new)(size_t cbSize, void *pMember);      ///< 'placement new' - like with C++, 'pMember' must be a pointer to the memory block, cbSize is the size of that block
  void (* placement_delete)(void *pMember, void *pObjData);   ///< 'placement delete' - like with C++, first param is pointer to mem block, 2nd is pointer to 'object of this type'

  uint8_t aData[sizeof(uint32_t)];   ///< actual data follows
} WBArrayObject;


/** \ingroup arrays
  * \brief Dynamic Array Object creator, C language compatible with C++
  *
  * \param nObjectSize The length of memory for each object
  * \param nInitialSize Pre-allocated size of the array
  * \param placement_new A C++ compatible 'placement new' constructor to initialize each new element with (can be NULL)
  * \param placement_delete A C++ compatible 'placement delete' destructor to free each deleted element with (can be NULL)
  * \returns A pointer to the allocated array as a \ref WBArrayObject.  You must call WBArrayRelease() on a non-NULL pointer return.
  *
  * Whenever an element is created, the 'placement_new' will be called (if not NULL).  Afterwards, the object will be copied using memcpy.
  * The default placement_new is a 'memset' to zero.  The default 'placement_delete' does nothing.
  * For objects that are not compatible to doing things this way, you should allocate an array of pointers and manage them separately.
  *
  * Header File:  arrays.h
**/
WBArrayObject * WBArray(int nObjectSize, int nInitialSize,
                        void *(* placement_new)(int cbSize, void *pMember),
                        void (* placement_delete)(void *pMember));

/** \ingroup arrays
  * \brief Dynamic Array Object - increase reference count
  *
  * \param pArrayObject A pointer to the \ref WBArrayObject
  *
  * Header File:  arrays.h
**/
void WBArrayAddRef(WBArrayObject *pArrayObj);

/** \ingroup arrays
  * \brief Dynamic Array Object - decrease reference count, destroy if zero
  *
  * \param pArrayObject A pointer to the \ref WBArrayObject
  *
  * Header File:  arrays.h
**/
void WBArrayRelease(WBArrayObject *pArrayObj);

/** \ingroup arrays
  * \brief Dynamic Array Object - get array size
  *
  * \param pArrayObject A pointer to the \ref WBArrayObject
  * \returns The size of the array (current number of entries in it)
  *
  * Header File:  arrays.h
**/
uint32_t WBArrayGetSize(WBArrayObject *pArrayObj);

/** \ingroup arrays
  * \brief Dynamic Array Object - Get element at specified position
  *
  * \param pArrayObject A pointer to the \ref WBArrayObject
  * \param nIndex The index of the desired entry.  '-1' is last, 0 is first.
  * \returns A pointer to the object stored in the array.  The object itself is stored there.  If the object is a pointer, it must be de-referenced accordingly.
  *
  * Header File:  arrays.h
**/
const void * WBArrayGetAt(const WBArrayObject *pArrayObj, int nIndex);

/** \ingroup arrays
  * \brief Dynamic Array Object - Get element at specified position
  *
  * \param pArrayObject A pointer to the \ref WBArrayObject
  * \param nIndex The index of the desired entry.  '-1' is last, 0 is first.
  * \param pObject A pointer to the object to be stored.  If the object array consists of an array of pointers, this is the address of the new pointer to add, not the value of the pointer itself.
  *
  * \returns 0 on success, or an error code on error
  *
  * This function will allocate space (as needed) and then do a memcpy into the appropriate 'slot' from pObject, using the size of the element as the # of bytes to copy.
  * If you are adding a pointer to an array of pointers, keep in mind that you must pass the address of the pointer, and not the value of the pointer, as 'pObject'.
  *
  * Whenever an element is created, the 'placement_new' will be called (if not NULL).  Afterwards, the object will be copied using memcpy.
  * The default placement_new is a 'memset' to zero.  The default 'placement_delete' does nothing.
  * For objects that are not compatible to doing things this way, you should allocate an array of pointers and manage them separately.
  *
  * Header File:  arrays.h
**/
int WBArrayInsert(WBArrayObject *pArrayObj, int nIndex, void *pObject);

/** \ingroup arrays
  * \brief Dynamic Array Object - Remove an element at specified position
  *
  * \param pArrayObject A pointer to the \ref WBArrayObject
  * \param nIndex The index of the desired entry.  '-1' is last, 0 is first.
  * \returns A pointer to a copy of the data that was just removed (This pointer will be different from the one previously located within the array)
  *
  * Whenever an element is removed, the 'placement_delete' will be called (if not NULL).  This function will also return a pointer to a copy of the
  * object as it was AFTER the placement delete operator was called on it.  This memory block is transient and could be overwritten by another call
  * to 'WBArrayRemove()' if another thread is also deleting elements from the array.  In a sense, it is advisory only and probably only useful for
  * debug and single-thread access of the \ref WBArrayObject.
  *
  * Header File:  arrays.h
**/
void * WBArrayRemove(WBArrayObject *pArrayObj, void *pObject, int nIndex);



#ifdef __cplusplus
};
#endif // __cplusplus

#endif // _ARRAYS_H_INCLUDED_
