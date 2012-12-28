//******************************************************************************
//
// QFVFY.C
//
// Verification routines for Quick Find EGO line text.
//
// Copyright (C) 1989-92 by Quick Solutions Inc; All rights reserved.
//
//******************************************************************************
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>

#include "qf.h"
#include "qfdos.h"

#include "crc.h"

struct _selfcheck {
   char code[8];
   ULONG crcval;
} selfcheck = {
"REBEIB\xD7\0"};

#define PBUFLEN 10240

// This is my address, encrypted so noone can change it.

char szAddress[] = {
   "ßßİİÛÛÙÙ××ÕÕÓÓÑÑÏÏ¿‚…Š…Ç¤ËÅ¡Š„ƒº­×ıûûùù÷÷õõóóññïïííóûøé"
       "¦¶­ª­¦µĞÑ‰ı³™——••““‘‘‹‹‰şÈÈÁÖ×ÌÂÊ³¿ÔÑ»»¯©§®­Ÿ“%s"
};

//******************************************************************************
//
// P-SPEC:  N/A
//
// NAME:    Display message telling user that the file has been tampered with.
//          (DisplayVerificationFailure)
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
void PASCAL DisplayVerificationFailure(void)
{
   register int iIndex;

   printf ("\nYour QF.EXE file has been altered in some way.  In case of a virus of some\n");
   printf("kind, execution of this program will be halted.\n\n");
   printf ("Please re-download this from another source, or write the author for an\n");
   printf ("authentic copy.  Please send a self addressed, stamped disk mailer, with\n");
   printf ("a letter stating where you got your copy of Quick Find, to the following\n");
   printf("address:\n\n");

   // Decrypt the encrypted address

   iIndex = 0;

   for (iIndex = 0; iIndex < sizeof(szAddress); iIndex++)
      szAddress[iIndex] =
          (char)(szAddress[iIndex] ^ ((255 - (iIndex & 254)) | (127 ^ iIndex)));

   printf("%s", szAddress);

   return;
}

//******************************************************************************
//
// P-SPEC:  N/A
//
// NAME:    Check program title string for modification
//          (QFVerify)
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
int PASCAL QFVerify(void)
{
   ULONG ulFileCRC;
   char *pch = szMessages[0];

   if (selfcheck.crcval == 0)
      return (1);

   InitializeCRC(ulFileCRC);

   while (*pch) {
      ulFileCRC = UpdateCRC(*pch, ulFileCRC);
      pch++;
   }

   PostConditionCRC(ulFileCRC);

   if (ulFileCRC != selfcheck.crcval)
      return (3);

   return (2);
}

//******************************************************************************
//
// P-SPEC:  N/A
//
// NAME:    Patch the executable file with the CRC value of the title string.
//          (QFWriteVerifyInfo)
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
int PASCAL QFWriteVerifyInfo(const PSTR szFileName)
{
   int fh;

   LONG ulFileCRC, lSize, lOffset = 0L;

   int iIndex;

   unsigned nBytesRead;

   char *inbuf, *pch;

   int bFound = 0;

   if ((fh = open(szFileName, O_RDWR | O_BINARY)) < 0)
      return (1);

   lSize = lmin(filelength(fh), PBUFLEN);

   if ((inbuf = (char *)malloc((size_t) lSize)) == NULL) {
      DisplayMessage(IDS_OUTOFMEMORY, __FILE__, __LINE__);
      close(fh);
      c_break(0);
   }

   bFound = FALSE;

   while (!bFound && (nBytesRead = read(fh, inbuf, (unsigned int)lSize)) > 0) {
      pch = inbuf;

      for (iIndex = 0; iIndex < (int)nBytesRead; iIndex++, lOffset++, pch++)
         if (strncmp(pch, selfcheck.code, strlen(selfcheck.code)) == 0) {
            bFound = TRUE;
            break;
         }
   }

   if (!bFound) {
      free(inbuf);
      close(fh);
      return (3);
   }

   pch = szMessages[0];

   InitializeCRC(ulFileCRC);

   while (*pch) {
      ulFileCRC = UpdateCRC(*pch, ulFileCRC);
      pch++;
   }

   PostConditionCRC(ulFileCRC);

   selfcheck.crcval = ulFileCRC;
   lseek(fh, lOffset, SEEK_SET);
   write(fh, &selfcheck, sizeof(struct _selfcheck));
   close(fh);

   free(inbuf);
   return (4);
}
