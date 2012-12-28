//*****************************************************************************
//
// QFW32.C
//
// API layer of DOS functions to DOS/WIN32 Family API functions.
//
// Copyright 1993 by Quick Solutions Inc;  All rights reserved.
//
//*****************************************************************************

#include "qf.h"
#include "string.h"

#include <io.h>
#include <direct.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void _dos_getdrive(unsigned *puiDrive)
{
   *puiDrive = _getdrive();

   return;
}

void _dos_setdrive(unsigned usDrive, unsigned *puNumDrives)
{
   USHORT usCurrentDrive = 0;
   ULONG ulDriveMap = 0L;
   USHORT usDriveCount = 0L;
   char szDrive[10] = "?:\\";

   *puNumDrives = 0;

   // find out how many logical drives are attached to the system
   for (usCurrentDrive = 0; usCurrentDrive < 26; usCurrentDrive++) {
      *szDrive = (char)'@' + usCurrentDrive;

      if (GetDriveType(szDrive) <= 1)
         usDriveCount++;
   }

   _chdrive(usDrive);

   *puNumDrives = usDriveCount;

   return;
}

int floppies(void)
{
   USHORT usNumDrives = 0;
   char szDrive[10] = "?:\\";
   USHORT usCurrentDrive;

   // find out how many floppy drives are attached to the system
   for (usCurrentDrive = 0; usCurrentDrive < 26; usCurrentDrive++) {
      *szDrive = (char)'@' + usCurrentDrive;

      if (GetDriveType(szDrive) == DRIVE_REMOVABLE)
         usNumDrives++;
   }

   return (usNumDrives);
}

int isvalid(int iDrive)
{
   char szDrive[10];

   szDrive[0] = '@' + iDrive;
   szDrive[1] = ':';
   szDrive[2] = '\\';
   szDrive[3] = '\0';

   if (GetDriveType(szDrive) != -1)
      return (TRUE);

   return (FALSE);
}

#ifndef WIN32
static int hfileStdOut = 0xFFFF;
static int hfNew = 1;
#else
static HANDLE hfileStdOut = (HANDLE) 0xFFFF;
static HANDLE hfNew = (HANDLE) 0xFFFF;
#endif

int PASCAL assign(PSTR pszFileName)
{
#if 0
   int hfOld;

   if (!pszFileName)
      pszFileName = (const PSTR)"PRN";

   hfOld = open(pszFileName, O_RDWR | O_TEXT | O_CREAT, S_IREAD | S_IWRITE);

   if (hfNew != -1) {
      if (DosDupHandle(hfNew, &hfileStdOut) == 0) {
         if (DosDupHandle(hfOld, &hfNew) == 0) {
            hfNew = hfOld;
            return (1);
         }
      }
   }
#endif
   return (0);
}

int PASCAL unassign(void)
{
#if 0
   close(hfNew);

   hfNew = 1;

   if (DosDupHandle(hfileStdOut, &hfNew) == 0) {
      close(hfileStdOut);
      hfileStdOut = 0xFFFF;
      hfNew = 1;
      return (1);
   }
#endif
   return (0);
}

int wherey(void)
{
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

   GetConsoleScreenBufferInfo(hConsole, &consoleInfo);

   return (consoleInfo.dwCursorPosition.Y);
}

int wherex(void)
{
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

   GetConsoleScreenBufferInfo(hConsole, &consoleInfo);

   return (consoleInfo.dwCursorPosition.X);
}

void gotoxy(int x, int y)
{
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   COORD dwCursorPosition;

   dwCursorPosition.X = x;
   dwCursorPosition.Y = y;

   SetConsoleCursorPosition(hConsole, dwCursorPosition);

   return;
}

void clreol(void)
{
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   COORD cursorPos;
   DWORD dwNumChars;

   cursorPos.X = wherex();
   cursorPos.Y = wherey();
   FillConsoleOutputCharacter(hConsole, ' ', 81L, cursorPos, &dwNumChars);

   gotoxy(cursorPos.X, cursorPos.Y);

   return;
}

void PASCAL KeyFlush(void)
{
   HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);

   FlushConsoleInputBuffer(hConsole);
   return;
}

int PASCAL KeyInput(void)
{
   HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
   INPUT_RECORD input;
   DWORD dwNumChars = 0;

   do {
      KeyFlush();

      ReadConsoleInput(hConsole, &input, 1, &dwNumChars);

      if (dwNumChars == 1 && input.EventType == KEY_EVENT
          && input.Event.KeyEvent.bKeyDown)
         break;
   }
   while (TRUE);

   return (input.Event.KeyEvent.uChar.AsciiChar);
}

unsigned _dos_findfirst(const char *pszPathName, unsigned attrib,
                        struct _find_t *pfind_t)
{
   USHORT usReturn = TRUE;

   pfind_t->search_handle =
       _findfirst((char *)pszPathName, (struct _finddata_t *)pfind_t);

   if (pfind_t->search_handle && pfind_t->search_handle != -1)
      return (FALSE);
}

unsigned _dos_findnext(struct _find_t *pfind_t)
{
   USHORT usReturn = TRUE;

   if (pfind_t->search_handle && pfind_t->search_handle != -1)
      usReturn =
          _findnext(pfind_t->search_handle, (struct _finddata_t *)pfind_t);

   return (usReturn);
}
