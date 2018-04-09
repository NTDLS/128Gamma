////////////////////////////////////////////////////////////////////////

int StatusDialog (HWND hDlg, int Msg, int wParam, int lParam)
{
	static HWND SystemMenu;
	static DWORD TopMost = 0;
	static DWORD LastValue = 0;

	for(;;)
	{
		if(Msg==WM_INITDIALOG)
		{
			StatusDialog_hWnd = hDlg;

			/* Set Window Icon And Caption */
			SendMessage(StatusDialog_hWnd, WM_SETTEXT, FALSE,(LPARAM)TitleCaption);
			SendMessage(StatusDialog_hWnd, WM_SETICON, TRUE,(LPARAM)LoadIcon(GetWindowInstance(AboutDialog_hWnd), MAKEINTRESOURCE(1)));

			/* Get TopMost Value From Registry */
			if(GetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\StatusDialog","TopMost",TopMost))
			{
				if(TopMost == 1) // If Window Is Supposed To Be TopMost
				{
					CheckDlgButton(StatusDialog_hWnd, 101, BST_CHECKED);
					SetWindowPos(StatusDialog_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				}
			}
			else{ // If Key Not Found Create Key And Value
					CreateKey(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\StatusDialog");
					SetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\StatusDialog","TopMost",1);
					CheckDlgButton(StatusDialog_hWnd, 101, BST_CHECKED);
					SetWindowPos(StatusDialog_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}

			Status = "";

			SetTimer(StatusDialog_hWnd,0,10,NULL);

			/* Get SystemMenu Handle | Delete & Append Menu Items */
			SystemMenu = GetSystemMenu(StatusDialog_hWnd,FALSE);
			DeleteMenu(SystemMenu, SC_SIZE, MF_BYCOMMAND);
			DeleteMenu(SystemMenu, SC_MAXIMIZE, MF_BYCOMMAND);

			/* Get Dialog Item Handles */
			StatusText_hWnd = GetDlgItem(StatusDialog_hWnd,500);
			StatusBar_hWnd = GetDlgItem(StatusDialog_hWnd, 1000);

			SendMessage(StatusBar_hWnd, PBM_SETPOS,0,0);
			CenterWindow(StatusDialog_hWnd);

			break;
		}
		//----------------------------------------------------------------------

		if(Msg==WM_COMMAND)
		{
			if(wParam==101) // Always On Top
			{
				if(IsDlgButtonChecked(StatusDialog_hWnd, 101))
				{
					SetWindowPos(StatusDialog_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
					SetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\StatusDialog","TopMost",1);
				}
				else{
					SetWindowPos(StatusDialog_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
					SetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\StatusDialog","TopMost",0);
				}

			}

			if(wParam == 100) // OK / Cancel
			{
				if(DoneFlag) // If Acting As OK Button
				{
					Verifiy = 1;
					KillTimer(StatusDialog_hWnd,0);
					EndDialog(StatusDialog_hWnd,0);
					break;
				}
				else{ // If Acting As Cancel Button
					CancelFlag = 1;
					Status = "Please Wait...";
					EnableWindow(GetDlgItem(StatusDialog_hWnd,100),FALSE);
					break;
				}
			}
		}

		if(Msg == WM_TIMER)
		{
			if(KillFlag == TRUE) // Kill Status Dialog Flag
			{
				KillTimer(StatusDialog_hWnd,0);
				EndDialog(StatusDialog_hWnd,0);
			}

			/* Update Status Bar */
			if(LastValue != StatusValue)
			{
				LastValue = StatusValue;
				SendMessage(StatusBar_hWnd, PBM_SETPOS,StatusValue,0);
			}

			/* If Done With Process (DoneFlag) */
			if(DoneFlag == TRUE)
			{
				ShowWindow(StatusDialog_hWnd,TRUE);
				if(CancelFlag)Status = "Canceled!";
				SetText(GetDlgItem(StatusDialog_hWnd,100),"Ok");
				EnableWindow(GetDlgItem(StatusDialog_hWnd,100),TRUE);
				MessageBeep(MB_ICONASTERISK);
				KillTimer(StatusDialog_hWnd,0);
			}

			/* Update Status Text */
			if(strlen(Status) > 0)
			{
				SetText(StatusText_hWnd,Status);
				Status = "";
			}
			break;
		}

		if(Msg == WM_CLOSE) //Close Dialog
		{
			if(DoneFlag) // If Acting As OK Button
			{
				Verifiy = 1;
				KillTimer(StatusDialog_hWnd,0);
				EndDialog(StatusDialog_hWnd,0);
				break;
			}
			else{ // If Acting As Cancel Button
				CancelFlag = 1;
				Status = "Please Wait...";
				EnableWindow(GetDlgItem(StatusDialog_hWnd,100),FALSE);
				break;
			}
		}

		break;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////

