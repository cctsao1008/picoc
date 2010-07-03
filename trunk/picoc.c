#include "picoc.h"

#define CALL_MAIN "main();"


/* initialise everything */
void Initialise()
{
    BasicIOInit();
    HeapInit();
    TableInit();
    VariableInit();
    LexInit();
    TypeInit();
    IncludeInit();
#ifdef BUILTIN_MINI_STDLIB
    LibraryInit(&GlobalTable, "c library", &CLibrary);
    CLibraryInit();
#endif
    PlatformLibraryInit();
}

/* free memory */
void Cleanup()
{
    PlatformCleanup();
    IncludeCleanup();
    ParseCleanup();
    LexCleanup();
    VariableCleanup();
    TypeCleanup();
    TableStrFree();
}

/* platform-dependent code for running programs is in this file */
#ifdef UNIX_HOST
int main(int argc, char **argv)
{
    int ParamCount = 1;
    int DontRunMain = FALSE;
    
    if (argc < 2)
    {
        printf("Format: picoc <csource1.c>...    - run a program (calls main to start it)\n"
               "        picoc -i                 - interactive mode\n"
               "        picoc -m <csource1.c>... - run a program without calling main\n");
        exit(1);
    }
    
    Initialise();
    
    if (strcmp(argv[ParamCount], "-m") == 0)
    {
        DontRunMain = TRUE;
        ParamCount++;
    }
        
    if (strcmp(argv[ParamCount], "-i") == 0)
        ParseInteractive();
    else
    {
        if (PlatformSetExitPoint())
        {
            Cleanup();
            return 1;
        }
        
        for (; ParamCount < argc; ParamCount++)
            PlatformScanFile(argv[ParamCount]);
        
        if (!DontRunMain)
            Parse("startup", CALL_MAIN, strlen(CALL_MAIN), TRUE);
    }
    
    Cleanup();
    return 0;
}
#else
# ifdef SURVEYOR_HOST
int picoc(char *SourceStr)
{    
    int ix;
    
    Initialise();
    if (SourceStr) {
        for (ix=0; ix<strlen(SourceStr); ix++)  /* clear out ctrl-z from XMODEM transfer */
            if (SourceStr[ix] == 0x1A)
                SourceStr[ix] = 0x20;
        /*printf("%s\n\r", SourceStr);*/  /* display program source */
        /*printf("=====================\n");*/
    }
    ExitBuf[40] = 0;
    PlatformSetExitPoint();
    if (ExitBuf[40]) {
        printf("leaving picoC\n\r");
        Cleanup();
        return 1;
    }
        
    if (SourceStr)    
        Parse("test.c", SourceStr, strlen(SourceStr), TRUE);
    ParseInteractive();
    Cleanup();
    return 0;
}
# else
#  ifdef SRV1_UNIX_HOST
/*
 * Howard - this was my guess at what might suit you.
 * Please feel free to change this to whatever type of main function suits you best 
 */
int picoc(char *SourceFile, int Interactive)
{    
    Initialise();
    
    if (Interactive)
        ParseInteractive();
    else
    {
        if (PlatformSetExitPoint())
        {
            Cleanup();
            return 1;
        }
        
        PlatformScanFile(SourceFile);
    }
    
    Cleanup();
    return 0;
}
#  endif
# endif
#endif

