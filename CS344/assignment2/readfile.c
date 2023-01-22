#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){

  if (argc == 1){
    perror("No file specified");
    return 0;
  }

  char buffer[2048];
  int file = open(argv[1],O_RDONLY);

  if (file == -1){
    perror("File not opened");
    return 0;
  }

  int size = read(file, buffer, sizeof(buffer));
  write(1,buffer,size);

  close(file);
  
}