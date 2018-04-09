/*

 Definitions.H - Variable Definition File (Various & Assorted)

*/

//--------------------------------------------------------------------

#if defined(__cplusplus)
#error 128Gamma was designed to be compiled using C not C++.
#endif

#define TitleCaption "128Gamma Encryption"
#define GammaVersion "Version 3.5 - Not Revised"

#define FlagLEN 8			// Length Of Encrypt/Pack Flags

#define PACCompress "128CPack"
#define ENCCompress "128CFile"

#define PACFlag "128Pack~"
#define ENCFlag "128File~"

#define ABIFlag "ABICODER"

#define BufferSize 512	  // Size Of Buffer Used For Copy Processes
#define MegaByte 1048576  // One Megabyte (1,048,576 Bytes)

#define ShortVersion "3.5"
#define MaxFiles 5000
#define MaxPackFileSize MegaByte * 100
#define MaxFileSize MegaByte * 250

//--------------------------------------------------------------------
		/* Thread Handles, ID's And Prototypes*/ // Begin

// --------

DWORD WINAPI EncryptProcess_Thread(LPVOID);
HANDLE EncryptProcess_Thread_Handle;
long EncryptProcess_Thread_ID;

DWORD WINAPI DecryptProcess_Thread(LPVOID);
HANDLE DecryptProcess_Thread_Handle;
long DecryptProcess_Thread_ID;

DWORD WINAPI PackProcess_Thread(LPVOID);
HANDLE PackProcess_Thread_Handle;
long PackProcess_Thread_ID;

DWORD WINAPI UnpackProcess_Thread(LPVOID);
HANDLE UnpackProcess_Thread_Handle;
long UnpackProcess_Thread_ID;

// --------

DWORD WINAPI CompressEncryptProcess_Thread(LPVOID);
HANDLE CompressEncryptProcess_Thread_Handle;
long CompressEncryptProcess_Thread_ID;

DWORD WINAPI DecompressDecryptProcess_Thread(LPVOID);
HANDLE DecompressDecryptProcess_Thread_Handle;
long DecompressDecryptProcess_Thread_ID;

DWORD WINAPI CompressPackProcess_Thread(LPVOID);
HANDLE CompressPackProcess_Thread_Handle;
long CompressPackProcess_Thread_ID;

DWORD WINAPI DecompressUnpackProcess_Thread(LPVOID);
HANDLE DecompressUnpackProcess_Thread_Handle;
long DecompressUnpackProcess_Thread_ID;

// --------

DWORD WINAPI StatusDialog_Thread(LPVOID );
HANDLE StatusDialog_Thread_Handle;
long StatusDialog_Thread_ID;

		/* Thread Handles, ID's And Prototypes*/ // End
//--------------------------------------------------------------------
		/* Various Function Prototypes */ // Begin

void Error(char *);
void ClearBuffer(void);
void Update_LastUsed(void);
void FreeMemory(void);
void Clear_ReadTemp(void);

int PasswordDialog (HWND, int , int , int );
int AboutDialog (HWND, int , int , int );
int StatusDialog (HWND , int , int , int );
int CompatibleDialog (HWND , int , int , int );

int PackFiles(char *,char *);
int UnPackFiles(char *,char *);

int IncodeMain(char *, char *);
int DecodeMain(char *, char *);

int Compress(char *, char *);
int Decompress(char *, char *);

int EncryptProcess(void);
int DecryptProcess(void);
int PackProcess(void);
int UnpackProcess(void);
int CompressEncryptProcess(void);
int DecompressDecryptProcess(void);
int CompressPackProcess(void);
int DecompressUnpackProcess(void);

int ProcessInput(char *);
int CancelProcess(void);

int Get_FileNames (char *);
int Get_SubFileNames (char *, char *);
int PackFolder(char *, char *);
int CleanUp(int, int);
int PreCheckFile(char *);
int SetTempVariables(void);

BOOL SetPriority(void);

LRESULT CALLBACK SetColor(int,int,int,int);

		/* Various Function Prototypes */ // End
//--------------------------------------------------------------------
		/* Various Global Variables */ // Begin

char Buffer[BufferSize];
char CompressTemp[257];
char PackTemp[257];
char Password[75];
char ReadTemp[1024];
char InputFileName[257];
char OutputFileName[257];

struct _Package{
char *FolderNames[MaxFiles+2];
char *FileNames[MaxFiles+2];
DWORD FileCount;
DWORD FolderCount;
}; struct _Package Package;

char *Status;
char *CodedKey;
//char *ShortVersion;
//DWORD MaxFiles		  = 0;
//DWORD MaxPackFileSize = 0;
//DWORD MaxFileSize	  = 0;

long double StatusValue = 0;

		/* Various Global Variables */ // End
//--------------------------------------------------------------------
		/* Various Windows Handles */ // Begin

HWND hInstance			   = 0;
HWND PasswordDialog_hWnd   = 0;
HWND AboutDialog_hWnd	   = 0;
HWND StatusDialog_hWnd	   = 0;
HWND CompatibleDialog_hWnd = 0;

HWND StatusText_hWnd = 0;
HWND StatusBar_hWnd  = 0;

		/* Various Windows Handles */ // End
//--------------------------------------------------------------------
		/* Global Variables Used As Flags (BOOL) */ // Begin

BOOL CompressEncrypt   = FALSE;
BOOL DecompressDecrypt = FALSE;
BOOL CompressPack	   = FALSE;
BOOL DecompressUnpack  = FALSE;

BOOL Encrypt = FALSE;
BOOL Decrypt = FALSE;
BOOL Pack	 = FALSE;
BOOL Unpack  = FALSE;

BOOL DeleteSource = FALSE;
BOOL CancelFlag = FALSE;
BOOL Verifiy	= FALSE;
BOOL DoneFlag	= FALSE;
BOOL KillFlag	= FALSE;
BOOL UseFlags	= TRUE;

		/* Global Variables Used As Flags */ // End
//--------------------------------------------------------------------


