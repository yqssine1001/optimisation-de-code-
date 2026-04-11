#!/bin/bash
PROF=/home/m/mounlaur
AFL_PATH=$PROF/AFL/afl-2.52b

make clean
make FUZZ=1 FINAL=1 ASAN=1
# launch the fuzzing campaign (ended by ^C)
echo "run AFL !"
AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES=1 AFL_SKIP_CPUFREQ=1 $AFL_PATH/afl-fuzz -m none -i afl_in/ -o afl_out/ ./bin/main @@