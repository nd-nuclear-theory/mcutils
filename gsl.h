/****************************************************************
  gsl.h                       

  Defines wrappers for gsl functions.
         
  Language: C++

  Mark A. Caprio
  University of Notre Dame

  - 3/26/17 (mac): Created, based on code from spncci utilities (aem).

****************************************************************/

#ifndef MCUTILS_GSL_H_
#define MCUTILS_GSL_H_

#include "gsl/gsl_sf.h"

namespace mcutils {

  // GSL factorial wrappers
  //
  // Use more efficient int implementation for int arguments, else use
  // generic gamma function for double arguments.

  inline double Factorial(int x)
  {
    return gsl_sf_fact(x);
  }

  inline double Factorial(double x)
  {
    return gsl_sf_gamma(x+1);
  }

  // GSL combinatorial function wrappers

  inline int Choose(int x, int y)
  {
    int choose=0;
    if ((x>=y)&&(y>=0))
      {
        gsl_sf_result result;
        gsl_sf_choose_e(x,y,&result);
        choose=result.val;
      }
    return choose;
  }

} // namespace


#endif
