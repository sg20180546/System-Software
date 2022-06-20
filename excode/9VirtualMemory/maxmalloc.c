#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  void* a= malloc(sizeof(long)*1024*1024*1024*3);
  if(!a) printf("malloc failed\n");
  else printf("memory allocated\n");
  return 0;
}