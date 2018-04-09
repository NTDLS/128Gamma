
/////////////////////////////////////////////////////////////////////

int MainDialog (HWND hDlg, int Msg, int wParam, int lParam)
{
	static HWND SystemMenu = 0;

	while(1) // Loop Forever
	{
		if(Msg==WM_INITDIALOG)
		{
			MainDialog_hWnd=hDlg;

			SystemMenu = GetSystemMenu(MainDialog_hWnd,FALSE);
			AppendMenu(SystemMenu,MF_SEPARATOR,0,0);
			AppendMenu(SystemMenu,MF_STRING,1000,"www.NetWorkDLS.com");
			AppendMenu(SystemMenu,MF_STRING,1001,"Version");
			AppendMenu(SystemMenu,MF_STRING,1002,"About");

			SendMessage(MainDialog_hWnd,(UINT)WM_SETTEXT,(WPARAM)0,(LPARAM)TitleCaption);
			SendMessage(MainDialog_hWnd,WM_SETICON,TRUE,(LPARAM)LoadIcon(GetWindowInstance(MainDialog_hWnd),MAKEINTRESOURCE(100)));

			GetValues(); // Get Inital Values

			CenterWindow(MainDialog_hWnd);
			break;
		}

		if(Msg==WM_COMMAND)
		{
			if(wParam == 1001) // - Priority Class Idle
			{
				CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_UNCHECKED);
				LastValue_1 = 1001;
				CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_CHECKED);
			}
			if(wParam == 1002) // - Priority Class Normal
			{
				CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_UNCHECKED);
				LastValue_1 = 1002;
				CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_CHECKED);
			}
			if(wParam == 1003) // - Priority Class High
			{
				CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_UNCHECKED);
				LastValue_1 = 1003;
				CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_CHECKED);
			}
			if(wParam == 1004) // - Priority Class Real Time
			{
				CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_UNCHECKED);
				LastValue_1 = 1004;
				CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_CHECKED);
			}

			// ------------------------------

			if(wParam == 2001) // - Thread Priority Idle
			{
				CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_UNCHECKED);
				LastValue_2 = 2001;
				CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
			}
			if(wParam == 2002) // - Thread Priority Lowest
			{
				CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_UNCHECKED);
				LastValue_2 = 2002;
				CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
			}
			if(wParam == 2003) // - Thread Priority Below Normal
			{
				CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_UNCHECKED);
				LastValue_2 = 2003;
				CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
			}
			if(wParam == 2004) // - Thread Priority Normal
			{
				CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_UNCHECKED);
				LastValue_2 = 2004;
				CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
			}
			if(wParam == 2005) // - Thread Priority Above Normal
			{
				CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_UNCHECKED);
				LastValue_2 = 2005;
				CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
			}
			if(wParam == 2006) // - Thread Priority Highest
			{
				CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_UNCHECKED);
				LastValue_2 = 2006;
				CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
			}
			if(wParam == 2007) // - Thread Priority Real Time
			{
				CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_UNCHECKED);
				LastValue_2 = 2007;
				CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
			}

			if(wParam==100) //- Okay (Dialog Command)
			{
				if(SetValues()) // Set New Values To Registry
				{
					EndDialog(MainDialog_hWnd,0);
				}
			}
			if(wParam==200) //- About (Dialog Command)
			{
				ShowWindow(MainDialog_hWnd, SW_HIDE);
				DialogBox(MainInstance,MAKEINTRESOURCE(1000),MainDialog_hWnd,AboutDialog);
				ShowWindow(MainDialog_hWnd, SW_SHOW);
			}
			if(wParam==300) //- Exit (Dialog Command)
			{
				EndDialog(MainDialog_hWnd,0);
			}
			break;
		}

		if(Msg==WM_SYSCOMMAND)
		{
			if(LOWORD(wParam)==1000) //- NetWorkDLS.Com (SysMenu Command)
				ShellExecute(0,"Open","Http://www.NetWorkDLS.com",NULL,NULL,SW_SHOWNORMAL);

			if(LOWORD(wParam) == 1001) //- Version (SysMenu Command)
				MessageBox(MainDialog_hWnd, FileVersion, TitleCaption, 0 + MB_ICONASTERISK + MB_TASKMODAL);

			if(LOWORD(wParam) == 1002)
				DialogBox(MainInstance,MAKEINTRESOURCE(1000),MainDialog_hWnd,AboutDialog); //Start Dialog
		}

		if(Msg==WM_CLOSE) //Exit (Windows Message)
		{
			EndDialog(MainDialog_hWnd,0);
		}

		break;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////

