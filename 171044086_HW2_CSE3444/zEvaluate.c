


//Hamza Yogurtcuoglu_171044086
//23.03.2019 22:42
// Create a new process (fork) for each directory to get the sizes.
/*############################################################################*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*----------------------------------------------------------------------------*/
/*------------------------------Includes--------------------------------------*/
/*----------------------------------------------------------------------------*/


#include "zEvaluate.h"


int evaluateZParameter(FILE* fp , char  currentDirectory []){

	fseek(fp,strlen("PID\tSIZE\t\tPATH\n"),SEEK_SET);
	char *buffer = (char*)malloc(sizeof(char)*4096);
	size_t size = 4096;
	
	char  pidArray1[4096],pidArray2[4096],pidArray3[4096];
	
	int subTotal = 0 ; 
	
	while(getline(&buffer,&size,fp) >= 0)
	{	
		int subSize = 0;
		int subFlag = 1; 
		sscanf(buffer,"%s %s %s",pidArray1,pidArray2,pidArray3);

	
		if (!(strcmp(pidArray3,"file")==0&&strcmp(pidArray2,"Special")==0) )
		{
			int i = 0;
			for ( ; i < strlen(currentDirectory); ++i)
			 {
				if (!(currentDirectory[i] == pidArray3 [i]))
				{
					subFlag = 0;
				}

			 } 
			 int slashControl = 1;
			 for (int j = i; j < strlen(pidArray3); ++j)
			 {
			 	if (pidArray3[j] == '/')
			 	{
			 		slashControl ++ ;
			 	}
			 }


			 if (subFlag && slashControl == 2)
			 {
			 	sscanf(pidArray2,"%d",&subSize);
			 	subTotal += subSize;
			 }
		}
	}
	free(buffer);
	return subTotal; 
}

void processNumber(FILE* filePointer){
	fseek(filePointer,strlen("PID\tSIZE\t\tPATH\n"),SEEK_SET);
	char *buffer = (char*)malloc(sizeof(char)*4096);
	size_t size = 4096;
	int fileLineNumber = 0;
	while(getline(&buffer,&size,filePointer) >= 0)
	{
		++fileLineNumber;
	}
	int allArray[fileLineNumber];
	fseek(filePointer,strlen("PID\tSIZE\t\tPATH\n"),SEEK_SET);
	int i = 0;
	while(getline(&buffer,&size,filePointer) >= 0)
	{
		sscanf(buffer,"%d",&allArray[i]);
		++i;
	}
	int processNumber = 0;
	i = 0;
	for (; i<fileLineNumber; i++) 
	{ 
		int j; 
		for (j=0; j<i; j++) 
			if (allArray[i] == allArray[j]) 
				break; 
		if (i == j) 
			processNumber++;
	} 
	free(buffer);
	fprintf(filePointer,"%d child processes created. Main process is %d.",processNumber, getpid() );
	printf("%d child processes created. Main process is %d.\n",processNumber, getpid() );

}

/*############################################################################*/
/*          				 End of HW02    				                  */
/*############################################################################*/

