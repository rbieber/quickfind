//*****************************************************************************
//
//          Name:   QF
//       Version:   8.00
//
//      Synopsis:   QF filespec [switches]
//
//   Description:   "Whereis" Utility.  Includes .ARC, .PAK, .DWC, .ZIP,
//                  .ZOO, .LZS, and .LZH searching, along with a global
//                  delete function.
//
//    Written By:   Ronald C. Bieber    -  September 9, 1989
//                                         March 12, 1995
//
//  Org Compiler:   Microsoft C Version 5.1
//  Compiler Now:   Microsoft C Version 8.00
//
//      Copyright 1989-95 Ronald C. Bieber;  All rights reserved
//
//      Do not distribute modified versions without my permission.
//      Do not remove or alter this notice or any other copyright notice.
//      If you use this in your own program you must distribute source code.
//      Do not use any of this in a commercial product.
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
#include <memory.h>
#include <signal.h>

#include <conio.h>              // for putch

#include "qf.h"
#include "qfdos.h"
#include "compress.h"
#include "wildcard.h"

unsigned usFirstDisk = 0;       // Drive number at start of program

USHORT usLineCount = 2,         // Line counter for /P switch
    fSwitches = 0;              // Global variable to hold command
                                       // line options
int nDeleted = 0;               // count of nDeleted files

char szFileName[MAXPATH],       // Filespec to search for
 szOriginalPath[MAXPATH],       // Current directory at start of program
 out[MAXPATH],                  // name of output file
 curdir[MAXPATH];               // Directory on disk before search

ULONG ulTotal = 0L,             // Total number of files found
    ulTotalFileSize = 0L,       // Total size of files found
    ulDirs = 0L,                // Total directories found
    ulSubTotal = 0L;            // Total size for subtotal option

BYTE bError = FALSE;            // Error flag

void PASCAL ParseEnvironmentOptions(PSTR szEnvName);

//*****************************************************************************
//
// Main Program
//
//*****************************************************************************
int main(int argc, const char **argv)
{
   int iDrive = 0;
   unsigned int iNumDrives;

   PSTR pchTemp = NULL;

   setvbuf(stdout, NULL, _IONBF, 0);
   
   usFirstDisk = fSwitches = 0;

   DisplayMessage(IDS_EGOLINE, VERSION);

   ParseEnvironmentOptions("QF_OPT");

   if (!(parms(&argc, (const PPSTR) argv + 1) - 1))
      usage(TRUE);
   else
      argv++;

#ifndef _WINDOWS
   signal(SIGINT, c_break);     // set ctrl-break handler
#endif

#if !defined(_WINDOWS) && !defined(__OS2__) && !defined(WIN32)
   _harderr(handler);           // set hardware error handler
#endif

   // get original drive and directory ...
   _dos_getdrive(&usFirstDisk);
   getcwd(szOriginalPath, sizeof(szOriginalPath));

   _dos_setdrive(usFirstDisk, (unsigned int *) &iNumDrives);
   splitfilespec((char *) strupr((char *) *argv), szFileName, (unsigned int *) &iDrive);

   if (!isvalid(iDrive) || bError)  // invalid drive specified.
   {
      BELL();
      DisplayMessage(IDS_INVALIDDRIVE);
      exit(1);
   }

   if ((fSwitches & DELETEFILE) && (fSwitches & NOPROMPT))
      if (!ShowNoPromptMessage(iDrive, szFileName))
         c_break(0);

   if (fSwitches & ALLDRIVES) {
      for (iDrive = 1; iDrive <= floppies(); iDrive++) {
         if (isvalid(iDrive) && !bError) {
            _dos_setdrive(iDrive, &iNumDrives);
            walkdirs("\\", 0, filefind);
         }

         if (bError)
            bError = FALSE;
      }

      for (floppies() + 1; iDrive <= iNumDrives; iDrive++) {
         if (isvalid(iDrive) && !bError) {
            _dos_setdrive(iDrive, &iNumDrives);
            getcwd(curdir, sizeof(curdir));  // save current directory on
            walkdirs("\\", 0, filefind);  // alternate drives
            chdir(curdir);
         }

         if (bError)
            bError = FALSE;
      }
   } else {
      _dos_setdrive(iDrive, &iNumDrives);
      getcwd(curdir, sizeof(curdir));  //  save current directory on alternate drives

      if (fSwitches & FROM_CURDIR)  //  Only search current directory and subdirs
         walkdirs(szOriginalPath, 1, filefind);
      else
         walkdirs("\\", 0, filefind);

      chdir(curdir);
   }

   _dos_setdrive(usFirstDisk, &iNumDrives);
   chdir(szOriginalPath);

   // print totals

   if (!(fSwitches & NOFILEINFO)) {
      if (fSwitches & SUBDIRTOT) {
         if (ulDirs || ulTotal) {
            PSTR pchNumFiles = NULL;

            printf("\n   ================ ===========\n");

            pchTemp = LongToString(ulTotalFileSize, pchTemp, 11, LTS_PADLEFT);
            pchNumFiles = LongToString(ulTotal + ulDirs, NULL, 16, LTS_PADLEFT);

            printf
                ("   %s %s total files/bytes in all directories\n",
                 pchNumFiles, pchTemp);

            if (pchNumFiles)
               free(pchNumFiles);

            usLineCount += 3;
         }
      }

      if (ulDirs || ulTotal) {
         printf("\n");

         if (ulDirs) {
            pchTemp = LongToString(ulDirs, pchTemp, 11, LTS_PADLEFT);
            printf(" %s directory(s) found.\n", pchTemp);
         }

         pchTemp = LongToString(ulTotal, pchTemp, 11, LTS_PADLEFT);
         printf(" %s file(s) found.\n", pchTemp);
      } else
         printf("\nNo files found.\n");

      if ((fSwitches & DELETEFILE) && ulTotal) {
         pchTemp = LongToString((LONG) nDeleted, pchTemp, 11, LTS_PADLEFT);
         printf(" %s file(s) deleted.\n", pchTemp);
      }

      if (fSwitches & LOG) {
         unassign();
         printf(" done.");
      }
   }
   // free the memory that MIGHT have been allocated by LongToString.
   if (pchTemp)
      free(pchTemp);

   return (0);
}

/****************************************************************************
 *
 *          Name:   parms
 *      Synopsis:   int parms(num_args, args);
 *                  int     num_args;       number of arguments in array
 *                                          (usually 'argc')
 *                  char    **args;         argument list
 *
 *   Description:   This function retrieves command line parameters and
 *                  switches (/ex1 /ex2 -ex1 -ex2).  Any string on the
 *                  command line preceeded with a '-' or '/' character
 *                  is treated as a command line switch, all others as
 *                  parameters to the program (file specifications, etc).
 *
 *                  Options can be placed anywhere on the command line.
 *
 *                  Valid options (arguments beginning with '-' or '/'
 *                  that are accepted by the program) are removed from
 *                  the original array, so after this function the only
 *                  things left in the array are non-options.
 *
 *       Returns:   Number of arguments left in the passed array.
 *
 ****************************************************************************/
int PASCAL parms(PINT piArgc, PPSTR ppArgv)
{
   PPSTR ppArg = ppArgv;
   PSTR pch;

   int iNumItems = 0;
   BOOL bIsValid;

   while (*ppArg) {

      if (**ppArg == '?' && !*(ppArg + 1))
         usage(TRUE);

      if (**ppArg == '/' || **ppArg == '-') {
         bIsValid = TRUE;       // assume valid argument

         pch = *ppArg + 1;

         switch (toupper(*pch)) {
         case 'A':
            fSwitches |= ALLDRIVES; // search all drives
            break;

         case 'B':             // no extra file info
            fSwitches |= NOFILEINFO;
            break;

         case 'C':
            switch (toupper(*(pch + 1))) {
            case 'F':
               fSwitches |= COMPRESSED;   // search compressed files
               break;

            case 'O':
               fSwitches |= COMPRESSED_ONLY; // only search compressed files
               break;

            case 0:            // current directory down.
               fSwitches |= FROM_CURDIR;
               break;
            }
            break;

         case 'D':
            fSwitches |= DELETEFILE;   // delete found files
            break;

         case '?':
         case 'E':
            if (!*(pch + 1))
               usage(toupper(*pch) == 'E');
            else
               bIsValid = FALSE;
            break;

#if defined(REDIRECT_ENABLED)
         case 'R':
#ifndef _WINDOWS
            fSwitches |= LOG;   // redirect output

            if (*(pch + 1))
               pch = strupr((*(pch + 1) == ':' ? pch + 2 : pch + 1));
            else
               pch = "PRN";

            DisplayMessage(IDS_REDIRECTING, pch);

            if (!assign(pch))
               DisplayMessage(IDS_REDIRECTFAILED);
#endif
            break;
#endif

         case 'N':
            fSwitches |= NOPROMPT;  // delete without prompting
            break;

         case 'P':
            fSwitches |= PAUSE; // pause mode
            break;

         case 'T':
            fSwitches |= SUBDIRTOT; // show subdirectory totals
            break;

         default:
            bIsValid = FALSE;
            break;
         }
      } else
         bIsValid = FALSE;

      if (!bIsValid) {
         ppArgv++;
         ppArg++;
         continue;
      }

      iNumItems++;
      memcpy(ppArgv, ppArg + 1, sizeof(PSTR) * (*piArgc - iNumItems));
   }

   *piArgc -= iNumItems;

   return (*piArgc);
}

/******************************************************************************
 *
 *          Name:   filefind
 *      Synopsis:   int filefind(filespec);
 *                  PSTR filespec;        name of file to search for
 *
 *   Description:   Searches directory for filespec
 *
 *
 *       Returns:   Number of files found.
 *
 *****************************************************************************/
int PASCAL filefind(PSTR szFileName)
{
   FILESTUFF fs;

   int bDone, iFound = 0, temp = 0, bPrinted = 0;

   char dir[MAXPATH], tempstr[MAXPATH];

   PSTR pchNumFound = NULL;
   PSTR pch = NULL;

   struct _find_t ffblk;

   memset(&fs, 0, sizeof(fs));

   ulSubTotal = 0;

   if (!(fSwitches & (COMPRESSED | COMPRESSED_ONLY)))
      bDone = _dos_findfirst(szFileName, S_ATTR, &ffblk);
   else
      bDone = _dos_findfirst("*.*", S_ATTR, &ffblk);

   while (!bDone) {
      fs.byType = 0;

      if (*ffblk.name == '.' && (ffblk.attrib & _A_SUBDIR)) {
         bDone = _dos_findnext(&ffblk);
         continue;
      }

      strcpy(fs.szName, ffblk.name);   //  fill in fs structure
      *fs.szArchiveName = 0;
      fs.ulSize = ffblk.size;

#ifndef WIN32
      fs.usDate = ffblk.wr_date;
      fs.usTime = ffblk.wr_time;
#else
      {
         DOSDATETIME filetime;
         struct tm *tm;

         tm = localtime(&ffblk.time_write);
         filetime.date.iMonth = tm->tm_mon + 1;
         filetime.date.iDay = tm->tm_mday;
         filetime.date.iYear = tm->tm_year - 80;
         fs.usDate = filetime.iValue;
         filetime.time.iTwoSecs = tm->tm_sec;
         filetime.time.iMinutes = tm->tm_min;
         filetime.time.iHours = tm->tm_hour;
         fs.usTime = filetime.iValue;
      }
#endif

      if (ffblk.attrib & _A_SUBDIR)
         fs.byType |= FS_DIRECTORY;

      if (!(fSwitches & COMPRESSED_ONLY)) {
         if (Match(szFileName, ffblk.name)) {
            if (++iFound == 1) {
               if (!bPrinted) {
                  DisplayMessage(IDS_DIRECTORYLINE, getcwd(dir, MAXPATH));
                  bPrinted++;
                  usLineCount += 2;
               }
            }

            if (!(fs.byType & FS_MAINARCHIVE))
               ShowFileInfo(&fs);

            if ((fSwitches & DELETEFILE)
                && !(ffblk.attrib & _A_SUBDIR)) {
               sprintf(tempstr,
                       (dir[strlen(dir) - 1] !=
                        '\\' ? "%s\\%s" : "%s%s"), dir, ffblk.name);

               if (MaybeDelete(tempstr, !(fSwitches & NOPROMPT)))
                  nDeleted++;
            }

            if (ffblk.attrib & _A_SUBDIR) {
               if (iFound > 0)
                  iFound--;

               ulDirs++;
            }
         }
      }
      // search compressed files if specified ...
      if (fSwitches & COMPRESSED || fSwitches & COMPRESSED_ONLY) {
         LPFNARCHIVESEARCH lpfn = GetArchiveSearchFunction(ffblk.name);

         if (lpfn) {
            fs.byType |= FS_MAINARCHIVE;
            strcpy(fs.szArchiveName, ffblk.name);

            iFound += (*lpfn) (&fs, szFileName, &bPrinted);
         }
      }

      bDone = _dos_findnext(&ffblk);
   }

   if ((fSwitches & SUBDIRTOT) && !(fSwitches & NOFILEINFO)) {
      if (iFound > 0) {
         printf("   ---------------- -----------\n");

         pch = LongToString(ulSubTotal, NULL, 11, LTS_PADLEFT);
         pchNumFound = LongToString((ULONG) iFound, NULL, 16, LTS_PADLEFT);

         printf("   %s %s total bytes\n", pchNumFound, pch);

         if (pch)
            free(pch);

         if (pchNumFound)
            free(pchNumFound);

         usLineCount += 2;
         ulTotalFileSize += ulSubTotal;
      }
   }

   return (iFound);
}

/******************************************************************************
 *
 *          Name:   walkdirs
 *      Synopsis:   void walkdirs(startdir);
 *                  PSTR startdir;          name of directory to start in.
 *
 *   Description:   starting at startdir, steps through all remaining
 *                  directories and calls filefind() to find files matching
 *                  a given szFileName.
 *
 *****************************************************************************/
void PASCAL walkdirs(PSTR szStartDir, BOOL bCurrent, int (PASCAL * fcn) (PSTR))
{
   struct _find_t files;
   int bDone, result = 0;

   if (chdir(szStartDir) == -1)
      return;

   ulTotal += (*fcn) (szFileName);
   bDone = _dos_findfirst("*.*", _A_SUBDIR | _A_HIDDEN, &files);

   while (!bDone) {
      if ((files.attrib & _A_SUBDIR) && *files.name != '.')
         walkdirs(files.name, 0, fcn);

      bDone = _dos_findnext(&files);
   }

   chdir("..");
}

/******************************************************************************
 *
 *          Name:   splitfilespec
 *      Synopsis:   void splitfilespec(szFileName, file, drive);
 *                  PSTR szFileName,          szFileName to split
 *                          *file;              variable to hold filename.ext
 *                  int     drive;              variable to store new drive #
 *
 *   Description:   parses szFileName and returns the drive number and filename
 *
 *****************************************************************************/
void PASCAL splitfilespec(const PSTR szFileSpec, PSTR szFile, unsigned *piDrive)
{
   char szDrive[_MAX_DRIVE],
       szDir[_MAX_DIR], szFName[_MAX_FNAME], szExt[_MAX_EXT];

   _splitpath(szFileSpec, szDrive, szDir, szFName, szExt);

   if (*szDir && *(szDir + 1))
      printf("\nIgnoring path:  %s\n", szDir);

   if (strlen(szDrive) > 1)
      *piDrive = *szDrive - 64;
   else
      _dos_getdrive(piDrive);

   if (!*szFName) {
      *szFName = '*';
      *(szFName + 1) = 0;
   }

   if (!*szExt) {
      *szExt = '.';
      *(szExt + 1) = '*';
      *(szExt + 2) = 0;
   }

   sprintf(szFile, "%s%s", szFName, szExt);
}

/******************************************************************************
 *
 *          szFName:   c_break
 *      Synopsis:   void cbreak(void);
 *
 *   Description:   Control Break interrupt handler
 *
 *****************************************************************************/
void c_break(int c)
{
   unsigned int iTemp;

#ifndef _WINDOWS
   if (fSwitches & LOG)
      unassign();
#endif

   chdir(curdir);
   _dos_setdrive(usFirstDisk, &iTemp);

   if (chdir(szOriginalPath))
      DisplayMessage(IDS_CANTCHANGEDIRECTORY, szOriginalPath);

   exit(3);
}

/******************************************************************************
 *
 *          szFName:   handler
 *      Synopsis:   int handler(errval, ax, bx, bp, si);
 *
 *
 *   Description:   Hardware error interrupt handler.
 *
 *****************************************************************************/
#if !defined(_WINDOWS) && !defined(__OS2__)
int far handler(unsigned int errval, unsigned int ax, unsigned __far * devhdr)
{
   BYTE cDrive = ((BYTE) ((unsigned)errval & 0xFF)) + (BYTE) 'A';

   bError = TRUE;

   if ((ax & 0xFF) == 2) {
      DisplayMessage(IDS_CANTREADDRIVE, cDrive);
      usLineCount += 2;
   }
#if !defined(__OS2__) && !defined(WIN32)
   _hardretn(-1);
#endif

   return(_HARDERR_IGNORE);
}
#endif
void PASCAL ParseEnvironmentOptions(PSTR szEnvName)
{
   PPSTR ppOptions = NULL;
   PPSTR ppTemp = NULL;

   PSTR pEnv = NULL, pToken = NULL;

   int iNumOptions = 0, iIndex;

   if ((pEnv = getenv(szEnvName)) != NULL) {
      pToken = strtok(pEnv, " \0\r");

      while (pToken) {
         ppTemp = (PPSTR) realloc(ppOptions, ++iNumOptions * sizeof(PSTR));

         if (!ppTemp) {
            iNumOptions--;
            break;
         }

         ppOptions = ppTemp;
         ppOptions[iNumOptions - 1] = strdup(pToken);

         if (!ppOptions[iNumOptions - 1])
            break;

         pToken = strtok(NULL, " \0\r");
      }

      // I was freeing this pointer, but it was causing a GPF under OS/2.
      // I guess you're not supposed to.
      // free(pEnv);
   }

   if (ppOptions) {
      // allocate NULL terminator ...
      if ((ppTemp = (PPSTR) realloc(ppOptions, (++iNumOptions) * sizeof(PSTR)))) {
         ppOptions = ppTemp;
         ppOptions[iNumOptions - 1] = NULL;
      }
      // now make a copy for freeing all the strings later, in case the
      // command line parser overwrites our pointers.

      if ((ppTemp = (PPSTR) malloc(sizeof(PSTR) * iNumOptions)))
         memcpy((PSTR) ppTemp, (PSTR) ppOptions, sizeof(PSTR) * iNumOptions);

      // now call the command line parser
      iIndex = iNumOptions;
      parms(&iIndex, ppOptions);

      // free all the strings we have duplicated.
      if (ppTemp)
         for (iIndex = 0; iIndex < iNumOptions; iIndex++)
            if (ppTemp[iIndex])
               free(ppTemp[iIndex]);

      // free the environment arrays ...
      if (ppOptions)
         free(ppOptions);

      if (ppTemp)
         free(ppTemp);
   }

   return;
}
