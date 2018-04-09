/*
 Processes.H - Functions Used To Call Appropriate Compress/Decompress
				Encrypt/Decrypt, Pack/Unpack

int CompressEncryptProcess(void)
int DecompressDecryptProcess(void)
int CompressPackProcess(void)
int DecompressUnpackProcess(void)
int EncryptProcess(void)
int DecryptProcess(void)
int PackProcess(void)
int UnpackProcess(void)

*/

/////////////////////////////////////////////////////////////////////

int CompressEncryptProcess(void)
{
	char xFile[257];
	float xFileSize = 0;
	char StatusTemp[1024];
	CleanUp(0,1);

	_splitpath(InputFileName,NULL,NULL,xFile,NULL);
	sprintf(OutputFileName,"%s.128",InputFileName);

	if(FileAccess(OutputFileName,FExist))
	{
		if(MessageBox(StatusDialog_hWnd, "Target File Already Exist. Overwrite?" , TitleCaption, 0 + MB_YESNO + MB_DEFBUTTON2 + MB_ICONQUESTION) == IDNO)
		{
			CleanUp(0,1);
			CancelFlag = TRUE;
			DoneFlag = TRUE;
			return(0);
		}
	}

	xFileSize = Get_FileSize(InputFileName);
	if(xFileSize > MaxFileSize)
	{
		char StatusTemp[257];
		sprintf(StatusTemp,"128Gamma %s Cannot Encrypt File Larger Than %d MB.",ShortVersion,MaxFileSize/1024/1024);
		MessageBox (StatusDialog_hWnd, StatusTemp ,TitleCaption,0 + MB_SYSTEMMODAL);
		CleanUp(0,1);
		DoneFlag = 1;
		return(0);
	 }

	CleanUp(0,1);

	xFileSize = Get_FileSize(InputFileName);
	sprintf(StatusTemp,"Compressing %.3f MB",xFileSize/1024/1024);
	Status = StatusTemp;

	if(Compress(InputFileName,CompressTemp))
	{
		xFileSize = Get_FileSize(CompressTemp);
		sprintf(StatusTemp,"Encrypting %.3f MB",xFileSize/1024/1024);
		Status = StatusTemp;

		if(IncodeMain(CompressTemp,OutputFileName))
		{
			if(DeleteSource)xDeleteObject(InputFileName);
			Status = "Data Successfully Encrypted!";
		}
	}

	CleanUp(0,1);

	if(CancelFlag)
		xDeleteObject(OutputFileName);

	DoneFlag = 1;

	return(0);
}

/////////////////////////////////////////////////////////////////////

int DecompressDecryptProcess(void)
{
	float xFileSize = 0;
	char StatusTemp[1024];
	CleanUp(0,1);

	_splitpath(InputFileName,NULL,NULL,OutputFileName,NULL);

	if(FileAccess(OutputFileName,FExist))
	{
		if(MessageBox(StatusDialog_hWnd, "Target File Already Exist. Overwrite?" , TitleCaption, 0 + MB_YESNO + MB_DEFBUTTON2 + MB_ICONQUESTION) == IDNO)
		{
			CleanUp(0,1);
			CancelFlag = TRUE;
			DoneFlag = TRUE;
			return(0);
		}
	}

	xFileSize = Get_FileSize(InputFileName);
	sprintf(StatusTemp,"Decrypting %.3f MB",xFileSize/1024/1024);
	Status = StatusTemp;


	if(DecodeMain(InputFileName,CompressTemp))
	{
		xFileSize = Get_FileSize(CompressTemp);
		sprintf(StatusTemp,"Decompressing %.3f MB",xFileSize/1024/1024);
		Status = StatusTemp;

		if(Decompress(CompressTemp,OutputFileName))
		{
			if(DeleteSource)xDeleteObject(InputFileName);
			Status = "Data Successfully Decrypted!";
		}
	}

	CleanUp(0,1);

	if(CancelFlag)
		xDeleteObject(OutputFileName);

	DoneFlag = 1;

	return(0);
}

//////////////////////////////////////////////////////////////////////

int CompressPackProcess(void)
{
	char StatusTemp[1024];
	float xFileSize = 0;

	CleanUp(0,1);
	sprintf(OutputFileName,"%s.128Pkg",InputFileName);

	if(FileAccess(OutputFileName,FExist))
	{
		if(MessageBox(StatusDialog_hWnd, "Target File Already Exist. Overwrite?" , TitleCaption, 0 + MB_YESNO + MB_DEFBUTTON2 + MB_ICONQUESTION) == IDNO)
		{
			CleanUp(0,1);
			CancelFlag = TRUE;
			DoneFlag = TRUE;
			return(0);
		}
	}

	if(PackFiles(InputFileName,PackTemp))
	{
		xFileSize = Get_FileSize(PackTemp);
		sprintf(StatusTemp,"Compressing %.3f MB",xFileSize/1024/1024);
		Status = StatusTemp;

		if(Compress(PackTemp,CompressTemp))
		{
			xFileSize = Get_FileSize(CompressTemp);
			sprintf(StatusTemp,"Encrypting %.3f MB",xFileSize/1024/1024);
			Status = StatusTemp;

			/* IncodeMain(Source,Target); */
			if(IncodeMain(CompressTemp,OutputFileName))
			{
				if(DeleteSource)
				{
					Status = "Deleting Old Files And Folders.";
					xDeleteObject(InputFileName);
				}
				Status = "Data Successfully Packed!";
			}
		}
	}

	CleanUp(0,1);

	if(CancelFlag)
		xDeleteObject(OutputFileName);

	DoneFlag = 1;

	return(0);
}

//////////////////////////////////////////////////////////////////////

int DecompressUnpackProcess(void)
{
	char StatusTemp[1024];
	float xFileSize = 0;

	CleanUp(0,1);

	_splitpath(InputFileName,NULL,NULL,OutputFileName,NULL);

	if(FileAccess(OutputFileName,FExist))
	{
		if(MessageBox(StatusDialog_hWnd, "Target File Already Exist. Overwrite?" , TitleCaption, 0 + MB_YESNO + MB_DEFBUTTON2 + MB_ICONQUESTION) == IDNO)
		{
			CleanUp(0,1);
			CancelFlag = TRUE;
			DoneFlag = TRUE;
			return(0);
		}
	}

	xFileSize = Get_FileSize(InputFileName);
	sprintf(StatusTemp,"Decrypting %.3f MB",xFileSize/1024/1024);
	Status = StatusTemp;

	if(DecodeMain(InputFileName,CompressTemp))
	{
		xFileSize = Get_FileSize(CompressTemp);
		sprintf(StatusTemp,"Decompressing %.3f MB",xFileSize/1024/1024);
		Status = StatusTemp;

		if(Decompress(CompressTemp,PackTemp))
		{
			CreateDirectory(OutputFileName,0);
			if(UnPackFiles(PackTemp,OutputFileName))
			{
				if(DeleteSource)xDeleteObject(InputFileName);
				Status = "Data Successfully Unpacked!";
			}
		}
	}

	CleanUp(0,1);

	if(CancelFlag)
		xDeleteObject(OutputFileName);

	DoneFlag = 1;

	return(0);
}

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////

int EncryptProcess(void)
{
	char xFile[257];
	float xFileSize = 0;
	char StatusTemp[1024];
	CleanUp(0,1);

	_splitpath(InputFileName,NULL,NULL,xFile,NULL);
	sprintf(OutputFileName,"%s.128",InputFileName);

	if(FileAccess(OutputFileName,FExist))
	{
		if(MessageBox(StatusDialog_hWnd, "Target File Already Exist. Overwrite?" , TitleCaption, 0 + MB_YESNO + MB_DEFBUTTON2 + MB_ICONQUESTION) == IDNO)
		{
			CleanUp(0,1);
			CancelFlag = TRUE;
			DoneFlag = TRUE;
			return(0);
		}
	}

	xFileSize = Get_FileSize(InputFileName);
	if(xFileSize > MaxFileSize)
	{
		char StatusTemp[257];
		CleanUp(0,1);
		DoneFlag = 1;
		sprintf(StatusTemp,"128Gamma %s Cannot Encrypt File Larger Than %d MB.",ShortVersion,MaxFileSize/1024/1024);
		MessageBox (StatusDialog_hWnd, StatusTemp ,TitleCaption,0 + MB_SYSTEMMODAL);
		return(0);
	 }

	CleanUp(0,1);

	xFileSize = Get_FileSize(InputFileName);
	sprintf(StatusTemp,"Encrypting %.3f MB",xFileSize/1024/1024);
	Status = StatusTemp;

	if(IncodeMain(InputFileName,OutputFileName))
	{
		if(DeleteSource)xDeleteObject(InputFileName);
		Status = "Data Successfully Encrypted!";
	}

	CleanUp(0,1);

	if(CancelFlag)
		xDeleteObject(OutputFileName);

	DoneFlag = 1;

	return(0);
}

/////////////////////////////////////////////////////////////////////

int DecryptProcess(void)
{
	float xFileSize = 0;
	char StatusTemp[1024];
	CleanUp(0,1);

	_splitpath(InputFileName,NULL,NULL,OutputFileName,NULL);

	if(FileAccess(OutputFileName,FExist))
	{
		if(MessageBox(StatusDialog_hWnd, "Target File Already Exist. Overwrite?" , TitleCaption, 0 + MB_YESNO + MB_DEFBUTTON2 + MB_ICONQUESTION) == IDNO)
		{
			CleanUp(0,1);
			CancelFlag = TRUE;
			DoneFlag = TRUE;
			return(0);
		}
	}

	xFileSize = Get_FileSize(InputFileName);
	sprintf(StatusTemp,"Decrypting %.3f MB",xFileSize/1024/1024);
	Status = StatusTemp;

	if(DecodeMain(InputFileName,OutputFileName))
	{
		Status = "Data Successfully Decrypted!";
		if(DeleteSource)xDeleteObject(InputFileName);
	}

	CleanUp(0,1);

	if(CancelFlag)
		xDeleteObject(OutputFileName);

	DoneFlag = 1;

	return(0);
}

//////////////////////////////////////////////////////////////////////

int PackProcess(void)
{
	char StatusTemp[1024];
	float xFileSize = 0;

	CleanUp(0,1);
	sprintf(OutputFileName,"%s.128Pkg",InputFileName);

	if(FileAccess(OutputFileName,FExist))
	{
		if(MessageBox(StatusDialog_hWnd, "Target File Already Exist. Overwrite?" , TitleCaption, 0 + MB_YESNO + MB_DEFBUTTON2 + MB_ICONQUESTION) == IDNO)
		{
			CleanUp(0,1);
			CancelFlag = TRUE;
			DoneFlag = TRUE;
			return(0);
		}
	}

	if(PackFiles(InputFileName,PackTemp))
	{
		xFileSize = Get_FileSize(PackTemp);
		sprintf(StatusTemp,"Encrypting %.3f MB",xFileSize/1024/1024);
		Status = StatusTemp;

		/* IncodeMain(Source,Target); */
		if(IncodeMain(PackTemp,OutputFileName))
		{
			if(DeleteSource)
			{
				Status = "Deleting Old Files And Folders.";
				xDeleteObject(InputFileName);
			}
			Status = "Data Successfully Packed!";
		}
	}

	CleanUp(0,1);

	if(CancelFlag)
		xDeleteObject(OutputFileName);

	DoneFlag = 1;

	return(0);
}

//////////////////////////////////////////////////////////////////////

int UnpackProcess(void)
{
	char StatusTemp[1024];
	float xFileSize = 0;

	CleanUp(0,1);

	_splitpath(InputFileName,NULL,NULL,OutputFileName,NULL);

	if(FileAccess(OutputFileName,FExist))
	{
		if(MessageBox(StatusDialog_hWnd, "Target File Already Exist. Overwrite?" , TitleCaption, 0 + MB_YESNO + MB_DEFBUTTON2 + MB_ICONQUESTION) == IDNO)
		{
			CleanUp(0,1);
			CancelFlag = TRUE;
			DoneFlag = TRUE;
			return(0);
		}
	}

	xFileSize = Get_FileSize(InputFileName);
	sprintf(StatusTemp,"Decrypting %.3f MB",xFileSize/1024/1024);
	Status = StatusTemp;

	if(DecodeMain(InputFileName,PackTemp))
	{
		CreateDirectory(OutputFileName,0);
		if(UnPackFiles(PackTemp,OutputFileName))
		{
			Status = "Data Successfully Unpacked!";
			if(DeleteSource)xDeleteObject(InputFileName);
		}
	}

	CleanUp(0,1);

	if(CancelFlag)
		xDeleteObject(OutputFileName);

	DoneFlag = 1;

	return(0);
}

/////////////////////////////////////////////////////////////////////

