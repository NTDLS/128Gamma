////////////////////////////////////////////////////////////////////////

int CompatibleDialog (HWND hDlg, int Msg, int wParam, int lParam)
{
	static HWND AIBLink_hWnd;
	static HCURSOR hHand;

	for(;;)
	{
		if(Msg==WM_INITDIALOG)
		{
			CompatibleDialog_hWnd = hDlg;
			AIBLink_hWnd = GetDlgItem(CompatibleDialog_hWnd,2500);
			hHand = LoadCursor(hInstance,MAKEINTRESOURCE(100));
			SendMessage(CompatibleDialog_hWnd, WM_SETTEXT, FALSE,(LPARAM)"Compatible Software");
			SendMessage(CompatibleDialog_hWnd, WM_SETICON, TRUE,(LPARAM)LoadIcon(GetWindowInstance(CompatibleDialog_hWnd), MAKEINTRESOURCE(1)));
			CenterWindow(CompatibleDialog_hWnd);
			break;
		}
		//----------------------------------------------------------------------

		if(IsMouseOver(AIBLink_hWnd))
		{
			SetCursor(hHand);
			if(Msg==WM_LBUTTONUP)
			{
				ShellExecute(0,"Open","Http://www.ABISoft.net",NULL,NULL,SW_SHOWNORMAL);
				EndDialog(CompatibleDialog_hWnd,0);
			}
		}

		if(Msg == WM_CTLCOLORSTATIC)
		{
			if((HANDLE)lParam == AIBLink_hWnd)
			{
				return SetColor(RGB(0,0,255),RGB(191,191,191),wParam,lParam);
			}
		}

		if(Msg==WM_COMMAND)
		{
			EndDialog(CompatibleDialog_hWnd,0);
			AboutDialog_hWnd = 0;
			break;
		}
		break;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////


