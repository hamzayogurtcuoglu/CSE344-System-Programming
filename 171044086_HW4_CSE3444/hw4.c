

//Hamza Yogurtcuoglu_171044086
//31.03.2019 9:45
//Create a new process (fork) for each directory to get the sizes.
//Each processes intercommunicate by pipe
//Main Process is reading fifo for all result.

/*############################################################################*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*----------------------------------------------------------------------------*/
/*------------------------------Includes--------------------------------------*/
/*----------------------------------------------------------------------------*/

#define FIFO_NAME "/tmp/171044086sizes"  // Debian doesn't create fifo in shared memory 
#include "zEvaluate.h"	// Fifo Reading 			
#include <signal.h> 

#define maximumNumber 4096

void fifoReading(FILE* fp);
int postOrderApply (char *path, int pathfun (char *path1));
int sizepathfun (char *path);

int zFlag = 0;


// Signal handling for finishing processes
// Signal function is done according to me
// Because of late announce

void signal_function(int signal) 
{ 
    printf("\n PROCESS(%d PID) :  ARE STOPPED\n",getpid()); 
    //process exit according to signal.
    exit(signal);
}

int main(int argc, char const *argv[])
{
	signal(SIGINT, signal_function); 
	int ret = mkfifo(FIFO_NAME, 0666);
	if (ret<0)
	{
		printf("YOU DID'T CREATE A FIFO\n" );
		unlink(FIFO_NAME);
		exit(0);
	}


	/*Usage*/
	if(argc != 2 && argc != 3){
		fprintf(stderr, "buNeDuFPF: uses a depth-first search strategy to display the sizes of the subdirectories\n");
		fprintf(stderr, "Usage: %s argument(-z) or path if you use argument third that is path \n", argv[0]);
	return -1;
	}

	if (argc == 2 || (strcmp(argv[1],"-z") == 0 && argc == 3))
	{
		//Cleaning the file if it has text 

		//if every time end of file is append .
		FILE* fp = fopen(FIFO_NAME,"a+");
		
		if(fp != NULL)
		{
			printf("PID\tSIZE\t\tPATH\n");
		
			//fflush(fp);
			
			pid_t pid = fork();
			
			if (pid == 0)
			{	
				char* dirPathName;
				
				if ((strcmp(argv[1],"-z") == 0 && argc == 3))
				{
					zFlag = 1;

					dirPathName = (char*)malloc(sizeof(char)*(strlen(argv[2])+1));
					
					strcpy(dirPathName,argv[2]);
				}else{
					
					dirPathName = (char*)malloc(sizeof(char)*(strlen(argv[1])+1));
				
					strcpy(dirPathName,argv[1]);
				}

				int result =  postOrderApply(dirPathName, sizepathfun);
				
				fprintf(fp,"%d\t%d\t\t%s\n",getpid(),result,dirPathName );
				fclose(fp);
				free(dirPathName);
				//exit the child
				unlink(FIFO_NAME);
				exit(0);
			}
			else
			{
				fclose(fp);
				fp = fopen(FIFO_NAME,"r");
				//Reading
				fifoReading(fp);
			}
			fclose(fp);
		}else{
			fprintf(stderr, "buNeDuFPF: uses a depth-first search strategy to display the sizes of the subdirectories\n");
			fprintf(stderr, "Usage: %s argument(-z) or path if you use argument third that is path \n", argv[0]);
		}
	}else{
		fprintf(stderr, "buNeDuFPF: uses a depth-first search strategy to display the sizes of the subdirectories\n");
		fprintf(stderr, "Usage: %s argument(-z) or path if you use argument third that is path \n", argv[0]);
	}
	unlink(FIFO_NAME);
	return 0;
}
	
//Uses a depth-first search strategy to display the sizes of the subdirectories

//Each processes intercommunicate by pipe
int postOrderApply (char *path, int pathfun (char *path1))
{
	int pipeNumber = 0;
	int directory_size = 0;
	DIR *d = opendir(path);
	FILE* fp = fopen(FIFO_NAME,"a+");
    
	if(fp != NULL)
	{	int pipefd[maximumNumber][2]; //pipe 
		pid_t pid;
		int x = 0;

		if (d == 0)
		{	//if item can not open will return 0
			return 0;
		}
		char filePath[maximumNumber];
		struct dirent *dir;
		int directorySize = 0;
		
		while ((dir = readdir(d)) != NULL)
		{
			if ((strcmp(dir->d_name, ".") == 0) || (strcmp(dir->d_name, "..") == 0))
			{
				continue;
			}
			//Integrate the for next directory or files ...
			sprintf(filePath, "%s/%s", path, dir->d_name);

			//PIPE CREATING
			if (dir->d_type == DT_DIR)
			{	
				if (pipe(pipefd[pipeNumber]) == -1) {
               		perror("YOU DID'T CREATE A PIPE");
               		exit(EXIT_FAILURE);
     			}
     			pipeNumber++; // PIPENUMBER KEEPING 

				pid = fork();
				 
				if (pid >= 0)
				{
					if (pid == 0)
					{	//Integrate the for next directory or files ...
						sprintf(filePath,"%s/%s", path, dir->d_name);
						closedir(d); // closing the opening directory
						//if path is directory recursive continue...
						directorySize = postOrderApply(filePath, pathfun)  ;
						if (zFlag != 0 )
						{	

							//PIPE NOT READING JUST WRITING
							fflush(fp);
							close(pipefd[pipeNumber-1][0]); // JUST READING
							write(pipefd[pipeNumber-1][1],&directorySize,sizeof(int));
							close(pipefd[pipeNumber-1][1]); //JUST WRITING

							fprintf(fp,"%d\t%d\t\t%s\n",getpid(),directorySize,filePath );
								
						}else{

							fprintf(fp,"%d\t%d\t\t%s\n",getpid(),directorySize,filePath );
						}

						fflush(fp);
						fclose(fp);
						exit(0);
					}
				}			
			}
			else
			{

				if(dir->d_type == DT_REG){
						directory_size += pathfun(filePath);
				}
				if(dir->d_type == DT_FIFO || dir->d_type == DT_LNK)
				{
						fprintf(fp,"%d\t%d\t\tSpecial_file_%s\n",getpid(),-1,dir->d_name);
						fflush(fp);					
				}
			}
		}
	
		int readingPipeValue = 0; // BUFFER FOR READING PIPE
		while(wait(&x) > 0);
		if (zFlag != 0) // IF PROGRAM HAS '-z' PARAMETER
		{	
			int pipeNum = 0;
			close(pipefd[pipeNum][1]);
			//PIPE NOT WRITING JUST READING
			while(read(pipefd[pipeNum][0], &readingPipeValue, sizeof(int)) != -1 ) {
				
				close(pipefd[pipeNum][0]); // JUST READING
				directory_size += readingPipeValue;	
				pipeNum++;
				//IF PIPE NUMBER EQUAL WHILE LOOP BREAK CALLED
				if (pipeNumber == pipeNum) 
						break;
				close(pipefd[pipeNum][1]); // JUST WRITING
			}
		}
		fclose(fp);  // closing the opening file pointer
		closedir(d); // closing the opening directory
	}
	return directory_size;
}

/*The function outputs path along with other information obtained by calling stat for path .
The function returns the size in blocks of the file given by path or -1 if path does not corresponds to an
ordinary file.
*/
int sizepathfun (char *path)
{
	struct stat status;
	int size = 0;
	lstat(path, &status);
	//-1 if path does not corresponds to an ordinary file.
	if (!(S_ISREG(status.st_mode)||S_ISDIR(status.st_mode) ))
	{
		return -1 ;

	}else{
		// Size of directory or file are returned.
		size = status.st_size;
		return size/1024;
	} 
}
/*############################################################################*/
/*          				 End of HW04    				                  */
/*############################################################################*/