
/////////////////////////////////////////////////////////////////////

void RegError(HWND xDialog)
{
	KillTimer(xDialog,0);
	MessageBox (xDialog, "Either 128Gamma Is Not Installed Or \r\n There Has Been An Internal Error." ,TitleCaption,0 + MB_ICONHAND + MB_SYSTEMMODAL);
	exit(0);
}

/////////////////////////////////////////////////////////////////////

void Refresh(HWND xDialog)
{
	char StringBuffer[1024];
	unsigned long SizeOfStringBuffer = sizeof(StringBuffer);

	// What Day Was It Last Used ?
	if(!GetStringValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\LastRun","Date",StringBuffer,SizeOfStringBuffer))
	{
		RegError(xDialog);
	}
	else{
		SetText(GetDlgItem(xDialog,1001),StringBuffer);
	}

	// What Time Was It Last Used ?
	if(!GetStringValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\LastRun","Time",StringBuffer,SizeOfStringBuffer))
	{
		RegError(xDialog);
	}
	else{
		SetText(GetDlgItem(xDialog,1002),StringBuffer);
	}

	// Who Last Used It ?
	if(!GetStringValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\LastRun","User",StringBuffer,SizeOfStringBuffer))
	{
		RegError(xDialog);
	}
	else{
		SetText(GetDlgItem(xDialog,1003),StringBuffer);
	}

}

/////////////////////////////////////////////////////////////////////

