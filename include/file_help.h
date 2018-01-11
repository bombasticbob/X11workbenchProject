//////////////////////////////////////////////////////////////////////////////////////////
//                  __  _  _             _            _          _                      //
//                 / _|(_)| |  ___      | |__    ___ | | _ __   | |__                   //
//                | |_ | || | / _ \     | '_ \  / _ \| || '_ \  | '_ \                  //
//                |  _|| || ||  __/     | | | ||  __/| || |_) |_| | | |                 //
//                |_|  |_||_| \___|_____|_| |_| \___||_|| .__/(_)_| |_|                 //
//                                |_____|               |_|                             //
//                                                                                      //
//                         helper utilities for file I/O                                //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////

/*****************************************************************************

    X11workbench - X11 programmer's 'work bench' application and toolkit
    Copyright (c) 2010-2016 by Bob Frazier (aka 'Big Bad Bombastic Bob')
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


#ifndef _FILE_HELP_H_INCLUDED_
#define _FILE_HELP_H_INCLUDED_

// read a file into a buffer, returning the buffer and its size

#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


// use NAME_MAX to determine max buffer sizes for things that use a file name
// maximum path name limits are OS dependent.

#ifdef WIN32
#ifndef NAME_MAX
#define NAME_MAX MAX_PATH
#endif // NAME_MAX
#else // !WIN32
// POSIX systems normally define NAME_MAX in limits.h (or something it includes)
#ifndef NAME_MAX
#ifdef MAXNAMLEN
#define NAME_MAX MAXNAMLEN /* BSD defines MAXNAMLEN */
#else
#define NAME_MAX 255 /* standard value for BSD and Linux */
#endif // MAXNAMLEN
#endif // NAME_MAX
#endif // WIN32, !WIN32


// sub-groups for documentation

/** \defgroup file_help_buf File Buffer 'helper' functions
  * \ingroup file_help
  * Utility functions and structures associated with line-based buffered file I/O.
  *
  * An abstracted set of API functions that allow you to manage line-based text editing
  * using a basic structure as a 'File Buffer' object.
**/

/** \defgroup file_help_io File I/O 'helper' functions
  * \ingroup file_help
  * Utility functions and structures associated with files, file info and directory searching
  *
  * File helper utilities that enable you to more easily enumerate the contents of a directory,
  * make a backup copy of an existing file, get a file's type or permissions, or canonicalize a file name.\n
**/

/** \ingroup file_help_buf
  * \struct __file_help_buf__
  * \copydoc file_help_buf_t
**/
/** \ingroup file_help_buf
  * \typedef file_help_buf_t
  * \brief basic 'buffered I/O' object structure for 'FileBuf' APIs
  *
  * This structure is intended to assist implementing buffered I/O for files
  * that contain multiple lines separated by linefeeds.  A 'file edit'
  * window or dialog control could use one of these objects to assist with
  * displaying and editing text based on the contents of a file.  It would
  * also be possible to construct a \ref file_help_buf_t object from a
  * memory buffer as well as from a text file.
  *
  * \code

  typedef struct __file_help_buf__
  {
    struct __file_help_buf__ *pPrev; // pointer to 'previous' item in linked list (NULL for owner object)
    struct __file_help_buf__ *pNext; // pointer to 'next' item in linked list (NULL for last object)
    long lBufferSize;                // size of entire buffer
    long lBufferCount;               // number of bytes of valid data
    long lLineCount;                 // number of lines in 'cData' when ppLineBuf not NULL
    long lLineBufSize;               // size of memory block pointed to by 'ppLineBuf'
    int  iFlags;                     // various bit flags
    char **ppLineBuf;                // array of pointers to beginning of each line (WBAlloc'd TODO: make it part of 'cData'?)
    char cData[sizeof(char *)];      // the data itself (aligned to size of a pointer)
  } file_help_buf_t;

  * \endcode
  *
  * TODO:  'ppLineBuf' should point to an area within cData.  If 'pNext' is valid, subsequent lines
  * could be pointed to by the 'ppLineBuf' within the structure pointed to by 'pNext'.  In this
  * way line pointers can be re-structured without re-allocating memory.  Additionally, there is
  * no reason why lines must be contiguous, so inserting and removing lines can make use of the
  * additional allocated space pointed to by 'pNext'.\n
  * As it would become fragmented, it would also be a good idea to re-build the structure
  * periodically using FBParseFileBuf()
  *
**/
typedef struct __file_help_buf__
{
  struct __file_help_buf__ *pPrev; ///< pointer to 'previous' item in linked list (NULL for owner object)
  struct __file_help_buf__ *pNext; ///< pointer to 'next' item in linked list (NULL for last object)
  long lBufferSize;                ///< size of entire buffer
  long lBufferCount;               ///< number of bytes of valid data
  long lLineCount;                 ///< number of lines in 'cData' when ppLineBuf not NULL
  long lLineBufSize;               ///< size of memory block pointed to by 'ppLineBuf'
  int  iFlags;                     ///< various bit flags
  char **ppLineBuf;                ///< array of pointers to beginning of each line (WBAlloc'd TODO: make it part of 'cData'?)
  char cData[sizeof(char *)];      ///< the data itself (aligned to size of a pointer)
} file_help_buf_t;

/** \ingroup file_help_buf
  * \enum __file_help_buf_flags__
  * \hideinitializer
  * \brief bit flags for \ref file_help_buf_t 'iFlags' member
  *
  * These are the bit flag definitions for the \ref file_help_buf_t 'iFlags' member
**/
enum __file_help_buf_flags__
{
  file_help_buf_dirty      = 1,    ///< TRUE when file is 'dirty' (not written)
  file_help_buf_reserved01 = 2,    ///< reserved
  file_help_buf_reserved02 = 4,    ///< reserved
  file_help_buf_reserved03 = 8,    ///< reserved
  file_help_buf_reserved04 = 16,   ///< reserved
  file_help_buf_reserved05 = 32,   ///< reserved
  file_help_buf_reserved06 = 64,   ///< reserved
  file_help_buf_reserved07 = 128,  ///< reserved
  file_help_buf_reserved08 = 256,  ///< reserved

  file_help_buf_last       = 0x80000000L
};

/** \ingroup file_help_buf
  * \brief Inline function, returns TRUE if \ref file_help_buf_t is 'dirty'
  *
  * \param pBuf A valid pointer to a file_help_buf_t
  * \returns Non-zero if the buffer has the 'dirty' flag set, zero otherwise.
  *
  * Use this function (in lieu of directly checking iFlags) to determine if the
  * contents of the \ref file_help_buf_t is 'dirty' and needs to be written to disk.
**/
static __inline__ int FBIsFileBufDirty(const file_help_buf_t *pBuf)
{
  return pBuf && (pBuf->iFlags & file_help_buf_dirty);
}

/** \ingroup file_help_buf
  * \brief Construct a \ref file_help_buf_t from a file handle
  *
  * \param iHandle A handle to an open file (must allow random access).
  * \returns A valid pointer to a file_help_buf_t, or NULL on error
  *
  * Use this function to construct a \ref file_help_buf_t from a file handle.
  * On success the function returns a valid pointer, that must be subsequently
  * destroyed using \ref FBDestroyFileBuf().\n
  * On error, this function returns a NULL pointer.
**/
file_help_buf_t *FBGetFileBufViaHandle(int iHandle);

/** \ingroup file_help_buf
  * \brief Construct a \ref file_help_buf_t from a buffer
  *
  * \param pBuf A pointer to a memory buffer that contains the text data
  * \param cbBuf The length of the data pointed to by 'pBuf'
  * \returns A valid pointer to a file_help_buf_t, or NULL on error
  *
  * Use this function to construct a \ref file_help_buf_t from a buffer.  Suitable
  * for 'memory only' operations.
  * On success the function returns a valid pointer, that must be subsequently
  * destroyed using \ref FBDestroyFileBuf().\n
  * On error, this function returns a NULL pointer.
**/
file_help_buf_t *FBGetFileBufFromBuffer(const char *pBuf, long cbBuf);

/** \ingroup file_help_buf
  * \brief Construct a \ref file_help_buf_t from a file
  *
  * \param szFileName An ASCII string containing the file name
  * \returns A valid pointer to a file_help_buf_t, or NULL on error
  *
  * Use this function to construct a \ref file_help_buf_t from a named file.
  * On success the function returns a valid pointer, that must be subsequently
  * destroyed using \ref FBDestroyFileBuf().\n
  * On error, this function returns a NULL pointer.
**/
file_help_buf_t *FBGetFileBuf(const char *szFileName);


/** \ingroup file_help_buf
  * \brief Destroy a \ref file_help_buf_t object
  *
  * \param pFB A valid pointer to a file_help_buf_t
  *
  * Use this function to destroy a \ref file_help_buf_t constructured by one of the
  * 'FB' constructor utilities \ref FBGetFileBufViaHandle(), \ref FBGetFileBufFromBuffer(),
  * or \ref FBGetFileBuf().
**/
void FBDestroyFileBuf(file_help_buf_t *pFB);


/** \ingroup file_help_buf
  * \brief Parse or Re-Parse the data for a \ref file_help_buf_t object
  *
  * \param pFB A valid pointer to a file_help_buf_t
  * \returns 0 on success, non-zero on error
  *
  * Use this function to parse or re-parse the data within the \ref file_help_buf_t
  * object.  This will re-allocate memory as needed to accomplish the desired goal.\n
  * The function returns 0 on success, non-zero on error.\n
  * The resulting parsed data is placed into the 'ppLineBuf' member.
**/
int FBParseFileBuf(file_help_buf_t *pFB);
  // parses or re-parses buffer, populating ppLineBuf

/** \ingroup file_help_buf
  * \brief Write the \ref file_help_buf_t object's text data to a file using a filename
  *
  * \param szFileName An ASCII file name to which to write the text data (an existing file of the same name will be overwritten)
  * \param pFB A valid pointer to a \ref file_help_buf_t
  * \returns 0 on success, non-zero on error
  *
  * Use this function to write the text data for a \ref file_help_buf_t object into a
  * file using the filename.  This also clears the 'dirty' flag.\n
  * The function returns 0 on success, non-zero on error.\n
**/
int FBWriteFileBuf(const char *szFileName, const file_help_buf_t *pFB);

/** \ingroup file_help_buf
  * \brief Write the \ref file_help_buf_t object's text data to a file using an open file handle
  *
  * \param iHandle An open file handle to the output file (requires random access)
  * \param pFB A valid pointer to a \ref file_help_buf_t
  * \returns 0 on success, non-zero on error
  *
  * Use this function to write the text data for a \ref file_help_buf_t object into a
  * file using an open file handle.  This also clears the 'dirty' flag.\n
  * The function returns 0 on success, non-zero on error.\n
**/
int FBWriteFileBufHandle(int iHandle, const file_help_buf_t *pFB);


/** \ingroup file_help_buf
  * \brief Insert text into a \ref file_help_buf_t object at a specific byte offset
  *
  * \param ppBuf A valid pointer to a pointer variable for a \ref file_help_buf_t object\n
  * This parameter is also used for the returned pointer to the \ref file_help_buf_t, which may change as needed.
  * \param cbOffset The offset within the text for insertion.  This must NOT split MBCS or UTF-8 characters!
  * \param pData A pointer to the text data to be inserted
  * \param cbData The number of bytes of text data to insert.
  *
  * Use this function to insert a specific number of bytes of text into a \ref file_help_buf_t object
  * at a specific byte offset.\n
  * This function sets the 'dirty' flag.
**/
void FBInsertIntoFileBuf(file_help_buf_t **ppBuf, long cbOffset, const void *pData, long cbData);

/** \ingroup file_help_buf
  * \brief Delete text from a \ref file_help_buf_t object at a specific byte offset
  *
  * \param pBuf A valid pointer to a \ref file_help_buf_t
  * \param cbOffset The offset within the text for insertion.  This must NOT split MBCS or UTF-8 characters!
  * \param cbDelFrom The number of bytes of text to remove.  This must NOT split MBCS or UTF-8 characters!
  *
  * Use this function to delete a specified number of bytes of text from a \ref file_help_buf_t object
  * at a specific byte offset\n
  * This function sets the 'dirty' flag.
**/
void FBDeleteFromFileBuf(file_help_buf_t *pBuf, long cbOffset, long cbDelFrom);


/** \ingroup file_help_buf
  * \brief Insert a line of text into a \ref file_help_buf_t object at a specific line index
  *
  * \param ppBuf A valid pointer to a pointer variable for a \ref file_help_buf_t object\n
  * This parameter is also used for the returned pointer to the \ref file_help_buf_t, which may change as needed.
  * \param lLineNum The 0-based line index at which to perform the insert.  Specifying an index greater
  * than or equal to the total number of lines inserts the line at the end of the text.
  * \param szLine A pointer to a line of ASCII text.  An ending line feed, if missing, is implied.
  *
  * Use this function to insert a line of text into a \ref file_help_buf_t object
  * at a specific 0-based line number.\n
  * This function sets the 'dirty' flag.\n
  * NOTE: this is a line-based abstraction for \ref FBInsertIntoFileBuf().  Automatically re-parses as needed
**/
void FBInsertLineIntoFileBuf(file_help_buf_t **ppBuf, long lLineNum, const char *szLine);


/** \ingroup file_help_buf
  * \brief Delete a line of text from a \ref file_help_buf_t object at a specific line index
  *
  * \param pBuf A valid pointer to a \ref file_help_buf_t
  * \param lLineNum The 0-based line index at which to perform the delete.  An index greater than or
  * equal to the total number of lines will be ignored.
  *
  * Use this function to delete a line of text from a \ref file_help_buf_t object
  * at a specific 0-based line number.\n
  * This function sets the 'dirty' flag.\n
  * NOTE: this is a line-based abstraction for FBDeleteFromFileBuf().  Automatically re-parses as needed
**/
void FBDeleteLineFromFileBuf(file_help_buf_t *pBuf, long lLineNum);


/** \ingroup file_help_buf
  * \brief Insert a line of text into a \ref file_help_buf_t object at a specific line index
  *
  * \param ppBuf A valid pointer to a pointer variable for a \ref file_help_buf_t object\n
  * This parameter is also used for the returned pointer to the \ref file_help_buf_t, which may change as needed.
  * \param lLineNum The 0-based line index at which to replace text.  Specifying an index greater
  * than or equal to the total number of lines inserts the line at the end of the text, similar to \ref FBInsertLineIntoFileBuf().
  * \param szLine A pointer to a line of ASCII text.  An ending line feed, if missing, is implied.
  *
  * Use this function to replace a line of text within a \ref file_help_buf_t object
  * at a specific 0-based line number.\n
  * This function sets the 'dirty' flag.\n
  * NOTE: this is a line-based abstraction for both \ref FBInsertIntoFileBuf() and \ref FBDeleteFromFileBuf().
  * Automatically re-parses as needed.
**/
void FBReplaceLineInFileBuf(file_help_buf_t **ppBuf, long lLineNum, const char *szLine);


/** \ingroup file_help_buf
  * \brief Write a file from a regular buffer using a file name
  *
  * \param szFileName A const pointer to a 0-byte terminated string containing a file name
  * \param pBuf A const pointer to a buffer containing data to write, or NULL if no data is to be written
  * \param cbBuf The byte count to write from 'pBuf'.
  * \returns A value of zero if successful, non-zero on error
**/
static __inline__ int FBWriteFileFromBuffer(const char *szFileName, const char *pBuf, unsigned int cbBuf)
{
int iRval;
file_help_buf_t *pFB;


  pFB = FBGetFileBufFromBuffer(pBuf, cbBuf);

  if(!pFB)
  {
//    WB_ERROR_PRINT("ERROR - %s - unable to create file buf\n", __FUNCTION__);
    return -1;
  }

  iRval = FBWriteFileBuf(szFileName, pFB);

  FBDestroyFileBuf(pFB);

//  if(iRval < 0)
//  {
//    WB_ERROR_PRINT("ERROR - %s - unable to create file \"%s\" from file buf\n", __FUNCTION__, szFileName);
//  }

  return iRval;
}



/** \ingroup file_help_io
  * \brief read a file's contents into a buffer, returning the length of the buffer
  *
  * \param szFileName A const pointer to a string containing the file name
  * \param ppBuf A pointer to a 'char *' buffer that is allocated via WBAlloc() and returned by the function
  * \returns a positive value on success indicating the length of the data in the returned buffer, or negative on error.
  *  A return value of zero indicates an empty file.
  *
  * Use this function to read the entire contents of a file into a memory buffer.
**/
size_t WBReadFileIntoBuffer(const char *szFileName, char **ppBuf);

/** \ingroup file_help_io
  * \brief read a file's contents into a buffer, returning the length of the buffer
  *
  * \param szFileName A const pointer to a string containing the file name
  * \param pBuf A const pointer to a buffer that contains the data to write
  * \param cbBuf The length of data to write to the file.
  * \returns a value of zero on success, or non-zero on error (the actual error should be in 'errno')
  *
  * Use this function to write the entire contents of a buffer to a file.  The file will be overwritten
  * if it already exists.
**/
int WBWriteFileFromBuffer(const char *szFileName, const char *pBuf, size_t cbBuf);


// SYSTEM INDEPENDENT FILE STATUS, LISTINGS, AND INFORMATION

/** \ingroup file_help_io
  * \brief replicate permissions on a target file based on another file's permissions
  *
  * \param szProto The file to be used as a permission 'prototype'.  If it represents a symbolic
  * link, the file that the link points to will be used.
  * \param szTarget The target file.  If it represents a symbolic link, the link itself will be used.
  * \returns non-zero on error, zero on success
  *
  * Use this function to replicate the permissions of one file onto another.  Useful when
  * making a backup of an original file, and/or creating a new file with the same name/characteristics
  * as the old file, but not necessarily 'written in place'.
**/
int WBReplicateFilePermissions(const char *szProto, const char *szTarget);

/** \ingroup file_help_io
  * \brief Return allocated string containing the current working directory
  *
  * \returns valid pointer or NULL on error
  *
  * A convenience function that wraps the 'getcwd()' API and returns a 'WBAlloc'd pointer to a string.
  * The caller must free any non-NULL return values using WBFree()
**/
char *WBGetCurrentDirectory(void);

/** \ingroup file_help_io
  * \brief Return whether a file is a directory or a symlink to a directory
  *
  * \param szName The file name to query. Can be a symbolic link.  The name [and target path for a symlink] must exist.
  * \returns non-zero if the file is (or the symlink points to) a directory, 0 otherwise
  *
  * Use this function to determine if the specified file is a directory, or is a symbolic
  * link that points to a directory.
**/
int WBIsDirectory(const char *szName);

/** \ingroup file_help_io
  * \brief Return the canonical path for a file name (similar to POSIX 'realpath()' funtion)
  *
  * \param szFileName The file name to query.  If the name contains symbolic links, they will be resolved.
  * \returns A WBAlloc'd buffer containing the canonical path, or NULL on error.
  *
  * Use this function to determine the canonical name for a specified path.  If the path does not exist,
  * those elements of the path that DO exist will be resolved, and the remaining parts of the name will
  * be added to form the canonical path.  If an element of the specified directory name is NOT actually
  * a directory (or a symlink to a directory), the function will return NULL indicating an error.
  *
  * The caller must free any non-NULL return values using WBFree()
**/
char *WBGetCanonicalPath(const char *szFileName);

/** \ingroup file_help_io
  * \brief Allocate a 'Directory List' object for a specified directory spec
  *
  * \param szDirSpec The directory specification, using standard wildcard specifiers on the file spec only
  * \returns A pointer to a 'Directory List' object, or NULL on error
  *
  * Use this function to list a directory by creating a 'Directory List' object and
  * then subsequently passing it to 'WBNextDirectoryEntry' to obtain information about
  * each entry in the directory.  This function does NOT return '.' or '..' as valid
  * file names.\n
  *
  * The pointer returned by this function must be destroyed using \ref WBDestroyDirectoryList()\n
  *
  * NOTE:  you may not specify a wildcard within a directory name unless it is the the specification
  * for the file name that you are searching for within its parent directory.  The directory name
  * must also exist or the function will return a NULL (error).  If the specified path name ends
  * in a '/' the file spec will be assumed to be '*'.
**/
void *WBAllocDirectoryList(const char *szDirSpec);

/** \ingroup file_help_io
  * \brief Destroy a 'Directory List' object allocated by \ref WBAllocDirectoryList()
  *
  * \param pDirectoryList A pointer to a 'Directory List' object allocated by \ref WBAllocDirectoryList()
  *
  * Use this function to destroy a 'Directory List' object allocated by \ref WBAllocDirectoryList()
**/
void WBDestroyDirectoryList(void *pDirectoryList);

/** \ingroup file_help_io
  * \brief Obtain information about the next entry in a 'Directory List'
  *
  * \param pDirectoryList A pointer to the 'Directory List' object
  * \param szNameReturn A pointer to a buffer that receives the file name
  * \param cbNameReturn The size of the 'szNameReturn' buffer (in bytes)
  * \param pdwModeAttrReturn A pointer to an unsigned long integer that receives the mode/attribute bits
  * \returns An integer value < 0 on error, > 0 on EOF, or 0 if a matching entry was found
  *
  * Use this function to get the next file name from the contents of a directory listing that is
  * specified by a 'Directory List' object created by \ref WBAllocDirectoryList()
**/
int WBNextDirectoryEntry(void *pDirectoryList, char *szNameReturn,
                         int cbNameReturn, unsigned long *pdwModeAttrReturn);

/** \ingroup file_help_io
  * \brief Construct a fully qualified canonical path from a 'Directory List' object and a file name
  *
  * \param pDirectoryList A pointer to a 'Directory List' object.  May be NULL.
  * \param szFileName A pointer to a file within the directory.  This file does not need to exist.
  * \returns A 'WBAlloc'd buffer containing the fully qualified file name as an ASCII string.
  *
  * Use this function to get a canonical file name for a file within a directory listing.
  *
  * The caller must free any non-NULL return values using WBFree()
**/
char *WBGetDirectoryListFileFullPath(const void *pDirectoryList, const char *szFileName);

/** \ingroup file_help_io
  * \brief Obtain the target of a symbolic link
  *
  * \param szFileName A pointer to the file name of the symbolic link
  * \returns A 'WBAlloc'd buffer containing the link target path as an ASCII string (unaltered)
  *
  * Use this function to retrieve a symbolic link's target file name.  May be a relative path.
  * For a canonical equivalent, use WBGetCanonicalPath()
  *
  * The caller must free any non-NULL return values using WBFree()
**/
char *WBGetSymLinkTarget(const char *szFileName);

/** \ingroup file_help_io
  * \brief Obtain the target of a symbolic link file name with respect to a 'Directory List' object
  *
  * \param pDirectoryList A pointer to a 'Directory List' object.  May be NULL.
  * \param szFileName A pointer to the file name of the symbolic link
  * \returns A 'WBAlloc'd buffer containing the unmodified link target path as an ASCII string
  *
  * Use this function to retrieve a symbolic link's target file name with respect to a 'Directory List'
  * object (for relative paths).  The link target is unmodified, and may be a relative path.
  * For a canonical equivalent, use WBGetCanonicalPath()
  *
  * The caller must free any non-NULL return values using WBFree()
**/
char *WBGetDirectoryListSymLinkTarget(const void *pDirectoryList, const char *szFileName);

/** \ingroup file_help_io
  * \brief Obtain the 'stat' flags for a file name, resolving links as needed
  *
  * \param szFileName A pointer to the file name of the symbolic link
  * \param pdwModeAttrReturn A pointer to the unsigned long integer that receives the 'stat' flags
  * \returns A zero value if successful, non-zero otherwise (same return as 'stat()')
  *
  * Use this function to retrieve flags for a regular file, directory, or the target of a symbolic link
**/
int WBStat(const char *szFileName, unsigned long *pdwModeAttrReturn);

/** \ingroup file_help_io
  * \brief Obtain the 'stat' flags for a file name, resolving links as needed, with respect to a 'Directory List' object
  *
  * \param pDirectoryList A pointer to a 'Directory List' object.  May be NULL.
  * \param szFileName A pointer to the file name of the symbolic link
  * \param pdwModeAttrReturn A pointer to the unsigned long integer that receives the 'stat' flags
  * \returns A zero value if successful, non-zero otherwise (same return as 'stat()')
  *
  * Use this function to retrieve flags for a regular file, directory, or the target of a symbolic link with
  * respect to a 'Directory List' object (for relative paths)
**/
int WBGetDirectoryListFileStat(const void *pDirectoryList, const char *szFileName,
                               unsigned long *pdwModeAttrReturn);


#ifdef __cplusplus
};
#endif // __cplusplus


#endif // _FILE_HELP_H_INCLUDED_

