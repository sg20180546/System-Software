#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void program_break_test() {
  printf("%10p\n", sbrk(0));

  char *bl = malloc(131* 1024);//2*20, 1024KB
    printf("malloc'd at: %10p\n", bl);
  printf("sbrk : %10p\n", sbrk(0));
  printf("free : %p\n",bl);
  free(bl);
  printf("sbrk : %10p\n", sbrk(0));
}

int main(int argc, char **argv) {
  program_break_test();

  return 0;
}