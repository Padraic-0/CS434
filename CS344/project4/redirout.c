//Created by Padraic Bergin
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){

  if (argc < 3){
    perror("Expecting 2+ arguments, got less");
    exit(0);
  }
  char** command = argv + 2;
  int fd = open(argv[1], O_CREAT|O_WRONLY, 0644);

  int pfd[2];
  
  
  pipe(pfd);

  pid_t pid = fork();

  if(pid == 0){
    dup2(pfd[1],1);
    close(pfd[0]);
    execvp(command[0], command);
  }
  else{
    close(pfd[1]);
    char buffer[2056];
    int bytes_read = read(pfd[0],buffer,sizeof(buffer));
    write(fd, buffer, bytes_read);
    close(fd);
  }

  return 0;
}