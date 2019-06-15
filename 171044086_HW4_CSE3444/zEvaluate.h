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


void fifoReading(FILE* filePointer);

#endif
