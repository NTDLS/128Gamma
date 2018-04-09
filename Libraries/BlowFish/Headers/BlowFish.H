#define NumberOfRounds 16

unsigned long P[18];
unsigned long S[4][256];

unsigned long BF_F(unsigned long x);
void BF_BlowFish(unsigned char *key, int keyLen);
void BF_Encrypt(unsigned long *xL, unsigned long *xR);
void BF_Decrypt(unsigned long *xL, unsigned long *xR);
int  BF_Interface(char *,char *, char *, int);

