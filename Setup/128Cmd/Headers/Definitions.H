#if defined(__cplusplus)
#error 128Gamma was designed to be compiled using C not C++.
#endif

#define TitleCaption "128Gamma Console Encryption"
#define GammaVersion "Version 1.0  - Revision A"

#define FlagLEN 8           // Length Of Encrypt/Pack Flags

#define PACCompress "128CPack"
#define ENCCompress "128CFile"

#define PACFlag "128Pack~"
#define ENCFlag "128File~"

#define ABIFlag "ABICODER"

#define VersionInfo 10000    // VersionInfo SystemMenu Identifier
#define CompatibleInfo 10500 // CompatibleDialog SystemMenu Identifier

#define BufferSize 512    // Size Of Buffer Used For Copy Processes
#define MegaByte 1048576  // One Megabyte (1,048,576 Bytes)

#define ShortVersion "1.0"
#define MaxFileSize MegaByte * 100

char *GetTempDirectory(void);
char *LCase (char *);

int PreCheckFile(char *);
int ProcessInput(char *,char *,char *);

BOOL FAccess(char *, short);

DWORD FileSize(char *);

void Error(char *);

char Buffer[BufferSize];
char *Status;
char GlobalFileName[257];

char *ShortVersion;

DWORD MaxFiles        = 0;
DWORD MaxPackFileSize = 0;
DWORD MaxFileSize     = 0;

BOOL UseFlags = TRUE;
BOOL CompressEncrypt = FALSE;

