

// Hamza Yogurtcuoglu_171044086
// 23.03.2019 22:42
// Create a new process (fork) for each directory to get the sizes.
/*############################################################################*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*----------------------------------------------------------------------------*/
/*------------------------------Includes--------------------------------------*/
/*----------------------------------------------------------------------------*/


#include "zEvaluate.h"

void processNumber(FILE* fp);
int postOrderApply (char *path, int pathfun (char *path1));
int sizepathfun (char *path);

int zFlag = 0;

int main(int argc, char const *argv[])
{

	/*Usage*/
	if(argc != 2 && argc != 3){
		fprintf(stderr, "buNeDu: uses a depth-first search strategy to display the sizes of the subdirectories\n");
		fprintf(stderr, "Usage: %s argument(-z) or path if you use argument third that is path \n", argv[0]);
	return -1;
	}

	if (argc == 2 )
	{
		zFlag = 0;
		//Cleaning the file if it has text 
		remove("171044086sizes.txt");
		//if every time end of file is append . 
		FILE* fp = fopen("171044086sizes.txt","a+");
		if(fp)
		{
			fprintf(fp,"PID\tSIZE\t\tPATH\n");
			
			printf("PID\tSIZE\t\tPATH\n");
		
			fflush(fp);
			
			pid_t pid = fork();
			
			if (pid == 0)
			{
				char* dirPathName = (char*)malloc(sizeof(char)*(strlen(argv[1])+1));
				
				strcpy(dirPathName,argv[1]);
				
				int result =  postOrderApply(dirPathName, sizepathfun);
				
				fprintf(fp,"%d\t%d\t\t%s\n",getpid(),result,dirPathName );
				
				printf("%d\t%d\t\t%s\n",getpid(),result,dirPathName );
				fclose(fp);
				free(dirPathName);
				//exit the child
				exit(0);
			}
			else
			{
				wait(NULL);
				processNumber(fp);
			}
			fclose(fp);
		}
	}
	else if ( strcmp(argv[1],"-z") == 0 && argc == 3)
	{	//according to '-z' parameter calling postOrderApply Function
		zFlag = 1;
		//Cleaning the file if it has text 
		remove("171044086sizes.txt");
		//if every time end of file is append . 
		FILE* fp = fopen("171044086sizes.txt","a+");
		if(fp)
		{	
			fprintf(fp,"PID\tSIZE\t\tPATH\n");
			printf("PID\tSIZE\t\tPATH\n");
			fflush(fp);
			//Creating child
			pid_t pid = fork();
			if (pid == 0)
			{	//Child open the directory
				char* dirPathName = (char*)malloc(sizeof(char)*(strlen(argv[2])+1));
				
				strcpy(dirPathName,argv[2]);
				// child return total size
				int total_size = postOrderApply(dirPathName, sizepathfun);
				// '-z' parameter includes subdirectory of path 
				total_size += evaluateZParameter(fp ,dirPathName);
				
				fprintf(fp,"%d\t%d\t\t%s\n",getpid(), total_size  ,dirPathName  );
				
				printf("%d\t%d\t\t%s\n",getpid(), total_size  ,dirPathName  );
				fclose(fp);
				free(dirPathName);
				//exit the child
				exit(0);
			}
			else
			{	//Parent is waiting for its children
				wait(NULL);
				processNumber(fp);
			}
			fclose(fp);
		}
	}
	

	return 0;
}


//Uses a depth-first search strategy to display the sizes of the subdirectories
int postOrderApply (char *path, int pathfun (char *path1))
{
	int directory_size = 0;
	DIR *d = opendir(path);
	FILE* fp = fopen("171044086sizes.txt","a+");
	int fl = flock(fileno(fp),LOCK_SH);
	
	if(fp)
	{
		fseek(fp,0,SEEK_END);
		pid_t pid;
		int x = 0;

		if (d == 0)
		{	//if item can not open will return 0
			return 0;
		}

		char filePath[4096];
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

			if (dir->d_type == DT_DIR)
			{
				pid = fork();
				if (pid >= 0)
				{
					if (pid == 0)
					{	//Integrate the for next directory or files ...
						sprintf(filePath,"%s/%s", path, dir->d_name);
						closedir(d); // closing the opening directory
						//if path is directory recursive continue...
						directorySize = postOrderApply(filePath, pathfun)  ;
						
						if(fl == 0)
						{	
							fl = flock(fileno(fp),LOCK_UN);
							if (zFlag){

								directorySize += evaluateZParameter(fp , filePath);
								fseek(fp,0,SEEK_END);
								fprintf(fp,"%d\t%d\t\t%s\n",getpid(),directorySize,filePath );
								printf("%d\t%d\t\t%s\n",getpid(),directorySize,filePath );
							}else{

								fprintf(fp,"%d\t%d\t\t%s\n",getpid(),directorySize,filePath );
								printf("%d\t%d\t\t%s\n",getpid(),directorySize,filePath );
							}
							
							fl = flock(fileno(fp),LOCK_SH);
							fflush(fp);
						}		
						exit(0);
					}
				}
				else
				{
					wait(NULL); 
				}				
			}
			else
			{

				if(dir->d_type == DT_REG){
						directory_size += pathfun(filePath);
				}
				if(dir->d_type == DT_FIFO || dir->d_type == DT_LNK)
				{
					if(fl == 0)
					{	
						fl = flock(fileno(fp),LOCK_UN);
						// Special file not adding to total size 
						fprintf(fp,"%d\t\t\tSpecial file %s\n",getpid(),dir->d_name);
						// Special file not adding to total size 
						printf("%d\t\t\tSpecial file %s\n",getpid(),dir->d_name);
						
						fl = flock(fileno(fp),LOCK_SH);
						fflush(fp);
					}
				}
			}
		}
		while(wait(&x) > 0);
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
/*          				 End of HW02    				                  */
/*############################################################################*/

