/*
 Functions.H -	Functions File (Various & Assorted)

void Error(char *Message)
int CleanUp(int Kill, int Delete)
int PreCheckFile(char *FileToCheck)
int SetTempVariables(void)
int ProcessInput(char *xInput)
void ClearBuffer(void)
DWORD WINAPI StatusDialog_Thread(LPVOID data)
DWORD WINAPI EncryptProcess_Thread(LPVOID data)
DWORD WINAPI DecryptProcess_Thread(LPVOID data)
DWORD WINAPI PackProcess_Thread(LPVOID data)
DWORD WINAPI UnpackProcess_Thread(LPVOID data)
DWORD WINAPI CompressEncryptProcess_Thread(LPVOID data)
DWORD WINAPI DecompressDecryptProcess_Thread(LPVOID data)
DWORD WINAPI CompressPackProcess_Thread(LPVOID data)
DWORD WINAPI DecompressUnpackProcess_Thread(LPVOID data)
void Update_LastUsed(void)
void FreeMemory(void)
void Clear_ReadTemp(void)
BOOL SetPriority(void)

*/

//--------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////

void Error(char *Message)
{
	KillFlag = 1;
	MessageBox(GetActiveWindow(), Message ,TitleCaption,0 + MB_ICONHAND + MB_SYSTEMMODAL + MB_TASKMODAL);
	CleanUp(1,1);
}

//////////////////////////////////////////////////////////////////////

int CleanUp(int Kill, int Delete)
{
	if(Delete)
	{
		_fcloseall(); //Attempt To Close All Open Files... (If Any)
		xDeleteObject(CompressTemp);
		xDeleteObject(PackTemp);
	}

	if(Kill)
	{
		_fcloseall(); //Attempt To Close All Open Files... (If Any)
		sleep(500); // Hopfully Give Time For Files To Flush.
		xDeleteObject(CompressTemp);
		xDeleteObject(PackTemp);
		exit(0);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////

int PreCheckFile(char *FileToCheck)
{
	FILE * FileToCheck_File;
	char xDrive[255], xDir[255], xFile[255], xExt[255];
	_splitpath(FileToCheck,xDrive,xDir,xFile,xExt);
	char LocalBuffer[9];

	/* Check For File Access */
	if(!FileAccess(FileToCheck,FExist)) // Check Exist
		Error("PreCheckFile: File Does Not Exist. Error 14.");

	if(IsDirectory(FileToCheck)) // If Input Is A Directory
		return 1;

	if((FileToCheck_File = fopen(FileToCheck,"rb")) == NULL) // Check Read Access
		Error("PreCheckFile: File Does Not Allow Read Access. Error 15.");
	if(FileToCheck_File)fclose(FileToCheck_File);


	DWORD FileSize_Temp = Get_FileSize(FileToCheck);
	if(FileSize_Temp == -1)return 0;

	if(FileSize_Temp > MaxFileSize)
	{
		char MessageTemp[257];
		sprintf(MessageTemp,"128Gamma %s Requires That Files Be Under %d MB",ShortVersion,MaxFileSize/1024/1024);
		MessageBox(GetActiveWindow(),MessageTemp,TitleCaption,+ MB_SYSTEMMODAL);
		return 0;
	}

	if(FileSize_Temp < 1)
	{
		char MessageTemp[257];
		sprintf(MessageTemp,"128Gamma %s Requires That Files Contain Atleast Some Data.",ShortVersion);
		MessageBox(GetActiveWindow(),MessageTemp,TitleCaption,+ MB_SYSTEMMODAL);
		return 0;
	}

	/* Open Source File */
	if((FileToCheck_File = fopen(FileToCheck,"rb")) == NULL)
		Error("PreCheckFile: Could Not Open Source For Read. Error: 16.");

	int LocalLoop = 0;
	while(LocalLoop != 9)
	{
		LocalBuffer[LocalLoop] = '\0';
		LocalLoop++;
	}

	/* Read File Flag ( If Any ) */
	fread(LocalBuffer,sizeof(char),FlagLEN,FileToCheck_File);

	if(strcmp(LocalBuffer,ENCFlag) == 0) // If File Encryted
	{
		if(FileToCheck_File)
			fclose(FileToCheck_File);
		return 2;
	}

	if(strcmp(LocalBuffer,PACFlag) == 0) // If File Packed
	{
		if(FileToCheck_File)
			fclose(FileToCheck_File);
		return 3;
	}

	if(strcmp(LocalBuffer,ENCCompress) == 0) // If File Compressed Encryted
	{
		if(FileToCheck_File)
			fclose(FileToCheck_File);
		return 4;
	}

	if(strcmp(LocalBuffer,PACCompress) == 0) // If File Compressed Packed
	{
		if(FileToCheck_File)
			fclose(FileToCheck_File);
		return 5;
	}

	if(strcmp(LocalBuffer,ABIFlag) == 0) // If Encryted By ABI-Coder
	{
		if(FileToCheck_File)
			fclose(FileToCheck_File);
		return 6;
	}

	if(FileToCheck_File)
		fclose(FileToCheck_File);
	return 7; // If File Not Encrypted
}

//////////////////////////////////////////////////////////////////////

int SetTempVariables(void)
{
	sprintf(PackTemp,"%s\\%s",Get_TempDirectory(),"Pack.128G");
	sprintf(CompressTemp,"%s\\%s",Get_TempDirectory(),"Compress.128G");

	if(!CompressTemp)
		return 1;
	if(!PackTemp)
		return 1;

	return 0;
}

/////////////////////////////////////////////////////////////////////

int ProcessInput(char *xInput)
{
	short ENCMethod = 0;

	if(strlen(xInput) > 0) // If Any Program Input Params
	{
		sprintf(InputFileName,"%s",xInput);

		/*
		0 = Cancel , Dir = 1 , Encrypted = 2 , Packed = 3
		Compressed Encrypted = 4 , Compressed Packed = 5
		ABI-Coder File = 6, Not Encrypted = 7
		*/

		ENCMethod = PreCheckFile(InputFileName);

		if(ENCMethod == 0) // Cancel
			return 0;

		if(ENCMethod == 1) // Directory
		{
			Pack = TRUE; // Encrypt File Flag

			DialogBox(hInstance,MAKEINTRESOURCE(100),0,PasswordDialog);
			CleanUp(0,1);
			return(0);
		}

		if(ENCMethod == 2) // Encrypted
		{
			Decrypt = TRUE; // Decrypt File Flag

			DialogBox(hInstance,MAKEINTRESOURCE(100),0,PasswordDialog);
			CleanUp(0,1);
			return(0);
		}

		if(ENCMethod == 3) //  Packed
		{
			Unpack = TRUE; // Unpack File Flag

			DialogBox(hInstance,MAKEINTRESOURCE(100),0,PasswordDialog);
			CleanUp(0,1);
			return(0);
		}

		if(ENCMethod == 4) //  Compressed Encrypted
		{
			DecompressDecrypt = TRUE; // Decompress Decrypt File Flag

			DialogBox(hInstance,MAKEINTRESOURCE(100),0,PasswordDialog);
			CleanUp(0,1);
			return(0);
		}

		if(ENCMethod == 5) //  Compressed Packed
		{
			DecompressUnpack = TRUE; // Decompress Unpack File Flag

			DialogBox(hInstance,MAKEINTRESOURCE(100),0,PasswordDialog);
			CleanUp(0,1);
			return(0);
		}

		if(ENCMethod == 6) // Encrypted by ABI-Coder
		{
			Decrypt = TRUE;   // Decrypt File Flag
			DialogBox(hInstance,MAKEINTRESOURCE(100),0,PasswordDialog);
			CleanUp(0,1);
			return(0);
		}

		if(ENCMethod == 7) // NOT Encrypted
		{
			Encrypt = TRUE; // Encrypt File Flag

			DialogBox(hInstance,MAKEINTRESOURCE(100),0,PasswordDialog);
			CleanUp(0,1);
			return(0);
		}
	}

	CleanUp(0,1);

	return 1;
}

///////////////////////////////////////////////////////////////////////

void Clear_ReadTemp(void)
{
	int LocalLoop = 0;
	while(LocalLoop < sizeof(ReadTemp))
	{
		ReadTemp[LocalLoop] = '\0';
		LocalLoop++;
	}
}

///////////////////////////////////////////////////////////////////////

void ClearBuffer(void)
{
	int LocalLoop = 0;
	while(LocalLoop < sizeof(Buffer))
	{
		Buffer[LocalLoop] = '\0';
		LocalLoop++;
	}
}

///////////////////////////////////////////////////////////////////////

DWORD WINAPI StatusDialog_Thread(LPVOID data)
{
	DialogBox(hInstance,MAKEINTRESOURCE(200),0,StatusDialog);
	return ((DWORD)data);
}

/////////////////////////////////////////////////////////////////////

DWORD WINAPI EncryptProcess_Thread(LPVOID data)
{
	/* Set 128Gamma Priority */
	if( SetPriority() ) return((DWORD)data);

	EncryptProcess();
	return ((DWORD)data);
}

/////////////////////////////////////////////////////////////////////

DWORD WINAPI DecryptProcess_Thread(LPVOID data)
{
	/* Set 128Gamma Priority */
	if( SetPriority() ) return((DWORD)data);

	DecryptProcess();
	return ((DWORD)data);
}

/////////////////////////////////////////////////////////////////////

DWORD WINAPI PackProcess_Thread(LPVOID data)
{
	/* Set 128Gamma Priority */
	if( SetPriority() ) return((DWORD)data);

	PackProcess();
	return ((DWORD)data);
}

/////////////////////////////////////////////////////////////////////

DWORD WINAPI UnpackProcess_Thread(LPVOID data)
{
	/* Set 128Gamma Priority */
	if( SetPriority() ) return((DWORD)data);

	UnpackProcess();
	return ((DWORD)data);
}

/////////////////////////////////////////////////////////////////////

DWORD WINAPI CompressEncryptProcess_Thread(LPVOID data)
{
	/* Set 128Gamma Priority */
	if( SetPriority() ) return((DWORD)data);

	CompressEncryptProcess();
	return ((DWORD)data);
}

/////////////////////////////////////////////////////////////////////

DWORD WINAPI DecompressDecryptProcess_Thread(LPVOID data)
{
	/* Set 128Gamma Priority */
	if( SetPriority() ) return((DWORD)data);

	DecompressDecryptProcess();
	return ((DWORD)data);
}

/////////////////////////////////////////////////////////////////////

DWORD WINAPI CompressPackProcess_Thread(LPVOID data)
{
	/* Set 128Gamma Priority */
	if( SetPriority() ) return((DWORD)data);

	CompressPackProcess();
	return ((DWORD)data);
}

/////////////////////////////////////////////////////////////////////

DWORD WINAPI DecompressUnpackProcess_Thread(LPVOID data)
{
	/* Set 128Gamma Priority */
	if( SetPriority() ) return((DWORD)data);

	DecompressUnpackProcess();
	return ((DWORD)data);
}

//////////////////////////////////////////////////////////////////////

void Update_LastUsed(void)
{
	SetStringValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\LastRun","User",Get_UserName());
	SetStringValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\LastRun","Date",Get_LongDate());
	SetStringValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\LastRun","Time",Get_Time());
}

//////////////////////////////////////////////////////////////////////

void FreeMemory(void)
{
	while(Package.FolderCount != -1)
	{
		free(Package.FolderNames[Package.FolderCount]);
		Package.FolderNames[Package.FolderCount] = '\0';
		Package.FolderCount--;
	}
	while(Package.FileCount != -1)
	{
		free(Package.FileNames[Package.FileCount]);
		Package.FileNames[Package.FileCount] = '\0';
		Package.FileCount--;
	}
}

//////////////////////////////////////////////////////////////////////

BOOL SetPriority(void)
{
	DWORD Priority = 0;
	BOOL HighOrRealTime = FALSE;
	HANDLE CurrentProcessHandle = GetCurrentProcess();
	HANDLE CurrentThreadHandle = GetCurrentThread();

	DWORD ProcessSET = 0;
	DWORD ThreadSET = 0;
	BOOL ThreadBoostSET = 0;
	BOOL ProcessBoostSET = 0;

	if(GetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\Priority","Process",Priority))
	{
		ProcessSET = Priority;
		if(Priority > 2)HighOrRealTime = TRUE;
	}
	else SetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\Priority","Process",2);

	Priority = 0;

	if(GetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\Priority","Thread",Priority))
	{
		ThreadSET = Priority;
		if(Priority > 5)HighOrRealTime = TRUE;
	}
	else SetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\Priority","Thread",4);

	Priority = 0;

	if(GetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\Priority","ThreadBoost",Priority))
	{
		if(Priority == 0)ThreadBoostSET = TRUE; //Disable Boost
		if(Priority == 1)ThreadBoostSET = FALSE; //Enable Boost
	}
	else SetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\Priority","ThreadBoost",1);

	Priority = 0;

	if(GetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\Priority","ProcessBoost",Priority))
	{
		if(Priority == 0)ProcessBoostSET = TRUE; //Disable Boost
		if(Priority == 1)ProcessBoostSET = FALSE; //Enable Boost
	}
	else SetDWORDValue(HKEY_CLASSES_ROOT,"*\\Shell\\128Gamma_Encryption\\AppInfo\\Priority","ProcessBoost",1);


	if(HighOrRealTime == TRUE)
	{
		if(MessageBox(StatusDialog_hWnd,"128Gamma Is Running In High, RealTime Or TimeCritical Mode. \r\nThis Severly Cripples Windows Performance. \r\nContinue Anyway? ",TitleCaption,4+256+MB_ICONQUESTION + MB_SYSTEMMODAL) != 6)
		{
			CancelFlag = TRUE;
			DoneFlag = 1;
			return TRUE;
		}
	}

	SetApplicationPriority(CurrentProcessHandle,CurrentThreadHandle, ProcessSET,ThreadSET,ThreadBoostSET,ProcessBoostSET);

	return FALSE; // No Error... Return False
}

//////////////////////////////////////////////////////////////////////

