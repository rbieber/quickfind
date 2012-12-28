//****************************************************************************
//
//          Name:   WILDCARD.C
//
//   Description:   Matches strings containing wildcard characters (* and ?)
//                  with real filenames.
//
//        Author:   Ronald C. Bieber
//          Date:   April 21, 1989
//
//   Description:   Matches strings containing wildcard characters (* and ?)
//                  with real filenames.
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
#include <string.h>
#include <ctype.h>

#include "qf.h"
#include "wildcard.h"

//******************************************************************************
//
// P-SPEC:  N/A
//
// NAME:    Parse the Filename into a nmblk structure.
//          (ParseName)
//
// DATE:    January 11, 1992
//
// INPUT:
//
// OUTPUT:
//
// DESCRIPTION:
//
//    Parses the filename given in *pattern and stores it in the structure
//    nmblk, seperating the name and extension.
//
//******************************************************************************
void PASCAL ParseName(PFILENAME pnbName, PSTR szPattern)
{
   char szDrive[_MAX_DRIVE],
       szDir[_MAX_DIR], szName[_MAX_FNAME], szExt[_MAX_EXT];

   _splitpath(szPattern, szDrive, szDir, szName, szExt);

   strcpy(pnbName->szFName, szName);

   if (*szExt)
      strcpy(pnbName->szExt, szExt + 1);

   return;
}

//******************************************************************************
//
// P-SPEC:  N/A
//
// NAME:    Match a pattern containing wildcards to a real name.
//          (MatchWildcards)
//
// DATE:    January 11, 1992
//
// INPUT:
//
// OUTPUT:  [TRUE | FALSE]
//
// DESCRIPTION:
//
//    This function does the actual wildcard matching.
//
//******************************************************************************
int PASCAL MatchWildcards(PSTR szName1, PSTR szName2)
{
   char c;
   int iIndex = -1;

   while ((c = szName1[++iIndex]) != (char)NULL ||
          szName2[iIndex] != (char)NULL) {
      switch (c) {
      case ANY:
         break;

      case REST:
         return (TRUE);

      default:
         if (toupper(szName2[iIndex]) != toupper(szName1[iIndex]))
            return (FALSE);
      }
   }

   return (1);
}

/******************************************************************************
 *
 *          Name:   Match
 *      Synopsis:   int Match(pattern, to);
 *                  char    *pattern;       name to Match (*, ? included)
 *                  char    *to;            filename to Match it to
 *
 *   Description:   User called function to Match wildcard characters
 *                  with a "for-real" type filename.
 *
 *       Returns:   True if a Match was found, false if not.
 ****************************************************************************/
int PASCAL Match(PSTR szPattern, PSTR szMatchTo)
{
   FILENAME fnName1, fnName2;

   memset((char *)&fnName1, 0, sizeof(FILENAME));
   memset((char *)&fnName2, 0, sizeof(FILENAME));

   ParseName(&fnName1, szPattern);
   ParseName(&fnName2, szMatchTo);

   if (MatchWildcards(fnName1.szFName, fnName2.szFName) &&
       MatchWildcards(fnName1.szExt, fnName2.szExt))
      return (TRUE);
   else
      return (FALSE);
}
