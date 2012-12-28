//*****************************************************************************
//
// COMPRESS.C
//
// Functions to search .ARC, .PAK, .ZOO, .ZIP, .DWC, .LZS, and .LZH files.
//
// Copyright 1989-91 by Quick Solutions Inc;  All rights reserved.
//
//*****************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <direct.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <time.h>

#include "qf.h"
#include "qfdos.h"

#include "compress.h"
#include "wildcard.h"

#define  BUFSZ    8192L

typedef struct {
   char szExtension[4];
   LPFNARCHIVESEARCH lpfn;
} JUMPTABLEENTRY;

JUMPTABLEENTRY archiveSearch[] = {
   {"ARC", SearchARC},
   {"PAK", SearchARC},
   {"ZIP", SearchZIP},
   {"ZOO", SearchZOO},
   {"DWC", SearchDWC},
   {"LZH", SearchLZH},
   {"LZS", SearchLZH}
};

PSTR PASCAL GetFileName(PSTR);
int PASCAL FindEOCD(int);

/******************************************************************************
 *
 *          Name:   SearchARC
 *      Synopsis:   int SearchARC(pfs->szArchiveName, szFileName);
 *                  char    *pfs->szArchiveName;           archive name to search
 *                  char    *szFileName;        name of file to search for
 *
 *   Description:   Searches archive (*.ARC) files for 'szFileName'
 *
 *
 *       Returns:   1 if found in archive, 0 if not.
 *
 *****************************************************************************/
int PASCAL SearchARC(PFILESTUFF pfs, PSTR szFileName, PINT pbPrinted)
{
   int fh, iFound = 0, nBytesRead;

   PSTR pch;

   ARCHIVE arcEntry;

   // open archive file.
   if ((fh = open(pfs->szArchiveName, O_RDONLY | O_BINARY)) < 0) {
      DisplayMessage(IDS_CANTOPENFILE, pfs->szArchiveName);
      return (0);
   }

   while ((nBytesRead = read(fh, (char *)&arcEntry, sizeof(ARCHIVE))) > 0) {
      if (arcEntry.cArchiveMark != ARCMARK)  // check archive marker
      {
         DisplayMessage(IDS_ARCHIVECORRUPT, pfs->szArchiveName);
         break;
      }

      if (arcEntry.cHeaderVersion < 0) // check header version.
      {
         DisplayMessage(IDS_ARCHIVECORRUPT, pfs->szArchiveName);
         break;
      }

      if (!arcEntry.cHeaderVersion) // end of archive
         break;

      if (arcEntry.cHeaderVersion == 1)   // old style archive, header is shorter
      {
         lseek(fh, tell(fh) - (ULONG) sizeof(LONG), SEEK_SET);
         arcEntry.cHeaderVersion = 2;
         arcEntry.ulSize = arcEntry.ulLength;
      }

      if (arcEntry.cHeaderVersion > 0) {
         if (Match(szFileName, arcEntry.szName)) {
            if (++iFound == 1) {
               if (!(*pbPrinted)) {
                  pch = getcwd(NULL, MAXPATH);
                  DisplayMessage(IDS_DIRECTORYLINE, pch);
                  free(pch);
                  usLineCount += 2;
                  *pbPrinted = 1;
               }

               if (!(pfs->byType & FS_DIRECTORY))
                  ShowFileInfo(pfs);
            }

            strcpy(pfs->szName, arcEntry.szName);
            pfs->byType |= FS_ARCHIVE; // It IS an archive
            pfs->ulSize = arcEntry.ulLength;
            pfs->usDate = arcEntry.usDate;
            pfs->usTime = arcEntry.usTime;
            ShowFileInfo(pfs);
         }
      }

      lseek(fh, (long)arcEntry.ulSize, SEEK_CUR);
   }

   close(fh);
   return (iFound);
}

/******************************************************************************
 *
 *          Name:   SearchDWC
 *      Synopsis:   int SearchDWC(pfs->szArchiveName, szFileName);
 *                  char    *pfs->szArchiveName;           archive name to search
 *                  char    *szFileName;        name of file to search for
 *
 *   Description:   Searches archive (*.DWC) files for 'szFileName'
 *
 *
 *       Returns:   1 if found in archive, 0 if not.
 *
 *****************************************************************************/
int PASCAL SearchDWC(PFILESTUFF pfs, PSTR szFileName, PINT pbPrinted)
{
   int hFile, nBytesRead, iCount, iFound = 0;

   char *pch;

   LONG lOffset;

   DWCENTRY DWCEntry;
   DWCARCHIVE DWCArchive;

   DOSDATETIME filetime;

   struct tm *tm;

   // open DWCArchive file ...
   if ((hFile = open(pfs->szArchiveName, O_RDONLY | O_BINARY)) < 0) {
      DisplayMessage(IDS_CANTOPENFILE, pfs->szArchiveName);
      return (0);
   }

   lseek(hFile, (ULONG) (filelength(hFile) - (LONG) sizeof(DWCARCHIVE)),
         SEEK_SET);
   read(hFile, (char *)&DWCArchive, sizeof(DWCARCHIVE));

   if (strnicmp(DWCArchive.szID, "DWC", 3) != 0)   //  not a .DWC file
   {
      DisplayMessage(IDS_ARCHIVECORRUPT, pfs->szArchiveName);
      close(hFile);
      return (iFound);
   }
   // calculate offset to DWC directory entries
   // and position the file pointer there.
   lOffset = DWCArchive.lSize + (DWCArchive.lEntries * DWCArchive.lEntrySize);

   lseek(hFile, (long)filelength(hFile) - lOffset, SEEK_SET);

   iCount = 0;

   while ((unsigned)iCount++ < (unsigned)DWCArchive.lEntries) {
      //  read DWC directory DWCEntry

      nBytesRead = read(hFile, (char *)&DWCEntry, sizeof(DWCENTRY));

      if (nBytesRead < 1)
         break;

      if (Match(szFileName, DWCEntry.szName))   //  compare szName with filespec
      {
         if (++iFound == 1) {
            if (!(*pbPrinted)) {
               pch = getcwd(NULL, MAXPATH);
               DisplayMessage(IDS_DIRECTORYLINE, pch);
               free(pch);
               usLineCount += 2;
               *pbPrinted = 1;
            }

            if (!(pfs->byType & FS_DIRECTORY))
               ShowFileInfo(pfs);
         }

         pfs->byType |= FS_ARCHIVE; // it IS an archive
         strcpy(pfs->szName, DWCEntry.szName);
         pfs->ulSize = DWCEntry.lSize;

         tm = localtime(&DWCEntry.lFileTime);
         filetime.date.iMonth = tm->tm_mon + 1;
         filetime.date.iDay = tm->tm_mday;
         filetime.date.iYear = tm->tm_year - 80;
         pfs->usDate = filetime.iValue;
         filetime.time.iTwoSecs = tm->tm_sec;
         filetime.time.iMinutes = tm->tm_min;
         filetime.time.iHours = tm->tm_hour;
         pfs->usTime = filetime.iValue;

         ShowFileInfo(pfs);
      }
   }

   close(hFile);
   return (iFound);
}

/******************************************************************************
 *
 *          Name:   SearchZIP
 *      Synopsis:   int SearchZIP(pfs->szArchiveName, szFileName, pbPrinted);
 *                  char    *pfs->szArchiveName;           name to search
 *                  char    *szFileName;        name of file to search for
 *                  int     pbPrinted;            1 if filename has been output
 *
 *   Description:   Searches ZIP (*.ZIP) files for 'szFileName'
 *
 *
 *       Returns:   1 if found in ZIP, 0 if not.
 *
 *****************************************************************************/
int PASCAL SearchZIP(PFILESTUFF pfs, PSTR szFileName, PINT pbPrinted)
{
   char *pch, szEntryName[MAXPATH];

   int hFile, iFound = 0, iIndex;

   ZIPENTRY cd;
   ZIPARCHIVE eocd;

   // open ZIP file ...

   if ((hFile = open(pfs->szArchiveName, O_RDONLY | O_BINARY)) < 0) {
      DisplayMessage(IDS_CANTOPENFILE, pfs->szArchiveName);
      return (0);
   }

   if (!FindEOCD(hFile))        // find end of central directory header
   {
      DisplayMessage(IDS_ARCHIVECORRUPT, pfs->szArchiveName);
      close(hFile);
      return (0);
   }

   read(hFile, (char *)&eocd, sizeof(ZIPARCHIVE));

   if (eocd.sig == END_SIG) {
      lseek(hFile, eocd.offset_of_central_dir, SEEK_SET);

      for (iIndex = 0; (unsigned)iIndex < eocd.tot_entries; iIndex++) {
         read(hFile, (char *)&cd, sizeof(ZIPENTRY));
         if (cd.sig == CENTRAL_SIG) {
            read(hFile, szEntryName, cd.filename_length);
            szEntryName[cd.filename_length] = 0;

            // compare name with file specification.
            if (Match(szFileName, GetFileName(szEntryName))) {
               if (++iFound == 1) {
                  if (!(*pbPrinted)) {
                     pch = getcwd(NULL, MAXPATH);
                     DisplayMessage(IDS_DIRECTORYLINE, pch);
                     free(pch);
                     usLineCount += 2;
                     *pbPrinted = 1;
                  }

                  if (!(pfs->byType & FS_DIRECTORY))
                     ShowFileInfo(pfs);

               }

               pfs->byType |= FS_ARCHIVE; // It IS an archive

               strcpy(pfs->szName, GetFileName(szEntryName));
               pfs->ulSize = cd.uncompressed_size;
               pfs->usDate = cd.date;
               pfs->usTime = cd.time;
               ShowFileInfo(pfs);
            }

            lseek(hFile, (long)(cd.extra_field_length + cd.file_comment_length),
                  SEEK_CUR);
         } else {
            DisplayMessage(IDS_ARCHIVECORRUPT, pfs->szArchiveName);
            break;
         }
      }
   }

   close(hFile);
   return (iFound);
}

/******************************************************************************
 *
 *          Name:   SearchZOO
 *      Synopsis:   int SearchZOO(pfs->szArchiveName, szFileName, pbPrinted);
 *                  char    *pfs->szArchiveName;           name to search
 *                  char    *szFileName;        name of file to search for
 *                  int     pbPrinted;            1 if filename has been output
 *
 *   Description:   Searches ZOO (*.ZOO) files for 'szFileName'
 *
 *
 *       Returns:   1 if found in ZOO, 0 if not.
 *
 *****************************************************************************/
int PASCAL SearchZOO(PFILESTUFF pfs, PSTR szFileName, PINT pbPrinted)
{
   int hFile;
   char *pch;

   ZOOARCHIVE ZOOArchive;
   ZOOENTRY ZOOEntry;

   int nBytesRead, iFound = 0;

   if ((hFile = open(pfs->szArchiveName, O_RDONLY | O_BINARY)) < 0) {
      DisplayMessage(IDS_CANTOPENFILE, pfs->szArchiveName);
      return (0);
   }
   // Read zoo header ...
   nBytesRead = read(hFile, (char *)&ZOOArchive, sizeof(ZOOARCHIVE));

   if (ZOOArchive.type != ZOO_H_TYPE)  //  not a zoo or zoo is corrupted
   {
      DisplayMessage(IDS_ARCHIVECORRUPT, pfs->szArchiveName);
      close(hFile);
      return (iFound);
   }

   lseek(hFile, ZOOArchive.zoo_start, SEEK_SET);   //  position file pointer at start of zoo

   for (;;) {
      //  read directory entry
      nBytesRead = read(hFile, (char *)&ZOOEntry, sizeof(ZOOENTRY));

      if (nBytesRead < 1)
         break;

      if (ZOOEntry.zoo_tag != ZOO_TAG) //  ERROR !!!
      {
         DisplayMessage(IDS_ARCHIVECORRUPT, pfs->szArchiveName);
         break;
      }

      if (ZOOEntry.next == 0L)  //  no more entries
         break;

      if (Match(szFileName, ZOOEntry.fname)) //  do compare
      {
         if (++iFound == 1) {
            if (!(*pbPrinted)) {
               pch = getcwd(NULL, MAXPATH);
               DisplayMessage(IDS_DIRECTORYLINE, pch);
               free(pch);
               usLineCount += 2;
               *pbPrinted = 1;
            }

            if (!(pfs->byType & FS_DIRECTORY))
               ShowFileInfo(pfs);
         }

         pfs->byType |= FS_ARCHIVE; // it IS an archive

         strcpy(pfs->szName, ZOOEntry.fname);
         pfs->ulSize = ZOOEntry.org_size;
         pfs->usDate = ZOOEntry.date;
         pfs->usTime = ZOOEntry.time;
         ShowFileInfo(pfs);
      }

      lseek(hFile, ZOOEntry.next, SEEK_SET);
   }

   close(hFile);
   return (iFound);
}

/******************************************************************************
 *
 *          Name:   SearchLZH
 *      Synopsis:   int SearchLZH(pfs->szArchiveName, szFileName, pbPrinted);
 *                  char    *pfs->szArchiveName;           name to search
 *                  char    *szFileName;        name of file to search for
 *                  int     pbPrinted;            1 if filename has been output
 *
 *   Description:   Searches LHARC (*.LZH) and LARC (*.LZS) files for
 *                  'szFileName'
 *
 *       Returns:   1 if found in .LZH (or .LZS), 0 if not.
 *
 *****************************************************************************/
int PASCAL SearchLZH(PFILESTUFF pfs, PSTR szFileName, PINT pbPrinted)
{
   int hFile, iFound = 0, iItemsRead;
   USHORT iCRC;

   char *szEntryName, *pch;

   ULONG ulOffset;

   LZHENTRY header;

   if ((hFile = open(pfs->szArchiveName, O_RDONLY | O_BINARY)) < 0) {
      DisplayMessage(IDS_CANTOPENFILE, pfs->szArchiveName);
      return (0);
   }

   if ((szEntryName = (char *)malloc(80)) == NULL) {
      DisplayMessage(IDS_OUTOFMEMORY, __FILE__, __LINE__);
      close(hFile);
      exit(1);
   }

   for (;;) {
      ulOffset = tell(hFile);

      memset((void *)&header, '\0', sizeof(LZHENTRY));
      iItemsRead = read(hFile, (void *)&header, sizeof(LZHENTRY));

      if (header.no_bytes == 0) //  Finished.  Reached end of archive
         break;

      if (header.type[0] != '-' && header.type[1] != 'l' && header.type[4] != '-')  //  make sure archive is valid
      {
         DisplayMessage(IDS_ARCHIVECORRUPT, pfs->szArchiveName);
         break;
      }

      memset(szEntryName, 0, 80);

      read(hFile, szEntryName, header.name_len);
      szEntryName[header.name_len] = '\0';

      read(hFile, (char *)&iCRC, sizeof(iCRC));

      if (Match(szFileName, GetFileName(szEntryName)))   //  do compare
      {
         if (++iFound == 1) {
            if (!(*pbPrinted)) {
               pch = getcwd(NULL, MAXPATH);
               DisplayMessage(IDS_DIRECTORYLINE, pch);
               free(pch);
               usLineCount += 2;
               *pbPrinted = 1;
            }

            if (!(pfs->byType & FS_DIRECTORY))
               ShowFileInfo(pfs);
         }

         strcpy(pfs->szName, GetFileName(szEntryName));
         pfs->byType |= FS_ARCHIVE; // it IS an archive
         pfs->ulSize = header.orig_size;
         pfs->usDate = header.date;
         pfs->usTime = header.time;
         ShowFileInfo(pfs);
      }

      ulOffset += (ULONG) (header.no_bytes + header.size_now + sizeof(iCRC));

      lseek(hFile, ulOffset, SEEK_SET);
   }

   close(hFile);

   free(szEntryName);           //  free previous allocated memory

   return (iFound);
}

/******************************************************************************
 *
 *          Name:   FindEOCD
 *      Synopsis:   int FindEOCD(zh);
 *                  int     zh;         file handle of zip file
 *
 *   Description:   Searches from the tail end of the file forward for
 *                  the "End of Central Directory" file signature.
 *
 *       Returns:   TRUE if it was found.  Otherwise, FALSE
 *
 *****************************************************************************/
int PASCAL FindEOCD(hZIPFile)
int hZIPFile;
{
   char *pchBuffer, *pchBufPtr;

   LONG lSize, lIndex;

   int iTries = 1;

   BOOL bDone = 0;

   lIndex = 0L;

   lSize = min(filelength(hZIPFile) - 1L, BUFSZ);

   lseek(hZIPFile, -lSize, SEEK_END);

   if ((pchBuffer = (char *)malloc((size_t) lSize)) == NULL) {
      DisplayMessage(IDS_OUTOFMEMORY, __FILE__, __LINE__);
      exit(1);
   }

   while (tell(hZIPFile) != 0L && !bDone) {
      if (lseek(hZIPFile, -(lSize * (long)iTries++), SEEK_END) == -1L)
         break;

      pchBufPtr = pchBuffer + read(hZIPFile, pchBuffer, (int)lSize);

      while (pchBufPtr > pchBuffer) {
         if ((ULONG) * ((ULONG *) pchBufPtr) == END_SIG) {
            bDone = 1;
            break;
         }

         pchBufPtr--;
         lIndex++;

         if (pchBufPtr <= pchBuffer)
            break;
      }

      if (bDone)
         break;
   }

   free(pchBuffer);

   if (bDone)
      lseek(hZIPFile, -lIndex, SEEK_END);

   return (bDone ? 1 : 0);
}

/*
 *  GetFileName();
 *
 *      Strips a path from the filename specified in *name, and
 *      returns the filename.
 *
 */
PSTR PASCAL GetFileName(PSTR szPathName)
{
   char *pchName;

   pchName = strrchr(szPathName, '\\');

   if (!pchName)
      pchName = strrchr(szPathName, '/');

   if (pchName)
      pchName++;

   return (pchName ? pchName : szPathName);
}

LPFNARCHIVESEARCH PASCAL GetArchiveSearchFunction(PSTR pchFilename)
{
   char *pchExtension = strrchr(pchFilename, '.');
   int iEntry;

   LPFNARCHIVESEARCH lpfn = (LPFNARCHIVESEARCH) NULL;

   if (pchExtension) {
      pchExtension++;

      for (iEntry = 0; iEntry < sizeof(archiveSearch) / sizeof(JUMPTABLEENTRY);
           iEntry++)
         if (stricmp(archiveSearch[iEntry].szExtension, pchExtension) == 0) {
            lpfn = archiveSearch[iEntry].lpfn;
            break;
         }
   }

   return (lpfn);
}
