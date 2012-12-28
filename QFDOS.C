#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <io.h>
#include <conio.h>
#include <ctype.h>

#include <memory.h>

#include "qf.h"
#include "qfdos.h"

char *szMessages[] = {
#ifdef __OS2__
   "Quick File Finder for OS/2 version %s\n"
#else
#ifdef WIN32
   "Quick File Finder for Windows NT version %s\n"
#else
   "Quick File Finder for FreeDOS version %s\n"
#endif
#endif
       "Copyright 1989-95 by Ronald C. Bieber; All rights reserved.\n",
   "\n%s\n",                    // show a directory line
   "\rUnable to find file %s\n",
   "\nInvalid drive specification.\n",
   "Out of memory: FILE %s, LINE %d\n",
   "\nRedirecting output to: %s ...",
   "Redirection failed.\n",
   "   %s: archive corrupted.\n",
   "   %s: unable to open file.\n",
   "\nError reading drive %c:\n",
   "Unable to change to directory %s\n"
};

int DisplayMessage(int nIndex, ...)
{
   int nReturnValue = 0;
   va_list va;

   va_start(va, nIndex);
   nReturnValue = vprintf(szMessages[nIndex], va);
   va_end(va);

   return (nReturnValue);
}

BOOL PASCAL ShowNoPromptMessage(int iDrive, PSTR pszFileSpec)
{
   char c;                      //  answer to prompt

   BELL();

   printf
       ("\nWARNING!  This will delete all files matching the pattern \"%s\" on\n",
        pszFileSpec);

   if (fSwitches & ALLDRIVES)
      printf("%10cALL drives on your system.  ", ' ');
   else
      printf("%10cdrive %c:.  ", ' ', (char)iDrive + '@');

#ifndef _WINDOWS
   printf("Continue? [N]");

   c = yesno(((fSwitches & ALLDRIVES) ? 49 : 32), wherey(), 'N');
#else
   printf("Continue? [N]\b\b");
   do {
      c = getchar();
      c = toupper(c);
   }
   while (c != 'Y' && c != 'N');
#endif

   printf("\n");

   if (c != 'Y')
      c_break(0);

   return (TRUE);
}

BOOL PASCAL MaybeDelete(PSTR pszDeleteFile, BOOL bConfirm)
{
   int iColumn, iRow;

   BOOL bReturnCode = TRUE;

   if (bConfirm) {
#ifndef _WINDOWS
      iColumn = wherex();
      iRow = wherey();

      gotoxy(iColumn, iRow);
      printf("Delete? [N]");
      bReturnCode =
          !(yesno(iColumn + 9, iRow, 'N') == 'Y' ? unlink(pszDeleteFile) : 1);
      gotoxy(iColumn, iRow);
      clreol();
#else
      char c;

      printf("Delete? [N]\b\b");

      do {
         c = getchar();
         c = toupper(c);
      }
      while (c != 'N' && c != 'Y');

      if (c == 'Y')
         bReturnCode = !unlink(pszDeleteFile);
      else
         bReturnCode = FALSE;

      printf("%11c %12c", '\b', ' ');

#endif
   } else
      bReturnCode = !unlink(pszDeleteFile);

   printf("%s\n", bReturnCode ? "Deleted." : "Skipped.");
   usLineCount++;

   return (bReturnCode);
}

/******************************************************************************
 *
 *          Name:   yesno
 *      Synopsis:   char    *yesno(line);
 *                  char    *line;          string to get from console
 *
 *   Description:   Gets a single character answer from the console.
 *
 *       Returns:   The character entered.
 *
 *
 *****************************************************************************/
char PASCAL yesno(int iColumn, int iRow, char cChar)
{
   int c = 0;
   BOOL bDone = FALSE;

   while (!bDone) {
#ifndef _WINDOWS
      gotoxy(iColumn, iRow);

      KeyFlush();
      c = KeyInput();
#else
      c = getchar();
#endif

      switch (toupper(c)) {
      case 'Y':
      case 'N':
         putc(c, stdout);
         bDone = TRUE;
         break;

      case VK_ESCAPE:
         printf("\n");
         c_break(0);
         break;

      case VK_RETURN:
         c = cChar;
         bDone = TRUE;
         break;

      default:
         BELL();
#ifndef _WINDOWS
         printf("\b");
#endif
         break;
      }
   }

   return ((char)toupper(c));
}

/******************************************************************************
 *
 *          Name:   usage
 *      Synopsis:   void usage(int bShowExtra);
 *
 *   Description:   Display program usage and available command cChar
 *                  options.
 *
 ******************************************************************************/
void PASCAL usage(BOOL bShowExtra)
{
   printf("\nusage:  QF [d:] filespec [switches] [>outfile]\n\n");
   printf("Switches:\n");
   printf("    /A          Search for files on all drives\n");
   printf("    /B          Bare display; no extra file information\n");
   printf
       ("    /C          Search only current directory and it's subdirectories\n");
   printf("    /CF         Search compressed files\n");
   printf("    /CO         Only search compressed files\n");
   printf("    /D          Delete found files\n");
#if 0
   printf("    /E          Display list of archive formats supported ...\n");
#endif
   printf("    /N          Do not confirm deletions (used with /D)\n");
   printf("    /P          Pause mode; press a key to continue ...\n");

#if defined(REDIRECT_ENABLED)
   printf
       ("    /R[:file]   Redirect output; if no file specified, default to printer\n");
#endif
   printf("    /T          Print subdirectory totals\n");

   if (bShowExtra) {
      printf("\nCompressed file formats currently supported:\n\n");
      printf
          ("    PKARC/PKPAK - PKware, Inc.            PKZIP - PKware, Inc.\n");
      printf
          ("    PAK         - Nogate Consulting       DWC   - Dean W. Cooper\n");
      printf("    ARC         - SEA, Inc.               ZOO   - Rahul Dhesi\n");
      printf
          ("    LARC        - Miki/Okumarua/Masuyama  LHARC - Haruyasu Yoshizaki\n");
      printf("Last compiled on %s\n", __DATE__);
   }

   exit(0);
}
