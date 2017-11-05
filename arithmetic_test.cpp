/******************************************************************************
  
  Created by M. A. Caprio on 2/17/11.
  
******************************************************************************/

#include "arithmetic.h"

#include <iostream>

int main(int argc, char **argv)
{

  std::cout << "sqr" << std::endl;
  std::cout << mcutils::sqr(2) << " " << mcutils::sqr(2.1) << std::endl;
  std::cout << mcutils::sqr<int>(2) << " " << mcutils::sqr<double>(2.1) << std::endl;  // or with explicit typenames

  std::cout << "KroneckerDelta & UnitStep" << std::endl;
  for (int i=-2; i<=2; ++i)
    std::cout << i << " " << mcutils::KroneckerDelta(i,0) << " " << mcutils::UnitStep(i) << std::endl;

  // termination
  return 0;
}
