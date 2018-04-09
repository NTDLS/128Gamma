// 128Gamma Data Compression (Method_1)

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void PutBit(int);
void FlushBitBuffer(void);
void InitTree(void);
void InsertNode(int);
void DeleteNode(int);
void StartModel(void);
void UpdateModel(int);
static void Output(int);
void EncodeChar(int);
void EncodePosition(int);
void EncodeEnd(void);
void StartDecode(void);

int BinarySearchSym(unsigned int);
int BinarySearchPos(unsigned int);
int GetBit(void);
int DecodeChar(void);
int DecodePosition(void);

FILE *CompressSource, *CompressTarget;

#define SizeOfRingBuffer   4096 /* Size of ring buffer */
#define MatchLengthLimit   60	/* Upper limit for MatchLength */
#define THRESHOLD		   2	/* Encode string into position and length if MatchLength is greater than this */
#define NIL SizeOfRingBuffer	/* Index for root of binary search trees */

unsigned char  TextBuffer[SizeOfRingBuffer + MatchLengthLimit - 1];
			   /* ring buffer of size SizeOfRingBuffer, with
				  extra MatchLengthLimit-1 bytes to facilitate
				  string comparison
			   */

int MatchPosition;

int MatchLength;  /* MatchLength of longest match.
					 These are set by the InsertNode()
					 procedure.
				  */

int LChild[SizeOfRingBuffer + 1];	// Left Child
int RChild[SizeOfRingBuffer + 257]; // Right Child
int Parent[SizeOfRingBuffer + 1];	// Parent

///////////////////////////////////////////////////////////////////////

void PutBit(int bit)  /* Output one bit (bit = 0,1) */
{
	static unsigned int  buffer = 0, mask = 128;

	if (bit) buffer |= mask;
	if ((mask >>= 1) == 0) {
		if (putc(buffer, CompressTarget) == EOF)
			Error("Compression Error: Bit Output Compression Error. Error 2.");
		buffer = 0;
		mask = 128;
	}
}

///////////////////////////////////////////////////////////////////////

void FlushBitBuffer(void)  /* Send remaining bits */
{
	int LocalLoop = 0;
	for (LocalLoop = 0; LocalLoop < 7; LocalLoop++) PutBit(0);
}

///////////////////////////////////////////////////////////////////////

int GetBit(void)  /* Get one bit (0 or 1) */
{
	static unsigned int LoaclBuffer, Mask = 0;

	if ((Mask >>= 1) == 0) {
		LoaclBuffer = getc(CompressSource);
		Mask = 128;
	}
	return( (LoaclBuffer & Mask) != 0);
}

///////////////////////////////////////////////////////////////////////

/********** multiple binary trees **********/

///////////////////////////////////////////////////////////////////////

void InitTree(void)  /* Initialize trees */
{
	int LocalLoop;

	/* For LocalLoop = 0 to SizeOfRingBuffer - 1, RChild[LocalLoop] and LChild[LocalLoop]
	will be the right and left children of node LocalLoop.
	These nodes need not be initialized. Also, Parent[LocalLoop] is the
	parent of node LocalLoop. These are initialized to NIL (= SizeOfRingBuffer),
	which stands for 'not used.' For LocalLoop = 0 to 255,
	RChild[SizeOfRingBuffer + LocalLoop + 1] is the root of the tree for strings
	that begin with character LocalLoop.  These are initialized
	to NIL.  Note there are 256 trees. */

	/* Root */
	for (LocalLoop = SizeOfRingBuffer + 1; LocalLoop <= SizeOfRingBuffer + 256; LocalLoop++) RChild[LocalLoop] = NIL;
	/* Node */
	for (LocalLoop = 0; LocalLoop < SizeOfRingBuffer; LocalLoop++) Parent[LocalLoop] = NIL;
}

///////////////////////////////////////////////////////////////////////
/* Inserts string of length MatchLengthLimit, TextBuffer[r..r+MatchLengthLimit-1], into one of the
trees (TextBuffer[r]'th tree) and returns the longest-match position
and length via the global variables MatchPosition and MatchLength.
If MatchLength = MatchLengthLimit, then removes the old node in favor of the new
one, because the old one will be deleted sooner.
Note r plays double role, as tree node and position in buffer. */

void InsertNode(int r)
{
	int Value_A, Value_B, Cmp, Temp;
	unsigned char *LocalKey;

	Cmp = 1;
	LocalKey = &TextBuffer[r];
	Value_B = SizeOfRingBuffer + 1 + LocalKey[0];
	RChild[r] = LChild[r] = NIL;
	MatchLength = 0;

	for ( ; ; )
	{
		if (Cmp >= 0)
		{
			if (RChild[Value_B] != NIL) Value_B = RChild[Value_B];
			else{
				RChild[Value_B] = r;
				Parent[r] = Value_B;
				return;
			}
		}
		else{
			if (LChild[Value_B] != NIL)
			{
				Value_B = LChild[Value_B];
			}
			else{
				LChild[Value_B] = r;
				Parent[r] = Value_B;
				return;
			}
		}

		for (Value_A = 1; Value_A < MatchLengthLimit; Value_A++)
		{
			if((Cmp = LocalKey[Value_A] - TextBuffer[Value_B + Value_A]) != 0)	break;
		}

		if (Value_A > THRESHOLD)
		{
			if (Value_A > MatchLength)
			{
				MatchPosition = (r - Value_B) & (SizeOfRingBuffer - 1);
				if ((MatchLength = Value_A) >= MatchLengthLimit) break;
			}
			else if (Value_A == MatchLength){
				if ((Temp = (r - Value_B) & (SizeOfRingBuffer - 1)) < MatchPosition)
					MatchPosition = Temp;
			}
		}

	}
	Parent[r] = Parent[Value_B];
	LChild[r] = LChild[Value_B];
	RChild[r] = RChild[Value_B];
	Parent[LChild[Value_B]] = r;
	Parent[RChild[Value_B]] = r;
	if(RChild[Parent[Value_B]] == Value_B) RChild[Parent[Value_B]] = r;
	else LChild[Parent[Value_B]] = r;
	Parent[Value_B] = NIL;	/* remove Value_B */
}

///////////////////////////////////////////////////////////////////////

void DeleteNode(int p)	/* Delete node p from tree */
{
	int  Value_A;

	if (Parent[p] == NIL) return;  /* not in tree */
	if (RChild[p] == NIL) Value_A = LChild[p];
	else if (LChild[p] == NIL) Value_A = RChild[p];
	else {
		Value_A = LChild[p];
		if (RChild[Value_A] != NIL) {
			do {
				Value_A = RChild[Value_A];
			}
			while (RChild[Value_A] != NIL);
			RChild[Parent[Value_A]] = LChild[Value_A];
			Parent[LChild[Value_A]] = Parent[Value_A];
			LChild[Value_A] = LChild[p];
			Parent[LChild[p]] = Value_A;
		}
		RChild[Value_A] = RChild[p];
		Parent[RChild[p]] = Value_A;
	}
	Parent[Value_A] = Parent[p];
	if (RChild[Parent[p]] == p) RChild[Parent[p]] = Value_A;
	else				   LChild[Parent[p]] = Value_A;
	Parent[p] = NIL;
}

///////////////////////////////////////////////////////////////////////
/********** Arithmetic Compression **********/

#define M	15

/*	Q1 ( = 2 to the M) must be sufficiently large, but not so
large as the unsigned long 4 * Q1 * (Q1 - 1) overflows.  */

#define Q1	(1UL << M)
#define Q2	(2 * Q1)
#define Q3	(3 * Q1)
#define Q4	(4 * Q1)
#define MAX_CUM (Q1 - 1)

#define N_CHAR	(256 - THRESHOLD + MatchLengthLimit)
/* character code = 0, 1, ..., N_CHAR - 1 */

unsigned long int low = 0, high = Q4, value = 0;

int  shifts = 0;  /* counts for magnifying low and high around Q2 */
int  char_to_sym[N_CHAR], sym_to_char[N_CHAR + 1];

unsigned int
sym_freq[N_CHAR + 1],  /* frequency for symbols */
sym_cum[N_CHAR + 1],   /* cumulative freq for symbols */
position_cum[SizeOfRingBuffer + 1];   /* cumulative freq for positions */

///////////////////////////////////////////////////////////////////////

void StartModel(void)  /* Initialize model */
{
	int ch, sym, i;

	sym_cum[N_CHAR] = 0;
	for (sym = N_CHAR; sym >= 1; sym--) {
		ch = sym - 1;
		char_to_sym[ch] = sym;
		sym_to_char[sym] = ch;
		sym_freq[sym] = 1;
		sym_cum[sym - 1] = sym_cum[sym] + sym_freq[sym];
	}
	sym_freq[0] = 0;  /* sentinel (!= sym_freq[1]) */
	position_cum[SizeOfRingBuffer] = 0;
	for (i = SizeOfRingBuffer; i >= 1; i--)
		position_cum[i - 1] = position_cum[i] + 10000 / (i + 200);
	/* empirical distribution function (quite tentative) */
	/* Please devise a better mechanism! */
}

///////////////////////////////////////////////////////////////////////

void UpdateModel(int sym)
{
	int i, c, ch_i, ch_sym;

	if (sym_cum[0] >= MAX_CUM) {
		c = 0;
		for (i = N_CHAR; i > 0; i--) {
			sym_cum[i] = c;
			c += (sym_freq[i] = (sym_freq[i] + 1) >> 1);
		}
		sym_cum[0] = c;
	}
	for (i = sym; sym_freq[i] == sym_freq[i - 1]; i--) ;
	if (i < sym) {
		ch_i = sym_to_char[i];
		ch_sym = sym_to_char[sym];
		sym_to_char[i] = ch_sym;
		sym_to_char[sym] = ch_i;
		char_to_sym[ch_i] = sym;
		char_to_sym[ch_sym] = i;
	}
	sym_freq[i]++;
	while (--i >= 0) sym_cum[i]++;
}

///////////////////////////////////////////////////////////////////////

static void Output(int bit)  /* Output 1 bit, followed by its complements */
{
	PutBit(bit);
	for ( ; shifts > 0; shifts--) PutBit(! bit);
}

///////////////////////////////////////////////////////////////////////

void EncodeChar(int ch)
{
	int  sym;
	unsigned long int  range;

	sym = char_to_sym[ch];
	range = high - low;
	high = low + (range * sym_cum[sym - 1]) / sym_cum[0];
	low +=		 (range * sym_cum[sym	 ]) / sym_cum[0];
	for ( ; ; ) {
		if (high <= Q2) Output(0);
		else if (low >= Q2) {
			Output(1);
			low -= Q2;
			high -= Q2;
		}
		else if (low >= Q1 && high <= Q3) {
			shifts++;
			low -= Q1;
			high -= Q1;
		}
		else break;
		low += low;
		high += high;
	}
	UpdateModel(sym);
}

///////////////////////////////////////////////////////////////////////

void EncodePosition(int position)
{
	unsigned long int  range;

	range = high - low;
	high = low + (range * position_cum[position    ]) / position_cum[0];
	low +=		 (range * position_cum[position + 1]) / position_cum[0];
	for ( ; ; ) {
		if (high <= Q2) Output(0);
		else if (low >= Q2) {
			Output(1);
			low -= Q2;
			high -= Q2;
		}
		else if (low >= Q1 && high <= Q3) {
			shifts++;
			low -= Q1;
			high -= Q1;
		}
		else break;
		low += low;
		high += high;
	}
}

///////////////////////////////////////////////////////////////////////

void EncodeEnd(void)
{
	shifts++;
	if (low < Q1) Output(0);
	else Output(1);
	FlushBitBuffer();  /* flush bits remaining in buffer */
}

///////////////////////////////////////////////////////////////////////
/* 1	  if x >= sym_cum[1],
N_CHAR if sym_cum[N_CHAR] > x,
i such that sym_cum[i - 1] > x >= sym_cum[i] otherwise */

int BinarySearchSym(unsigned int x)
{
	int i, j, k;

	i = 1;
	j = N_CHAR;
	while (i < j) {
		k = (i + j) / 2;
		if (sym_cum[k] > x) i = k + 1;
		else j = k;
	}
	return i;
}

///////////////////////////////////////////////////////////////////////
/* 0 if x >= position_cum[1],
SizeOfRingBuffer - 1 if position_cum[SizeOfRingBuffer] > x,
i such that position_cum[i] > x >= position_cum[i + 1] otherwise */

int BinarySearchPos(unsigned int x)
{
	int i, j, k;

	i = 1;
	j = SizeOfRingBuffer;
	while (i < j) {
		k = (i + j) / 2;
		if (position_cum[k] > x) i = k + 1;
		else j = k;
	}
	return i - 1;
}

///////////////////////////////////////////////////////////////////////

void StartDecode(void)
{
	int i;

	for (i = 0; i < M + 2; i++)
		value = 2 * value + GetBit();
}

///////////////////////////////////////////////////////////////////////

int DecodeChar(void)
{
	int  sym, ch;
	unsigned long int  range;

	range = high - low;
	sym = BinarySearchSym((unsigned int)
			(((value - low + 1) * sym_cum[0] - 1) / range));
	high = low + (range * sym_cum[sym - 1]) / sym_cum[0];
	low +=		 (range * sym_cum[sym	 ]) / sym_cum[0];
	for ( ; ; ) {
		if (low >= Q2) {
			value -= Q2;
			low -= Q2;
			high -= Q2;
		}
		else if (low >= Q1 && high <= Q3) {
			value -= Q1;
			low -= Q1;
			high -= Q1;
		}
		else if (high > Q2) break;
		low += low;
		high += high;
		value = 2 * value + GetBit();
	}
	ch = sym_to_char[sym];
	UpdateModel(sym);
	return ch;
}

///////////////////////////////////////////////////////////////////////

int DecodePosition(void)
{
	int position;
	unsigned long int  range;

	range = high - low;
	position = BinarySearchPos((unsigned int)
			(((value - low + 1) * position_cum[0] - 1) / range));
	high = low + (range * position_cum[position    ]) / position_cum[0];
	low +=		 (range * position_cum[position + 1]) / position_cum[0];
	for ( ; ; ) {
		if (low >= Q2) {
			value -= Q2;
			low -= Q2;
			high -= Q2;
		}
		else if (low >= Q1 && high <= Q3) {
			value -= Q1;
			low -= Q1;
			high -= Q1;
		}
		else if (high > Q2) break;
		low += low;
		high += high;
		value = 2 * value + GetBit();
	}
	return position;
}

///////////////////////////////////////////////////////////////////////
/********** Compress and Decompress **********/

int Compress(char *SourceFileName, char *TargetFileName)
{
	int  i, c, len, r, s, Last_MatchLength;
	unsigned long int PrintCount = 0, CodeSize = 0, TextSize = 0, StatusValue;

	DWORD SourceFileSize = 0;
	long double Current_Size = 0;

	if((CompressSource = fopen(SourceFileName,"rb")) == NULL)
	{
		if(CompressSource)fclose(CompressSource);
		Error("Compression Error: Could Not Open Source File. Error 3.");
		return 0;
	}

	if((CompressTarget = fopen(TargetFileName,"wb")) == NULL)
	{
		if(CompressTarget)fclose(CompressTarget);
		Error("Compression Error: Could Not Open Target File. Error 4.");
		return 0;
	}

	fseek(CompressSource, 0L, SEEK_END);
	TextSize = ftell(CompressSource);
	SourceFileSize = TextSize;

	/* output size of text */
	if(fwrite(&TextSize, sizeof TextSize, 1, CompressTarget) < 1)
	{
		Error("Compression Error: File Write Error. Error 5.");
		return 0;
	}

	CodeSize += sizeof TextSize;
	if (TextSize == 0) return 0;

	rewind(CompressSource);
	TextSize = 0;
	StartModel();
	InitTree();
	s = 0;
	r = SizeOfRingBuffer - MatchLengthLimit;

	for (i = s; i < r; i++) TextBuffer[i] = ' ';

	for (len = 0; len < MatchLengthLimit && (c = getc(CompressSource)) != EOF; len++)
	{
		TextBuffer[r + len] = c;
		Current_Size++; // Compensate For File Size
		StatusValue = ((Current_Size / SourceFileSize) * 100);
	}

	TextSize = len;

	for (i = 1; i <= MatchLengthLimit; i++) InsertNode(r - i);
	InsertNode(r);

	do
	{
		if (MatchLength > len) MatchLength = len;
		if (MatchLength <= THRESHOLD) {
			MatchLength = 1;
			EncodeChar(TextBuffer[r]);
		}
		else{
			EncodeChar(255 - THRESHOLD + MatchLength);
			EncodePosition(MatchPosition - 1);
		}

		Last_MatchLength = MatchLength;

		for (i = 0; i < Last_MatchLength && (c = getc(CompressSource)) != EOF; i++)
		{
			Current_Size++; // Compensate For File Size

			DeleteNode(s);
			TextBuffer[s] = c;
			if (s < MatchLengthLimit - 1) TextBuffer[s + SizeOfRingBuffer] = c;
			s = (s + 1) & (SizeOfRingBuffer - 1);
			r = (r + 1) & (SizeOfRingBuffer - 1);
			InsertNode(r);
		}

		if((TextSize += i) > PrintCount)
		{
			StatusValue = ((Current_Size / SourceFileSize) * 100);
			printf("Compress  : %12ld %% \r", StatusValue);
			PrintCount += 1024;
		}

		while (i++ < Last_MatchLength)
		{
			DeleteNode(s);
			s = (s + 1) & (SizeOfRingBuffer - 1);
			r = (r + 1) & (SizeOfRingBuffer - 1);
			if (--len) InsertNode(r);
		}

	}
	while(len > 0); // End Of Do Loop

	EncodeEnd();

	/* Update Status One Last Time */
	StatusValue = ((Current_Size / SourceFileSize) * 100);
	printf("Compress  : %12ld %% \r", StatusValue);

	printf("\n");

	if(CompressSource)fclose(CompressSource);
	if(CompressTarget)fclose(CompressTarget);
	return 1;
}

///////////////////////////////////////////////////////////////////////

int Decompress(char *SourceFileName, char *TargetFileName)
{
	int  i, j, k, r, c;

	unsigned long int PrintCount = 0, TextSize = 0, StatusValue;

	DWORD SourceFileSize = 0;
	long double Current_Size = 0;

	if((CompressSource = fopen(SourceFileName,"rb")) == NULL)
	{
		if(CompressSource)fclose(CompressSource);
		Error("Compression Error: Could Not Open Source File. Error 6.");
		return 0;
	}
	if((CompressTarget = fopen(TargetFileName,"wb")) == NULL)
	{
		if(CompressTarget)fclose(CompressTarget);
		Error("Compression Error: Could Not Open Target File. Error 7.");
		return 0;
	}

	/* read size of text */
	if(fread(&TextSize, sizeof TextSize, 1, CompressSource) < 1)
		Error("Compression Error: File Read Error. Error 8.");

	if(TextSize == 0) return 0;
	SourceFileSize = TextSize;

	StartDecode();
	StartModel();

	for(i = 0; i < SizeOfRingBuffer - MatchLengthLimit; i++) TextBuffer[i] = ' ';

	r = SizeOfRingBuffer - MatchLengthLimit;

	for(Current_Size = 0; Current_Size < TextSize; )
	{
		c = DecodeChar();
		if (c < 256)
		{
			putc(c, CompressTarget);
			TextBuffer[r++] = c;
			r &= (SizeOfRingBuffer - 1);
			Current_Size++;
		}
		else{
			i = (r - DecodePosition() - 1) & (SizeOfRingBuffer - 1);
			j = c - 255 + THRESHOLD;
			for (k = 0; k < j; k++) {
				c = TextBuffer[(i + k) & (SizeOfRingBuffer - 1)];
				putc(c, CompressTarget);
				TextBuffer[r++] = c;
				r &= (SizeOfRingBuffer - 1);
				Current_Size++;
			}
		}

		if (Current_Size > PrintCount)
		{
			StatusValue = ((Current_Size / SourceFileSize) * 100);
			printf("Decompress : %12ld %% \r", StatusValue);
			PrintCount += 1024;
		}
	}

	/* Update Status One Last Time */
	StatusValue = ((Current_Size / SourceFileSize) * 100);
	printf("Decompress : %12ld %% \r", StatusValue);

	printf("\n");

	if(CompressSource)fclose(CompressSource);
	if(CompressTarget)fclose(CompressTarget);

	return 1;
}

///////////////////////////////////////////////////////////////////////

