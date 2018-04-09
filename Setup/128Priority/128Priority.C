#include <Windows.H>
#include <WindowsX.H>
#include <ShellAPI.H>
#include <Stdio.H>
#include <Stdlib.H>

#include "Headers/Registry.H"	// Function Prototypes For Registry.Dll
#include "Headers/Windows.H"	// Windows Def & Functions

#include "Headers/Definitions.H"
#include "Headers/Functions.H"
#include "Dialogs/MainDialog.H"
#include "Dialogs/AboutDialog.H"

/////////////////////////////////////////////////////////////////////

int WINAPI WinMain (HWND hInstance, HWND hPrev, LPSTR Cmd, int nCmdShow)
{
	HANDLE xMutex;

	if( (xMutex = OpenMutex(SYNCHRONIZE, FALSE, GammaTitle)) )
	{
		MessageBox (NULL, "You Cannot Change The Priority While 128Gamma Is Running." , TitleCaption, 0 + MB_ICONASTERISK + MB_SYSTEMMODAL);
		ReleaseMutex(xMutex);
		CloseHandle(xMutex);
		return 0;
	}

	if( (xMutex = OpenMutex(SYNCHRONIZE, FALSE, TitleCaption)) )
	{
		MessageBox (NULL, "Only One Instance Can Be Run At A Time!" , TitleCaption, 0 + MB_ICONASTERISK + MB_SYSTEMMODAL);
		ReleaseMutex(xMutex);
		CloseHandle(xMutex);
		return 0;
	}
	xMutex = CreateMutex(0, 0, TitleCaption);

	MainInstance = hInstance;

	DialogBox(MainInstance,MAKEINTRESOURCE(100),0,MainDialog);

	ReleaseMutex(xMutex);
	CloseHandle(xMutex);

	return 0;
}

/////////////////////////////////////////////////////////////////////

