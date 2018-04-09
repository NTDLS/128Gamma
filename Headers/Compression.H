/*

 Compression.H - Compression Definition File

*/

//////////////////////////////////////////////////////////////////////

void PutBit(int);
void FlushBitBuffer(void);
void InitTree(void);
void InsertNode(int);
void DeleteNode(int);
void StartModel(void);
void UpdateModel(int);
void Output(int);
void EncodeChar(int);
void EncodePosition(int);
void EncodeEnd(void);
void StartDecode(void);
void Initialize(void);

int BinarySearchSym(unsigned int);
int BinarySearchPos(unsigned int);
int GetBit(void);
int DecodeChar(void);
int DecodePosition(void);
int Compress(char *, char *);
int Decompress(char *, char *);

//////////////////////////////////////////////////////////////////////

FILE *CompressSource, *CompressTarget;

/* Size Of Ring Buffer */
#define SizeOfRingBuffer 4096

/* Upper Limit For MatchLength */
#define MatchLengthLimit 60

/* Encode String Into Position And Length
    If MatchLength Is Greater Than This */
#define THRESHOLD        2

/* Index For Root Of Binary Search Trees */
#define NIL SizeOfRingBuffer

/* Ring Buffer Of Size SizeOfRingBuffer, With Extra MatchLengthLimit-1
    Bytes To Facilitate String Comparison */
unsigned char  TextBuffer[SizeOfRingBuffer + MatchLengthLimit - 1];

/* Set By The InsertNode() Procedure. */
int MatchPosition;

/* MatchLength Of Longest Match. Set By The InsertNode() Procedure. */
int MatchLength;

/* Left Child. Set By The InsertNode() Procedure. */
int LChild[SizeOfRingBuffer + 1];

/* Right Child. Set By The InsertNode() Procedure. */
int RChild[SizeOfRingBuffer + 257];

/* Parent. Set By The InsertNode() Procedure. */
int Parent[SizeOfRingBuffer + 1];

/* Used And Set By PutBit() Procedure */
unsigned int PutBitBuffer = 0, PutBitMask = 128;

/* Used And Set By GetBit() Procedure */
unsigned int GetBitBuffer = 0, GetBitMask = 0;

///////////////////////////////////////////////////////////////////////
/********** Arithmetic Compression **********/

#define M   15

/* Q1 ( = 2 To The M) Must Be Sufficiently Large, But Not So
    Large As The Unsigned Long 4 * Q1 * (Q1 - 1) OverFlows.  */

#define Q1  (1UL << M)
#define Q2  (2 * Q1)
#define Q3  (3 * Q1)
#define Q4  (4 * Q1)
#define MAX_CUM (Q1 - 1)

/* Character Code = 0, 1, ..., N_CHAR - 1 */
#define N_CHAR  (256 - THRESHOLD + MatchLengthLimit)

unsigned long int Low = 0, High = Q4, Value = 0;

    /* Counts For Magnifying Low And High Around Q2 */
int Shifts = 0;

int Character2Symbol[N_CHAR], Symbol2Character[N_CHAR + 1];

    /* Frequency For Symbols */
unsigned int SymbolsFrequency[N_CHAR + 1];

    /* Cumulative Frequency For Symbols */
unsigned int CumulativeSymbols[N_CHAR + 1];

    /* Cumulative Frequency For Positions */
unsigned int CumulativePosition[SizeOfRingBuffer + 1];

//////////////////////////////////////////////////////////////////////

