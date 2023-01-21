#include <stdio.h> //printf
#include <unistd.h> //open, close, read, write

//gcc -Wall -Wextra -o foo foo.c
// /foo

int main(void){
    char *buf = "hello world!\n";

    write(1, buf, 14);
}