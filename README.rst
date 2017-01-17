================================================================================
                   Superoptimizer for AVX512F popcount
================================================================================

Warning: this is a pre early draft.


How it works?
--------------------------------------------------

The input to the superoptimizer is a semantic description of
instructions from an assembly program. The description is accompanied by
binary opcodes. The binary form was obtained directly from the compiled
binary by cutting output of ``objdump -d``.

The superoptimizer tries to generate a permutation of the input,
preserving all constraints. It starts from **the working program** and
in each iteration swaps two random instruction. If such a modified
program doesn't violate the constraints it is saved on disc in binary
form.

Then a runner script gets the generated binary and **patches** exe
program, then run this program and record its output. According to the
result, it classifies a patch into valid or invalid category. The script
avoids testing the same patches.

The brilliant idea of patching exe comes from Nate Kurz.


How to run?
--------------------------------------------------

The superoptimizer is split into two programs:

* **avx512popcnt** --- permutes assembly input and produce binary dumps (files);
  it tries to produce correct program, applying several constraints.

* **runner.py** --- consumes files created by avx512popcnt, patch
  compiled program, run it, and record output.

These two programs have to be run in separate sessions. First run
``runner.py``, which create all needed directories, then run
``avx512popcnt`` which will generate programs.

Warning: both applications run an infinite loop, they need to be killed
manually (Ctrl-C should be enough.)
