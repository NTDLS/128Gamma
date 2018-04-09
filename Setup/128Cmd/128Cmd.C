#include <Windows.H>
#include <WindowsX.H>
#include <Io.H>
#include <Stdio.H>
#include <Stdlib.H>

#include "Headers/Definitions.H"
#include "Headers/Functions.H"

//////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
	if(argc == 5)
	{
		if(strcmp(LCase(argv[4]),"/c") == 0)
			CompressEncrypt = TRUE;
		else printf("Argument (%s) Unknown.\n\n",argv[4]);
	}

	if(argc == 4 || argc == 5)
	{
		ProcessInput(argv[1],argv[2],argv[3]);
	}
	else{
		printf("\n %s\n",GammaVersion);
		printf("\n 128Cmd.Exe [Input] [Output] [Password] [Option]\n");
		printf("---------------------------------------------\n");
		printf("   [Input] : Name of file to be encrypted.\n");
		printf("  [Output] : Name of encrypted file to be written to.\n");
		printf("[Password] : Key to encrypt the file with.\n");
		printf("  [Option] : /C = Compress \n");
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////

