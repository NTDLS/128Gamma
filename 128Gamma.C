/*
 128Gamma.C - Main 128Gamma Source File

int WINAPI WinMain(HWND hInst, HWND hPrev, LPSTR Cmd, int nCmdShow)

*/
			/*---( 128Gamma - Data Encryption Project )---*/
				 /*---( Started February 7, 1998 )---*/

#include <Windows.H>
#include <WindowsX.H>
#include <IO.H>
#include <Stdio.H>
//#include <Stdlib.H>
//#include <string.h>
//#include <ctype.h>
#include <ShellAPI.H>

#include "Headers/Definitions.H"

#include "Libraries/Registry/Registry.H"	// Registry Def & Functions
#include "Libraries/Windows/Windows.H"		// Windows Def & Functions

#include "Headers/DeleteObject.H"

#include "Headers/Functions.H"

#include "Headers/PackFiles.H"
#include "Headers/UnpackFiles.H"

#include "Dialogs/Password.H"
#include "Dialogs/About.H"
#include "Dialogs/Status.H"
#include "Dialogs/Compatible.H"

#include "Headers/BlowFish.H"
#include "Headers/128Coding.H"

#include "Headers/Processes.H"	 // Crypting Processes Functions
#include "Headers/Compression.C" // Compression Functions

int WINAPI WinMain(HWND, HWND, LPSTR, int);

////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HWND hInst, HWND hPrev, LPSTR Cmd, int nCmdShow)
{
	HANDLE xMutex;

	printf("0\n");

	if( (xMutex = OpenMutex(SYNCHRONIZE, FALSE, "128Gamma Priority")) )
	{
		MessageBox (NULL, "You Cannot Run 128Gamma While You Are Changing The Priority." , TitleCaption, 0 + MB_ICONASTERISK + MB_SYSTEMMODAL);
		ReleaseMutex(xMutex);
		CloseHandle(xMutex);
		return 0;
	}

	if( (xMutex = OpenMutex(SYNCHRONIZE, FALSE, TitleCaption)) )
	{
		MessageBox (NULL, "Only One Instance Of 128Gamma Can Run At A Time!" , TitleCaption, 0 + MB_ICONASTERISK + MB_SYSTEMMODAL);
		ReleaseMutex(xMutex);
		CloseHandle(xMutex);
		return 0;
	}
	xMutex = CreateMutex(0, 0, TitleCaption);

	hInstance = hInst;

	if(SetTempVariables())
		Error("Main: Error Setting Temporary Files. Error 1.");

	if(strcmp(Cmd,"128GammaSignature") == 0)
	{
		char xSignature[30];
		sprintf(xSignature,"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",65,117,116,104,101,110,116,105,99,32,49,50,56,71,97,109,109,97,32,83,105,103,110,97,116,117,114,101);
		MessageBox(0, xSignature ,GammaVersion, 0 + MB_TASKMODAL);
		ReleaseMutex(xMutex);
		CloseHandle(xMutex);
		return 0;
	}

	if(ProcessInput(Cmd)) // If No (Or Invalad) Input Diaplay About Box
		DialogBox(hInstance,MAKEINTRESOURCE(1000),0,AboutDialog);

	ReleaseMutex(xMutex);
	CloseHandle(xMutex);

	return 0;
}

////////////////////////////////////////////////////////////////////////

