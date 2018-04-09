#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

FILE *MyFILE;
static char FileName[257];
static int LocalLoop;
static int LoopCount;

int main()
{
	while(LocalLoop < 25)
	{
		LocalLoop++;
		LoopCount = 0;

		sprintf(FileName,"Test Dir %d",LocalLoop);
		CreateDirectory(FileName,0);

		sprintf(FileName,"Test File %d.Txt",LocalLoop);
		if((MyFILE = fopen(FileName,"w")) == NULL)
		{
			printf("Could Not Write %s .",FileName);
			return 0;
		}

		while(LoopCount < 1024)
		{
			LoopCount++;
			fprintf(MyFILE,".");
		}
		if(MyFILE)fclose(MyFILE);
	}

	return 0;
}
