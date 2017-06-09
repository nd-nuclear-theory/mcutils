/****************************************************************
  profiling_test.cpp

  Mark A. Caprio
  University of Notre Dame

****************************************************************/

#include <iostream>

#include "profiling.h"

int main(int argc, char **argv)
{
  
  const int iterations = 10000000;

  mcutils::Timer timer;
  float x=0;
  for (int i=0; i<iterations; ++i) {x+=0.1;};
  timer.Stop();
  std::cout << x << std::endl;  // needed to avoid loop being optimized away!
  
  std::cout << "(Time: " << timer.ElapsedTime() << ")" << std::endl;

  timer.Start();
  for (int i=0; i<iterations; ++i) {x+=0.1;};
  timer.Stop();
  std::cout << x << std::endl;  // needed to avoid loop being optimized away!

  std::cout << "(Time: " << timer.ElapsedTime() << ")" << std::endl;

  // termination
  std::exit(EXIT_SUCCESS);
}
