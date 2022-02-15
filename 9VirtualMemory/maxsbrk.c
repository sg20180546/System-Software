#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    printf("sbrk at %p\n",sbrk(1));
    printf("sbrk at %p\n",sbrk(1));
  void* a= sbrk(1024*1024*512);
  if(!a) printf("sbrk failed\n");
  else printf("memory allocated at %p\n",a);
  printf("sbrk at %p\n",sbrk(1));
  free(a);
  return 0;
}