#include <Windows.H>
#include <WindowsX.H>
#include <Stdio.H>
#include <Stdlib.H>

#include "Delta.H"

///////////////////////////////////////////////////////////////////////

/*
	DWORD CodedKey = MakeKey(Key);

	EncryptString(StringToEncrypt,CodedKey);
	DecryptString(EncryptedString,CodedKey);
*/

///////////////////////////////////////////////////////////////////////

__declspec(dllexport) char *EncryptString(char *xString, DWORD xKey)
{
	int LocalLoop = 0;
	int StringLength = strlen(xString);

	char *xStringArray = (char *) malloc(StringLength);
	char *FinalString = (char *) malloc(StringLength);

	DWORD NewKey;

	sprintf(xStringArray,"%s",xString);
	NewKey = xKey;

	while(LocalLoop != StringLength)
	{
		FinalString[LocalLoop] = xStringArray[LocalLoop] + NewKey;
		NewKey = FinalString[LocalLoop];
		LocalLoop++;
	}

	return FinalString;
}

///////////////////////////////////////////////////////////////////////

__declspec(dllexport) char *DecryptString(char *xString, DWORD xKey)
{
	int LocalLoop = 0;
	int StringLength = strlen(xString);

	char *xStringArray = (char *) malloc(StringLength);
	char *FinalString = (char *) malloc(StringLength);

	DWORD NewKey;

	sprintf(xStringArray,"%s",xString);

	NewKey = xKey;

	while(LocalLoop != StringLength)
	{
		FinalString[LocalLoop] = xStringArray[LocalLoop] - NewKey;
		NewKey = xStringArray[LocalLoop];
		LocalLoop++;
	}

	return FinalString;}

///////////////////////////////////////////////////////////////////////

__declspec(dllexport) DWORD MakeKey(char *xKey)
{
	DWORD LocalLoop  = 0;
	DWORD KeyLength  = strlen(xKey);
	DWORD xCodedKey = 0;

	char *xKeyArray = (char *) malloc(KeyLength + 1);
	sprintf(xKeyArray,"%s",xKey);

	while(LocalLoop != KeyLength)
	{
		xCodedKey += xKeyArray[LocalLoop];

		if ( xCodedKey > 250 )
			xCodedKey = (xCodedKey % LocalLoop);

		LocalLoop++;
	}

	return xCodedKey;
}

///////////////////////////////////////////////////////////////////////

