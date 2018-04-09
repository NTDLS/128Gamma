/*

 PackFiles.H - Pack Files & Folders Into One File

int PackFiles(char *SourceDirectory, char *DestinationFile)
int Get_FileNames(char *xInputDir)
int Get_SubFileNames(char *xInputDir, char *xSubDir)

*/

///////////////////////////////////////////////////////////////////////

int PackFiles(char *SourceDirectory, char *DestinationFile)
{

	FILE *LocalTarget;
	FILE *LocalSource;

	DWORD SourceFileSize = 0;

	float xFileCount = 0;
	float xLocalLoop = 0;

	DWORD LocalLoop = 0;
	DWORD CurrentBuffer = 0;

	char LocalTemp[1024];
	char StatusTemp[1024];

	if(Get_FileNames(SourceDirectory)) // Get Names Of Folders And Dirs
	{
		return 0;
	}

	if(Package.FileCount == 0)
	{
		Status = "Folder Contains No Files";
		return 0;
	}

	/* Open Target File For Write */
	if((LocalTarget = fopen(DestinationFile,"wb")) == NULL)
	{
		if(LocalTarget)fclose(LocalTarget);
		Error("PackFiles: Could Not Open File For Write. Error 17.");
		return 0;
	}

	//Write Number Of Files (Begin)
	if(Package.FileCount < 100000)
		fprintf(LocalTarget,"0");
	if(Package.FileCount < 10000)
		fprintf(LocalTarget,"0");
	if(Package.FileCount < 1000)
		fprintf(LocalTarget,"0");
	if(Package.FileCount < 100)
		fprintf(LocalTarget,"0");
	if(Package.FileCount < 10)
		fprintf(LocalTarget,"0");
	fprintf(LocalTarget,"%d",Package.FileCount);
	//Write Number Of Files (End)

	fprintf(LocalTarget,"|");

	//Write Number Of Folders (Begin)
	if(Package.FolderCount < 100000)
		fprintf(LocalTarget,"0");
	if(Package.FolderCount < 10000)
		fprintf(LocalTarget,"0");
	if(Package.FolderCount < 1000)
		fprintf(LocalTarget,"0");
	if(Package.FolderCount < 100)
		fprintf(LocalTarget,"0");
	if(Package.FolderCount < 10)
		fprintf(LocalTarget,"0");
	fprintf(LocalTarget,"%d",Package.FolderCount);
	//Write Number Of Folders (End)

	fprintf(LocalTarget,"|");

	//Write File Name Leingths & Names (Begin)
	Status = "Writing File Names";

	LocalLoop = 0;
	while(LocalLoop < Package.FileCount)
	{
		if(strlen(Package.FileNames[LocalLoop]) < 100)
			fprintf(LocalTarget,"0");
		if(strlen(Package.FileNames[LocalLoop]) < 10)
			fprintf(LocalTarget,"0");
		fprintf(LocalTarget,"%d",strlen(Package.FileNames[LocalLoop]));
		fprintf(LocalTarget,"%s",Package.FileNames[LocalLoop]);
		LocalLoop++;
	}
	//Write File Name Leingths & Names (End)

	fprintf(LocalTarget,"|");

	//Write Folder Name Leingths & Names (Begin)
	Status = "Writing Folder Names";

	LocalLoop = 0;
	while(LocalLoop < Package.FolderCount)
	{
		if(strlen(Package.FolderNames[LocalLoop]) < 100)
			fprintf(LocalTarget,"0");
		if(strlen(Package.FolderNames[LocalLoop]) < 10)
			fprintf(LocalTarget,"0");
		fprintf(LocalTarget,"%d",strlen(Package.FolderNames[LocalLoop]));
		fprintf(LocalTarget,"%s",Package.FolderNames[LocalLoop]);
		LocalLoop++;
	}
	//Write Folder Name Leingths & Names (End)

	sprintf(StatusTemp,"Packing %d Files && %d Folders",Package.FileCount,Package.FolderCount);
	Status = StatusTemp;

	xFileCount = Package.FileCount; // xFileCount Is float Used For ProgressBar

	fprintf(LocalTarget,"|");

	//Write File Size & Data (Begin)
	LocalLoop = 0;
	while(LocalLoop < Package.FileCount)
	{
		sprintf(LocalTemp,"%s\\%s",SourceDirectory,Package.FileNames[LocalLoop]);

		SourceFileSize = Get_FileSize(LocalTemp);
		if(SourceFileSize == -1)
		{
			CancelFlag = TRUE;
			return 0;
		}

		if(SourceFileSize > MaxPackFileSize)
		{
			char StatusTemp[257];
			CancelFlag = TRUE;
			sprintf(StatusTemp,"128Gamma %s Cannot Pack Files Larger Than %d MB Each.",ShortVersion,MaxPackFileSize/1024/1024);
			MessageBox (StatusDialog_hWnd, StatusTemp ,TitleCaption,0 + MB_SYSTEMMODAL);
			return 0;
		}

		/* Write File Size */
		if(SourceFileSize < 100000000)
			fprintf(LocalTarget,"0");
		if(SourceFileSize < 10000000)
			fprintf(LocalTarget,"0");
		if(SourceFileSize < 1000000)
			fprintf(LocalTarget,"0");
		if(SourceFileSize < 100000)
			fprintf(LocalTarget,"0");
		if(SourceFileSize < 10000)
			fprintf(LocalTarget,"0");
		if(SourceFileSize < 1000)
			fprintf(LocalTarget,"0");
		if(SourceFileSize < 100)
			fprintf(LocalTarget,"0");
		if(SourceFileSize < 10)
			fprintf(LocalTarget,"0");
		fprintf(LocalTarget,"%d",SourceFileSize);

		/* Open Target File */
		if((LocalSource = fopen(LocalTemp,"rb")) == NULL)
		{
			if(LocalSource)fclose(LocalSource);
			if(LocalTarget)fclose(LocalTarget);
			Error("PackFiles: Could Not Open File For Read. Error 18.");
			return 0;
		}

		/* Write File Data */
		do
		{
			if(CancelFlag)
			{
				if(LocalSource)fclose(LocalSource);
				if(LocalTarget)fclose(LocalTarget);
				unlink(DestinationFile);
				CleanUp(0,1);
				return 0;
			}

			Buffer[0] = '\0';
			CurrentBuffer = fread(Buffer,sizeof(char),BufferSize,LocalSource);
			fwrite(Buffer,sizeof(char),CurrentBuffer,LocalTarget);
		}
		while(CurrentBuffer == BufferSize);

		if(LocalSource)fclose(LocalSource);

		LocalLoop++;
		xLocalLoop = LocalLoop;
		StatusValue = ((xLocalLoop / xFileCount) * 100);
	}
	//Write File Size & Data (End)

	FreeMemory();

	if(LocalSource)fclose(LocalSource);
	if(LocalTarget)fclose(LocalTarget);

	return 1 ;
}

///////////////////////////////////////////////////////////////////////

int Get_FileNames(char *xInputDir)
{
	WIN32_FIND_DATA FindData;
	HANDLE FileHandle;

	int Found = 0;

	DWORD LocalLoop = 0;

	char FileCountStatus[257];
	char DirTemp[257];

	sprintf(DirTemp,"%s\\%s",xInputDir,"*.*");
	FileHandle = FindFirstFile(DirTemp,&FindData);
	Found = (int) FileHandle;


	while (Found != 0)
	{
		if( strcmp(FindData.cFileName,".") && strcmp(FindData.cFileName,"..") ) // If (Not) NULL Or Root
		{
			sprintf(FileCountStatus,"%d Files, %d Folders",Package.FileCount,Package.FolderCount);
			Status = FileCountStatus;
			/* If Found Data Is A Directory */
			if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				Package.FolderNames[Package.FolderCount] = (char *) malloc(257);
				sprintf(Package.FolderNames[Package.FolderCount],"%s",FindData.cFileName);
				Package.FolderCount++;
			}
			else{	/* If Found Data Is A File */
				Package.FileNames[Package.FileCount] = (char *) malloc(257);
				sprintf(Package.FileNames[Package.FileCount],"%s",FindData.cFileName);
				Package.FileCount++;
			}
		}

		if(Package.FileCount > MaxFiles || Package.FolderCount > MaxFiles)
		{
			char StatusTemp[257];
			CancelFlag = TRUE;
			FindClose(FileHandle);
			sprintf(StatusTemp,"128Gamma %s Can Pack A Max Of %d Files And Folders",ShortVersion,MaxFiles);
			MessageBox (StatusDialog_hWnd, StatusTemp ,TitleCaption,0 + MB_SYSTEMMODAL);
			return 1;
		}

		if(CancelFlag)
		{
			FindClose(FileHandle);
			CleanUp(0,1);
			return 1;
		}
		Found = FindNextFile(FileHandle, &FindData);
	}
	FindClose(FileHandle);

	if(Package.FolderCount != 0) // If AtLeast One Folder Was Found
	{
		LocalLoop = 0;
		while(LocalLoop < Package.FolderCount)
		{
			if(Get_SubFileNames(xInputDir,Package.FolderNames[LocalLoop]))
			{
				return 1;
			}
			LocalLoop++;
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////

int Get_SubFileNames(char *xInputDir, char *xSubDir)
{
	WIN32_FIND_DATA FindData;
	HANDLE FileHandle;

	int Found = 0;
	int NeededMemory = 0;

	char FileCountStatus[257];
	char DirTemp[257];

	BOOL FoundDir = FALSE;

	sprintf(DirTemp,"%s\\%s\\%s",xInputDir,xSubDir,"*.*");
	FileHandle = FindFirstFile(DirTemp,&FindData);
	Found = (int) FileHandle;

	while (Found != 0)
	{
		/* If Found Data Is A Directory */
		if( strcmp(FindData.cFileName,".") && strcmp(FindData.cFileName,"..") ) // If (Not) NULL Or Root
		{
			sprintf(FileCountStatus,"%d Files, %d Folders",Package.FileCount,Package.FolderCount);
			Status = FileCountStatus;
			if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				NeededMemory = strlen(xSubDir) + strlen(FindData.cFileName) + 10;
				Package.FolderNames[Package.FolderCount] = (char *) malloc(NeededMemory);
				sprintf(Package.FolderNames[Package.FolderCount],"%s\\%s",xSubDir,FindData.cFileName);
				FoundDir = TRUE;
				Package.FolderCount++;
			}
			else{ /* If Found Data Is A File */
				NeededMemory = strlen(xSubDir) + strlen(FindData.cFileName) + 10;
				Package.FileNames[Package.FileCount] = (char *) malloc(NeededMemory);
				sprintf(Package.FileNames[Package.FileCount],"%s\\%s",xSubDir,FindData.cFileName);
				Package.FileCount++;
			}
		}

		if(Package.FileCount > MaxFiles || Package.FolderCount > MaxFiles)
		{
			char StatusTemp[257];
			FindClose(FileHandle);
			CancelFlag = TRUE;
			sprintf(StatusTemp,"128Gamma %s Can Pack A Max Of %d Files And Folders",ShortVersion,MaxFiles);
			MessageBox (StatusDialog_hWnd, StatusTemp ,TitleCaption,0 + MB_SYSTEMMODAL);
			return 1;
		}

		if(CancelFlag)
		{
			FindClose(FileHandle);
			CleanUp(0,1);
			return 1;
		}
		Found = FindNextFile(FileHandle, &FindData);
	}
	FindClose(FileHandle);

	return 0;
}

///////////////////////////////////////////////////////////////////////

