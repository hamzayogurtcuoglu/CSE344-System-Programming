#ifndef EVALUATEZ
#define EVALUATEZ


#include <fcntl.h>
#include <sys/types.h>
#include <sys/file.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h> 
#include <unistd.h>
#include <dirent.h> 


int evaluateZParameter(FILE* fp,char []);
void processNumber(FILE* filePointer);

#endif
