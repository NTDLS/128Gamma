////////////////////////////////////////////////////////////////////////

int AboutDialog (HWND hDlg, int Msg, int wParam, int lParam)
{
	for(;;)
	{
		if(Msg==WM_INITDIALOG)
		{
			AboutDialog_hWnd = hDlg;
			SendMessage(AboutDialog_hWnd, WM_SETTEXT, FALSE,(LPARAM)TitleCaption);
			CenterWindow(AboutDialog_hWnd);
			break;
		}
		//----------------------------------------------------------------------

		if(Msg==WM_COMMAND)
		{
			EndDialog(AboutDialog_hWnd,0);
			AboutDialog_hWnd = 0;

			if(wParam==101) // Goto NetWorkDLS.Com
				ShellExecute(0,"Open","Http://www.NetWorkDLS.com",NULL,NULL,SW_SHOWNORMAL);

			if(wParam==103) // Compatible Software Dialog
				DialogBox(hInstance,MAKEINTRESOURCE(1500),0,CompatibleDialog);

			break;
		}
		break;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////

