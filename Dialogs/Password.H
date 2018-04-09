////////////////////////////////////////////////////////////////////////

int PasswordDialog (HWND hDlg, int Msg, int wParam, int lParam)
{
	static HWND SystemMenu;
	static HWND PasswordBitStatus;
	static HWND PasswordBits_hWnd;
	static HWND PasswordEdit_hWnd;
	static HWND InfoTextStatic_hWnd;

	float PasswordLen = 0;
	float PasswordBits = 0;
	float EffeciencyBits = 0;

	char LocalTemp[257];

	DWORD RegTemp_UseCompression = 0;
	static BOOL UseCompression = FALSE;

	DWORD DWORDRegTemp = 0;

	for(;;)
	{
		// --------------------------------------

		if(Msg==WM_INITDIALOG)
		{
			PasswordDialog_hWnd = hDlg;

			//--------------------------------------------------------

			/* Get TopMost Value From Registry */
			if(GetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\PasswordDialog","TopMost",DWORDRegTemp))
			{
				if(DWORDRegTemp == 1) // If Window Is Supposed To Be TopMost
				{
					CheckDlgButton(PasswordDialog_hWnd, 103, BST_CHECKED);
					SetWindowPos(PasswordDialog_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				}
			}
			else{ // If Key Not Found Create Key And Value
					CreateKey(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\PasswordDialog");
					SetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\PasswordDialog","TopMost",1);
					CheckDlgButton(PasswordDialog_hWnd, 103, BST_CHECKED);
					SetWindowPos(PasswordDialog_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}

			//--------------------------------------------------------

			/* Get DeleteSource Value From Registry */
			if(GetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\Settings","DeleteSource",DWORDRegTemp))
			{
				if(DWORDRegTemp == 1) // If DeleteSource Is Supposed To Be Checked
				{
					CheckDlgButton(PasswordDialog_hWnd, 107, BST_CHECKED);
					DeleteSource = TRUE;
				}
			}
			else{ // If Key Not Found Create Key And Value
					CreateKey(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\Settings");
					SetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\Settings","DeleteSource",1);
					DeleteSource = FALSE;
			}

			//--------------------------------------------------------

			if(!DecompressUnpack && !DecompressDecrypt)
			{
				/* Get UseCompression Value From Registry */
				if(GetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\Settings","UseCompression",RegTemp_UseCompression))
				{
					if(RegTemp_UseCompression == 0)
					{
						UseCompression = FALSE;
						CheckDlgButton(PasswordDialog_hWnd, 105, BST_UNCHECKED);
					}
					if(RegTemp_UseCompression == 1)
					{
						UseCompression = TRUE;
						CheckDlgButton(PasswordDialog_hWnd, 105, BST_CHECKED);
					}
				}
				else{ // If Key Not Found Create Key And Value
					SetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\Settings","UseCompression",0);
				}
			}

			//--------------------------------------------------------

			/* Get SystemMenu Handle | Delete & Append Menu Items */
			SystemMenu = GetSystemMenu(PasswordDialog_hWnd,FALSE);
			DeleteMenu(SystemMenu, SC_SIZE, MF_BYCOMMAND);
			DeleteMenu(SystemMenu, SC_MAXIMIZE, MF_BYCOMMAND);
			AppendMenu(SystemMenu,MF_SEPARATOR,0,0);
			AppendMenu(SystemMenu,MF_STRING,1000,"Version");
			AppendMenu(SystemMenu,MF_STRING,1001,"Compatible Software");
			AppendMenu(SystemMenu,MF_STRING,1002,"About");
			AppendMenu(SystemMenu,MF_STRING,1003,"www.NetWorkDLS.com");

			//--------------------------------------------------------

			/* Set Window Icon And Caption */
			SendMessage(PasswordDialog_hWnd, WM_SETICON, TRUE,(LPARAM)LoadIcon(GetWindowInstance(PasswordDialog_hWnd), MAKEINTRESOURCE(1)));
			SendMessage(PasswordDialog_hWnd, WM_SETTEXT, FALSE,(LPARAM)TitleCaption);

			CenterWindow(PasswordDialog_hWnd);
			SetTimer(PasswordDialog_hWnd,1,1000,NULL);

			//--------------------------------------------------------

			/* Get Dialog Item Handles */
			InfoTextStatic_hWnd = GetDlgItem(PasswordDialog_hWnd,500);
			PasswordEdit_hWnd = GetDlgItem(PasswordDialog_hWnd,1000);
			PasswordBitStatus = GetDlgItem(PasswordDialog_hWnd,1500);
			PasswordBits_hWnd = GetDlgItem(PasswordDialog_hWnd,2500);

			//--------------------------------------------------------

			if(Encrypt)
				SetText(InfoTextStatic_hWnd,"Enter A Password To Be Used To Encrypt This Data");
			if(Decrypt || DecompressDecrypt)
				SetText(InfoTextStatic_hWnd,"Enter The Password That Was Used To Encrypt This Data.");
			if(Pack)
				SetText(InfoTextStatic_hWnd,"Enter A Password To Be Used To Pack This Data.");
			if(Unpack || DecompressUnpack)
				SetText(InfoTextStatic_hWnd,"Enter The Password That Was Used To Pack This Data.");

			//--------------------------------------------------------

			/* If decompressing a file or package check
				"Compress Data" button and disable it */
			if(DecompressUnpack || DecompressDecrypt)
			{
				EnableWindow(GetDlgItem(PasswordDialog_hWnd,105),FALSE);
				CheckDlgButton(PasswordDialog_hWnd, 105, BST_CHECKED);
			}

			/* If unpacking files or decrypting a file UnCheck
			"Compress Data" button and disable it */
			if(Unpack || Decrypt)
			{
				EnableWindow(GetDlgItem(PasswordDialog_hWnd,105),FALSE);
				CheckDlgButton(PasswordDialog_hWnd, 105, BST_UNCHECKED);
			}

			/* Enable Password Bit Status Information If Encrypting */
			if(Encrypt || Pack )
			{
				EnableWindow(PasswordBitStatus,TRUE);
				EnableWindow(PasswordBits_hWnd,TRUE);
				EnableWindow(GetDlgItem(PasswordDialog_hWnd,2000),TRUE);
			}

			//--------------------------------------------------------

			break;
		}

		// --------------------------------------

		if(Msg == 307) // Key Down
		{
			PasswordLen = SendMessage(PasswordEdit_hWnd, WM_GETTEXTLENGTH,0,0);

			if(PasswordLen >= 8)EnableWindow(GetDlgItem(PasswordDialog_hWnd,100),TRUE);
			if(PasswordLen <  8)EnableWindow(GetDlgItem(PasswordDialog_hWnd,100),FALSE);

			if(Encrypt || Pack)
			{
				EffeciencyBits = (PasswordLen / 56) * 100;
				SendMessage(PasswordBitStatus, PBM_SETPOS,EffeciencyBits,0);

				PasswordBits = (PasswordLen * 8); /* 8-Bits Per Char */
				sprintf(LocalTemp,"(%.0f Bits)",PasswordBits);
				SetText(PasswordBits_hWnd,LocalTemp);
			}
		}

		// --------------------------------------

		/* If User Presses F1 */
		if( (Msg == WM_HELP) )
		{
			char HelpFile[1024];
			unsigned long SizeOfHelpFile = sizeof(HelpFile);
			/* Get 128Gamma HelpPath */
			if(GetStringValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\Path","HelpPath",HelpFile,SizeOfHelpFile))
			{
				if(FileAccess(HelpFile,FExist)) // If Help File Exist
				{
					SetWindowPos(PasswordDialog_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
					CheckDlgButton(PasswordDialog_hWnd, 103, BST_UNCHECKED);
					ShellExecute(PasswordDialog_hWnd,"open",HelpFile,"","",SW_MAXIMIZE);
				}
				else{ // If Help File Does Not Exist
					MessageBox(PasswordDialog_hWnd, "Help File Not Found. \r\nPlease Reinstall 128Gamma." ,TitleCaption,0 + MB_ICONHAND + MB_SYSTEMMODAL);
				}
			}
			else // If Registry Error Or Key Not Found
				MessageBox(PasswordDialog_hWnd, "Registry Key Not Found. \r\nPlease Reinstall 128Gamma." ,TitleCaption,0 + MB_ICONHAND + MB_SYSTEMMODAL);

			SetFocus(PasswordEdit_hWnd);
		}

		// --------------------------------------

		/* Received A Command Through The Dialog */
		if(Msg == WM_COMMAND)
		{
			if(wParam==103) // Always On Top (Toggle)
			{
				if(IsDlgButtonChecked(PasswordDialog_hWnd, 103))
				{
					SetWindowPos(PasswordDialog_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
					SetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\PasswordDialog","TopMost",1);
				}
				else{
					SetWindowPos(PasswordDialog_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
					SetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\PasswordDialog","TopMost",0);
				}

				SetFocus(PasswordEdit_hWnd);
			}

			if(wParam==105) // Compress Data (Toggle)
			{
				if(IsDlgButtonChecked(PasswordDialog_hWnd, 105))
				{
					UseCompression = TRUE;
//					  EnableWindow(GetDlgItem(PasswordDialog_hWnd,107),FALSE);

					SetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\Settings","UseCompression",1);
				}
				else{
					UseCompression = FALSE;
					SetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\Settings","UseCompression",0);
				}

				SetFocus(PasswordEdit_hWnd);
			}

			if(wParam==107) // Delete Source (Toggle)
			{
				if(IsDlgButtonChecked(PasswordDialog_hWnd, 107))
				{
					DeleteSource = TRUE;
					SetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\Settings","DeleteSource",1);
				}
				else{
					DeleteSource = FALSE;
					SetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\Settings","DeleteSource",0);
				}

				SetFocus(PasswordEdit_hWnd);
			}

			if(wParam == 100) // Ok (Button)
			{
				/* Get keyfFrom PasswordEdit_hWnd on the PasswordDialog
				   and set it to the global veriable *CodedKey
				*/
				CodedKey = GetText(PasswordEdit_hWnd);

				/* Create StatusDialog Thread */
				StatusDialog_Thread_Handle = CreateThread(NULL,0,StatusDialog_Thread,(LPVOID)0,0,&StatusDialog_Thread_ID);

				EnableWindow(GetDlgItem(PasswordDialog_hWnd,100),FALSE);

				while(!StatusText_hWnd) //-- Wait For StatusText_hWnd
				{sleep(10);}
				while(!StatusBar_hWnd)	//-- Wait For StatusBar_hWnd
				{sleep(10);}

				/* if packing and "Compress Data" button is checked
					Set CompressPack (Flag) to TRUE and
					Pack (Flag) to FALSE
				*/
				if(UseCompression && Pack)
				{
					CompressPack = TRUE; // Compress & Pack File Flag
					Pack = FALSE;		 // Pack Files Flag
				}

				/* if encrypting and "Compress Data" button is checked
					Set CompressEncrypt (Flag) to TRUE and
					Encrypt (Flag) to FALSE
				*/
				if(UseCompression && Encrypt)
				{
					CompressEncrypt = TRUE; // Compress & Encrypt File Flag
					Encrypt = FALSE;		// Encrypt File Flag
				}


				if(Encrypt) //Encrypt
				{
					Update_LastUsed();
					ShowWindow(PasswordDialog_hWnd,FALSE);
					EncryptProcess_Thread_Handle = CreateThread(NULL,0,EncryptProcess_Thread,(LPVOID)0,0,&EncryptProcess_Thread_ID);
				}

				if(Decrypt) //Decrypt
				{
					Update_LastUsed();
					ShowWindow(PasswordDialog_hWnd,FALSE);
					DecryptProcess_Thread_Handle = CreateThread(NULL,0,DecryptProcess_Thread,(LPVOID)0,0,&DecryptProcess_Thread_ID);
				}

				if(Pack) //PackFiles
				{
					Update_LastUsed();
					ShowWindow(PasswordDialog_hWnd,FALSE);
					PackProcess_Thread_Handle = CreateThread(NULL,0,PackProcess_Thread,(LPVOID)0,0,&PackProcess_Thread_ID);
				}

				if(Unpack) //UnpackFiles
				{
					Update_LastUsed();
					ShowWindow(PasswordDialog_hWnd,FALSE);
					UnpackProcess_Thread_Handle = CreateThread(NULL,0,UnpackProcess_Thread,(LPVOID)0,0,&UnpackProcess_Thread_ID);
				}

				if(CompressEncrypt) // Compress And Encrypt File
				{
					Update_LastUsed();
					ShowWindow(PasswordDialog_hWnd,FALSE);
					CompressEncryptProcess_Thread_Handle = CreateThread(NULL,0,CompressEncryptProcess_Thread,(LPVOID)0,0,&CompressEncryptProcess_Thread_ID);
				}

				if(DecompressDecrypt) // Decompress And Decrypt File
				{
					Update_LastUsed();
					ShowWindow(PasswordDialog_hWnd,FALSE);
					DecompressDecryptProcess_Thread_Handle = CreateThread(NULL,0,DecompressDecryptProcess_Thread,(LPVOID)0,0,&DecompressDecryptProcess_Thread_ID);
				}

				if(CompressPack) // Compress And Pack Files
				{
					Update_LastUsed();
					ShowWindow(PasswordDialog_hWnd,FALSE);
					CompressPackProcess_Thread_Handle = CreateThread(NULL,0,CompressPackProcess_Thread,(LPVOID)0,0,&CompressPackProcess_Thread_ID);
				}

				if(DecompressUnpack) // Decompress And Unpack Files
				{
					Update_LastUsed();
					ShowWindow(PasswordDialog_hWnd,FALSE);
					DecompressUnpackProcess_Thread_Handle = CreateThread(NULL,0,DecompressUnpackProcess_Thread,(LPVOID)0,0,&DecompressUnpackProcess_Thread_ID);
				}

				SetFocus(PasswordEdit_hWnd);
			}

			if(wParam == 102) //Cancel (Button)
			{
				CleanUp(0,1);
				EndDialog(PasswordDialog_hWnd,0);
			}

			if(wParam==101) //About (Button)
			{
				ShowWindow(PasswordDialog_hWnd,FALSE);
				DialogBox(hInstance,MAKEINTRESOURCE(1000),PasswordDialog_hWnd,AboutDialog);
				ShowWindow(PasswordDialog_hWnd,TRUE);
				SetFocus(PasswordEdit_hWnd);
			}
			break;
		}

		// --------------------------------------

		/* Received A Timer Hit Through The Dialog */
		if(Msg == WM_TIMER)
		{
			if(Verifiy)
			{
				CleanUp(0,1);
				EndDialog(PasswordDialog_hWnd,0);
				if(!DoneFlag)DoneFlag = 1;
			}
		}

		// --------------------------------------

		/* Received A SystemMenu Command Through The Dialog */
		if(Msg == WM_SYSCOMMAND)
		{
			if(LOWORD(wParam) == 1000)
				MessageBox(PasswordDialog_hWnd, GammaVersion, TitleCaption, 0 + MB_ICONASTERISK + MB_TASKMODAL);

			if(LOWORD(wParam) == 1001)
				DialogBox(hInstance,MAKEINTRESOURCE(1500),PasswordDialog_hWnd,CompatibleDialog);

			if(LOWORD(wParam) == 1002){

				ShowWindow(PasswordDialog_hWnd,FALSE);
				DialogBox(hInstance,MAKEINTRESOURCE(1000),PasswordDialog_hWnd,AboutDialog);
				ShowWindow(PasswordDialog_hWnd,TRUE);
				SetFocus(PasswordEdit_hWnd);
			}

			if(LOWORD(wParam) == 1003)
				ShellExecute(0,"Open","Http://www.NetWorkDLS.com",NULL,NULL,SW_SHOWNORMAL);
		}

		// --------------------------------------

		if(Msg == WM_CLOSE) //Close Dialog
		{
			CleanUp(0,1);
			EndDialog(PasswordDialog_hWnd,0);
		}

		// --------------------------------------

		break;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////

