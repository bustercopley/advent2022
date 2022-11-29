# Advent Of Code 2022

Solutions for Advent Of Code 2022, in C++

# Prerequisites

* GNU Make
* GCC g++ (or LLVM clang++ with libstdc++)
* PCRE2
* Curl

# Building and running

* `make test TARGET=01`

    * Downloads the puzzle input https://adventofcode.com/2022/day/1/input
      into the file "input/01" if that file does not already exist, using
      the session cookie in the file ".cookie".

    * Compiles "src/01.cpp" and runs it.
      It is the executable's responsibility to read the input file "input/01"
      and print the solution on standard output.

* `make debug CONFIG=debug TARGET=01`

    * Downloads the input, compiles "src/01.cpp" with debug flags, runs it
      under gdb in batch mode, prints a backtrace, and quits.

# Licensing

This project is licensed under the MIT License. See LICENSE.txt for details.
