#include <stdio.h>
int main (void){
  int x = 12;
  float y = 3.14;
  char* s = "hello world!";

  printf("x is %d, y is %f\n%s\n",x,y,s);
  for (int i = 0; i < 5; i++){
    int val = 5*i;
    printf("%d x %d = %d\n", i,5,val);
  }

}