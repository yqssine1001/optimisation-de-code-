#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void shazam(char *in) {
  int n = strlen(in);
  char *out = malloc(n);
  strcpy(out, in);
  free(out);
}

void bof(char *in) {
  char test;
  if(test < *in)
    puts("Vrai\n");
}

int main(int argc, char **argv) {
  if (argc < 2) return 1;
  shazam(argv[1]);
  puts(argv[1]);
  bof(argv[1]);
  return 0;
}