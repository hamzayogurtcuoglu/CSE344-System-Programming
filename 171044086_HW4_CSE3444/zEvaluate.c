


//Hamza Yogurtcuoglu_171044086
//31.03.2019 09:45
// Create a new process (fork) for each directory to get the sizes.
//Each processes intercommunicate by pipe
//Main Process is reading fifo for all result.
//
/*############################################################################*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*----------------------------------------------------------------------------*/
/*------------------------------Includes--------------------------------------*/
/*----------------------------------------------------------------------------*/

#define maximumNumber 4096
#include "zEvaluate.h"

//FIFO READING FUNCTION
void fifoReading(FILE* filePointer){
	char *buffer = (char*)malloc(sizeof(char)*maximumNumber);
	int allArray[maximumNumber];
	size_t size = maximumNumber;
	int fileLineNumber = 0;
	// FIFO READING LOOP
	while(getline(&buffer,&size,filePointer) >= 0)
	{	printf("%s",buffer );
		sscanf(buffer,"%d",&allArray[fileLineNumber]);
		++fileLineNumber;
	}

	//PROCESS NUMBER IS COUNTED ----------- 
	int i = 0;	
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
	printf("%d child processes created. Main process is %d.\n",processNumber, getpid() );
	//PROCESS NUMBER IS COUNTED -----------
}

/*############################################################################*/
/*          				 End of HW04    				                  */
/*############################################################################*/

