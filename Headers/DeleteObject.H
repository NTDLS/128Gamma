//--------------------------------------------------------------------

#include <Windows.H>
#include <WindowsX.H>
#include <Stdio.H>
#include <Stdlib.H>

//////////////////////////////////////////////////////////////////////

void GetFileNames (char *);
int GetSubFileNames (char *, char *);
int DeleteTree(char *);
int xDeleteObject(char *);

struct _DeleteFolder {
	char *SourceDirectory;
	char *FolderNames[10240];
	char *FileNames[10240];
	int FileCount;
	int FolderCount;
}; struct _DeleteFolder Delete;

//////////////////////////////////////////////////////////////////////

int xDeleteObject(char *xObjectName)
{
	if(IsDirectory(xObjectName)) // If xObjectName Is A Directory
	{
		if(!DeleteTree(xObjectName))
			return 1;
	}
	else{
		if(!DeleteFile(xObjectName))  // If xObjectName Is A File
			return 1;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////

int DeleteTree(char *xInputName)
{
	EnableWindow(GetDlgItem(StatusDialog_hWnd,100),FALSE);

	float TotalEntries = 0;
	float CurrentCount = 0;

	Delete.SourceDirectory = xInputName;

	GetFileNames(Delete.SourceDirectory);

	char DeleteTemp[1024];

	TotalEntries = (Delete.FileCount + Delete.FolderCount + 1);

	do
	{
		sprintf(DeleteTemp,"%s\\%s",Delete.SourceDirectory,Delete.FileNames[Delete.FileCount]);
		DeleteFile(DeleteTemp);
		free(Delete.FileNames[Delete.FileCount]);
		Delete.FileNames[Delete.FileCount] = '\0';
		Delete.FileCount--;

		StatusValue = ((CurrentCount / TotalEntries) * 100);
		CurrentCount++;

	}while(Delete.FileCount != -1);

	do
	{
		sprintf(DeleteTemp,"%s\\%s",Delete.SourceDirectory,Delete.FolderNames[Delete.FolderCount]);
		RemoveDirectory(DeleteTemp);
		free(Delete.FolderNames[Delete.FolderCount]);
		Delete.FolderNames[Delete.FolderCount] = '\0';
		Delete.FolderCount--;

		StatusValue = ((CurrentCount / TotalEntries) * 100);
		CurrentCount++;

	}while(Delete.FolderCount != -1);

	RemoveDirectory(Delete.SourceDirectory);

	return 1;
}

//////////////////////////////////////////////////////////////////////

void GetFileNames(char *xInputDir)
{
	WIN32_FIND_DATA FindData;
	HANDLE FileHandle;
	int Found = 0;
	char DirTemp[257];

	sprintf(DirTemp,"%s\\%s",xInputDir,"*.*");
	FileHandle = FindFirstFile(DirTemp,&FindData);

	Found = (int) FileHandle;

	while (Found != 0)
	{
		if( strcmp(FindData.cFileName,".") && strcmp(FindData.cFileName,"..") ) // If (Not) NULL Or Root
		{
			if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				Delete.FolderNames[Delete.FolderCount] = (char *) malloc(1024);
				sprintf(Delete.FolderNames[Delete.FolderCount],"%s",FindData.cFileName);
				Delete.FolderCount++;
			}
			else {
				Delete.FileNames[Delete.FileCount] = (char *) malloc(1024);
				sprintf(Delete.FileNames[Delete.FileCount],"%s",FindData.cFileName);
				Delete.FileCount++;
			}
		}
		Found = FindNextFile(FileHandle, &FindData);
	}

	int LocalLoop;

	LocalLoop = 0;

	if(Delete.FolderCount != 0) // If we got another folder to look in
	{
		LocalLoop = 0;

		while(LocalLoop < Delete.FolderCount)
		{
			GetSubFileNames(Delete.SourceDirectory,Delete.FolderNames[LocalLoop]);
			LocalLoop++;
		}
	}

	FindClose(FileHandle);
}

//////////////////////////////////////////////////////////////////////

int GetSubFileNames(char *xInputDir, char *xSubDir)
{
	WIN32_FIND_DATA FindData;
	HANDLE FileHandle;

	int Found = 0;
	char DirTemp[257];

	sprintf(DirTemp,"%s\\%s\\%s",xInputDir,xSubDir,"*.*");
	FileHandle = FindFirstFile(DirTemp,&FindData);
	int FoundDir = 0;

	Found = (int) FileHandle;
	while (Found != 0)
	{
		if( strcmp(FindData.cFileName,".") && strcmp(FindData.cFileName,"..") ) // If (Not) NULL Or Root
		{
			if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				Delete.FolderNames[Delete.FolderCount] = (char *) malloc(1024);
				sprintf(Delete.FolderNames[Delete.FolderCount],"%s\\%s",xSubDir,FindData.cFileName);
				Delete.FolderCount++;
				FoundDir = 1;
			}
			else{
				Delete.FileNames[Delete.FileCount] = (char *) malloc(1024);
				sprintf(Delete.FileNames[Delete.FileCount],"%s\\%s",xSubDir,FindData.cFileName);
				Delete.FileCount++;
			}
		}
		Found = FindNextFile(FileHandle, &FindData);
	}

	FindClose(FileHandle);

	return FoundDir;
}

///////////////////////////////////////////////////////////////////////

