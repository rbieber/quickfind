#define IDS_EGOLINE               0
#define IDS_DIRECTORYLINE         1
#define IDS_CANTFINDFILE          2
#define IDS_INVALIDDRIVE          3
#define IDS_OUTOFMEMORY           4
#define IDS_REDIRECTING           5
#define IDS_REDIRECTFAILED        6
#define IDS_ARCHIVECORRUPT        7
#define IDS_CANTOPENFILE          8
#define IDS_CANTREADDRIVE         9
#define IDS_CANTCHANGEDIRECTORY  10

extern char *szMessages[];

extern int  DisplayMessage(int, ...);
extern BOOL PASCAL ShowNoPromptMessage(int, PSTR);
extern char PASCAL yesno(int, int, char);
extern void PASCAL usage(BOOL);


