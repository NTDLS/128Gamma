#include "Headers/Registry.H"
#include "Headers/Interface.H"
#include "Headers/Compression.H"

//////////////////////////////////////////////////////////////////////

void Error(char *xMessage)
{
    printf("Error: %s\n",xMessage);
    exit(0);
}

//////////////////////////////////////////////////////////////////////

int PreCheckFile(char *FileToCheck)
{
    FILE * FileToCheck_File;

    unsigned int Attrib = 0;
    char xDrive[255], xDir[255], xFile[255], xExt[255];
    _splitpath(FileToCheck,xDrive,xDir,xFile,xExt);

    /* Get Input File Attributes */
    Attrib = GetFileAttributes(FileToCheck);

    /* Check For File Access */
    if(!FAccess(FileToCheck,0)) // Check Exist
        Error("File Does Not Exist. Error 9.");
    if(!FAccess(FileToCheck,4)) // Check Read Access
        Error("File Does Not Allow Read Access. Error 11.");

    if(Attrib & FILE_ATTRIBUTE_DIRECTORY) // If Input Is A Folder
    {
        /* If Directory Name Is 3 Char Long  ( c:\ d:\ ...Etc ) */
        if(strlen(FileToCheck) == 3)
        {
            char MessageTemp[257];
            sprintf(MessageTemp,"128Gamma Console %s Does Not Have Support For Full Drives At This Time.",ShortVersion);
            printf(MessageTemp);
            return 0;
        }
        return 1;
    }

    DWORD FileSize_Temp = FileSize(FileToCheck);
    if(FileSize_Temp == -1)return 0;

    if(FileSize_Temp > MaxFileSize)
    {
        char MessageTemp[257];
        sprintf(MessageTemp,"128Gamma Console %s Requires That Files Be Under %d MB",ShortVersion,MaxFileSize/1024/1024);
        printf(MessageTemp);
        return 0;
    }

    if(FileSize_Temp < 1)
    {
        char MessageTemp[257];
        sprintf(MessageTemp,"128Gamma Console %s Requires That Files Contain Some Data.",ShortVersion);
        printf(MessageTemp);
        return 0;
    }

    /* Open Source File */
    if((FileToCheck_File = fopen(FileToCheck,"rb")) == NULL)
        Error("Could Not Open Source For Read. Error: 13.");

    /* Read File Flag ( If Any ) */
    fread(Buffer,sizeof(char),FlagLEN,FileToCheck_File);

    if(strcmp(Buffer,ENCFlag) == 0) // If File Encryted
    {
        if(FileToCheck_File)
            fclose(FileToCheck_File);
        return 2;
    }

    if(strcmp(Buffer,PACFlag) == 0) // If File Packed
    {
        if(FileToCheck_File)
            fclose(FileToCheck_File);
        return 3;
    }

    if(strcmp(Buffer,ENCCompress) == 0) // If File Compressed Encryted
    {
        if(FileToCheck_File)
            fclose(FileToCheck_File);
        return 4;
    }

    if(strcmp(Buffer,PACCompress) == 0) // If File Compressed Packed
    {
        if(FileToCheck_File)
            fclose(FileToCheck_File);
        return 5;
    }

    if(strcmp(Buffer,ABIFlag) == 0) // If Encryted By ABI-Coder
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

/*
6   Check for read and write permission
4   Check for read permission
2   Check for write permission
1   Execute (ignored)
0   Check for existence of file
*/

BOOL FAccess(char *FileName, short Mode)
{
    if(access(FileName, Mode) == 0)
        return 1;
    return 0;
}

/////////////////////////////////////////////////////////////////////

DWORD FileSize(char *File2Size)
{
    FILE *Source;
    DWORD size;
    SetFileAttributes(File2Size,FILE_ATTRIBUTE_NORMAL);
    Source = CreateFile(File2Size,GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
    size = GetFileSize(Source,NULL);
    if(size == HFILE_ERROR)
    {
        char StatusTemp[1024];
        sprintf(StatusTemp,"Cannot Read: \n%s \nThe File May Be In Use. Error 14.",File2Size);
        Error(StatusTemp);
        return -1;
    }
    CloseHandle(Source);
    return size;
}

///////////////////////////////////////////////////////////////////////

int ProcessInput(char *InputName, char *OutputName, char *Password)
{
    short ENCMethod = 0;

    sprintf(GlobalFileName,"%s",InputName);

    printf("%s %s\n\n",TitleCaption,ShortVersion);

    if(strlen(Password) < 8)
    {
        printf("Key To Short... Must be at least 8 characters.\n");
        return 0;
    }

    if(strlen(Password) > 56)
    {
        printf("Key To Long... Must no longer than 56 characters.\n");
        return 0;
    }

    printf("Password Strength = %d-Bits\n",strlen(Password)*8);

    /*
    0 = Cancel , Dir = 1 , Encrypted = 2 , Packed = 3
    Compressed Encrypted = 4 , Compressed Packed = 5
    ABI-Coder File = 6, Not Encrypted = 7
    */

    ENCMethod = PreCheckFile(GlobalFileName);

    if(ENCMethod == 0) // Cancel
    {
        return(0);
    }

    if(ENCMethod == 1) // Directory
    {
        printf("Directory. (Not Supported), Use 128Gamma for windows.\n");
        return(0);
    }

    if(ENCMethod == 2) // Encrypted
    {
        printf("Decrypting %s, Size %d Bytes.\n",InputName,FileSize(InputName));
        BlowFish_Interface(InputName, OutputName, Password, 1);
        return(0);
    }

    if(ENCMethod == 3) //  Packed
    {
        printf("Packed. (Not Supported), Use 128Gamma for windows.\n");
        return(0);
    }

    if(ENCMethod == 4) //  Compressed Encrypted
    {
        char CompressTemp[257];
        sprintf(CompressTemp,"%s\\%s",GetTempDirectory(),"Compress.Tmp");
        printf("Compress Encrypting %s, Size %d Bytes.\n",InputName,FileSize(InputName));
        BlowFish_Interface(InputName, CompressTemp, Password, 1);
        Decompress(CompressTemp,OutputName);
        _fcloseall();
        unlink(CompressTemp);
        return(0);
    }

    if(ENCMethod == 5) //  Compressed Packed
    {
        printf("Compressed, Packed. (Not Supported), Use 128Gamma for windows.\n");
        return(0);
    }

    if(ENCMethod == 6) // Encrypted by ABI-Coder
    {
        printf("ABI-Coder Decrypting %s, Size %d Bytes.\n",InputName,FileSize(InputName));
        BlowFish_Interface(InputName, OutputName, Password, 1);
        return(0);
    }

    if(ENCMethod == 7) // NOT Encrypted
    {
        if(CompressEncrypt)
        {
            char CompressTemp[257];
            sprintf(CompressTemp,"%s\\%s",GetTempDirectory(),"Compress.Tmp");
            printf("Compress Encrypting %s, Size %d Bytes.\n",InputName,FileSize(InputName));
            Compress(InputName,CompressTemp);
            BlowFish_Interface(CompressTemp, OutputName, Password, 3);
            _fcloseall();
            unlink(CompressTemp);
        }
        else{
            printf("Encrypting %s, Size %d Bytes.\n",InputName,FileSize(InputName));
            BlowFish_Interface(InputName, OutputName, Password, 3);
        }
        return(0);
    }

    return(1);
}

//////////////////////////////////////////////////////////////////////

char *GetTempDirectory(void)
{
    char *Name;
    Name = (char *) malloc(256);
    GetTempPath(256,Name);
    return Name;
}

//////////////////////////////////////////////////////////////////////

char *LCase (char *InputString)
{
    char *StringTemp;
    StringTemp = (char *) malloc(256);
    sprintf(StringTemp,"%s",strlwr(InputString));
    return StringTemp;
}

//////////////////////////////////////////////////////////////////////

