#include <Windows.H>
#include <WindowsX.H>
#include <ShellAPI.H>
#include <Io.H>
#include <Stdio.H>

#include "Headers/Registry.H"	// Function Prototypes For Registry.Dll
#include "Headers/Windows.H"	// Windows Def & Functions

#include "Headers/Definitions.H"
#include "Headers/Functions.H"

#include "Dialogs/MainDialog.H"
#include "Dialogs/AboutDialog.H"

/////////////////////////////////////////////////////////////////////

int WINAPI WinMain (HWND hInstance, HWND hPrev, LPSTR Cmd, int nCmdShow)
{
	MainInstance = hInstance;
	DialogBox(MainInstance,MAKEINTRESOURCE(100),0,MainDilaog);
	return 0;
}

/////////////////////////////////////////////////////////////////////

