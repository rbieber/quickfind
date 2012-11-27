//****************************************************************************
//
//                                WILDCARD.H
//
//                  Structures, variables and constants used
//                  for wildcard matching.
//
//                      Date:       April 21, 1989
//                    Author:       Ronald C. Bieber
//
//***************************************************************************
#define REST         '*'
#define ANY          '?'

typedef struct tagFILENAME
   {
   char  szFName[_MAX_FNAME];
   char  szExt[_MAX_EXT];
   }
FILENAME;

typedef FILENAME        *PFILENAME;
typedef FILENAME NEAR   *NPFILENAME;
typedef FILENAME FAR    *LPFILENAME;

void PASCAL ParseName(PFILENAME, PSTR);          //  Split filename to nmblk struct
int  PASCAL MatchWildcards(PSTR, PSTR);          //  Match wildcard specs in arcs
int  PASCAL Match(PSTR, PSTR);                   //  User called function to Match *, ?

