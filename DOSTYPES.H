//*****************************************************************************
//
// DOSTYPES.H
//
// This file declares types needed in the DOS compiled version of the program.
//
// Copyright (C) 1989-91 by Quick Solutions, Inc;  All rights reserved.
//
//*****************************************************************************
typedef int BOOL;
typedef BOOL *PBOOL;
typedef long LONG;
typedef long *PLONG;
typedef int *PINT;

typedef int HFILE;
typedef unsigned short USHORT;
typedef unsigned long ULONG;
typedef unsigned char BYTE;

#ifndef FALSE
#define FALSE  0
#endif

#ifndef WIN32
#ifndef PASCAL
#define PASCAL __pascal
#endif

#ifndef FAR
#define FAR    _far
#endif

#ifndef NEAR
#define NEAR   _near
#endif
#else
#define PASCAL
#define FAR
#define NEAR
#endif                          // #ifndef WIN32

#ifndef TRUE
#define TRUE   !FALSE
#endif
