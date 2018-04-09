
/////////////////////////////////////////////////////////////////////

int MainDilaog (HWND hDlg, int Msg, int wParam, int lParam)
{
	static HWND SystemMenu;

	while(1) // Loop Forever
	{
		if(Msg==WM_INITDIALOG)
		{
			MainDialog_hWnd = hDlg;

			SendMessage(MainDialog_hWnd,(UINT)WM_SETTEXT,(WPARAM)0,(LPARAM)TitleCaption);
			SendMessage(MainDialog_hWnd,WM_SETICON,TRUE,(LPARAM)LoadIcon(GetWindowInstance(MainDialog_hWnd),MAKEINTRESOURCE(100)));

			SystemMenu = GetSystemMenu(MainDialog_hWnd,FALSE);
			AppendMenu(SystemMenu,MF_SEPARATOR,0,0);
			AppendMenu(SystemMenu,MF_STRING,1000,"Version");
			AppendMenu(SystemMenu,MF_STRING,1001,"www.NetWorkDLS.com");
			AppendMenu(SystemMenu,MF_STRING,1002,"About");
			AppendMenu(SystemMenu,MF_STRING,1003,"Authenticate");

			Refresh(MainDialog_hWnd);

			SetTimer(MainDialog_hWnd,0,1000,NULL); //SET TIMER
			CenterWindow(MainDialog_hWnd);
			break;
		}

		if(Msg == WM_COMMAND)
		{
			if(wParam == 100) // OK (Dialog Command)
			{
				KillTimer(MainDialog_hWnd,1);
				EndDialog(MainDialog_hWnd,0);
			}
			break;
		}

		if(Msg == WM_CLOSE) //Exit (Windows Message)
		{
			KillTimer(MainDialog_hWnd,1);
			EndDialog(MainDialog_hWnd,0);
		}

		if(Msg == WM_TIMER) //Timer  (Windows Message)
		{
			Refresh(MainDialog_hWnd);
		}

		if(Msg == WM_SYSCOMMAND)
		{
			if(LOWORD(wParam) == 1000)
				MessageBox(MainDialog_hWnd, GammaVersion, TitleCaption, 0 + MB_ICONASTERISK + MB_TASKMODAL);

			if(LOWORD(wParam) == 1001)
				ShellExecute(0,"Open","Http://www.NetWorkDLS.com",NULL,NULL,SW_SHOWNORMAL);

			if(LOWORD(wParam) == 1002) //- About (SysMenu Command)
				DialogBox(MainInstance,MAKEINTRESOURCE(1000),MainDialog_hWnd,AboutDialog); //Start Dialog

			if(LOWORD(wParam) == 1003)
			{
				char GammaPath[1024];
				unsigned long SizeOfGammaPath = sizeof(GammaPath);
				/* Get 128Gamma Executable File Name */
				if(GetStringValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\Path","Executable",GammaPath,SizeOfGammaPath))
				{
					if(FileAccess(GammaPath,FExist)) // If File Exist
					{
						ShellExecute(MainDialog_hWnd,"open",GammaPath,"128GammaSignature","",SW_NORMAL);
						KillTimer(MainDialog_hWnd,1);
						EndDialog(MainDialog_hWnd,0);
					}
					else{ // If File Does Not Exist
						MessageBox(MainDialog_hWnd, "128Gamma Executable Not Found. \r\nAuthentication Failed!" ,TitleCaption,0 + MB_ICONHAND + MB_SYSTEMMODAL);
					}
				}
				else // If Registry Error Or Key Not Found
					MessageBox(MainDialog_hWnd, "Registry Key Not Found. \r\nAuthentication Failed!" ,TitleCaption,0 + MB_ICONHAND + MB_SYSTEMMODAL);
			}
		}
		break;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////

