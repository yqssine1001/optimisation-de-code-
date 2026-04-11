#include <stdio.h>
#include <stdlib.h>

#include "CuTest.h"
#include "AllTests.h"

CuSuite *MaTestSuite();

void RunAllTests (void){
  CuString *output = CuStringNew ();
  CuSuite *suite = CuSuiteNew ();
  CuSuite *maSuite = MaTestSuite ();

  CuSuiteAddSuite (suite, maSuite);

  CuSuiteRun (suite);
  CuSuiteSummary (suite, output);
  CuSuiteDetails (suite, output);
  printf ("%s\n", output->buffer);
  CuSuiteDelete (suite);
  free (maSuite);
  CuStringDelete (output);
}
