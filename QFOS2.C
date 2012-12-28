//*****************************************************************************
//
// QFOS2.C
//
// API layer of DOS functions to DOS/OS/2 Family API functions.
//
// Copyright 1993 by Quick Solutions Inc;  All rights reserved.
//
//*****************************************************************************
#define INCL_KBD
#define INCL_VIO
#define INCL_DOSDEVICES

#include "qf.h"
#include "dos.h"
#include "string.h"
#include "io.h"
#include "fcntl.h"
#include "sys/types.h"
#include "sys/stat.h"

void _dos_getdrive(unsigned *puiDrive)
{
   USHORT usDrive = 0;
   ULONG ulDriveMap = 0L;

   if (DosQCurDisk(&usDrive, &ulDriveMap) == 0)
      *puiDrive = usDrive;

   return;
}

void _dos_setdrive(unsigned usDrive, unsigned *puNumDrives)
{
   USHORT usCurrentDrive = 0;
   ULONG ulDriveMap = 0L;
   USHORT usDriveCount = 0L;

   *puNumDrives = 0;

   // find out how many logical drives are attached to the system
   if (DosQCurDisk(&usCurrentDrive, &ulDriveMap) == 0) {
      for (usCurrentDrive = 0; usCurrentDrive < 26; usCurrentDrive++)
         if ((ulDriveMap >> usCurrentDrive) & 1)
            usDriveCount++;
   }

   DosSelectDisk(usDrive);

   *puNumDrives = usDriveCount;
   return;
}

unsigned _dos_findfirst(const char *pszPathName, unsigned attrib,
                        struct _find_t *pfind_t)
{
   HDIR hdir = HDIR_CREATE;
   USHORT usSearchCount = 1;
   FILEFINDBUF findbuf;
   USHORT usReturn = 1;
   HDIR FAR *lphDir = (HDIR FAR *) & pfind_t->reserved[0];

   PSZ pchName = (PSZ) (DWORD) (const char far *)pszPathName;

   usReturn = DosFindFirst(pchName, &hdir,
                           attrib,
                           &findbuf, sizeof(FILEFINDBUF), &usSearchCount, 0L);

   _fmemset((PSZ) pfind_t, 0, sizeof(struct find_t));

   if (!usReturn) {
      _fstrcpy(pfind_t->name, findbuf.achName);
      pfind_t->attrib = findbuf.attrFile;
      pfind_t->size = findbuf.cbFile;
      pfind_t->wr_time = (unsigned)*((unsigned *)&findbuf.ftimeLastWrite);
      pfind_t->wr_date = (unsigned)*((unsigned *)&findbuf.fdateLastWrite);
      *lphDir = hdir;
   }

   return (usReturn);
}

unsigned _dos_findnext(struct find_t *pfind_t)
{
   HDIR FAR *lphDir = (HDIR FAR *) & pfind_t->reserved[0];
   FILEFINDBUF findbuf;
   USHORT usReturn = 1;
   USHORT usFileCount = 1;

   usReturn = DosFindNext(*lphDir, &findbuf, sizeof(FILEFINDBUF), &usFileCount);

   if (usReturn == 0) {
      _fstrcpy(pfind_t->name, findbuf.achName);
      pfind_t->attrib = findbuf.attrFile;
      pfind_t->size = findbuf.cbFile;
      pfind_t->wr_time = (unsigned)*((unsigned *)&findbuf.ftimeLastWrite);
      pfind_t->wr_date = (unsigned)*((unsigned *)&findbuf.fdateLastWrite);
   }

   return (usReturn);
}

int floppies(void)
{
   USHORT usDrives = 0;

   DosDevConfig(&usDrives, DEVINFO_FLOPPY, 0);

   return (usDrives);
}

int isvalid(int iDrive)
{
   USHORT usDrive = 0, usReturn = FALSE;

   ULONG ulDriveMap = 0L;

   if (DosQCurDisk(&usDrive, &ulDriveMap) == 0) {
      if ((ulDriveMap >> (iDrive - 1)) & 1)
         usReturn = TRUE;
   }

   return (usReturn);
}

static int hfileStdOut = 0xFFFF;
static int hfNew = 1;

int PASCAL assign(PSTR pszFileName)
{
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

   return (0);
}

int PASCAL unassign(void)
{
   close(hfNew);

   hfNew = 1;

   if (DosDupHandle(hfileStdOut, &hfNew) == 0) {
      close(hfileStdOut);
      hfileStdOut = 0xFFFF;
      hfNew = 1;
      return (1);
   }

   return (0);
}

int wherey(void)
{
   USHORT x, y;

   VioGetCurPos(&y, &x, (HVIO) NULL);
   return (y);
}

int wherex(void)
{
   USHORT x, y;

   VioGetCurPos(&y, &x, (HVIO) NULL);
   return (x);
}

void gotoxy(int x, int y)
{
   VioSetCurPos(y, x, (HVIO) NULL);
   return;
}

void clreol(void)
{
   return;
}

void PASCAL KeyFlush(void)
{
   HKBD hkbd = (HKBD) NULL;

   KbdFlushBuffer(hkbd);

   return;
}

int PASCAL KeyInput(void)
{
   HKBD hkbd = (HKBD) NULL;
   KBDKEYINFO keyInfo;

   KbdCharIn(&keyInfo, IO_WAIT, hkbd);

   return ((int)keyInfo.chChar);
}
