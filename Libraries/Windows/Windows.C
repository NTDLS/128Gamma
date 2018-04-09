#include <Windows.H>
#include <WindowsX.H>
#include <IO.H>
#include <Stdio.H>
#include <Stdlib.H>

#include "Windows.H"

///////////////////////////////////////////////////////////////////////

//__declspec(dllexport)

/////////////////////////////////////////////////////////////////////

BOOL WINAPI __declspec(dllexport) LibMain(HINSTANCE hInst, DWORD Reason, LPVOID Reserved)
{
    switch (Reason)
    {
        case DLL_PROCESS_ATTACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
    }
    return TRUE;
}

/////////////////////////////////////////////////////////////////////

__declspec(dllexport) void SetText (HWND hDlg, char *Txt)
{
    SendMessage(hDlg,(UINT)WM_SETTEXT,(WPARAM)0,(LPARAM)Txt);
}

/////////////////////////////////////////////////////////////////////

__declspec(dllexport) char *GetText (HWND WindowHandle)
{
    char *ReturnTemp;
    int Length;
    ReturnTemp = (char *) malloc(256);
    Length = GetWindowTextLength(WindowHandle);
    GetWindowText(WindowHandle,ReturnTemp,Length+1);
    return ReturnTemp;
}

/////////////////////////////////////////////////////////////////////

__declspec(dllexport) void CenterWindow (HWND hWnd)
{
    RECT  wRect;
    DWORD  x;
    DWORD  y;
    GetWindowRect(hWnd,&wRect);
    x = (GetSystemMetrics(SM_CXSCREEN)-(wRect.right-wRect.left))/2;
    y = (GetSystemMetrics(SM_CYSCREEN)-(wRect.bottom-wRect.top+GetSystemMetrics(SM_CYCAPTION)))/2;
    SetWindowPos(hWnd,NULL,x,y,0,0,SWP_NOSIZE|SWP_NOZORDER);
}

/////////////////////////////////////////////////////////////////////////////

__declspec(dllexport) char * Get_Time ()
{
    SYSTEMTIME st;                     //SystemTime
    char *TimeTemp;                    //SystemTime (Time)
    TimeTemp = (char *) malloc(257);
    GetLocalTime(&st);
    GetTimeFormat(LOCALE_USER_DEFAULT,TIME_NOSECONDS,&st,NULL,TimeTemp,64);
    return (TimeTemp);
}

//////////////////////////////////////////////////////////////////////

__declspec(dllexport) char * Get_LongDate ()
{
    SYSTEMTIME st;              //SystemTime
    char *DateTemp;             //SystemTime (Date)
    DateTemp = (char *) malloc(257);
    GetLocalTime(&st);
    GetDateFormat(LOCALE_USER_DEFAULT,DATE_LONGDATE,&st,NULL,DateTemp,64);
    return (DateTemp);
}

//////////////////////////////////////////////////////////////////////

__declspec(dllexport) char * Get_ShortDate ()
{
    SYSTEMTIME st;              //SystemTime
    char *DateTemp;             //SystemTime (Date)
    DateTemp = (char *) malloc(257);
    GetLocalTime(&st);
    GetDateFormat(LOCALE_USER_DEFAULT,DATE_SHORTDATE,&st,NULL,DateTemp,64);
    return (DateTemp);
}

//////////////////////////////////////////////////////////////////////

__declspec(dllexport) char * Get_MachineName ()
{
    DWORD SizeOfName = 256;
    char *Name;
    Name = (char *) malloc(SizeOfName);
    GetComputerName(Name,&SizeOfName);
    return Name;
}

/////////////////////////////////////////////////////////////////////

__declspec(dllexport) char * Get_UserName ()
{
    DWORD SizeOfName = 256;
    char *Name;
    Name = (char *) malloc(SizeOfName);
    GetUserName(Name,&SizeOfName);
    return Name;
}

//////////////////////////////////////////////////////////////////////

__declspec(dllexport) char * Get_OsVersion ()
{
    DWORD dwVersion;
    char *xBuffer;
    OSVERSIONINFO OS;
    OS.dwOSVersionInfoSize = sizeof(OS);
    dwVersion = GetVersion();
    xBuffer = (char *) malloc(256);

    if(GetVersionEx(&OS))
    {
        if(OS.dwPlatformId == 1)
        {
            if(OS.dwMinorVersion == 0)
                sprintf(xBuffer, "Windows 95 %d.%d (Build %d)",LOBYTE(LOWORD(dwVersion)),HIBYTE(LOWORD(dwVersion)),OS.dwBuildNumber & 0xFFFF);
            if(OS.dwMinorVersion == 10)
                sprintf(xBuffer, "Windows 98 %d.%d (Build %d)",LOBYTE(LOWORD(dwVersion)),HIBYTE(LOWORD(dwVersion)),OS.dwBuildNumber & 0xFFFF);
            if(OS.dwMinorVersion == 90)
                sprintf(xBuffer, "Windows ME %d.%d (Build %d)",LOBYTE(LOWORD(dwVersion)),HIBYTE(LOWORD(dwVersion)),OS.dwBuildNumber & 0xFFFF);
        }

        if(OS.dwPlatformId == 2)
        {
            if(OS.dwMajorVersion == 3)
                sprintf(xBuffer, "Windows NT %d.%d (Build %d)",LOBYTE(LOWORD(dwVersion)),HIBYTE(LOWORD(dwVersion)),OS.dwBuildNumber & 0xFFFF);
            if(OS.dwMajorVersion == 4)
                sprintf(xBuffer, "Windows NT %d.%d (Build %d)",LOBYTE(LOWORD(dwVersion)),HIBYTE(LOWORD(dwVersion)),OS.dwBuildNumber & 0xFFFF);
            if(OS.dwMajorVersion == 5)
                sprintf(xBuffer, "Windows 2000 %d.%d (Build %d)",LOBYTE(LOWORD(dwVersion)),HIBYTE(LOWORD(dwVersion)),OS.dwBuildNumber & 0xFFFF);
        }
    }
    return xBuffer;
}

//////////////////////////////////////////////////////////////////////

__declspec(dllexport) char * Get_CurrentDirectory ()
{
    char *Name;
    Name = (char *) malloc(256);
    GetCurrentDirectory(256,Name);
    return Name;
}

/////////////////////////////////////////////////////////////////////

__declspec(dllexport) char * Get_TempDirectory ()
{
    char *Name;
    Name = (char *) malloc(256);
    GetTempPath(256,Name);
    return Name;
}

/////////////////////////////////////////////////////////////////////

__declspec(dllexport) char * Get_WindowsDirectory ()
{
    char *Name;
    Name = (char *) malloc(256);
    GetWindowsDirectory(Name,256);
    return Name;
}

/////////////////////////////////////////////////////////////////////

__declspec(dllexport) char * Get_SystemDirectory ()
{
    char *Name;
    Name = (char *) malloc(256);
    GetSystemDirectory(Name,256);
    return Name;
}

/////////////////////////////////////////////////////////////////////

__declspec(dllexport) char * UCase (char *InputString)
{
    char *StringTemp;
    StringTemp = (char *) malloc(256);
    sprintf(StringTemp,"%s",strupr(InputString));
    return StringTemp;
}

/////////////////////////////////////////////////////////////////////

__declspec(dllexport) char * LCase (char *InputString)
{
    char *StringTemp;
    StringTemp = (char *) malloc(256);
    sprintf(StringTemp,"%s",strlwr(InputString));
    return StringTemp;
}

/////////////////////////////////////////////////////////////////////

__declspec(dllexport) char * IntChar (int xInput)
{
    char *LocalTemp;
    LocalTemp = (char *) malloc(256);
    sprintf(LocalTemp,"%d",xInput);
    return (LocalTemp);
}

/////////////////////////////////////////////////////////////////////

__declspec(dllexport) int CharInt (char *xString)
{
    int LocalTemp;
    LocalTemp = atoi(xString);
    return LocalTemp;
}

/////////////////////////////////////////////////////////////////////

__declspec(dllexport) BOOL FileAccess(char *FileName, short Mode)
{
    if(access(FileName, Mode) == 0)
        return TRUE;
    return FALSE;
}

/////////////////////////////////////////////////////////////////////

__declspec(dllexport) DWORD Get_FileSize(char *File2Size)
{
    FILE *Source;
    DWORD size;
    Source = CreateFile(File2Size,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
    size = GetFileSize(Source,NULL);
    if(size == HFILE_ERROR) return -1;
    CloseHandle(Source);
    return size;
}

/////////////////////////////////////////////////////////////////////

__declspec(dllexport) BOOL IsDirectory(char *xRequest)
{
    int Attrib;
    Attrib = GetFileAttributes(xRequest);
    if(Attrib != 0xFFFFFFFF)
    {
        if(Attrib & FILE_ATTRIBUTE_DIRECTORY)
            return TRUE;
    }
    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

__declspec(dllexport) LRESULT CALLBACK SetColor (int TxtColr, int BkColr, int wParam, int lParam)
{
    static HBRUSH ReUsableBrush;
    DeleteObject(ReUsableBrush);
    ReUsableBrush = CreateSolidBrush(BkColr);
    SetTextColor((HDC)wParam,TxtColr);
    SetBkColor((HDC)wParam,BkColr);
    return (LRESULT)ReUsableBrush;
}

////////////////////////////////////////////////////////////////////////

__declspec(dllexport) int IsMouseOver(HWND xHandle)
{
    RECT Rectangle;
    POINT Pointer;
    memset(&Rectangle,0,sizeof(Rectangle));
    memset(&Pointer,0,sizeof(Pointer));

    GetWindowRect(xHandle,&Rectangle);
    GetCursorPos(&Pointer);

    return PtInRect(&Rectangle,Pointer);
}

////////////////////////////////////////////////////////////////////////

__declspec(dllexport) void SetApplicationPriority(HANDLE CurrentProcessHandle, HANDLE CurrentThreadHandle, short xPriorityClass, short xThreadPriority, BOOL ProcessBoost, BOOL ThreadBoost)
{
    if(xPriorityClass == 1)SetPriorityClass(CurrentProcessHandle,IDLE_PRIORITY_CLASS);
    if(xPriorityClass == 2)SetPriorityClass(CurrentProcessHandle,NORMAL_PRIORITY_CLASS);
    if(xPriorityClass == 3)SetPriorityClass(CurrentProcessHandle,HIGH_PRIORITY_CLASS);
    if(xPriorityClass == 4)SetPriorityClass(CurrentProcessHandle,REALTIME_PRIORITY_CLASS);

    if(xThreadPriority == 1)SetThreadPriority(CurrentThreadHandle,THREAD_PRIORITY_IDLE);
    if(xThreadPriority == 2)SetThreadPriority(CurrentThreadHandle,THREAD_PRIORITY_LOWEST);
    if(xThreadPriority == 3)SetThreadPriority(CurrentThreadHandle,THREAD_PRIORITY_BELOW_NORMAL);
    if(xThreadPriority == 4)SetThreadPriority(CurrentThreadHandle,THREAD_PRIORITY_NORMAL);
    if(xThreadPriority == 5)SetThreadPriority(CurrentThreadHandle,THREAD_PRIORITY_ABOVE_NORMAL);
    if(xThreadPriority == 6)SetThreadPriority(CurrentThreadHandle,THREAD_PRIORITY_HIGHEST);
    if(xThreadPriority == 7)SetThreadPriority(CurrentThreadHandle,THREAD_PRIORITY_TIME_CRITICAL);

    if(ProcessBoost == 0)SetProcessPriorityBoost(CurrentProcessHandle,TRUE); //Disable
    if(ProcessBoost == 1)SetProcessPriorityBoost(CurrentProcessHandle,FALSE); //Enable

    if(ThreadBoost == 0)SetThreadPriorityBoost(CurrentThreadHandle,TRUE); //Disable
    if(ThreadBoost == 1)SetThreadPriorityBoost(CurrentThreadHandle,FALSE); //Enable
}

////////////////////////////////////////////////////////////////////////

