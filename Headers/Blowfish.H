/*

 BlowFish.H - BlowFish Encryption Interface Function
	Used In Conjunction With BlowFish.Dll

int BlowFish_Interface(char *SourceName,char *TargetName, char *Key, int EFlag)

*/

///////////////////////////////////////////////////////////////////////

unsigned long BF_F(unsigned long x);
void BF_BlowFish(unsigned char *key, int keyLen);
void BF_Encrypt(unsigned long *xL, unsigned long *xR);
void BF_Decrypt(unsigned long *xL, unsigned long *xR);
int  BF_Interface(char *,char *, char *, int);

///////////////////////////////////////////////////////////////////////

int BlowFish_Interface(char *SourceName,char *TargetName, char *Key, int EFlag)
{
	unsigned long L = 1, R = 3, RemLong, FileSize, Process_FileSize;
	FILE *Source, *Target;
	DWORD i;

	// Create BlowFish Object
	BF_BlowFish((unsigned char*)Key, strlen(Key));

	// If File Size Mod 8 Is Not Zero Then Increase The Files Size
	if (EFlag == 3)
	{
		if((Source = fopen(SourceName,"rb")) == NULL)
		{
			char ErrorTemp[1024];
			sprintf(ErrorTemp,"BlowFish: Could Not Open %s For Read. Error 2.",SourceName);
			Error(ErrorTemp);
			if(Source)fclose(Source);
			return 0;
		}

		fseek(Source,0,SEEK_END);
		FileSize = ftell(Source);

		/* Ammount Of Bits Added To The End Of The Encrypted File
		To Make The FileSize mod 8 = 0, These Bits Are Later
		Removed */

		RemLong = (FileSize % 8);

		if (FileSize < 1)
		{
			Error("BlowFish: File Contains No Data. Error 3.");
			if(Source)fclose(Source);
			return 0;
		}
		if(Source)fclose(Source);
	}

	//Open Files
	if((Source = fopen(SourceName,"rb")) == NULL)
	{
		char ErrorTemp[1024];
		sprintf(ErrorTemp,"BlowFish: Could Not Open %s For Read. Error 4.",SourceName);
		Error(ErrorTemp);
		if(Source)fclose(Source);
		return 0;
	}

	// Seek To End Of File
	fseek(Source,0,SEEK_END);

	// Get byte position (File Size)
	FileSize = ftell(Source);

	// Calculate How Many Sets Of 8 For Encryption Loop
	Process_FileSize = (FileSize/sizeof(L))/2;

	// Seek Back To The Beginning
	fseek(Source,0,SEEK_SET);

	//Write And Read Header This Allows The Coder To Check The Key
	//Before Decryption The File

	if (EFlag == 3) // if Encrypting
	{
		if((Target = fopen(TargetName,"wb")) == NULL)
		{
			char ErrorTemp[1024];
			sprintf(ErrorTemp,"BlowFish: Could Not Open %s For Write. Error 5.",SourceName);
			Error(ErrorTemp);
			if(Source)fclose(Source);
			if(Target)fclose(Target);
			return 0;
		}

		if(UseFlags)
		{
			/* Write Flags To File */
			if(CompressEncrypt)fprintf(Target,ENCCompress);
			if(CompressPack)fprintf(Target,PACCompress);
			if(Pack)fprintf(Target,PACFlag);
			if(Encrypt)fprintf(Target,ENCFlag);
		}

		//write Header <nept35> 64-Bit
		//On Decryption This Is Decrypted Using The Provided Key
		//And Checked Against String <nept35> To See If The
		//Correct Key Was Used

		L = 7534140;		//<nep
		R = 7534132;		//t35>

		//Encrypt Header
		BF_Encrypt(&L, &R);

		//Write Header
		fwrite((char *)&L, sizeof(L), 1, Target);
		fwrite((char *)&R, sizeof(R), 1, Target);

		// Write Header Containing The Number Of Bits Added (Rem)
		// Older Versions Of ABI- CODER Add 4 Bits Insted Of 8

		if (EFlag == 2)
			fwrite((char *)&RemLong, 8, 1, Target);
		else
			fwrite((char *)&RemLong, 4, 1, Target);

		Process_FileSize++; // Add 1 To Process_FileSize
	}
	else { // Else If Decrypting

		if(UseFlags)
		{
			fseek(Source,FlagLEN,SEEK_SET); // Past Encrypt Flag
			Process_FileSize--;
		}

		// Read And Compare Header <nept35>
		fread((char *)&L, sizeof(L), 1, Source);
		fread((char *)&R, sizeof(R), 1, Source);

		//Decrypt Header
		BF_Decrypt(&L, &R);

		/* Compare If Decrypted Correctly,
		If Not Tell User Wrong Key */

		if (L != 7534140 || R != 7534132)
		{
			Status = "Wrong Key, Can't Decrypt";
			if(Source)fclose(Source);
			if(Target)fclose(Target);
			return 0;
		}

		if((Target = fopen(TargetName,"wb")) == NULL)
		{
			char ErrorTemp[1024];
			sprintf(ErrorTemp,"BlowFish: Could Not Open %s For Write. Error 6.",SourceName);
			Error(ErrorTemp);
			if(Source)fclose(Source);
			if(Target)fclose(Target);
			return 0;
		}

		// Because Header Does Not Count Suptract One
		Process_FileSize--;

		// Read Second Header Containing Rem
		fread((char *)&RemLong, 4, 1, Source);
	}

	unsigned long CurrentByte;
	unsigned long EndOfFile;
	long double Current_Size;
	DWORD SourceFileSize;

	CurrentByte = ftell(Source);
	fseek(Source,0,SEEK_END);
	EndOfFile = ftell(Source);
	fseek(Source,CurrentByte,SEEK_SET);
	SourceFileSize = EndOfFile - CurrentByte;
	Current_Size = 0;

	// Begin Encryption / Decryption
	for(i=0; i < Process_FileSize; i++)
	{
		Current_Size += 8;
		StatusValue = ((Current_Size / SourceFileSize) * 100);

		if(CancelFlag)
		{
			if(Source)fclose(Source);
			if(Target)fclose(Target);
			CleanUp(0,1);
			return 0;
		}

		// Read 8 Bytes From File
		fread((char *)&L, sizeof(L), 1, Source);
		fread((char *)&R, sizeof(R), 1, Source);

		// Call blowfish.cpp Functions
		if (EFlag == 3) BF_Encrypt(&L, &R);
		else BF_Decrypt(&L, &R);

		// Write 8 Bytes To File
		if (i==Process_FileSize-1 && EFlag==1)
		{
			if (RemLong < 4)
			{
				fwrite((char *)&L, (RemLong), 1, Target);
			}
			else {
				fwrite((char *)&L, sizeof(L), 1, Target);
				fwrite((char *)&R, sizeof(R) - (8-RemLong), 1, Target);
			}
		}
		else {
			fwrite((char *)&L, sizeof(L), 1, Target);
			fwrite((char *)&R, sizeof(R), 1, Target);
		}
	}

	//Close Files
	if(Source)fclose(Source);
	if(Target)fclose(Target);

	return 1;
}
