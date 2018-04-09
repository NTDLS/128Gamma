
/////////////////////////////////////////////////////////////////////

extern void SetText (HWND hDlg, char *Txt)
{
	SendMessage(hDlg,(UINT)WM_SETTEXT,(WPARAM)0,(LPARAM)Txt);
}

/////////////////////////////////////////////////////////////////////

extern char *GetText (HWND hDlg)
{
	static int StrCnt;
	static LPSTR StrFunc[16];
	static int T;
	static char Buffer[2048];
	if (++StrCnt==16) StrCnt = 0;
	if (StrFunc[StrCnt]) free (StrFunc[StrCnt]);
	StrFunc[StrCnt] = (char*) malloc (2048);
	memset (StrFunc[StrCnt],0,2048);
	T=GetWindowTextLength(hDlg);
	GetWindowText(hDlg,Buffer,T+1);
	sprintf(StrFunc[StrCnt],"%s",Buffer);
	return StrFunc[StrCnt];
}

/////////////////////////////////////////////////////////////////////

extern void CenterWindow (HWND hWnd)
{
	static	RECT  wRect;
	static	DWORD  x;
	static	DWORD  y;
	GetWindowRect(hWnd,&wRect);
	x=(GetSystemMetrics(SM_CXSCREEN)-(wRect.right-wRect.left))/2;
	y=(GetSystemMetrics(SM_CYSCREEN)-(wRect.bottom-wRect.top+GetSystemMetrics(SM_CYCAPTION)))/2;
	SetWindowPos(hWnd,NULL,x,y,0,0,SWP_NOSIZE|SWP_NOZORDER);
}

/////////////////////////////////////////////////////////////////////

