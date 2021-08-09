/****************************************************************
  eigen.h

  Matrix utilities for use with Eigen template library.
`
  Requires: Eigen, cppformat

  Mark A. Caprio
  University of Notre Dame

  + 11/13/16 (mac): Created, with code from shell project (h2mixer).
  + 01/26/17 (aem): Add IsZero matrix comparison function.
  + 02/17/17 (mac): Add row prefix option to FormatMatrix.
  + 02/27/17 (mac): Add NonzerosInMatrix nonzero counting function.
  + 07/05/19 (pjf): Return const reference from ChopMatrix to be able to
    use in expressions like `std::cout << ChopMatrix(matrix)`.
  + 04/09/21 (pjf): Add overload of ChopMatrix which takes const reference
    and returns a chopped copy.

****************************************************************/

#ifndef MCUTILS_EIGEN_H_
#define MCUTILS_EIGEN_H_

#include <iostream>
#include <sstream>
#include <string>

#include "eigen3/Eigen/Dense"
#include "fmt/format.h"

namespace mcutils
{
  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////
  // matrix arithmetic
  ////////////////////////////////////////////////////////////////

  template<typename tMatrixType>
    const tMatrixType& ChopMatrix(tMatrixType& matrix, double tolerance=1e-10)
    // Round near-zero entries in matrix to zero.
    //
    // Default tolerance value is same as Mathematica Chop function's.
    //
    // Template arguments:
    //   tMatrixType: Eigen matrix type
    //
    // Arguments:
    //   matrix (tMatrixType): matrix to chop
    //   tolerance (double, optional): truncation tolerance
    //
    // Returns:
    //   (const ref to tMatrixType): reference to chopped matrix
    {
      for (int i=0; i<matrix.rows(); ++i)
        for (int j=0; j<matrix.cols(); ++j)
          // Caution: Important to use std::abs() rather than integer abs().
          if (std::abs(matrix(i,j))<tolerance)
            matrix(i,j) = 0.;

      return matrix;
    }

  template<typename tMatrixType>
    tMatrixType ChopMatrix(const tMatrixType& matrix, double tolerance=1e-10)
    // Round near-zero entries in matrix to zero.
    //
    // Default tolerance value is same as Mathematica Chop function's.
    //
    // Template arguments:
    //   tMatrixType: Eigen matrix type
    //
    // Arguments:
    //   matrix (tMatrixType): matrix to chop
    //   tolerance (double, optional): truncation tolerance
    //
    // Returns:
    //   (const ref to tMatrixType): reference to chopped matrix
    {
      tMatrixType temp = matrix;
      ChopMatrix(temp, tolerance);
      return temp;
    }

  template<typename tMatrixType>
    int NonzerosInMatrix(tMatrixType& matrix, double tolerance=1e-10)
    // Count nonzero entries in matrix, rounding near-zero entries to zero.
    //
    // Default tolerance value is same as Mathematica Chop function's.
    //
    // Template arguments:
    //   tMatrixType: Eigen matrix type
    //
    // Arguments:
    //   matrix (tMatrixType): matrix to chop
    //   tolerance (double, optional): truncation tolerance
    {
      int nonzero_entries = 0;
      for (int i=0; i<matrix.rows(); ++i)
        for (int j=0; j<matrix.cols(); ++j)
          // Caution: Important to use std::abs() rather than integer abs().
          if (not (std::abs(matrix(i,j))<tolerance))
            ++nonzero_entries;
      return nonzero_entries;
    }

  template<typename tMatrixType>
    bool IsZero(tMatrixType& matrix,double tolerance=1e-10)
    // Check if absolute value of all matrix elements are less than tolerance
    //
    // Template arguments:
    //   tMatrixType: Eigen matrix type
    //
    // Arguments:
    //   matrix (tMatrixType): matrix to check if zero matrix
    //   tolerance (double, optional): truncation tolerance
    {
      int rows=matrix.rows();
      int cols=matrix.cols();
      for(int j=0; j<cols; ++j)
        for(int i=0; i<rows; ++i)
          {
            if(fabs(matrix(i,j))>tolerance)
                return false;
          }
      return true;
    }


  template<typename tMatrixType>
    void CompleteLowerTriangle(tMatrixType& matrix)
    // Populate lower triangle of matrix as mirror of upper triangle.
    //
    // Template arguments:
    //   tMatrixType: Eigen matrix type
    //
    // Arguments:
    //   matrix (tMatrixType): matrix to complete
    {
      for (int i=0; i<matrix.rows(); ++i)
        for (int j=0; j<matrix.cols(); ++j)
          if (i>j)
            matrix(i,j) = matrix(j,i);
    }

  ////////////////////////////////////////////////////////////////
  // matrix formatting
  ////////////////////////////////////////////////////////////////


  template<typename tMatrixType>
    std::string FormatMatrix(const tMatrixType& matrix, const std::string& format_string, const std::string& prefix_string="")
    // Format matrix with given Python-style format string.
    //
    // Example:
    //
    //   std::cout << FormatMatrix(matrix,"+.8e") <<std::endl;
    //
    // Template arguments:
    //   tMatrixType: Eigen matrix type
    //
    // Arguments:
    //   matrix (tMatrixType): matrix to chop
    //   format_string (std:string): Python-style format string
    {
      std::ostringstream os;

      // formatting configuration (loosely matching Eigen::IOFormat)
      const std::string entry_separator(" ");
      const std::string row_separator("\n");
      const std::string row_prefix(prefix_string);
      const std::string row_suffix("");
      const std::string matrix_prefix("");
      const std::string matrix_suffix("");

      const std::string full_format_string = "{:"+format_string+"}";

      os << matrix_prefix;
      // for each row
      for (int i=0; i<matrix.rows(); ++i)
        {
          os << row_prefix;
          // for each column
          for (int j=0; j<matrix.cols(); ++j)
            {
              os << fmt::format(full_format_string,matrix(i,j));
              if (j+1<matrix.cols())
                os << entry_separator;
            }
          os << row_suffix;
          if (i+1<matrix.rows())
            os << row_separator;
        }
      os << matrix_suffix;

      return os.str();
    }

  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////
} // namespace

#endif
