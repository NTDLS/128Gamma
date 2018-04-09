int CreateKey(char *);

char *FileName = "C:\\Public.Txt";

int CreateKey(char *xInput)
{
    FILE *LocalTarget;

    char *EncryptedString;
    DWORD StringLength;

    StringLength = strlen(xInput);
    EncryptedString = EncryptString(xInput,StringLength);

    /* Open Target File */
    if((LocalTarget = fopen(FileName,"w")) == NULL){
        printf("Could Not Open %s",FileName);
        return 0;
    }

    if(StringLength < 10)fprintf(LocalTarget,"%d",0);
    fprintf(LocalTarget,"%s",EncryptedString);
    if(LocalTarget)fclose(LocalTarget);

    return 0;
}
