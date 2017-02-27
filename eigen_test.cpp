/****************************************************************
  eigen_test.cpp

  Mark A. Caprio
  University of Notre Dame

****************************************************************/

#include <iostream>

#include "eigen.h"

int main(int argc, char **argv)
{
  Eigen::MatrixXd matrix = Eigen::MatrixXd::Zero(4,4);
  matrix(0,0) = 1.;

  std::cout << matrix << std::endl;
  std::cout << mcutils::NonzerosInMatrix(matrix) << std::endl;

  // termination
  std::exit(EXIT_SUCCESS);
}
