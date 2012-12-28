//***************************************************************************
//
//                             COMPRESS.H
//
//                Structures and function declarations
//                for .ARC, .PAK, .DWC, .ZIP, .ZOO, .LZS,
//                and .LZH file searching.
//
//                  Date:       September 9, 1989
//                Author:       Ronald C. Bieber
//
//***************************************************************************

//  .PAK and .ARC file header format

#define ARCMARK     26          //  archive marker

#pragma pack(1)
typedef struct tagARCHIVE {
   BYTE cArchiveMark;           // archive marker
   char cHeaderVersion;         // header version
   BYTE szName[13];             // file name
   ULONG ulSize;                // size of compressed file
   USHORT usDate;               // file date
   USHORT usTime;               // file time
   USHORT usCRC;                // cyclic redundancy check
   ULONG ulLength;              // size of uncompressed file
} ARCHIVE;

typedef ARCHIVE *PARCHIVE;
typedef ARCHIVE FAR *LPARCHIVE;
typedef ARCHIVE NEAR *NPARCHIVE;

//  DWC file header format

typedef struct tagDWCENTRY {
   char szName[13];             //  ... File name, Note: path is not saved here
   LONG lSize;                  //  ... size of file before compression in BYTEs
   LONG lFileTime;              //  ... time stamp on file before added to archive
   LONG lNewSize;               //  ... size of compressed file
   LONG lOffset;                //  ... Position of file in archive file
   char cMethod;                //  ... method of compression used on file
   char cCommentSize;           //  ... size of comment added to file
   char cDirSize;               //  ... size of directory name recorded on add
   USHORT uCRC;                 //  ... crc value computed for this file
} DWCENTRY;

typedef DWCENTRY *PDWCENTRY;
typedef DWCENTRY FAR *LPDWCENTRY;
typedef DWCENTRY NEAR *NPDWCENTRY;

typedef struct tagDWCARCHIVE {
   USHORT lSize;                //  ... size of archive structure, future expansion
   char lEntrySize;             //  ... size of directory entry, future expansion
   char szHeader[13];           //  ... name of header file to print on listings
   LONG lFileTime;              //  ... time stamp of last modification to archive
   LONG lEntries;               //  ... Number of entries in archive
   char szID[3];                //  ... the string "DWC" to identify archive
} DWCARCHIVE;

typedef DWCARCHIVE *PDWCARCHIVE;
typedef DWCARCHIVE FAR *LPDWCARCHIVE;
typedef DWCARCHIVE NEAR *NPDWCARCHIVE;

//  .ZIP Central and End of Central Directory Headers

#define LOCAL_SIG       0x04034b50  //  local file header signature
#define CENTRAL_SIG     0x02014b50  //  central directory structure signature
#define END_SIG         0x06054b50  //  end of central directory signature

typedef struct tagZIPENTRY {
   ULONG sig;
   USHORT ver_made_by;
   USHORT ver_to_extract;
   USHORT gen_purp;
   USHORT method;
   USHORT time;
   USHORT date;
   ULONG crc32;
   ULONG compressed_size;
   ULONG uncompressed_size;
   USHORT filename_length;
   USHORT extra_field_length;
   USHORT file_comment_length;
   USHORT disk_start;
   USHORT internal_attr;
   ULONG external_attr;
   ULONG local_header_offset;
} ZIPENTRY;

typedef ZIPENTRY *PZIPENTRY;
typedef ZIPENTRY FAR *LPZIPENTRY;
typedef ZIPENTRY NEAR *NPZIPENTRY;

typedef struct tagZIPARCHIVE {
   ULONG sig;
   USHORT diskno;
   USHORT disk_w_cd;
   USHORT tot_entries_this_disk;
   USHORT tot_entries;
   ULONG central_dir_size;
   ULONG offset_of_central_dir;
   USHORT comment_length;
} ZIPARCHIVE;

typedef ZIPARCHIVE *PZIPARCHIVE;
typedef ZIPARCHIVE FAR *LPZIPARCHIVE;
typedef ZIPARCHIVE NEAR *NPZIPARCHIVE;

//  .ZOO file constants

#define ZOO_H_TYPE       1      //  archive header type
#define SIZ_TEXT        20      //  Size of header text
#define PATHSIZE       256      //  Max length of pathname
#define FNAMESIZE       13      //  Size of DOS filename
#define LFNAMESIZE     256      //  Size of long filename

#define ZOO_TAG   (ULONG) 0xFDC4A7DCL

//  .ZOO file header format

typedef struct tagZOOARCHIVE {
   char text[SIZ_TEXT];         //  archive header text
   ULONG zoo_tag;               //  identifies archives
   LONG zoo_start;              //  where the archive's data starts
   LONG zoo_minus;              //  for consistency checking of zoo_start
   BYTE major_ver;
   BYTE minor_ver;              //  minimum version to extract all files
   BYTE type;                   //  type of archive header
   LONG acmt_pos;               //  position of archive comment
   USHORT acmt_len;             //  length of archive comment
   USHORT vdata;                //  BYTE in archive;  data about versions
} ZOOARCHIVE;

typedef ZOOARCHIVE *PZOOARCHIVE;
typedef ZOOARCHIVE FAR *LPZOOARCHIVE;
typedef ZOOARCHIVE NEAR *NPZOOARCHIVE;

//  .ZOO directory entry format

typedef struct tagZOOENTRY {
   ULONG zoo_tag;               //  tag -- redundancy check
   BYTE type;                   //  type of directory entry.  always 1 for now
   BYTE packing_method;         //  0 = no packing, 1 = normal LZW
   LONG next;                   //  pos'n of next directory entry
   LONG offset;                 //  position of this file
   USHORT date;                 //  DOS format date
   USHORT time;                 //  DOS format time
   USHORT file_crc;             //  CRC of this file
   LONG org_size;
   LONG size_now;
   BYTE major_ver;
   BYTE minor_ver;              //  minimum version needed to extract
   BYTE deleted;                //  will be 1 if deleted, 0 if not
   BYTE struc;                  //  file structure if any
   LONG comment;                //  points to comment;  zero if none
   USHORT cmt_size;             //  length of comment, 0 if none
   char fname[FNAMESIZE];       //  filename
   short var_dir_len;           //  length of variable part of dir entry
   BYTE tz;                     //  timezone where file was archived
   USHORT dir_crc;              //  CRC of directory entry

   //  fields for variable part of directory entry follow
   BYTE namlen;                 //  length of LONG filename
   BYTE dirlen;                 //  length of directory name
   char lfname[LFNAMESIZE];     //  LONG filename
   char dirname[PATHSIZE];      //  directory name
   USHORT system_id;            //  Filesystem ID
   ULONG fattr;                 //  File attributes -- 24 bits
   USHORT vflag;                //  version flag bits -- one BYTE in archive
   USHORT version_no;           //  file version number if any
} ZOOENTRY;

typedef ZOOENTRY *PZOOENTRY;
typedef ZOOENTRY FAR *LPZOOENTRY;
typedef ZOOENTRY NEAR *NPZOOENTRY;

//  LHARC and LARC header information

typedef struct LZHENTRY {
   char no_bytes;               //  number of BYTEs in header
   char checksum;               //  checksum of remaining BYTEs
   char type[5];                //  no NULL terminator!
   LONG size_now;               //  Size of compressed file
   LONG orig_size;              //  Original size of file
   WORD time;                   //  file time
   WORD date;                   //  file date
   WORD attr;                   //  file attributes
   BYTE name_len;               //  length of file name
} LZHENTRY;

#pragma pack()

typedef int (FAR PASCAL * LPFNARCHIVESEARCH) (PFILESTUFF, PSTR, PINT);

LPFNARCHIVESEARCH PASCAL GetArchiveSearchFunction(PSTR pchFilename);

// *********************** Function Declarations ****************************
extern int PASCAL SearchARC(PFILESTUFF, PSTR, PINT);  //  Search .ARC or .PAK for files
extern int PASCAL SearchZIP(PFILESTUFF, PSTR, PINT);  //  Search .ZIP for files
extern int PASCAL SearchZOO(PFILESTUFF, PSTR, PINT);  //  Search .ZOO for files
extern int PASCAL SearchLZH(PFILESTUFF, PSTR, PINT);  //  Search .LZH for files
extern int PASCAL SearchDWC(PFILESTUFF, PSTR, PINT);  //  Search .DWC for files
