/*
 UnPackFiles.H - Functions Used To Unpack Files From A Single File

int UnPackFiles(char *SourceFile, char *DestinationDir)

*/

////////////////////////////////////////////////////////////////////////

int UnPackFiles(char *SourceFile, char *DestinationDir)
{
	FILE *LocalTarget;
	FILE *LocalSource;

	char LocalTemp[1024];
	char StatusTemp[1024];

	float xFileCount = 0;
	float xLocalLoop = 0;

	DWORD LocalLoop = 0;
	int CurrentBuffer = 0;
	int NameLenTemp = 0;
	int NeededMemory = 0;

	long double SourceFileSize = 0;
	long double FileSizeLeft = 0;
	long double Current_Size = 0;

	/* Open Source File */
	if((LocalSource = fopen(SourceFile,"rb")) == NULL)
	{
		if(LocalSource)fclose(LocalSource);
		Error("UnPackFiles: Could Not Open File For Read. Error 19.");
		return 0;
	}

	/* Get Number Of Files	(DWORD FileCount) */
	Clear_ReadTemp();
	fread(ReadTemp,sizeof(char),6,LocalSource);
	Package.FileCount = atol(ReadTemp);

	fseek(LocalSource,1,SEEK_CUR);

	/* Get Number Of Folders  (DWORD FolderCount) */
	Clear_ReadTemp();
	fread(ReadTemp,sizeof(char),6,LocalSource);
	Package.FolderCount = atol(ReadTemp);

	xFileCount = Package.FileCount;

	fseek(LocalSource,1,SEEK_CUR);

	/* Get FileNames ( *FileNames[] )*/
	LocalLoop = 0;
	while(LocalLoop < Package.FileCount)
	{
		Clear_ReadTemp();
		fread(ReadTemp,sizeof(char),3,LocalSource);
		NameLenTemp = atoi(ReadTemp);
		Clear_ReadTemp();
		fread(ReadTemp,sizeof(char),NameLenTemp,LocalSource);
		NeededMemory = NameLenTemp + 10;
		Package.FileNames[LocalLoop] = (char *) malloc(NeededMemory);
		sprintf(Package.FileNames[LocalLoop],"%s",ReadTemp);
		LocalLoop++;
	}

	fseek(LocalSource,1,SEEK_CUR);

	/* Get FolderNames ( *FolderNames[] )*/
	LocalLoop = 0;
	while(LocalLoop < Package.FolderCount)
	{
		Clear_ReadTemp();
		fread(ReadTemp,sizeof(char),3,LocalSource);
		NameLenTemp = atoi(ReadTemp);
		Clear_ReadTemp();
		fread(ReadTemp,sizeof(char),NameLenTemp,LocalSource);
		NeededMemory = NameLenTemp + 10;
		Package.FolderNames[LocalLoop] = (char *) malloc(NeededMemory);
		sprintf(Package.FolderNames[LocalLoop],"%s",ReadTemp);
		LocalLoop++;
	}

	/* Create Folders */
	LocalLoop = 0;
	Status = "Creating Folders";
	while(LocalLoop < Package.FolderCount) // Print Folder Names
	{
		sprintf(LocalTemp,"%s\\%s",DestinationDir,Package.FolderNames[LocalLoop]);
		CreateDirectory(LocalTemp,0);
		LocalLoop++;
	}

	sprintf(StatusTemp,"UnPacking %d Files && %d Folders",Package.FileCount,Package.FolderCount);
	Status = StatusTemp;

	fseek(LocalSource,1,SEEK_CUR);

	// Write Data To Files (Begin)
	LocalLoop = 0;
	while(LocalLoop < Package.FileCount)
	{
		sprintf(LocalTemp,"%s\\%s",DestinationDir,Package.FileNames[LocalLoop]);

		/* Open Target File */
		if((LocalTarget = fopen(LocalTemp,"wb")) == NULL)
		{
			if(LocalTarget)fclose(LocalTarget);
			Error("UnPackFiles: Could Not Open File For Write. Error 20.");
			return 0;
		}

		Current_Size = 0;
		SourceFileSize = 0;
		FileSizeLeft = 0;

		/* Get Size Of File */
		Clear_ReadTemp();
		fread(ReadTemp,sizeof(char),9,LocalSource);
		SourceFileSize = atol(ReadTemp);

		Current_Size = 0;

		 // If File Can Be Completed In One Buffer
		if(SourceFileSize < BufferSize)
		{
			Buffer[0] = '\0';
			fread(Buffer,sizeof(char),SourceFileSize,LocalSource);
			fwrite(Buffer,sizeof(char),SourceFileSize,LocalTarget);
		}
		else{ // Else If File Bigger Than Buffer
			while(Current_Size < SourceFileSize)
			{
				FileSizeLeft = SourceFileSize - Current_Size;
				if(CancelFlag)
				{
					if(LocalSource)fclose(LocalSource);
					if(LocalTarget)fclose(LocalTarget);
					CleanUp(0,1);
					return 0;
				}

				/* If File Can Be Completed In One Buffer */
				if(FileSizeLeft < BufferSize)
				{
					Buffer[0] = '\0';
					fread(Buffer,sizeof(char),FileSizeLeft,LocalSource);
					fwrite(Buffer,sizeof(char),FileSizeLeft,LocalTarget);
					Current_Size += FileSizeLeft;
					break;
				}

				Buffer[0] = '\0';
				CurrentBuffer = fread(Buffer,sizeof(char),BufferSize,LocalSource);
				fwrite(Buffer,sizeof(char),CurrentBuffer,LocalTarget);
				Current_Size += BufferSize;
			}
		}
		if(LocalTarget)fclose(LocalTarget);
		LocalLoop++;
		xLocalLoop = LocalLoop;
		StatusValue = ((xLocalLoop / xFileCount) * 100);
	}
	// Write Data To Files (End)

	FreeMemory();

	if(LocalTarget)fclose(LocalTarget);
	if(LocalSource)fclose(LocalSource);

	return 1;
}

///////////////////////////////////////////////////////////////////////

