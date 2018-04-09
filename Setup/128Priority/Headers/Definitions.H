
#define TitleCaption "128Gamma Priority"
#define GammaTitle "128Gamma Encryption"
#define FileVersion "1.0 - Revision C"
#define RegistryKey "*\\Shell\\128Gamma_Encryption\\AppInfo\\Priority"

//---------------------(HWND & INSTANCE Definitions)
HWND  MainInstance; 		// WinMain (Application) Instance
HWND  MainDialog_hWnd;		// MainDialog Instance
HWND  AboutDialog_hWnd; 	// MainDialog Instance

//---------------------(Function Prototypes)
int WINAPI WinMain (HWND, HWND, LPSTR, int );
int GetValues(void);
int SetValues(void);

void RegError_Get(void);
void RegError_Set(void);

//---------------------(Dialog Prototypes)
int MainDialog(HWND, int ,int ,int);
int AboutDialog(HWND, int ,int ,int);

//---------------------(Various Global Variables)

int LastValue_1 = 0;
int LastValue_2 = 0;

DWORD PriorityClass = 0;
DWORD ThreadPriority = 0;
DWORD ThreadBoost = 0;
DWORD ProcessBoost = 0;

//----------------------------------------------------------------

