/*

128Coding.H - Encryption Algorithm Starter

448-Bit Key, 64-Bit Block Cypher BlowFish Algorithm

int IncodeMain(char *InputName, char *OutputName)
int DecodeMain(char *InputName, char *OutputName)

*/

//////////////////////////////////////////////////////////////////////

int IncodeMain(char *InputName, char *OutputName)
{
	if(BlowFish_Interface(InputName, OutputName, CodedKey, 3))
		return 1;
	else return 0;
}

//////////////////////////////////////////////////////////////////////

int DecodeMain(char *InputName, char *OutputName)
{
	if(BlowFish_Interface(InputName, OutputName, CodedKey, 1))
		return 1;
	else return 0;
}

//////////////////////////////////////////////////////////////////////

