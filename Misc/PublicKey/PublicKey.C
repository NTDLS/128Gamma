#include <Windows.H>
#include <WindowsX.H>
#include <ShellAPI.H>
#include <Stdio.H>

#include "Libraries/Delta/Delta.H"
#include "Headers/CreateKey.H"

#include "Headers/Definitions.H"
#include "Headers/Functions.H"

#include "Dialogs/MainDialog.H"

/////////////////////////////////////////////////////////////////////

extern int WINAPI WinMain (HWND hInstance, HWND hPrev, LPSTR Cmd, int nCmdShow)
{
	MainInstance = hInstance;
	DialogBox(MainInstance,MAKEINTRESOURCE(100),0,MainDilaog);
	return 0;
}

/////////////////////////////////////////////////////////////////////

