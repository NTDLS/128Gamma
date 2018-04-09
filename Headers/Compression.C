/* Gamma Data Compression (LZARI)

 Compression.C - Compression Function File

void PutBit(int bit)
void FlushBitBuffer(void) 
int GetBit(void)  
void InitTree(void) 
void InsertNode(int r)
void DeleteNode(int p) 
void StartModel(void) 
void UpdateModel(int sym)
void Output(int bit)  
void EncodeChar(int ch)
void EncodePosition(int position)
void EncodeEnd(void)
int BinarySearchSym(unsigned int x)
int BinarySearchPos(unsigned int x)
void StartDecode(void)
int DecodeChar(void)
int DecodePosition(void)
int Compress(char *SourceFileName, char *TargetFileName)
int Decompress(char *SourceFileName, char *TargetFileName)
void Initialize(void)

*/


#include "Headers/Compression.H"

///////////////////////////////////////////////////////////////////////

void PutBit(int bit)  /* Output One Bit (Bit = 0,1) */
{
	if (bit)
		PutBitBuffer |= PutBitMask;
	if ((PutBitMask >>= 1) == 0)
	{
		if( putc(PutBitBuffer, CompressTarget) == EOF )
			Error("Compression: Bit Output. Error 7.");
		PutBitBuffer = 0;
		PutBitMask = 128;
	}
}

///////////////////////////////////////////////////////////////////////

void FlushBitBuffer(void)  /* Send remaining bits */
{
	int LocalLoop = 0;
	for (LocalLoop = 0; LocalLoop < 7; LocalLoop++)
		PutBit(0);
}

///////////////////////////////////////////////////////////////////////

int GetBit(void)  /* Get one bit (0 or 1) */
{
	if ((GetBitMask >>= 1) == 0)
	{
		GetBitBuffer = getc(CompressSource);
		GetBitMask = 128;
	}
	return( (GetBitBuffer & GetBitMask) != 0);
}

///////////////////////////////////////////////////////////////////////

/********** Multiple Binary Trees **********/

///////////////////////////////////////////////////////////////////////

void InitTree(void)  /* Initialize Trees */
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
	for (LocalLoop = SizeOfRingBuffer + 1; LocalLoop <= SizeOfRingBuffer + 256; LocalLoop++)
		RChild[LocalLoop] = NIL;

	/* Node */
	for (LocalLoop = 0; LocalLoop < SizeOfRingBuffer; LocalLoop++)
		Parent[LocalLoop] = NIL;
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

	for (;;)
	{
		if (Cmp >= 0)
		{
			if (RChild[Value_B] != NIL) Value_B = RChild[Value_B];
			else {
				RChild[Value_B] = r;
				Parent[r] = Value_B;
				return;
			}
		}
		else {
			if (LChild[Value_B] != NIL)
			{
				Value_B = LChild[Value_B];
			}
			else {
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

	if(RChild[Parent[Value_B]] == Value_B)
		RChild[Parent[Value_B]] = r;
			else LChild[Parent[Value_B]] = r;

	Parent[Value_B] = NIL;	/* Remove Value_B */
}

///////////////////////////////////////////////////////////////////////

void DeleteNode(int p)	/* Delete node p from tree */
{
	int  Value_A;

	if (Parent[p] == NIL)
	{
		return;  /* Not In Tree */
	}

	if (RChild[p] == NIL)
	{
		Value_A = LChild[p];
	}
	else if (LChild[p] == NIL) {
		 Value_A = RChild[p];
	}
	else {
		Value_A = LChild[p];
		if (RChild[Value_A] != NIL)
		{
			do{
				Value_A = RChild[Value_A];
			} while (RChild[Value_A] != NIL);

			RChild[Parent[Value_A]] = LChild[Value_A];
			Parent[LChild[Value_A]] = Parent[Value_A];
			LChild[Value_A] = LChild[p];
			Parent[LChild[p]] = Value_A;
		}
		RChild[Value_A] = RChild[p];
		Parent[RChild[p]] = Value_A;
	}

	Parent[Value_A] = Parent[p];

	if (RChild[Parent[p]] == p)
		RChild[Parent[p]] = Value_A;
			else LChild[Parent[p]] = Value_A;

	Parent[p] = NIL;
}

///////////////////////////////////////////////////////////////////////
/********** Arithmetic Compression **********/

void StartModel(void)  /* Initialize model */
{
	int ch, sym, i;

	CumulativeSymbols[N_CHAR] = 0;
	for (sym = N_CHAR; sym >= 1; sym--)
	{
		ch = sym - 1;
		Character2Symbol[ch] = sym;
		Symbol2Character[sym] = ch;
		SymbolsFrequency[sym] = 1;
		CumulativeSymbols[sym - 1] = CumulativeSymbols[sym] + SymbolsFrequency[sym];
	}

	SymbolsFrequency[0] = 0;  /* sentinel (!= SymbolsFrequency[1]) */
	CumulativePosition[SizeOfRingBuffer] = 0;

	for (i = SizeOfRingBuffer; i >= 1; i--)
		CumulativePosition[i - 1] = CumulativePosition[i] + 10000 / (i + 200);

	/* Empirical Distribution Function (Quite Tentative) */
	/* Please Devise A Better Mechanism! */
}

///////////////////////////////////////////////////////////////////////

void UpdateModel(int sym)
{
	int i, c, ch_i, ch_sym;

	if (CumulativeSymbols[0] >= MAX_CUM)
	{
		c = 0;
		for (i = N_CHAR; i > 0; i--)
		{
			CumulativeSymbols[i] = c;
			c += (SymbolsFrequency[i] = (SymbolsFrequency[i] + 1) >> 1);
		}
		CumulativeSymbols[0] = c;
	}

	for (i = sym; SymbolsFrequency[i] == SymbolsFrequency[i - 1]; i--);

	if (i < sym)
	{
		ch_i = Symbol2Character[i];
		ch_sym = Symbol2Character[sym];
		Symbol2Character[i] = ch_sym;
		Symbol2Character[sym] = ch_i;
		Character2Symbol[ch_i] = sym;
		Character2Symbol[ch_sym] = i;
	}

	SymbolsFrequency[i]++;

	while (--i >= 0)
		CumulativeSymbols[i]++;
}

///////////////////////////////////////////////////////////////////////

void Output(int bit)  /* Output 1 Bit, Followed By Its Complements */
{
	PutBit(bit);
	for ( ; Shifts > 0; Shifts--)
		PutBit(! bit);
}

///////////////////////////////////////////////////////////////////////

void EncodeChar(int ch)
{
	int  sym;
	unsigned long int  range;

	sym = Character2Symbol[ch];
	range = High - Low;
	High = Low + (range * CumulativeSymbols[sym - 1]) / CumulativeSymbols[0];
	Low +=		 (range * CumulativeSymbols[sym    ]) / CumulativeSymbols[0];
	for (;;)
	{
		if (High <= Q2)
			Output(0);
		else if (Low >= Q2)
		{
			Output(1);
			Low -= Q2;
			High -= Q2;
		}
		else if (Low >= Q1 && High <= Q3)
		{
			Shifts++;
			Low -= Q1;
			High -= Q1;
		}
		else break;
		Low += Low;
		High += High;
	}
	UpdateModel(sym);
}

///////////////////////////////////////////////////////////////////////

void EncodePosition(int position)
{
	unsigned long int  range;

	range = High - Low;
	High = Low + (range * CumulativePosition[position	 ]) / CumulativePosition[0];
	Low +=		 (range * CumulativePosition[position + 1]) / CumulativePosition[0];
	for (;;)
	{
		if (High <= Q2) Output(0);
		else if (Low >= Q2)
		{
			Output(1);
			Low -= Q2;
			High -= Q2;
		}
		else if (Low >= Q1 && High <= Q3)
		{
			Shifts++;
			Low -= Q1;
			High -= Q1;
		}
		else break;
		Low += Low;
		High += High;
	}
}

///////////////////////////////////////////////////////////////////////

void EncodeEnd(void)
{
	Shifts++;
	if (Low < Q1)
		Output(0);
			else Output(1);

	FlushBitBuffer();  /* flush bits remaining in buffer */
}

///////////////////////////////////////////////////////////////////////
/* 1	  if x >= CumulativeSymbols[1],
N_CHAR if CumulativeSymbols[N_CHAR] > x,
i such that CumulativeSymbols[i - 1] > x >= CumulativeSymbols[i] otherwise */

int BinarySearchSym(unsigned int x)
{
	int i, j, k;

	i = 1;
	j = N_CHAR;
	while (i < j)
	{
		k = (i + j) / 2;
		if (CumulativeSymbols[k] > x)
			i = k + 1;
				else j = k;
	}
	return i;
}

///////////////////////////////////////////////////////////////////////
/* 0 if x >= CumulativePosition[1],
SizeOfRingBuffer - 1 if CumulativePosition[SizeOfRingBuffer] > x,
i such that CumulativePosition[i] > x >= CumulativePosition[i + 1] otherwise */

int BinarySearchPos(unsigned int x)
{
	int i, j, k;

	i = 1;
	j = SizeOfRingBuffer;
	while (i < j)
	{
		k = (i + j) / 2;
		if (CumulativePosition[k] > x)
			i = k + 1;
				else j = k;
	}
	return i - 1;
}

///////////////////////////////////////////////////////////////////////

void StartDecode(void)
{
	int i;
	for (i = 0; i < M + 2; i++)
		Value = 2 * Value + GetBit();
}

///////////////////////////////////////////////////////////////////////

int DecodeChar(void)
{
	int  sym, ch;
	unsigned long int  range;

	range = High - Low;
	sym = BinarySearchSym( (unsigned int) (((Value - Low + 1) * CumulativeSymbols[0] - 1) / range));
	High = Low + (range * CumulativeSymbols[sym - 1]) / CumulativeSymbols[0];
	Low +=		 (range * CumulativeSymbols[sym    ]) / CumulativeSymbols[0];

	for (;;)
	{
		if (Low >= Q2)
		{
			Value -= Q2;
			Low -= Q2;
			High -= Q2;
		}
		else if (Low >= Q1 && High <= Q3)
		{
			Value -= Q1;
			Low -= Q1;
			High -= Q1;
		}
		else if (High > Q2) break;

		Low += Low;
		High += High;
		Value = 2 * Value + GetBit();
	}

	ch = Symbol2Character[sym];
	UpdateModel(sym);
	return ch;
}

///////////////////////////////////////////////////////////////////////

int DecodePosition(void)
{
	int position;
	unsigned long int  range;

	range = High - Low;
	position = BinarySearchPos((unsigned int) (((Value - Low + 1) * CumulativePosition[0] - 1) / range));
	High = Low + (range * CumulativePosition[position	 ]) / CumulativePosition[0];
	Low +=		 (range * CumulativePosition[position + 1]) / CumulativePosition[0];
	for ( ; ; )
	{
		if (Low >= Q2)
		{
			Value -= Q2;
			Low -= Q2;
			High -= Q2;
		}
		else if (Low >= Q1 && High <= Q3)
		{
			Value -= Q1;
			Low -= Q1;
			High -= Q1;
		}
		else if (High > Q2) break;

		Low += Low;
		High += High;
		Value = 2 * Value + GetBit();
	}
	return position;
}

///////////////////////////////////////////////////////////////////////
/********** Compress and Decompress **********/

int Compress(char *SourceFileName, char *TargetFileName)
{
	Initialize();

	int  i=0, c=0, len=0, r=0, s=0, Last_MatchLength=0;
	unsigned long int PrintCount = 0, CodeSize = 0, TextSize = 0;

	DWORD SourceFileSize = 0;
	long double Current_Size = 0;

	if((CompressSource = fopen(SourceFileName,"rb")) == NULL)
	{
		if(CompressSource)fclose(CompressSource);
		Error("Compression: Could Not Open Source File. Error 8.");
		return 0;
	}

	if((CompressTarget = fopen(TargetFileName,"wb")) == NULL)
	{
		if(CompressTarget)fclose(CompressTarget);
		Error("Compression: Could Not Open Target File. Error 9.");
		return 0;
	}

	fseek(CompressSource, 0L, SEEK_END);
	TextSize = ftell(CompressSource);
	SourceFileSize = TextSize;

	/* output size of text */
	if(fwrite(&TextSize, sizeof TextSize, 1, CompressTarget) < 1)
	{
		Error("Compression: File Write Error. Error 10.");
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
			if(CancelFlag)
			{
				if(CompressSource)fclose(CompressSource);
				if(CompressTarget)fclose(CompressTarget);
				CleanUp(0,1);
				return 0;
			}
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

	if(CompressSource)fclose(CompressSource);
	if(CompressTarget)fclose(CompressTarget);
	return 1;
}

///////////////////////////////////////////////////////////////////////

int Decompress(char *SourceFileName, char *TargetFileName)
{
	Initialize();

	int  i=0, j=0, k=0, r=0, c=0;

	unsigned long int PrintCount = 0, TextSize = 0;

	DWORD SourceFileSize = 0;
	long double Current_Size = 0;

	if((CompressSource = fopen(SourceFileName,"rb")) == NULL)
	{
		if(CompressSource)fclose(CompressSource);
		Error("Compression: Could Not Open Source File. Error 11.");
		return 0;
	}
	if((CompressTarget = fopen(TargetFileName,"wb")) == NULL)
	{
		if(CompressTarget)fclose(CompressTarget);
		Error("Compression: Could Not Open Target File. Error 12.");
		return 0;
	}

	/* read size of text */
	if(fread(&TextSize, sizeof TextSize, 1, CompressSource) < 1)
		Error("Compression: File Read Error. Error 13.");

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
			if(CancelFlag)
			{
				if(CompressSource)fclose(CompressSource);
				if(CompressTarget)fclose(CompressTarget);
				CleanUp(0,1);
				return 0;
			}
			PrintCount += 1024;
		}
	}

	/* Update Status One Last Time */
	StatusValue = ((Current_Size / SourceFileSize) * 100);

	if(CompressSource)fclose(CompressSource);
	if(CompressTarget)fclose(CompressTarget);

	return 1;
}

///////////////////////////////////////////////////////////////////////

void Initialize(void)
{
	if(CompressSource)fclose(CompressSource);
	if(CompressTarget)fclose(CompressTarget);

	CompressSource = 0;
	CompressTarget = 0;

	PutBitBuffer = 0;
	PutBitMask = 128;

	GetBitBuffer = 0;
	GetBitMask = 0;

	Low = 0;
	High = Q4;
	Value = 0;
	Shifts = 0;

	DWORD LocalClearLoop = 0;


	MatchPosition = 0;
	MatchLength = 0;

	while(LocalClearLoop != SizeOfRingBuffer + MatchLengthLimit - 1)
	{
		TextBuffer[LocalClearLoop] = '\0';
		LocalClearLoop++;
	}LocalClearLoop = 0;
	while(LocalClearLoop != SizeOfRingBuffer + 1)
	{
		LChild[LocalClearLoop] = 0;
		LocalClearLoop++;
	}LocalClearLoop = 0;
	while(LocalClearLoop != SizeOfRingBuffer + 257)
	{
		RChild[LocalClearLoop] = 0;
		LocalClearLoop++;
	}LocalClearLoop = 0;
	while(LocalClearLoop != SizeOfRingBuffer + 1)
	{
		Parent[LocalClearLoop] = 0;
		LocalClearLoop++;
	}LocalClearLoop = 0;
	while(LocalClearLoop != N_CHAR)
	{
		Character2Symbol[LocalClearLoop] = 0;
		LocalClearLoop++;
	}LocalClearLoop = 0;
	while(LocalClearLoop != N_CHAR + 1)
	{
		Symbol2Character[LocalClearLoop] = 0;
		LocalClearLoop++;
	}LocalClearLoop = 0;
	while(LocalClearLoop != N_CHAR + 1)
	{
		SymbolsFrequency[LocalClearLoop] = 0;
		LocalClearLoop++;
	}LocalClearLoop = 0;
	while(LocalClearLoop != N_CHAR + 1)
	{
		CumulativeSymbols[LocalClearLoop] = 0;
		LocalClearLoop++;
	}LocalClearLoop = 0;
	while(LocalClearLoop != SizeOfRingBuffer + 1)
	{
		CumulativePosition[LocalClearLoop] = 0;
		LocalClearLoop++;
	}
}

///////////////////////////////////////////////////////////////////////

