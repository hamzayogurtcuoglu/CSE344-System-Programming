#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <dirent.h> 


int postOrderApply (char *path, int pathfun (char *path1)); //Uses a depth-first search strategy to display the sizes of the subdirectories

int sizepathfun (char *path); //The function returns the size in blocks of the file


int zFlag = 0;

int main(int argc, char const *argv[])
{
  char * beginFilePath;

  char * beginDirectoryORParamater;

   /*Usage*/
  if(argc != 2 && argc != 3){
    fprintf(stderr, "buNeDu: uses a depth-first search strategy to display the sizes of the subdirectories\n");
    fprintf(stderr, "Usage: %s argument(-z) or path if you use argument third that is path \n", argv[0]);
    return -1;
  }
  
  if (argv[1] !=NULL)
  {
    beginDirectoryORParamater = malloc((1+strlen(argv[1])) * sizeof(char));
    strcpy(beginDirectoryORParamater, argv[1]);    
  }
  if (argv[2] != NULL)
  { 
    beginFilePath = malloc((strlen(argv[2])+1) * sizeof(char));
    strcpy(beginFilePath, argv[2]);
  }

   //according to '-z' parameter calling postOrderApply Function

  if (!strcmp(beginDirectoryORParamater,"-z"))
  {
    zFlag = 1;
    int b = postOrderApply(beginFilePath, sizepathfun);    
    if (b != -1)
        printf("%d\t     %s\n",b,beginFilePath);
    else
        printf("NO SUCH A FILE OR DIRECTORY! \n");  

  } // there is no '-z' paramter calling this part
  else{
    int a = postOrderApply(beginDirectoryORParamater, sizepathfun);
    if (a != -1)
        printf("%d\t     %s\n",a,beginDirectoryORParamater);
    else
        printf("NO SUCH A FILE OR DIRECTORY! \n");  
    beginFilePath = NULL;  
  }
    
  if (beginFilePath != NULL)
      free(beginFilePath);
   
  if (beginDirectoryORParamater !=NULL)
     free(beginDirectoryORParamater);
  
  return 0;
}
//Uses a depth-first search strategy to display the sizes of the subdirectories
int postOrderApply (char *path, int pathfun (char *path1))
{
   DIR *d = opendir(path);
   struct dirent *dir;
   int total_size = 0;
   char filePath[4096]; //Maximum size of path in Unix system.
   struct stat buf;
   int x;
  //if item can not open will return -1 
  if (d == NULL)
  {
    return -1;
  
  }else{
         while ((dir = readdir(d)) != NULL)
     {
      if ( (strcmp(dir->d_name, ".") == 0) 
        || (strcmp(dir->d_name, "..") == 0))
       continue;
      //Integrate the for next directory or files ...
      sprintf(filePath, "%s/%s", path, dir->d_name);
      x = lstat (filePath, &buf);
      //if path is directory recursive continue...
      if (S_ISDIR(buf.st_mode)){
           int dir_size = postOrderApply(filePath, pathfun) + pathfun(path)/1024 - 4; //KB type of size
           printf("%d\t     ", ((dir_size))); 
           printf("%s\n", filePath);
           //if bunedu has a '-z' argument adding all total size or not
           if (zFlag == 1)
           {
             total_size += dir_size; 
           }      
      } // just adding if path is file
      else if(S_ISREG(buf.st_mode)){
       total_size += pathfun(filePath)/1024;
      }else{ // Special file not adding to total size 
        printf("Special file  %s\n",dir->d_name);
        pathfun(filePath);
      }
     }
    }
   closedir(d); // closing the opening directory
   return total_size;
}

/*The function outputs path along with other information obtained by calling stat for path .
The function returns the size in blocks of the file given by path or -1 if path does not corresponds to an
ordinary file.
*/
int sizepathfun (char *path)
{  
   struct stat status;
   struct dirent *dir;
   int size = 0;
   lstat(path, &status);
   //-1 if path does not corresponds to an ordinary file.
   if (!(S_ISREG(status.st_mode)||S_ISDIR(status.st_mode) ))
    {
      return -1 ;
    
    }else{
      // Size of directory or file are returned.
      size = status.st_size;
      return size;
    } 
}