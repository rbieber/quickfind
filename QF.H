//*****************************************************************************
//
// QF.H
//
// This file declares variables, structures and macros used by all of the
// Quick Find modules.
//
// Copyright (C) 1989-91 by Quick Solutions, Inc;  All rights reserved.
//
//*****************************************************************************
#ifndef __QF_H
#define __QF_H

#define VERSION "8.00"

#ifndef _MAX_PATH               // for use with Microsoft C 6.00
#include <stdlib.h>
#endif

#if defined(_DOS)
#pragma message("Compiling for MSDOS")
#include "dostypes.h"
#endif                          // ifndef __OS2__

#ifdef __OS2__
#pragma message("Compiling for OS/2 Text mode.")
#include <os2.h>
#endif

#if defined(WIN32)
#pragma message("Compiling QFW32 for CONSOLE mode.")

#include <wtypes.h>
#include <time.h>
#include <io.h>

struct find_t {
   unsigned attrib;
   time_t time_create;          /* -1 for FAT file systems */
   time_t time_access;          /* -1 for FAT file systems */
   time_t time_write;
   _fsize_t size;
   char name[_MAX_PATH];
   long search_handle;
};

#endif

#if defined (__OS2__) || defined(WIN32) || (_MSC_VER <= 600)
#define _find_t find_t

#if !defined(_DOS)
unsigned _dos_findfirst(const char *pszPathName, unsigned attrib,
                        struct _find_t *pfind_t);
unsigned _dos_findnext(struct _find_t *pfind_t);
void _dos_getdrive(unsigned *puiDrive);
void _dos_setdrive(unsigned usDrive, unsigned *puNumDrives);
#endif

#endif

#ifndef _WINDOWS
typedef char *PSTR;
typedef char **PPSTR;

#ifndef WIN32
typedef unsigned int WORD;
typedef unsigned int *PWORD;
typedef unsigned long DWORD;

#endif
#endif

#ifndef lmin
#define lmin(v1, v2) ((DWORD) v1 < (DWORD) v2 ? (DWORD) v1 : (DWORD) v2)
#endif

extern USHORT usLineCount,      // line counter for pause switch
 fSwitches;                     // global variable to hold command line
                                    // flags

extern unsigned usFirstDisk;    // drive number at start of program

#include <conio.h>

#ifdef _WINDOWS
#define BELL()       MessageBeep(0)
#else
#define BELL()       putchar('\x7')
#endif

#define S_ATTR    (_A_SUBDIR | _A_SYSTEM | _A_HIDDEN | _A_RDONLY | _A_ARCH)

#define  COMPRESSED_ONLY   0x8000   // search ONLY archive files
#define  COMPRESSED        0x4000   // search through archives
#define  FROM_CURDIR       0x2000   // search from current directory down.
#define  PAUSE             0x1000   // pause mode
#define  ALLDRIVES         0x0800   // check all drives
#define  DELETEFILE        0x0400   // delete found files ...
#define  NOPROMPT          0x0200   // Don't prompt for deletions
#define  SUBDIRTOT         0x0100   // print subdirectory totals
#define  LOG               0x0080   // log output to different device/file
#define  NOFILEINFO        0x0040   // don't print extra file information

#define  MAXPATH           260
#define  MAX_LINE          512

#define  FS_ARCHIVE        0x01
#define  FS_MAINARCHIVE    0x02
#define  FS_DIRECTORY      0x04
#define  FS_SHOWDIRECTORY  0x08

#ifndef  VK_ESCAPE
#define  VK_ESCAPE         0x1B
#define  VK_RETURN         0x0D
#endif

// Formatting flags for LongToString() function.
#define LTS_PADLEFT        0x0080
#define LTS_PADRIGHT       0x0060

typedef struct tagFILESTUFF     // structure used by ShowFileInfo()
{
   BYTE szName[_MAX_PATH];
   ULONG ulSize;
   USHORT usDate;
   USHORT usTime;
   BYTE byType;
   BYTE szArchiveName[13];
} FILESTUFF;

typedef FILESTUFF FAR *LPFILESTUFF;
typedef FILESTUFF NEAR *NPFILESTUFF;
typedef FILESTUFF *PFILESTUFF;

typedef struct tagDOSDATE       //  bit field definitions for DOS date
{
   unsigned iDay:5;
   unsigned iMonth:4;
   unsigned iYear:7;
} DOSDATE;

typedef DOSDATE *PDOSDATE;
typedef DOSDATE NEAR *NPDOSDATE;
typedef DOSDATE FAR *LPDOSDATE;

typedef struct tagDOSTIME       //  bit field definitions for DOS time
{
   unsigned iTwoSecs:5;
   unsigned iMinutes:6;
   unsigned iHours:5;
} DOSTIME;

typedef DOSTIME *PDOSTIME;
typedef DOSTIME NEAR *NPDOSTIME;
typedef DOSTIME FAR *LPDOSTIME;

typedef union tagDOSDATETIME    //  union signifying DOS date/time
{
   DOSDATE date;
   DOSTIME time;
   unsigned iValue;
} DOSDATETIME;

typedef DOSDATETIME *PDOSDATETIME;
typedef DOSDATETIME NEAR *NPDOSDATETIME;
typedef DOSDATETIME FAR *LPDOSDATETIME;

extern FILESTUFF fs;

//*****************************************************************************
//
// Function prototypes.
//
//***************************************************************************
extern void PASCAL ShowFileInfo(PFILESTUFF); // Display g_files structure
extern int floppies(void);      // Get number of floppy drives on system
extern int isvalid(int);        // Check to see if drive is valid

int PASCAL parms(PINT, PPSTR);  // Get command line options
void PASCAL walkdirs(PSTR, BOOL, int (PASCAL *) (PSTR)); // Get directories to search
int PASCAL filefind(PSTR);      // Match specified filespec

int far handler(unsigned, unsigned, unsigned far *); // Hardware error handler

#ifndef _WINDOWS
extern int PASCAL assign(PSTR); // redirect output to a file
extern int PASCAL unassign(void);   // 'un-redirection' routine
extern int wherey(void);
extern int wherex(void);
extern void gotoxy(int, int);
extern void clreol(void);
extern void PASCAL KeyFlush(void);
extern int PASCAL KeyInput(void);
#endif

extern BOOL PASCAL MaybeDelete(PSTR, BOOL);
PSTR PASCAL LongToString(ULONG, PSTR, int, WORD);
extern void PASCAL splitfilespec(const PSTR, PSTR, unsigned *);   //  Split file spec to name, drive
extern void c_break(int);      // Control-Break interrupt handler
#endif
