
/////////////////////////////////////////////////////////////////////

int MainDilaog (HWND hDlg, int Msg, int wParam, int lParam)
{
	static HWND SystemMenu;
	static HWND KeyEdit_hWnd;

	while(1) // Loop Forever
	{
		if(Msg==WM_INITDIALOG)
		{
			MainDialog_hWnd = hDlg;

			SendMessage(MainDialog_hWnd,(UINT)WM_SETTEXT,(WPARAM)0,(LPARAM)TitleCaption);
			SendMessage(MainDialog_hWnd,WM_SETICON,TRUE,(LPARAM)LoadIcon(GetWindowInstance(MainDialog_hWnd),MAKEINTRESOURCE(100)));

			KeyEdit_hWnd = GetDlgItem(MainDialog_hWnd,500);

			SystemMenu = GetSystemMenu(MainDialog_hWnd,FALSE);
			AppendMenu(SystemMenu,MF_SEPARATOR,0,0);
			AppendMenu(SystemMenu,MF_STRING,VersionInfo,"Version");

			CenterWindow(MainDialog_hWnd);
			break;
		}

		if(Msg == WM_COMMAND)
		{
			if(wParam == 100) // OK (Dialog Command)
			{
				CreateKey(GetText(KeyEdit_hWnd));
//				  EndDialog(MainDialog_hWnd,0);
			}
			break;
		}

		if(Msg == WM_CLOSE) //Exit (Windows Message)
		{
			EndDialog(MainDialog_hWnd,0);
		}

		if(Msg == WM_SYSCOMMAND)
		{
			if(LOWORD(wParam) == VersionInfo)
			{
				MessageBox(MainDialog_hWnd, GammaVersion, TitleCaption, 0 + MB_ICONASTERISK + MB_TASKMODAL);
			}
		}
		break;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////

