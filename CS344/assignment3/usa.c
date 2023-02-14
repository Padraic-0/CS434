
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
  while (1){
    char s[2048];
    char *words[128];
    printf("USA ");

    fflush(stdout);

    fgets(s, sizeof s, stdin);

    if ((words[0] = strtok(s, " \t\n\r")) != NULL ) {
      int i = 0;
      do {
        //printf("Token: \"%s\"\n", words[i]);
        i ++;
      } while ((words[i] = strtok(NULL, " \t\n\r")) != NULL);
    }

    //execvp(words[0], words);

    if (strcmp(words[0], "cd") == 0){
      //if (words[1] == NULL){
      //  chdir(getenv("HOME"));
      //}
      if (chdir(words[1]) == -1) {
        perror("failed to cd");
      }

    } else if (strcmp(words[0], "exit") == 0){
      exit(0);
    } else {

      pid_t pid = fork();

      if (pid == 0){
        execvp(words[0], words);
        perror("failed to execvp");
        exit(1);
      } else {
        wait(NULL);
      }
    }
  }
}