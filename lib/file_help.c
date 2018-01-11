//////////////////////////////////////////////////////////////////////////////////////////
//                  __  _  _             _            _                                 //
//                 / _|(_)| |  ___      | |__    ___ | | _ __     ___                   //
//                | |_ | || | / _ \     | '_ \  / _ \| || '_ \   / __|                  //
//                |  _|| || ||  __/     | | | ||  __/| || |_) |_| (__                   //
//                |_|  |_||_| \___|_____|_| |_| \___||_|| .__/(_)\___|                  //
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


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <dirent.h>
#include <fnmatch.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h> // for MAXPATHLEN and PATH_MAX (also includes limits.h in some cases)

#include "file_help.h"
#include "draw_text.h" // for string utilities, mostly
#include "window_helper.h" // for debug stuff


#ifndef NO_DEBUG
// debug function to dump parsed contents of file_help_buf_t
static void FBDumpParsedFileBuf(file_help_buf_t *pBuf)
{
int i1;
const char *p1;

  if(WB_UNLIKELY((WBGetDebugLevel() & DebugLevel_MASK) >= ((DebugLevel_Chatty) & DebugLevel_MASK)))
  {
    WBDebugPrint("dumping file buf - %d lines\n", (int)pBuf->lLineCount);

    for(i1=0; i1 < (int)pBuf->lLineCount; i1++)
    {
      p1 = pBuf->ppLineBuf[i1 + 1];
      if(!p1)
      {
        p1 = pBuf->ppLineBuf[i1] + strlen(pBuf->ppLineBuf[i1]);
      }

      while(p1 > pBuf->ppLineBuf[i1] && (*(p1 - 1) == '\n' || *(p1 - 1) == '\r'))
      {
        p1--;
      }

      if(p1 > pBuf->ppLineBuf[i1])
      {
        WBDebugPrint("%3d:  \"%.*s\"..\n", i1, (int)(p1 - pBuf->ppLineBuf[i1]), pBuf->ppLineBuf[i1]);
      }
      else
      {
        WBDebugPrint("%3d:  [empty]\n", i1);
      }
    }
  }
}
#endif // NO_DEBUG

file_help_buf_t *FBGetFileBuf(const char *szFileName)
{
  int iFile;
  file_help_buf_t *pRval;

  iFile = open(szFileName, O_RDONLY);
  if(iFile < 0)
  {
    return NULL;
  }

  pRval = FBGetFileBufViaHandle(iFile);
  close(iFile);

  return pRval;
}

file_help_buf_t *FBGetFileBufViaHandle(int iFile)
{
  file_help_buf_t *pRval;
  long lFileSize;

  lFileSize = (long)lseek(iFile, 0, SEEK_END);
  lseek(iFile, 0, SEEK_SET);

  if(lFileSize < 0)
  {
    return NULL;
  }

  pRval = (file_help_buf_t *)WBAlloc(sizeof(*pRval) + ((lFileSize + 256 + 128) & 0xffffff00L));

  if(pRval)
  {
    bzero(pRval, sizeof(*pRval)); // this also takes care of pNext, pPrev

    pRval->lBufferSize = sizeof(*pRval) + ((lFileSize + 256 + 128) & 0xffffff00L);
    pRval->lBufferCount = lFileSize;
    pRval->iFlags = 0;

    if(lFileSize)
    {
      if(read(iFile, pRval->cData, lFileSize) != lFileSize)
      {
        WBFree(pRval);
        pRval = NULL;
      }
    }
  }

  return pRval;
}

file_help_buf_t *FBGetFileBufFromBuffer(const char *pBuf, long cbBuf)
{
  file_help_buf_t *pRval;

  if(cbBuf < 0)
  {
    return NULL;
  }

  pRval = (file_help_buf_t *)WBAlloc(sizeof(*pRval) + ((cbBuf + 256 + 128) & 0xffffff00L));
  if(pRval)
  {
    bzero(pRval, sizeof(*pRval)); // this also takes care of pNext, pPrev

    pRval->lBufferSize = sizeof(*pRval) + ((cbBuf + 256 + 128) & 0xffffff00L);
    pRval->lBufferCount = cbBuf;
    pRval->iFlags = 0;

    if(cbBuf && pBuf)
    {
      memcpy(pRval->cData, pBuf, cbBuf);
    }
  }

  return pRval;
}

void FBDestroyFileBuf(file_help_buf_t *pBuf)
{
  if(!pBuf)
  {
    return;
  }

  if(pBuf->pNext)
  {
    FBDestroyFileBuf(pBuf->pNext); // recursively destroy (TODO:  loop instead?)
    pBuf->pNext = NULL;
  }

  // TODO:  ppLineBuf should be part of 'cData'.  For now it's WBAlloc'd

  if(pBuf->ppLineBuf)
  {
    WBFree(pBuf->ppLineBuf);
  }

  WBFree(pBuf);
}

int FBParseFileBuf(file_help_buf_t *pBuf)
{
  int i1, iLines;
  const char *p1, /* *p2,*/ *pEnd;

  // TODO:  ppLineBuf should be part of 'cData'.  For now it's WBAlloc'd

  if(pBuf->ppLineBuf)
  {
    WBFree(pBuf->ppLineBuf);
    pBuf->ppLineBuf = NULL;
    pBuf->lLineCount = 0;
    pBuf->lLineBufSize = 0;
  }

  // count the lines first
  for(i1=0, p1 = pBuf->cData, pEnd = pBuf->cData + pBuf->lBufferCount; p1 < pEnd; )
  {
// NOTE:  p2 not being used; commented out because of linux gcc warnings
//    p2 = p1;
    while(p1 < pEnd && *p1 != '\n')
    {
      p1++;
    }
    if(p1 < pEnd)
    {
      p1++; // skipping past the newline
    }

    i1++; // this counts the last line regardless of the '\n' being there or not
          // and works regardless of compiler behavior outside of a 'for' loop
  }

  // now allocate memory and REALLY assign the pointers
  iLines = i1;

  // TODO:  ppLineBuf should be part of 'cData'.  For now it's WBAlloc'd
  //        if I need room for more lines, attach another file_help_buf_t
  //        and chain it to 'pNext'

  pBuf->ppLineBuf = (char **)WBAlloc((iLines + 1) * sizeof(char **));
  if(!pBuf->ppLineBuf)
  {
    return -1;
  }

  pBuf->lLineBufSize = (iLines + 1) * sizeof(char **);
  pBuf->lLineCount = iLines;

  for(i1=0, p1 = pBuf->cData, pEnd = pBuf->cData + pBuf->lBufferCount; p1 < pEnd && i1 < iLines; i1++)
  {
    pBuf->ppLineBuf[i1] = (char *)p1;

    while(p1 < pEnd && *p1 != '\n')
    {
      p1++;
    }
    if(p1 < pEnd)
    {
      p1++; // skipping past the newline
    }
  }

  pBuf->ppLineBuf[iLines] = NULL;  // always (to mark "the end")

#ifndef NO_DEBUG
  FBDumpParsedFileBuf(pBuf);
#endif // NO_DEBUG

  return 0;
}

int FBWriteFileBuf(const char *szFileName, const file_help_buf_t *pBuf)
{
  int iFile, iRval;

  if(!pBuf)
  {
    return -1;
  }

  iFile = open(szFileName, O_CREAT | O_TRUNC | O_RDWR, 0666);  // always create with mode '666' (umask should apply)

  if(iFile < 0)
  {
    return -1;
  }

  iRval = FBWriteFileBufHandle(iFile, pBuf);
  close(iFile);

  return(iRval);
}

int FBWriteFileBufHandle(int iFile, const file_help_buf_t *pBuf)
{
  if(!pBuf)
  {
    return -1;
  }

  lseek(iFile, 0, SEEK_SET); // rewind

  if(pBuf->lBufferCount > 0)
  {
    if(write(iFile, pBuf->cData, pBuf->lBufferCount)
       != pBuf->lBufferCount)
    {
      return -2;
    }
  }

  ftruncate(iFile, pBuf->lBufferCount); // ensure file size is correct

  ((file_help_buf_t *)pBuf)->iFlags &= ~file_help_buf_dirty;
  return pBuf->lBufferCount;
}

static int SanityCheckFileBuf(file_help_buf_t *pBuf, const char *szFunction)
{
  if(!pBuf || pBuf->lBufferSize < sizeof(*pBuf)
     || pBuf->lBufferCount > pBuf->lBufferSize + sizeof(pBuf->cData) - sizeof(*pBuf))
  {
    WB_ERROR_PRINT("%s - error in file buf - %p %ld %ld (%ld)\n",
                   szFunction,
                   pBuf,
                   pBuf ? pBuf->lBufferSize : 0L,
                   pBuf ? pBuf->lBufferCount : 0L,
                   pBuf ? pBuf->lBufferSize + sizeof(pBuf->cData) - sizeof(*pBuf) : 0L);
    return -1;
  }

  return 0;
}

static int InternalGrowFileBuf(file_help_buf_t **ppBuf, long cbOffset, long cbData)
{ // return value is negative on error
  long lNewSize, lActualBufSize;
  void *pNew;

  if(!ppBuf)
  {
    WB_ERROR_PRINT("NULL 'ppBuf' in InternalGrowFileBuf\n");
    return -1;
  }

  if(SanityCheckFileBuf(*ppBuf, __FUNCTION__) < 0)
  {
    return -1;
  }

  lActualBufSize = (*ppBuf)->lBufferSize - sizeof(**ppBuf); // exclude 'cData' for the moment

  if(cbOffset > (*ppBuf)->lBufferCount)
  {
    lNewSize = cbOffset + cbData;
  }
  else
  {
    lNewSize = (*ppBuf)->lBufferCount + cbData;
  }

  if(lActualBufSize >= lNewSize)
  {
    return 0;  // no re-allocation
  }

  lNewSize = (lNewSize + sizeof(*ppBuf) + 256 + 128) & 0xffffff00L;

  WB_DEBUG_PRINT(DebugLevel_Excessive, "TEMPORARY:  reallocating - %d %d %d\n",
                 (int)(*ppBuf)->lBufferSize, (int)lActualBufSize, (int)lNewSize);

  pNew = (char *)WBReAlloc(*ppBuf, lNewSize);

  if(pNew)
  {
    *ppBuf = (file_help_buf_t *)pNew;

    (*ppBuf)->lBufferSize = lNewSize;

    // it's been my experience that the additional allocated memory may be uninitialized,
    // so zero it out, starting with the end of valid data.

    lActualBufSize = lNewSize - sizeof(**ppBuf) + sizeof((*ppBuf)->cData);

    if((*ppBuf)->lBufferCount < lActualBufSize) // just in case, test for this
    {
      memset((char *)((*ppBuf)->cData) + (*ppBuf)->lBufferCount, 0, lActualBufSize - (*ppBuf)->lBufferCount);
    }

    return lNewSize;  // returns the NEW size
  }

  WB_ERROR_PRINT("error re-allocating file buf\n");
  return -2;  // error re-allocating
}

void FBInsertIntoFileBuf(file_help_buf_t **ppBuf, long cbOffset, const void *pData, long cbData)
{
  // insert 'cbData' bytes of 'pData' at 'cbOffset' within *ppBuf, possibly re-allocating the
  // buffer and re-assigning it to *ppBuf.

  if(InternalGrowFileBuf(ppBuf, cbOffset, cbData) < 0)
  {
    return;
  }

  if((*ppBuf)->lBufferCount < cbOffset) // insert PAST THE END of the buffer
  {
    memset((char *)((*ppBuf)->cData) + (*ppBuf)->lBufferCount, '\n', cbOffset - (*ppBuf)->lBufferCount); // pad with newlines
    (*ppBuf)->lBufferCount = cbOffset; // since I effectively increased the data count to THIS
  }
  else if((*ppBuf)->lBufferCount > cbOffset) // insert into the middle of the buffer
  {
    memmove((char *)((*ppBuf)->cData) + cbOffset + cbData, (char *)((*ppBuf)->cData) + cbOffset, (*ppBuf)->lBufferCount - cbOffset);
  }

  memcpy((char *)((*ppBuf)->cData) + cbOffset, pData, cbData);
  (*ppBuf)->lBufferCount += cbData; // new buffer data count

  SanityCheckFileBuf(*ppBuf, __FUNCTION__);

  (*ppBuf)->iFlags |= file_help_buf_dirty;
}

void FBDeleteFromFileBuf(file_help_buf_t *pBuf, long cbOffset, long cbDelFrom)
{
  // remove 'cbDelFrom' bytes of data from 'pBuf' starting at offset 'cbOffset'

  if(cbOffset >= pBuf->lBufferCount || cbDelFrom <= 0)
  {
    return;
  }

  if(cbOffset + cbDelFrom > pBuf->lBufferCount)
  {
    cbDelFrom = pBuf->lBufferCount - cbOffset;
  }
  else if(cbOffset + cbDelFrom < pBuf->lBufferCount)
  {
    memcpy(pBuf->cData + cbOffset, pBuf->cData + cbOffset + cbDelFrom,
           pBuf->lBufferCount - cbOffset - cbDelFrom);
  }

  pBuf->lBufferCount -= cbDelFrom;
  memset(pBuf->cData + pBuf->lBufferCount, 0, cbDelFrom); // zero out what WAS there

  SanityCheckFileBuf(pBuf, __FUNCTION__);

  pBuf->iFlags |= file_help_buf_dirty;
}

void FBInsertLineIntoFileBuf(file_help_buf_t **ppBuf, long lLineNum, const char *szLine)
{
  int i1, i2;
  long lOffset;
  char *pDest;

  // if 'lLineNum' exceeds the current line count, add lines until it matches

  i2 = strlen(szLine);
  while(i2 && (szLine[i2 - 1] == '\n' || szLine[i2 - 1] == '\r')) // trim <return> <newline> [will add later]
  {
    i2--;  // I'll always need to have a newline, so don't include the newline if it already has one
  }

  if(!(*ppBuf)->ppLineBuf) // make sure it has been parsed, FIRST
  {
    FBParseFileBuf(*ppBuf);
    if(!(*ppBuf)->ppLineBuf)
    {
      WB_ERROR_PRINT("%s - no line buffer!\n", __FUNCTION__);
      return;  // can't (some kind of error)
    }
  }

  i1 = lLineNum - (*ppBuf)->lLineCount; // lLineNum is zero-based
  if(i1 < 0) // line number is valid
  {
    i1 = 0;

    pDest = (*ppBuf)->ppLineBuf[lLineNum];
    if(!pDest)
    {
      pDest = (*ppBuf)->cData + (*ppBuf)->lBufferCount;
    }
  }
  else
  {
    pDest = (*ppBuf)->cData + (*ppBuf)->lBufferCount + i1;  // TODO:  how do I properly deal with 'in between' lines?
  }

  lOffset = (char *)pDest - (char *)((*ppBuf)->cData); // byte offset of the insertion point

  FBInsertIntoFileBuf(ppBuf, lOffset, szLine, i2); // make room for 2 extra chars, hence 'i2 + 2'
  FBInsertIntoFileBuf(ppBuf, lOffset + i2, "\r\n", 2); // the trailing CRLF

  if(i1 > 0)
  {
    // fix added newlines, and make sure that 'szLine' ends with a newline as well

    WB_DEBUG_PRINT(DebugLevel_Chatty, "%s inserting %d blank lines\n", __FUNCTION__, i1);
    pDest = (*ppBuf)->cData + lOffset;  // re-assign since 'ppBuf' may have changed

    while(i1 > 0) // inserting newline chars for 'in between' blank lines
    {
      *(pDest - i1) = '\n';
      i1--;
    }
  }

  // re-parse file

  FBParseFileBuf(*ppBuf); // re-parse at the end
}

void FBDeleteLineFromFileBuf(file_help_buf_t *pBuf, long lLineNum)
{
//  int i1, i2;
  long lOffset, lEndOffset;
//  char *pDest;

  if(!pBuf->ppLineBuf)
  {
    FBParseFileBuf(pBuf);
  }

  // if 'lLineNum' exceeds the current line count, just return

  if(!pBuf->ppLineBuf || lLineNum > pBuf->lLineCount)
  {
    return;
  }
  if(lLineNum == pBuf->lLineCount)
  {
    lEndOffset = pBuf->lBufferCount;
  }
  else
  {
    lEndOffset = (char *)pBuf->ppLineBuf[lLineNum + 1] - (char *)(pBuf->cData);
  }

  lOffset = (char *)pBuf->ppLineBuf[lLineNum] - (char *)(pBuf->cData);

  if(lOffset < lEndOffset)
  {
    FBDeleteFromFileBuf(pBuf, lOffset, lEndOffset - lOffset);
  }

  FBParseFileBuf(pBuf);
}

void FBReplaceLineInFileBuf(file_help_buf_t **ppBuf, long lLineNum, const char *szLine)
{
  // TODO:  make this more efficient
  FBDeleteLineFromFileBuf(*ppBuf, lLineNum);
  FBInsertLineIntoFileBuf(ppBuf, lLineNum, szLine);
}


// FILE SYSTEM INDEPENDENT FILE AND DIRECTORY UTILITIES
// UNIX/LINUX versions - TODO windows versions?

size_t WBReadFileIntoBuffer(const char *szFileName, char **ppBuf)
{
off_t cbLen;
size_t cbF;
int cb1, iChunk;
char *pBuf;
int iFile;


  if(!ppBuf)
  {
    return (size_t)-1;
  }

  iFile = open(szFileName, O_RDONLY); // open read only (assume no locking for now)

  if(iFile < 0)
  {
    return (size_t)-1;
  }

  // how long is my file?

  cbLen = (unsigned long)lseek(iFile, 0, SEEK_END); // location of end of file

  if(cbLen == (off_t)-1)
  {
    *ppBuf = NULL; // make sure
  }
  else
  {
    lseek(iFile, 0, SEEK_SET); // back to beginning of file

    *ppBuf = pBuf = WBAlloc(cbLen + 1);

    if(!pBuf)
    {
      cbLen = (off_t)-1; // to mark 'error'
    }
    else
    {
      cbF = cbLen;

      while(cbF > 0)
      {
        iChunk = 1048576; // 1MByte at a time

        if(iChunk > cbF)
        {
          iChunk = (int)cbF;
        }

        cb1 = read(iFile, pBuf, iChunk);

        if(cb1 == -1)
        {
          if(errno == EAGAIN) // allow this
          {
            usleep(100);
            continue; // for now just do this
          }

          cbLen = -1;
          break;
        }
        else if(cb1 != iChunk) // did not read enough bytes
        {
          iChunk = cb1; // for now
        }

        WBDebugPrint("TEMPORARY:  read %d bytes\n", (int)iChunk);

        cbF -= iChunk;
        pBuf += iChunk;
      }
    }
  }

  close(iFile);

  WBDebugPrint("TEMPORARY WBReadFileIntoBuffer:  return %d  buf=%p\n", (int)cbLen, *ppBuf);

  return (size_t) cbLen;
}

int WBWriteFileFromBuffer(const char *szFileName, const char *pBuf, size_t cbBuf)
{
int iFile, iRval, iChunk;


  if(!pBuf)
  {
    return -1;
  }

  iFile = open(szFileName, O_CREAT | O_TRUNC | O_RDWR, 0666);  // always create with mode '666' (umask should apply)

  if(iFile < 0)
  {
    return -1;
  }

  while(cbBuf > 0)
  {
    // write chunks of 1Mb or size remaining

    iChunk = 1048576;
    if(iChunk > cbBuf)
    {
      iChunk = (int)cbBuf;
    }

    iRval = write(iFile, pBuf, iChunk);

    if(iRval < 0)
    {
      if(errno == EAGAIN)
      {
        usleep(100);

        // TODO:  time limit?  for now, no

        continue; // try again
      }

      close(iFile);
      return -1; // error
    }
    else if(iRval != iChunk) // TODO:  allow this??
    {
      WBDebugPrint("TEMPORARY:  writing file, only wrote %d of %d bytes\n", iRval, iChunk);
      iChunk = iRval;
    }

    pBuf += iChunk;
    cbBuf -= iChunk;
  }

  iRval = 0; // at this point, success!

  close(iFile);

  return iRval;
}

int WBReplicateFilePermissions(const char *szProto, const char *szTarget)
{
struct stat sb;
int iRval = 0;

  iRval = stat(szProto, &sb); // TODO:  lstat for symlink?
  if(!iRval)
  {
    // TODO:  chflags?
    // TODO:  what if it's a symlink?
    iRval = chmod(szTarget, sb.st_mode & 0777); // only set the rwx permissions, and ignore others
    if(!iRval)
    {
      if(geteuid() == 0 || getuid() == sb.st_uid) // only do this if owner matches or I'm root
      {
        iRval = chown(szTarget, sb.st_uid, sb.st_gid);
        if(iRval < 0 && geteuid() != 0)
        {
          iRval = chown(szTarget, -1, sb.st_gid); // don't change the user

          if(iRval < 0)
          {
            // don't bother changing anything - just warn??
            iRval = 0;  // for now...
          }
        }
      }
    }
  }

  return iRval;
}

char *WBGetCurrentDirectory(void)
{
char *pRval = WBAlloc(MAXPATHLEN + 2);
int i1;

  if(pRval)
  {
    if(!getcwd(pRval, MAXPATHLEN))
    {
      WBFree(pRval);
      pRval = NULL;
    }
  }

  // this function will always return something that ends in '/' (except on error)

  if(pRval)
  {
    i1 = strlen(pRval);

    if(i1 > 0 && pRval[i1 - 1] != '/')
    {
      pRval[i1] = '/';
      pRval[i1 + 1] = 0;
    }
  }

  return pRval;
}

int WBIsDirectory(const char *szFileName)
{
int bRval = 0;

#ifdef WIN32
WIN32_FIND_DATA fd;
HANDLE hFF;

  hFF = FindFirstFile(szFileName, &fd);

  if(hFF != INVALID_HANDLE_VALUE)
  {
    bRval = (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
    FindClose(hFF);
  }

#else // WIN32
struct stat sF;

  if(!stat(szFileName, &sF)) // NOTE:  'stat' returns info about symlink targets, not the link itself
    bRval = S_ISDIR(sF.st_mode);

#endif // WIN32

  return(bRval);
}

char *WBGetCanonicalPath(const char *szFileName)
{
char *pTemp, *p1, *p2, *p3, *p4, *pRval = NULL;
struct stat sF;

  pTemp = WBCopyString(szFileName);

  if(!pTemp)
  {
    return NULL;
  }

  // step 1:  eliminate // /./

  p1 = pTemp;
  while(*p1 && p1[1])
  {
    if(*p1 == '/' && p1[1] == '/')
    {
      memmove(p1, p1 + 1, strlen(p1 + 1) + 1);
    }
    else if(*p1 == '/' && p1[1] == '.' && p1[2] == '/')
    {
      memmove(p1, p1 + 2, strlen(p1 + 2) + 1);
    }
    else
    {
      p1++;
    }
  }

  // step 2:  resolve each portion of the path, deal with '~' '.' '..', build new path.

  if(*pTemp == '~' && (pTemp[1] == '/' || !pTemp[1])) // first look for '~' at the beginning (only allowed there)
  {
    p1 = getenv("HOME");
    if(!p1 || !*p1) // no home directory?
    {
      *pTemp = '.';  // for now change it to '.'
    }
    else
    {
      p3 = WBCopyString(p1);
      if(!p3)
      {
        WBFree(pTemp);
        return NULL;
      }

      if(p3[strlen(p3) - 1] != '/')
      {
        WBCatString(&p3, "/");
      }

      p2 = pTemp + 1;
      if(*p2 == '/')
      {
        p2++; // already have an ending / on the path
      }

      if(*p2)
      {
        WBCatString(&p3, p2);
      }

      WBFree(pTemp);
      pTemp = p3;
    }
  }

  p1 = pTemp;
  while(*p1)
  {
    p2 = strchr(p1, '/');
    if(!p2)
    {
      if(*p1 == '.') // check for ending in '.' or '..' and add a '/' so I can handle it correctly
      {
        if((p1[1] == '.' && !p1[2]) || !p1[1])
        {
          p2 = pTemp; // temporary
          WBCatString(&pTemp, "/");

          p1 = (p1 - p2) + pTemp; // restore relative pointer

          WB_ERROR_PRINT("TEMPORARY:  %s  %s\n", p1, pTemp);

          continue; // let's do this again, properly
        }
      }

      // no more paths, so this is "the name".
      if(!pRval) // no existing path, use CWD
      {
        pRval = WBGetCurrentDirectory();

        if(!pRval)
        {
          break;
        }
      }

      WBCatString(&pRval, p1);

      break;
    }
    else if(p2 == p1)
    {
      pRval = WBCopyString("/");
    }
    else
    {
      if(!pRval)
      {
        pRval = WBGetCurrentDirectory();
        if(!pRval)
        {
          break;
        }
      }

      // when I assemble these paths together, deal with '..' and
      // symbolic links.  Check for cyclic paths.

      if(p2 - p1 == 1 && p1[0] == '.') // the ./ path
      {
        p1 = p2 + 1; // just ignore this part
        continue;
      }
      else if(p2 - p1 == 2 && p1[0] == '.' && p1[1] == '.') // the ../ path
      {
        p1 = p2 + 1; // I need to fix the path while ignoring the '../' part

        p3 = pRval + strlen(pRval) - 1; // NOTE:  pRval ends in '/' and I want the one BEFORE that
        while(p3 > pRval)
        {
          if(*(p3 - 1) == '/')
          {
            *p3 = 0;
            break;
          }

          p3--;
        }

        if(p3 <= pRval) // did not find a preceding '/' - this is an error
        {
          WB_ERROR_PRINT("%s:%d - did not find preceding '/' - %s\n", __FUNCTION__, __LINE__, pRval);

          WBFree(pRval);
          pRval = NULL;

          break;
        }

        continue;
      }

      // TEMPORARY:  just copy as-is to test basic logic

      WBCatStringN(&pRval, p1, p2 - p1 + 1); // include the '/' at the end
      if(!pRval)
      {
        WB_ERROR_PRINT("%s:%d - WBCatStringN returned NULL pointer\n", __FUNCTION__, __LINE__);

        break;
      }

      // see if this is a symbolic link.  exclude testing '/'

      p3 = pRval + strlen(pRval) - 1;
      if(p3 > pRval)
      {
        *p3 = 0; // temporary
        if(lstat(pRval, &sF)) // get the file 'stat' and see if we're a symlink
        {
          // error, does not exist? - leave it 'as-is' for now
          *p3 = '/';  // restore it
        }
        else if(S_ISDIR(sF.st_mode)) // an actual directory - remains as-is
        {
          // don't do anything except restore the '/'
          *p3 = '/';  // restore it
        }
        else if(S_ISLNK(sF.st_mode)) // symlink
        {
          // now I get to put the symlink contents "in place".  If the symlink is
          // relative to the current directory, I'll want that.

          p4 = (char *)WBAlloc(MAXPATHLEN + 2);

          if(!p4)
          {
            WB_ERROR_PRINT("%s:%d - not enough memory for buffer\n", __FUNCTION__, __LINE__);

            WBFree(pRval);
            pRval = NULL;
            break;
          }
          else
          {
            int iLen = readlink(pRval, p4, MAXPATHLEN);

            if(iLen <= 0)
            {
              WB_ERROR_PRINT("%s:%d - readlink returned %d for %s\n", __FUNCTION__, __LINE__, iLen, pRval);

              WBFree(p4);
              WBFree(pRval);
              pRval = NULL;

              break;
            }

            p4[iLen] = 0; // assume < MAXPATHLEN for now...
            if(p4[0] == '/') // it's an absolute path
            {
              WBFree(pRval);
              pRval = p4;
            }
            else
            {
              while(p3 > pRval && *(p3 - 1) != '/') // scan back for a '/'
              {
                p3--;
              }

              *p3 = 0;
              WBCatString(&pRval, p4); // sub in the relative path
              WBFree(p4);
            }

            if(!WBIsDirectory(pRval)) // must be a directory!
            {
              WB_ERROR_PRINT("%s:%d - %s not a directory\n", __FUNCTION__, __LINE__, pRval);

              WBFree(pRval);
              pRval = NULL;
              break; // this is an error
            }
            else
            {
              WBCatString(&pRval, "/");

              if(pRval)
              {
                p4 = WBGetCanonicalPath(pRval); // recurse

                WBFree(pRval);
                pRval = p4; // new canonical version of symlink path
              }

              if(!pRval)
              {
                WB_ERROR_PRINT("%s:%d - NULL pRval\n", __FUNCTION__, __LINE__);

                break;
              }
            }
          }
        }
      }
    }

    p1 = p2 + 1;
  }

  // if the resulting path is a symbolic link, fix it
  if(pRval)
  {
    p1 = pRval + strlen(pRval) - 1;

    if(p1 > pRval && *p1 != '/') // does not end in a slash, so it should be a file...
    {
      while(p1 > pRval && *(p1 - 1) != '/')
      {
        p1--;
      }

      if(!lstat(pRval, &sF)) // get the file 'stat' and see if we're a symlink (ignore errors)
      {
        if(S_ISDIR(sF.st_mode)) // an actual directory - end with a '/'
        {
          WBCatString(&pRval, "/"); // add ending '/'
        }
        else if(S_ISLNK(sF.st_mode)) // symlink
        {
          // now I get to put the symlink contents "in place".  If the symlink is
          // relative to the current directory, I'll want that.

          p4 = (char *)WBAlloc(MAXPATHLEN + 2);

          if(!p4)
          {
            WB_ERROR_PRINT("%s:%d - not enough memory\n", __FUNCTION__, __LINE__);

            // TODO:  assign pRval to NULL ?
          }
          else
          {
            int iLen = readlink(pRval, p4, MAXPATHLEN);

            if(iLen <= 0)
            {
              WB_ERROR_PRINT("%s:%d - readlink returned %d for %s\n", __FUNCTION__, __LINE__, iLen, pRval);

              WBFree(p4);
              WBFree(pRval);
              pRval = NULL;
            }
            else
            {
              p4[iLen] = 0; // assume < MAXPATHLEN for now...
              if(p4[0] == '/') // it's an absolute path
              {
                WBFree(pRval); // new path for old
                pRval = p4;
              }
              else
              {
                p3 = pRval + strlen(pRval); // I won't be ending in '/' for this part so don't subtract 1
                while(p3 > pRval && *(p3 - 1) != '/') // scan back for the '/' in symlink's original path
                {
                  p3--;
                }

                *p3 = 0;
                WBCatString(&pRval, p4); // sub in the relative path
                WBFree(p4);
              }

              if(pRval && WBIsDirectory(pRval)) // is the result a directory?
              {
                WBCatString(&pRval, "/");
              }

              if(pRval)
              {
                p4 = WBGetCanonicalPath(pRval); // recurse to make sure I'm canonical (deal with '..' and '.' and so on)

                WBFree(pRval);
                pRval = p4; // new canonical version of symlink path
              }
            }
          }
        }
      }
    }
  }

  if(pTemp)
  {
    WBFree(pTemp);
    pTemp = NULL; // by convention
  }

  if(!pRval)
  {
    WB_ERROR_PRINT("%s:%d - returning NULL\n", __FUNCTION__, __LINE__);
  }

  return pRval;
}

// reading directories in a system-independent way

typedef struct __DIRLIST__
{
  const char *szPath, *szNameSpec;
#ifdef WIN32 // also true for WIN64
  WIN32_FIND_DATA fd;
  HANDLE hFF;
#else // !WIN32
  DIR *hD;
  struct stat sF;
  union
  {
    char cde[sizeof(struct dirent) + NAME_MAX + 2];
    struct dirent de;
  };
#endif // WIN32 or !WIN32 - that is the question
// actual composite 'search name' follows
} DIRLIST;

void *WBAllocDirectoryList(const char *szDirSpec)
{
DIRLIST *pRval;
char *p1, *p2;
int iLen, nMaxLen;
char *pBuf;

  if(!szDirSpec || !*szDirSpec)
  {
    WB_WARN_PRINT("WARNING - %s - invalid directory (NULL or empty)\n", __FUNCTION__);
    return NULL;
  }

  iLen = strlen(szDirSpec);
  nMaxLen = iLen + 32;

  pBuf = WBAlloc(nMaxLen);
  if(!pBuf)
  {
    WB_ERROR_PRINT("ERROR - %s - Unable to allocate memory for buffer size %d\n", __FUNCTION__, nMaxLen);
    return NULL;
  }

  if(szDirSpec[0] == '/') // path starts from the root
  {
    memcpy(pBuf, szDirSpec, iLen + 1);
  }
  else // for now, force a path of './' to be prepended to path spec
  {
    pBuf[0] = '.';
    pBuf[1] = '/';

    memcpy(pBuf + 2, szDirSpec, iLen + 1);
    iLen += 2;
  }

  // do a reverse scan until I find a '/'
  p1 = ((char *)pBuf) + iLen;
  while(p1 > pBuf && *(p1 - 1) != '/')
  {
    p1--;
  }

//  WB_ERROR_PRINT("TEMPORARY - \"%s\" \"%s\" \"%s\"\n", pBuf, p1, szDirSpec);

  if(p1 > pBuf)
  {
    // found, and p1 points PAST the '/'.  See if it ends in '/' or if there are wildcards present
    if(!*p1) // name ends in '/'
    {
      if(p1 == (pBuf + 1) && *pBuf == '/') // root dir
      {
        p1++;
      }
      else
      {
        *(p1 - 1) = 0;  // trim the final '/'
      }

      p1[0] = '*';
#ifdef WIN32
      p1[1] = '.';
      p1[2] = '*';
      p1[3] = 0;
#else // !WIN32
      p1[1] = 0;
#endif // WIN32
    }
    else if(strchr(p1, '*') || strchr(p1, '?'))
    {
      if(p1 == (pBuf + 1) && *pBuf == '/') // root dir
      {
        memmove(p1 + 1, p1, strlen(p1) + 1);
        *(p1++) = 0; // after this, p1 points to the file spec
      }
      else
      {
        *(p1 - 1) = 0;  // p1 points to the file spec
      }
    }
    else if(WBIsDirectory(pBuf)) // entire name is a directory
    {
      // NOTE:  root directory should NEVER end up here

      p1 += strlen(p1);
      *(p1++) = 0; // end of path (would be '/')
      p1[0] = '*';
#ifdef WIN32
      p1[1] = '.';
      p1[2] = '*';
      p1[3] = 0;
#else // !WIN32
      p1[1] = 0;
#endif // WIN32
    }
    else
    {
      WB_WARN_PRINT("TEMPORARY:  I am confused, %s %s\n", pBuf, p1);
    }
  }
  else
  {
    // this should never happen if I'm always prepending a './'
    // TODO:  make this more consistent, maybe absolute path?

    WB_WARN_PRINT("TEMPORARY:  should not happen, %s %s\n", pBuf, p1);

    if(strchr(pBuf, '*') || strchr(pBuf, '?')) // wildcard spec
    {
      p1 = (char *)pBuf + 1; // make room for zero byte preceding dir spec
      memmove(pBuf, p1, iLen + 1);
      *pBuf = 0;  // since it's the current working dir just make it a zero byte (empty string)
    }
    else if(WBIsDirectory(pBuf))
    {
      p1 = (char *)pBuf + iLen;
      *(p1++) = 0; // end of path (would be '/')
      p1[0] = '*';
#ifdef WIN32
      p1[2] = '*';
      p1[3] = 0;
#else // !WIN32
      p1[1] = 0;
#endif // WIN32
    }
  }

  pRval = WBAlloc(sizeof(DIRLIST) + iLen + strlen(p1) + 2);

  if(pRval)
  {
    pRval->szPath = pBuf;
    pRval->szNameSpec = p1;

    p2 = (char *)(pRval + 1);
    strcpy(p2, pBuf);
    p2 += strlen(p2);
    *(p2++) = '/';
    strcpy(p2, p1);
    p1 = (char *)(pRval + 1);

#ifdef WIN32
    pRval->hFF = FindFirstFile(p2, &(pRval->fd))
    if(pRval->hFF == INVALID_HANDLE_VALUE)
    {
      WBFree(pBuf);
      WBFree(pRval);

      pRval = NULL;
    }
#else // !WIN32
    pRval->hD = opendir(pBuf);

//    WB_ERROR_PRINT("TEMPORARY - opendir for %s returns %p\n", pBuf, pRval->hD);

    if(pRval->hD == NULL)
    {
      WB_WARN_PRINT("WARNING - %s - Unable to open dir \"%s\", errno=%d\n", __FUNCTION__, pBuf, errno);

      WBFree(pBuf);
      WBFree(pRval);

      pRval = NULL;
    }
#endif // WIN32,!WIN32
  }
  else
  {
    WB_ERROR_PRINT("ERROR - %s - Unable to allocate memory for DIRLIST\n", __FUNCTION__);
    WBFree(pBuf);  // no need to keep this around
  }

  return pRval;
}

void WBDestroyDirectoryList(void *pDirectoryList)
{
  if(pDirectoryList)
  {
    DIRLIST *pD = (DIRLIST *)pDirectoryList;

#ifdef WIN32
    if(pD->hFF != INVALID_HANDLE_VALUE)
    {
      FindClose(pD->hFF);
    }
#else // !WIN32
    if(pD->hD)
    {
      closedir(pD->hD);
    }
#endif // WIN32,!WIN32
    if(pD->szPath)
    {
      WBFree((void *)(pD->szPath));
    }

    WBFree(pDirectoryList);
  }
}

// returns < 0 on error, > 0 on EOF, 0 for "found something"

int WBNextDirectoryEntry(void *pDirectoryList, char *szNameReturn,
                         int cbNameReturn, unsigned long *pdwModeAttrReturn)
{
#ifdef WIN32
#else  // WIN32
struct dirent *pD;
struct stat sF;
#endif // WIN32
char *p1, *pBuf;
//static char *p2; // temporary
int iRval = 1;  // default 'EOF'
DIRLIST *pDL = (DIRLIST *)pDirectoryList;


  if(!pDirectoryList)
  {
    return -1;
  }

  // TODO:  improve this, maybe cache buffer or string length...
  pBuf = WBAlloc(strlen(pDL->szPath) + 8 + NAME_MAX);

  if(!pBuf)
  {
    return -2;
  }

  strcpy(pBuf, pDL->szPath);
  p1 = pBuf + strlen(pBuf);
  if(p1 > pBuf && *(p1 - 1) != '/') // it does not already end in /
  {
    *(p1++) = '/';  // for now assume this
    *p1 = 0;  // by convention
  }

#ifdef WIN32

  // for WIN32, copy 'previous' data first, then 'FindNextFile'.  On EOF mark
  // as EOF so that next call will detect it.

#else // !WIN32

  if(pDL->hD)
  {
    while((pD = readdir(pDL->hD))
          != NULL)
    {
      // skip '.' and '..'
      if(pD->d_name[0] == '.' &&
         (!pD->d_name[1] ||
          (pD->d_name[1] == '.' && !pD->d_name[2])))
      {
//        WB_ERROR_PRINT("TEMPORARY:  skipping %s\n", pD->d_name);
        continue;  // no '.' or '..'
      }

      strcpy(p1, pD->d_name);

      if(!lstat(pBuf, &sF)) // 'lstat' returns data about a file, and if it's a symlink, returns info about the link itself
      {
        if(!fnmatch(pDL->szNameSpec, p1, 0/*FNM_PERIOD*/))  // 'tbuf2' is my pattern
        {
          iRval = 0;

          if(pdwModeAttrReturn)
          {
            *pdwModeAttrReturn = sF.st_mode;
          }

          if(szNameReturn && cbNameReturn > 0)
          {
            strncpy(szNameReturn, p1, cbNameReturn);
          }

          break;
        }
//        else
//        {
//          p2 = pDL->szNameSpec;
//
//          WB_ERROR_PRINT("TEMPORARY:  \"%s\" does not match \"%s\"\n", p1, p2);
//        }
      }
      else
      {
        WB_WARN_PRINT("%s: can't 'stat' %s, errno=%d (%08xH)\n", __FUNCTION__, pBuf, errno, errno);
      }
    }
  }

#endif // WIN32,!WIN32

  if(pBuf)
  {
    WBFree(pBuf);
  }

  return iRval;

}

char *WBGetDirectoryListFileFullPath(const void *pDirectoryList, const char *szFileName)
{
char *pRval, *pBuf, *p1;
DIRLIST *pDL = (DIRLIST *)pDirectoryList;

  if(!pDirectoryList)
  {
    if(!szFileName || !*szFileName)
    {
      return NULL;
    }

    return WBGetCanonicalPath(szFileName);
  }

  if(szFileName && *szFileName == '/')
  {
    return WBGetCanonicalPath(szFileName); // don't need relative path
  }

  // TODO:  improve this, maybe cache buffer or string length...
  pBuf = (char *)WBAlloc(strlen(pDL->szPath) + 8 + (szFileName ? strlen(szFileName) : 0) + NAME_MAX);

  if(!pBuf)
  {
    return NULL;
  }

  strcpy(pBuf, pDL->szPath);
  p1 = pBuf + strlen(pBuf);
  if(p1 > pBuf && *(p1 - 1) != '/') // ends in a slash?
  {
    *(p1++) = '/';  // for now assume this
    *p1 = 0;  // by convention (though probably not necessary)
  }

  if(szFileName)
  {
    strcpy(p1, szFileName);
  }

  pRval = WBGetCanonicalPath(pBuf);
  WBFree(pBuf);

  return pRval;
}

char *WBGetSymLinkTarget(const char *szFileName)
{
char *pRval = WBAlloc(MAXPATHLEN + 2);

  if(pRval)
  {
    int iLen = readlink(szFileName, pRval, MAXPATHLEN);
    if(iLen <= 0)
    {
      WBFree(pRval);
      return NULL;
    }

    pRval[iLen] = 0; // assume < MAXPATHLEN for now...
  }

  return pRval;
}

char *WBGetDirectoryListSymLinkTarget(const void *pDirectoryList, const char *szFileName)
{
char *pTemp, *pRval;

  pTemp = WBGetDirectoryListFileFullPath(pDirectoryList, szFileName);

  if(!pTemp)
  {
    return NULL;
  }

  pRval = WBGetSymLinkTarget(pTemp);
  WBFree(pTemp);

  return pRval;
}

int WBStat(const char *szLinkName, unsigned long *pdwModeAttrReturn)
{
int iRval;
struct stat sF;


  iRval = stat(szLinkName, &sF);
  if(!iRval && pdwModeAttrReturn)
  {
    *pdwModeAttrReturn = sF.st_mode;
  }

  return iRval; // zero on success
}

int WBGetDirectoryListFileStat(const void *pDirectoryList, const char *szFileName,
                               unsigned long *pdwModeAttrReturn)
{
char *pTemp;
int iRval;

  pTemp = WBGetDirectoryListFileFullPath(pDirectoryList, szFileName);

  if(!pTemp)
  {
    return -1;
  }

//  WB_ERROR_PRINT("TEMPORARY:  stat on '%s' - \"%s\"\n", szFileName, pTemp);

  iRval = WBStat(pTemp, pdwModeAttrReturn);
  WBFree(pTemp);

  return iRval;
}

