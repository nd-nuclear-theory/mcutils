/****************************************************************
  eigen_test.cpp

  Mark A. Caprio, Patrick J. Fasano
  University of Notre Dame

****************************************************************/

#include <iostream>

#include "eigen.h"

void const_test(const Eigen::MatrixXd& matrix)
{
  std::cout << mcutils::ChopMatrix(matrix) << std::endl;
}

int main(int argc, char **argv)
{
  Eigen::MatrixXd matrix = Eigen::MatrixXd::Zero(4,4);
  matrix(0,0) = 1.;
  matrix(1,0) = 1.e-12;

  std::cout << "  non-const test:" << std::endl;
  std::cout << matrix << std::endl;
  std::cout << mcutils::NonzerosInMatrix(matrix) << std::endl;
  std::cout << mcutils::ChopMatrix(matrix) << std::endl;
  std::cout << matrix << std::endl;

  matrix(0,1) = 1.e-12;
  std::cout << "  const test:" << std::endl;
  std::cout << matrix << std::endl;
  const_test(matrix);
  std::cout << matrix << std::endl;

  // termination
  std::exit(EXIT_SUCCESS);
}
