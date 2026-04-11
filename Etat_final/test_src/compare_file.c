#include <stdio.h>
#include "compare_file.h"

int compare_file(const char* filename1, const char* filename2)
{
  FILE *f1; FILE *f2;
  int a1, a2;
  int diff = 0;

  f1 = fopen(filename1, "r");
  f2 = fopen(filename2, "r");
  if (f1 && f2){
    while(!feof(f1) && !feof(f2) && diff==0) {
      a1 = fgetc(f1);
      a2 = fgetc(f2);
      if (a1 != a2) {diff  = 1;}
    }
  } 
  fclose(f1);
  fclose(f2);

 return diff;
}

