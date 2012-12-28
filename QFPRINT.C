//****************************************************************************
//
//          Name:   QFPRINT.C
//
//   Description:   Module containing the functions necessary to
//                  print the contents of the 'filestuff' structure
//
//                      Date:       September 9, 1989
//                    Author:       Ronald C. Bieber
//
//      Copyright 1989 Ronald C. Bieber;  All rights reserved
//
//      Do not distribute modified versions without my permission.
//      Do not remove or alter this notice or any other copyright notice.
//      If you use this in your own program you must distribute source code.
//      Do not use any of this in a commercial product.
//
//****************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>
#include <malloc.h>
#include <memory.h>

#include "qf.h"
#include "qfdos.h"

char *months[13] =              // month names
{
   "   ", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
   "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

extern ULONG ulSubTotal;

//******************************************************************************
//
// P-SPEC:  N/A
//
// NAME:    Display a line of file information.
//          (ShowFileInfo)
//
// DATE:    January 11, 1992
//
// INPUT:
//
// OUTPUT:
//
// DESCRIPTION:
//
//******************************************************************************
void PASCAL ShowFileInfo(PFILESTUFF pfs)
{
   int iHours, iMinutes, iMonth, iDay, iYear;

   char szLine[MAX_LINE];
   PSTR pch;

   DOSDATETIME filetime;

   char cAM = 'a';

   memset(szLine, 0, sizeof(szLine));

   // if there is no filename, and there is no archive name, then
   // get the hell out of here, cause we don't want to print it.
   if ((pfs->byType & FS_MAINARCHIVE) && !*pfs->szArchiveName)
      return;

   if ((pfs->byType & FS_ARCHIVE) && !*pfs->szName)
      return;

   if (!(pfs->byType & FS_MAINARCHIVE))
      ulSubTotal += pfs->ulSize;

   if (!(fSwitches & NOFILEINFO)) {
      if (pfs->byType & FS_DIRECTORY)
         sprintf(szLine, "   %-16s          <DIR>    ", pfs->szName);
      else {
         if (pfs->byType & FS_MAINARCHIVE)
            sprintf(szLine, "   %-16s ", strupr(pfs->szArchiveName));
         else if (pfs->byType & FS_ARCHIVE)
            sprintf(szLine, "     *%-13s ", strlwr(pfs->szName));
         else
            sprintf(szLine, "   %-16s ", strlwr(pfs->szName));

         LongToString(pfs->ulSize, szLine + strlen(szLine), 11, LTS_PADLEFT);

         strcat(szLine, " bytes ");
      }

      filetime.iValue = pfs->usTime;

      iHours = filetime.time.iHours;
      iMinutes = filetime.time.iMinutes;

      filetime.iValue = pfs->usDate;

      iMonth = filetime.date.iMonth;
      iDay = filetime.date.iDay;
      iYear = filetime.date.iYear + 1980;

      if (iHours >= 12)         // adjust to 12 hour format
      {
         iHours -= 12;
         cAM = 'p';
      }

      if (!iHours)
         iHours = 12;

      pch = szLine + strlen(szLine);

      if (iMonth && iDay && iYear)
         sprintf(pch, (char *)" %2d:%2.2d %cm  %3s %2d %4.4d ",
                 iHours, iMinutes, cAM, (char *)months[iMonth], iDay, iYear);
      else
         sprintf(pch, (char *)"%-23.23c", (char)' ');
   } else {
      if (pfs->byType & FS_DIRECTORY)
         sprintf(szLine, "   %-16s", pfs->szName);
      else if (pfs->byType & FS_MAINARCHIVE)
         sprintf(szLine, "   %-16s ", strupr(pfs->szArchiveName));
      else if (pfs->byType & FS_ARCHIVE)
         sprintf(szLine, "     *%-13s ", strlwr(pfs->szName));
      else
         sprintf(szLine, "   %-16s ", strlwr(pfs->szName));
   }

   printf(szLine);

   if ((pfs->byType & FS_DIRECTORY) || (pfs->byType & FS_ARCHIVE) ||
       !(fSwitches & DELETEFILE))
      printf("\n", usLineCount);

   usLineCount++;

   if (fSwitches & PAUSE)
      if (usLineCount >= 24) {
         int col = wherex(), row = wherey(); // current cursor position

         printf("Program paused; press a key to continue ... (ESC to quit).");
         fflush(stdout);
         if (KeyInput() == VK_ESCAPE)
            c_break(0);

         gotoxy(col, row);
         clreol();

         usLineCount = 0;
      }

   if (pfs->byType & (FS_DIRECTORY | FS_MAINARCHIVE))
      pfs->byType &= ~(FS_DIRECTORY | FS_MAINARCHIVE);

   return;
}

//******************************************************************************
//
// P-SPEC:  N/A
//
// NAME:    Comma delimit a long value to a string
//          (LongToString)
//
// DATE:    January 11, 1992
//
// INPUT:   ULONG    - Value to format
//          PSTR     - where to put it (can be NULL, see DESCRIPTION)
//          int      - maximum length of string
//          WORD     - any flags.
//
// OUTPUT:  PSTR     - pointer to formatted buffer
//
// DESCRIPTION:
//
//    This function takes an unsigned LONG value and formats it to the
//    buffer specified by szDest with comma's and optional padding.
//    If szDest is NULL, then a buffer will be allocated and a pointer
//    returned to the caller.  It is up to the caller to free this memory.
//    if szDest is not NULL, a pointer to the passed buffer is returned.
//
//    If there was some kind of error, NULL is returned.
//
//******************************************************************************
PSTR PASCAL LongToString(ULONG lValue, PSTR szDest, int iMaxLen, WORD wFlags)
{
   char szStack[50];

   PSTR pch = szDest;
   PSTR pchStack = szStack;

   int iNumDigits = 0, iIndex = 0, iLength;

   BOOL bTempBuf = FALSE;

   unsigned long lTemp = lValue;
   unsigned long lResult = 0L;

   if (szDest == (char *)NULL)
      if ((pch = szDest = (char *)malloc(iMaxLen + 1)) == NULL)
         return (NULL);
      else
         bTempBuf = TRUE;

   memset(szStack, 0, sizeof(szStack));
   memset(szDest, 0, iMaxLen);

   do {
      lResult = (lTemp % 10L) + (unsigned long)'0';

      *pchStack = (char)lResult;
      iNumDigits++;
      pchStack++;
   }
   while ((lTemp /= 10));

   // now format the number ...

   while (iNumDigits) {
      if (!(iNumDigits % 3) && (pch != szDest)) {
         *pch = ',';
         pch++;
      }

      *pch = *(--pchStack);
      iNumDigits--;
      pch++;
   }

   *(szDest + min((WORD) (pch - szDest), (WORD) iMaxLen - 1)) = 0;

   // Do any padding that the caller specified.

   if (wFlags) {
      if ((pch = (char *)malloc(iMaxLen + 1)) != NULL) {
         iLength = strlen(szDest);

         memcpy(pch, szDest, iLength);
         memset(szDest, ' ', iMaxLen);

         *(szDest + iMaxLen) = 0;

         if (wFlags & LTS_PADLEFT)
            memcpy(szDest + (iMaxLen - iLength), pch, iLength);
         else if (wFlags & LTS_PADRIGHT)
            memcpy(szDest, pch, iLength);

         free(pch);
      } else if (bTempBuf) {
         free(szDest);
         szDest = NULL;
      }
   }

   return (szDest);
}
