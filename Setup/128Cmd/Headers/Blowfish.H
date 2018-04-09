//*********************************************************************

#define NumberOfRounds 16

unsigned long P[18];
unsigned long S[4][256];

unsigned long BF_F(unsigned long x);
void BF_BlowFish(unsigned char *key, int keyLen);
void BF_Encrypt(unsigned long *xL, unsigned long *xR);
void BF_Decrypt(unsigned long *xL, unsigned long *xR);
int  BF_Interface(char *,char *, char *, int);

#include "Headers/Variables.h"

//*********************************************************************
/* This Is The BlowFish Constructor.
   It Computes Teh S And P Boxes Based On
   The Key And The Predifinded sBox and pBox in Variables.H */
//*********************************************************************

void BF_BlowFish(unsigned char *key, int keyLen)
{
	int c1, c2, c3;
	unsigned long data, datal, datar;
	//Initialize 4 sboxes with the predifinded sBoxes in blowfishvariables.h
	//that contain the hexadecimal digits of pi (less the initial 3)
	for (c1=0; c1<4; c1++)
		for (c2 = 0; c2 < 256; c2++)
			S[c1][c2] = sBoxes[c1][c2];

	c2 = 0;
	//Initialize 4 sboxes with the predifinded pArray in blowfishvariables.h
	//that contain the hexadecimal digits of pi (less the initial 3)
	for (c1 = 0; c1 < NumberOfRounds + 2; ++c1) {
		data = 0x00000000;
		for (c3 = 0; c3 < 4; ++c3) {
			data = (data << 8) | key[c2];
			c2 = c2 + 1;
			if (c2 >= keyLen) c2 = 0;
		}
		//XOR P[0] with the first 32 bits of the key, XOR P[1] with the
		//second 32-bits of the key, and so on for all bits of the key
		//(possibly up to P14)
		P[c1] = pArray[c1] ^ data;
	}
	//all-zero strings
	datal = 0x00000000;
	datar = 0x00000000;

	//This loop first encryps the all-zero strings with the Blowfish
	//algorithm, then replaces P[0] and P[1] with output, then
	//encrypt the output again and replaces P[2] P[3] with that, and so
	//on for all the values of P, then for all the values of S total 521
	//iterations
	for (c1 = 0; c1 < NumberOfRounds + 2; c1 += 2) {
		BF_Encrypt(&datal, &datar);
		P[c1] = datal;
		P[c1 + 1] = datar;
	}
	for (c1 = 0; c1 < 4; ++c1)
		for (c2 = 0; c2 < 256; c2 += 2) {
			BF_Encrypt(&datal, &datar);
			S[c1][c2] = datal;
			S[c1][c2 + 1] = datar;
		}
} // end blowfish constructor

/* This is the blowfish encrypt function.  Encrypts the input 64-bit
data element which is first divided into two 32-bit halves: xL, xR.
It then executes xL = xL XOR P[c1], xR = BF_F(xL) XOR xR, Swap xL and xR
16 times. Afterthe sixteenth round, it swaps xL and xR again to undo
the last swap. Then xR = xR XOR P17 and xL = xL XOR P18. Finally it
recombines xL and xR to get the ciphertext. */

void BF_Encrypt(unsigned long *xL, unsigned long *xR)
{
	unsigned long tempX, XL, XR;
	//can't XOR pointers need to assign to variables
	XL=*xL;
	XR=*xR;
	for (int c1 = 0; c1<NumberOfRounds; c1++)
	{
		//xL = xL XOR Pi
		XL = XL ^ P[c1];
		//xR = BF_F(xL) XOR xR
		XR = BF_F(XL) ^ XR;
		//Swap xL and xR
		tempX=XL;
		XL=XR;
		XR=tempX;
	}
	//After 16th round swap again
	tempX=XL;
	XL=XR;
	XR=tempX;
	// xR = xR XOR P17 and xL = xL XOR P18
	XR = XR ^ P[NumberOfRounds];
	XL = XL ^ P[NumberOfRounds+1];
	*xL = XL;
	*xR = XR;
}
//*********************************************************************
// Decryption is the same as encryption but P is used in reverse order
//*********************************************************************

void BF_Decrypt(unsigned long *xL, unsigned long *xR)
{
	unsigned long tempX, XL, XR;
	//can't XOR pointers need to assign to variables
	XL=*xL;
	XR=*xR;
	for (int c1=NumberOfRounds+1; c1>1; --c1)
	{
		//xL = xL XOR Pi
		XL = XL ^ P[c1];
		//xR = BF_F(xL) XOR xR
		XR = BF_F(XL) ^ XR;
		//Swap xL and xR
		tempX=XL;
		XL=XR;
		XR=tempX;
	}
	//After 16th round swap again
	tempX=XL;
	XL=XR;
	XR=tempX;
	// xR = xR XOR P17 and xL = xL XOR P18
	XR = XR ^ P[1];
	XL = XL ^ P[0];
	*xL = XL;
	*xR = XR;
}

//*********************************************************************
/* Function BF_F divides xL into four eight-bit quarters: a, b, c, and d. Then
 BF_F(xL) = ((S1,a + S2,b mod 232) XOR S3,c) + S4,d mod 232 */
//*********************************************************************

unsigned long BF_F(unsigned long x)
{
	unsigned short a, b, c, d;
	unsigned long  result;
	d = x & 0x00FF;
	x >>= 8;
	c = x & 0x00FF;
	x >>= 8;
	b = x & 0x00FF;
	x >>= 8;
	a = x & 0x00FF;
	result = S[0][a] + S[1][b];
	result = result ^ S[2][c];
	result = result + S[3][d];
	return result;

}

///////////////////////////////////////////////////////////////////////
