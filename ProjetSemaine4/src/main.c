#include <stdio.h>
#include <stdlib.h>
#include "dico.h"
#include "AllTests.h"
#include "CuTest.h"

#ifndef FINAL
#define FINAL 0
#endif

int main(int argc, char *argv[]) {
#if FINAL
  return monDico(argc, argv);
#else
  RunAllTests();
  return 0;
  #endif
}
