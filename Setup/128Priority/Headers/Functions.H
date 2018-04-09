/////////////////////////////////////////////////////////////////////

void RegError_Set(void)
{
	MessageBox(MainDialog_hWnd, "An Error Has Occured Writing To The Registry Or \r\n There Has Been An Internal Error." ,TitleCaption,0 + MB_ICONHAND + MB_SYSTEMMODAL);
	exit(0);
}

/////////////////////////////////////////////////////////////////////

void RegError_Get(void)
{
	MessageBox(MainDialog_hWnd, "Either 128Gamma Is Not Installed Or \r\n There Has Been An Internal Error." ,TitleCaption,0 + MB_ICONHAND + MB_SYSTEMMODAL);
	exit(0);
}

/////////////////////////////////////////////////////////////////////

int GetValues(void)
{
	/* Get PriorityClass Value From Registry */
	if(GetDWORDValue(HKEY_CLASSES_ROOT,RegistryKey,"Process",PriorityClass))
	{
		if(PriorityClass == 1)
		{
			LastValue_1 = 1001;
			CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_CHECKED);
		}
		if(PriorityClass == 2)
		{
			LastValue_1 = 1002;
			CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_CHECKED);
		}
		if(PriorityClass == 3)
		{
			LastValue_1 = 1003;
			CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_CHECKED);
		}
		if(PriorityClass == 4)
		{
			LastValue_1 = 1004;
			CheckDlgButton(MainDialog_hWnd, LastValue_1, BST_CHECKED);
		}
	}
	else RegError_Get();

	/* Get ThreadPriority Value From Registry */
	if(GetDWORDValue(HKEY_CLASSES_ROOT,RegistryKey,"Thread",ThreadPriority))
	{
		if(ThreadPriority == 1)
		{
			LastValue_2 = 2001;
			CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
		}
		if(ThreadPriority == 2)
		{
			LastValue_2 = 2002;
			CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
		}
		if(ThreadPriority == 3)
		{
			LastValue_2 = 2003;
			CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
		}
		if(ThreadPriority == 4)
		{
			LastValue_2 = 2004;
			CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
		}
		if(ThreadPriority == 5)
		{
			LastValue_2 = 2005;
			CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
		}
		if(ThreadPriority == 6)
		{
			LastValue_2 = 2006;
			CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
		}
		if(ThreadPriority == 7)
		{
			LastValue_2 = 2007;
			CheckDlgButton(MainDialog_hWnd, LastValue_2, BST_CHECKED);
		}
	}
	else RegError_Get();

	/* Get ThreadBoost Value From Registry */
	if(GetDWORDValue(HKEY_CLASSES_ROOT,RegistryKey,"ThreadBoost",ThreadBoost))
	{
		if(ThreadBoost == 0)
			CheckDlgButton(MainDialog_hWnd, 3000, BST_UNCHECKED);
		if(ThreadBoost == 1)
			CheckDlgButton(MainDialog_hWnd, 3000, BST_CHECKED);

	}
	else RegError_Get();

	/* Get ProcessBoost Value From Registry */
	if(GetDWORDValue(HKEY_CLASSES_ROOT,RegistryKey,"ProcessBoost",ProcessBoost))
	{
		if(ProcessBoost == 0)
			CheckDlgButton(MainDialog_hWnd, 4000, BST_UNCHECKED);
		if(ProcessBoost == 1)
			CheckDlgButton(MainDialog_hWnd, 4000, BST_CHECKED);

	}
	else RegError_Get();

	return 0;
}

/////////////////////////////////////////////////////////////////////

int SetValues(void)
{
	BOOL HighOrRealTime = FALSE;

	//---------------------------------------------

	if(IsDlgButtonChecked(MainDialog_hWnd,1003))
		HighOrRealTime = TRUE;
	if(IsDlgButtonChecked(MainDialog_hWnd,1004))
		HighOrRealTime = TRUE;
	if(IsDlgButtonChecked(MainDialog_hWnd,2006))
		HighOrRealTime = TRUE;
	if(IsDlgButtonChecked(MainDialog_hWnd,2007))
		HighOrRealTime = TRUE;

	if(HighOrRealTime)
	{
		if(MessageBox(MainDialog_hWnd,"High, RealTime Or TimeCritical Mode Severly Cripples \r\nWindows Performance While Using 128Gamma. \r\nSave The Following Changes?",TitleCaption,4+256+MB_ICONQUESTION + MB_SYSTEMMODAL) == IDNO)
		{
			return 0;
		}
	}

	//---------------------------------------------

	if(IsDlgButtonChecked(MainDialog_hWnd,1001))
	{
		if(!SetDWORDValue(HKEY_CLASSES_ROOT,RegistryKey,"Process",1))
			RegError_Set();
	}

	if(IsDlgButtonChecked(MainDialog_hWnd,1002))
	{
		if(!SetDWORDValue(HKEY_CLASSES_ROOT,RegistryKey,"Process",2))
			RegError_Set();
	}

	if(IsDlgButtonChecked(MainDialog_hWnd,1003))
	{
		if(!SetDWORDValue(HKEY_CLASSES_ROOT,RegistryKey,"Process",3))
			RegError_Set();
	}

	if(IsDlgButtonChecked(MainDialog_hWnd,1004))
	{
		if(!SetDWORDValue(HKEY_CLASSES_ROOT,RegistryKey,"Process",4))
			RegError_Set();
	}

	//---------------------------------------------

	if(IsDlgButtonChecked(MainDialog_hWnd,2001))
	{
		if(!SetDWORDValue(HKEY_CLASSES_ROOT,RegistryKey,"Thread",1))
			RegError_Set();
	}

	if(IsDlgButtonChecked(MainDialog_hWnd,2002))
	{
		if(!SetDWORDValue(HKEY_CLASSES_ROOT,RegistryKey,"Thread",2))
			RegError_Set();
	}

	if(IsDlgButtonChecked(MainDialog_hWnd,2003))
	{
		if(!SetDWORDValue(HKEY_CLASSES_ROOT,RegistryKey,"Thread",3))
			RegError_Set();
	}

	if(IsDlgButtonChecked(MainDialog_hWnd,2004))
	{
		if(!SetDWORDValue(HKEY_CLASSES_ROOT,RegistryKey,"Thread",4))
			RegError_Set();
	}

	if(IsDlgButtonChecked(MainDialog_hWnd,2005))
	{
		if(!SetDWORDValue(HKEY_CLASSES_ROOT,RegistryKey,"Thread",5))
			RegError_Set();
	}

	if(IsDlgButtonChecked(MainDialog_hWnd,2006))
	{
		if(!SetDWORDValue(HKEY_CLASSES_ROOT,RegistryKey,"Thread",6))
			RegError_Set();
	}

	if(IsDlgButtonChecked(MainDialog_hWnd,2007))
	{
		if(!SetDWORDValue(HKEY_CLASSES_ROOT,RegistryKey,"Thread",7))
			RegError_Set();
	}

	//---------------------------------------------

	if(IsDlgButtonChecked(MainDialog_hWnd,3000))
	{
		if(!SetDWORDValue(HKEY_CLASSES_ROOT,RegistryKey,"ThreadBoost",1))
			RegError_Set();
	}
	else{
		if(!SetDWORDValue(HKEY_CLASSES_ROOT,RegistryKey,"ThreadBoost",0))
			RegError_Set();
	}

	if(IsDlgButtonChecked(MainDialog_hWnd,4000))
	{
		if(!SetDWORDValue(HKEY_CLASSES_ROOT,RegistryKey,"ProcessBoost",1))
			RegError_Set();
	}
	else{
		if(!SetDWORDValue(HKEY_CLASSES_ROOT,RegistryKey,"ProcessBoost",0))
			RegError_Set();
	}

	//---------------------------------------------

	return 1;
}

/////////////////////////////////////////////////////////////////////

