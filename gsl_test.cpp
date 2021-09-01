/****************************************************************
  gsl_test.cpp

  Mark A. Caprio
  University of Notre Dame

****************************************************************/  

#include <iostream>

#include <gsl.h>

int main(int argc, char **argv)
{
  
  std::cout << mcutils::Factorial(2) << " " << mcutils::Factorial(2.5) << std::endl;
  
  // termination
  return 0;
}
