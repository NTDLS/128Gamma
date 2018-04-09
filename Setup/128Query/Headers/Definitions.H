
#define TitleCaption "128Gamma Query"  // Application Title
#define GammaVersion "Version 1.0 - Revision B"

//---------------------(HWND & INSTANCE Definitions)

HWND  MainInstance; 		// WinMain (Application) Instance
HWND  MainDialog_hWnd;		// MainDialog Instance
HWND  AboutDialog_hWnd; 	// MainDialog Instance

//---------------------(Function Prototypes)

int WINAPI WinMain (HWND, HWND, LPSTR, int );

void Refresh(HWND);
void RegError(HWND);

//---------------------(Dialog Prototypes)

int MainDilaog(HWND, int ,int ,int);
int AboutDialog(HWND, int ,int ,int);

//----------------------------------------------------------------


