//Created by Padraic Bergin
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char *argv[]){

  char* directory;

  if (argc > 1){
    directory = argv[1];
  }
  else{
    char* cwd;
    char buff[128];
    directory = getcwd(buff, sizeof(buff));
  }
  
  //printf("%s", directory);
  //fflush(stdout);

  int pfd[2];
  
  
  pipe(pfd);

  pid_t pid = fork();

  if(pid == 0){
    dup2(pfd[0],0);
    close(pfd[1]);
    execlp("wc", "wc", "-l", (char*) NULL);
  }
  else{
    dup2(pfd[1],1);
    close(pfd[0]);
    execlp("ls", "ls", "-1a", directory, (char*) NULL);
  }

  return 0;
}