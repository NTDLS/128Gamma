// ***************************************************************
// * Blowfish Algorithm Implemented By Adam Berent				 *
// * ABI- Software Development. www.ABISoft.Net 								*
// ***************************************************************

#include "Headers/BlowFish.H"

//---------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////

int BlowFish_Interface(char *SourceName,char *TargetName, char *Key, int EFlag)
{
	unsigned long int L = 1, R = 3, RemLong, FileSize, Process_FileSize, StatusValue, PrintCount = 0;
	FILE *Source, *Target;
	DWORD i;

	// Create BlowFish object from blowfish.cpp
	BF_BlowFish((unsigned char*)Key, strlen(Key));

	// If file size mod 8 is not zero then increase the files size
	if (EFlag == 3)
	{
		if((Source = fopen(SourceName,"rb")) == NULL)
		{
			char ErrorTemp[1024];
			sprintf(ErrorTemp,"Could Not Open %s For Read. Error 19.",SourceName);
			Error(ErrorTemp);
			if(Source)fclose(Source);
			return 0;
		}

		fseek(Source,0,SEEK_END);
		FileSize = ftell(Source);

		/* Ammount of bits added to the end of the encrypted file
		to make the FileSize mod 8 = 0, these bits are later
		removed */

		RemLong = (FileSize % 8);

		if (FileSize < 1)
		{
			Error("File Contains No Data. Error 20.");
			if(Source)fclose(Source);
			return 0;
		}
		if(Source)fclose(Source);
	}

	//Open Files
	if((Source = fopen(SourceName,"rb")) == NULL)
	{
		char ErrorTemp[1024];
		sprintf(ErrorTemp,"Could Not Open %s For Read. Error 21.",SourceName);
		Error(ErrorTemp);
		if(Source)fclose(Source);
		return 0;
	}

	// Seek to end of file
	fseek(Source,0,SEEK_END);

	// Get byte position (File Size)
	FileSize = ftell(Source);

	// Calculate how many sets of 8 for encryption loop
	Process_FileSize = (FileSize/sizeof(L))/2;

	// Seek back to the beginning
	fseek(Source,0,SEEK_SET);

	//Write and read header this allows the coder to check the key
	//before decryption the file

	if (EFlag == 3) // if Encrypting
	{
		if((Target = fopen(TargetName,"wb")) == NULL)
		{
			char ErrorTemp[1024];
			sprintf(ErrorTemp,"Could Not Open %s For Write. Error 22.",SourceName);
			Error(ErrorTemp);
			if(Source)fclose(Source);
			if(Target)fclose(Target);
			return 0;
		}

		if(UseFlags)
		{
			/* Write Flags To File */
			if(CompressEncrypt) fprintf(Target,ENCCompress);
			else fprintf(Target,ENCFlag);
		}

		//write header <nept35> 64 bit
		//on decryption this is decrypted using the provided key
		//and checked against string <nept35> to see if the
		//correct key was used

		L = 7534140;		//<nep
		R = 7534132;		//t35>

		//Encrypt header
		BF_Encrypt(&L, &R);

		//Write header
		fwrite((char *)&L, sizeof(L), 1, Target);
		fwrite((char *)&R, sizeof(R), 1, Target);

		// Write header containing the number of bits added (Rem)
		// older versions of ABI- CODER add 4 bits insted of 8

		if (EFlag == 2)
			fwrite((char *)&RemLong, 8, 1, Target);
		else
			fwrite((char *)&RemLong, 4, 1, Target);

		Process_FileSize++; // Add 1 to Process_FileSize
	}
	else { // Else if decrypting

		if(UseFlags)
		{
			fseek(Source,FlagLEN,SEEK_SET); // Past Encrypt Flag
			Process_FileSize--;
		}

		// Read and compare header <nept35>
		fread((char *)&L, sizeof(L), 1, Source);
		fread((char *)&R, sizeof(R), 1, Source);

		//Decrypt header
		BF_Decrypt(&L, &R);

		/* Compare if decrypted correctly,
		If not so tell user wrong key */

		if (L != 7534140 || R != 7534132)
		{
			printf("Wrong Key, Can't Decrypt.\n");
			if(Source)fclose(Source);
			return 0;
		}

		if((Target = fopen(TargetName,"wb")) == NULL)
		{
			char ErrorTemp[1024];
			sprintf(ErrorTemp,"Could Not Open %s For Write. Error 22.",SourceName);
			Error(ErrorTemp);
			if(Source)fclose(Source);
			if(Target)fclose(Target);
			return 0;
		}

		// Because header does not count suptract one
		Process_FileSize--;

		// Read second header containing Rem
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

	// Begin encryption
	for(i=0; i < Process_FileSize; i++)
	{
		Current_Size += 8;

		if(Current_Size> PrintCount)
		{
			StatusValue = (( (Current_Size-8) / SourceFileSize) * 100);
			printf("128Coding : %12ld %% \r", StatusValue);
			PrintCount += 1024;
		}

		// Read 8 bytes from file
		fread((char *)&L, sizeof(L), 1, Source);
		fread((char *)&R, sizeof(R), 1, Source);

		// Call blowfish.cpp Functions
		if (EFlag == 3)
			BF_Encrypt(&L, &R);
		else BF_Decrypt(&L, &R);

		// Write 8 bytes to file
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

	printf("128Gamma Console Process Complete.\r");
	printf("\n");

	//close files
	if(Source)fclose(Source);
	if(Target)fclose(Target);

	return 1;
}
