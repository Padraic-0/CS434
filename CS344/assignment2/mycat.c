#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char *argv[]){

  char* buffer[2048];
  char* newline = "\n";

  if (argc == 1){
    while (1){
      int size = read(0,buffer,sizeof(buffer));
      write(1,buffer,size);
    }
  }

  for (int i = 1; i < argc; i++){
    int file = open(argv[i],O_RDONLY);
    if (file == -1){
      perror("File not opened");
      return 0;
    }
    int size = 1;
    while(size > 0){
    size = read(file,buffer,sizeof(buffer));
    write(1,buffer,size);
    }
    write(1,newline,1);
    close(file);
  }




  return 0;
}