#define LoByte(x) ((unsigned char)((x) & 0xff))
#define HiByte(x) ((unsigned char)((unsigned short)(x) >> 8))

#define FAccess  6   // Check for read and write permission
#define FRead    4   // Check for read permission
#define FWrite   2   // Check for write permission
#define FExecute 1   // Execute (Ignored)
#define FExist   0   // Check for existence of file

////////////////////////////////////////////////////////////////////////


void CenterWindow (HWND);
void SetText (HWND, char *);
char *GetText (HWND);

char * IntChar (int);
int CharInt (char *);

char * LCase (char *);
char * UCase (char *);

char * Get_Time (void);
char * Get_LongDate (void);
char * Get_ShortDate (void);

BOOL  FileAccess(char *, short);
DWORD Get_FileSize(char *);

char * Get_TempDirectory (void);
char * Get_WindowsDirectory (void);
char * Get_SystemDirectory (void);
char * Get_CurrentDirectory (void);

char * Get_OsVersion (void);
char * Get_UserName (void);
char * Get_MachineName (void);

int IsMouseOver(HWND);

BOOL IsDirectory(char *);

LRESULT CALLBACK SetColor(int,int,int,int);

void SetApplicationPriority(HANDLE, HANDLE, short, short, BOOL, BOOL);

////////////////////////////////////////////////////////////////////////


