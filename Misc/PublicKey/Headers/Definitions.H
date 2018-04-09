
#define TitleCaption "Public Key"  // Application Title
#define GammaVersion "Version 1.0 - Not Revised"

#define VersionInfo 10000
#define GammaSignature 10001

//---------------------(HWND & INSTANCE Definitions)

static HWND MainInstance;		  // WinMain (Application) Instance
static HWND MainDialog_hWnd;	  // MainDialog Instance

//---------------------(Function Prototypes)

extern int WINAPI WinMain (HWND, HWND, LPSTR, int );

extern char *GetText (HWND);

extern void SetText (HWND, char *);
extern void CenterWindow (HWND);

//---------------------(Dialog Prototypes)
extern int MainDilaog(HWND, int ,int ,int);
//----------------------------------------------------------------

