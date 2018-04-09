
///////////////////////////////////////////////////////////////////////////////

int AboutDialog (HWND hDlg, int Msg, int wParam, int lParam)
{
	while(1)
	{
		if(Msg==WM_INITDIALOG)
		{
			AboutDialog_hWnd = hDlg;
			CenterWindow(AboutDialog_hWnd);
			break;
		}
		if(Msg==WM_COMMAND)
		{
			if(wParam==101) // Goto NetWorkDLS.Com
				ShellExecute(0,"Open","Http://www.NetWorkDLS.com",NULL,NULL,SW_SHOWNORMAL);
			EndDialog(AboutDialog_hWnd,0);
			break;
		}
		break;
	}
	return(0);
}

///////////////////////////////////////////////////////////////////////////////
