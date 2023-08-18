# mcutils installation guide #

Mark A. Caprio  
Department of Physics, University of Notre Dame
Patrick J. Fasano  
Physics Division, Argonne National Laboratory

+ 08/17/2023 (pjf): Created, modified from `am/INSTALL.md`.

----------------------------------------------------------------

# 1. Standalone compilation and installation (optional)

Configure the project using CMake using:

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  % cmake -B build/ .
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Some supporting modules (`am`, `Eigen`, `gsl`, and `fmt`) must be installed or
otherwise be made visible to CMake.

To compile the library itself:

  ~~~~~~~~~~~~~~~~
  % cmake --build build/
  ~~~~~~~~~~~~~~~~

If you have not installed the dependencies, but, e.g., only built them, you may
need to tell CMake where to find the built-but-not-installed libraries:

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cmake -B build . \
      -Dam_DIR=~/code/am/build \
      -Dfmt_DIR=~/code/fmt/build
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To compile the test codes:

  ~~~~~~~~~~~~~~~~
  % cmake --build build/ -- tests
  ~~~~~~~~~~~~~~~~

These test codes can then be run:

  ~~~~~~~~~~~~~~~~
  % ./build/arithmetic_test
  % ./build/eigen_test
  % ./build/halfint_test
  % ./build/gsl_test
  % ./build/io_test
  % ./build/memoizer_test
  % ./build/profiling_test
  % ./build/vector_tuple_test
  ~~~~~~~~~~~~~~~~

To install the library (here with prefix `~/install`):
  ~~~~~~~~~~~~~~~~
  % cmake --install build/ --prefix ~/install
  ~~~~~~~~~~~~~~~~
