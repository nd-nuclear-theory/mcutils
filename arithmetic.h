/****************************************************************
  arithmetic.h

  Defines arithmetic shorthands.

  Created by Mark A. Caprio on 2/17/11.
  Drawing upon libmc/mcutils C code.

  - 2/23/11 (mac): Rename from mc_arithmetic to arithmetic.
  - 3/26/17 (mac): Import KroneckerDelta from spncci's utilities (aem).
  - 6/9/17 (mac): Move into namespace mcutils.
  - 11/5/17 (mac): Add UnitStep based on acm (wcl).
  - 09/24/21 (pjf): Make functions constexpr.

****************************************************************/

#ifndef MCUTILS_ARITHMETIC_H_
#define MCUTILS_ARITHMETIC_H_

// ONLYIF(cond,x) evaluates and returns x only if cond is true
#define ONLYIF(cond,x) ( (cond) ? (x) : 0)

namespace mcutils
{
  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////

  template <typename T>
    inline constexpr
    T sqr(const T& x)
    // sqr(x) returns the arithmetic square of x by self-multiplication
    //   Note: Use of inline template avoids double evaluation of x which
    //   would occur in a macro implementation.
    {
      return x*x;
    }

  template <typename T>
    inline constexpr
    int KroneckerDelta(const T& x, const T& y)
    // Calculate Kronecker delta of variables x and y.
    //
    // That is, returns 1 if x==y, 0 otherwise.
    {
      return (x==y) ? 1 : 0;
    }

  template <typename T>
    inline constexpr
    int UnitStep(const T& x)
    // Calculate unit step function.
    //
    // That is, returns 1 if x>=0, 0 otherwise.
    {
      return (x>=0) ? 1 : 0;
    }


}  // namespace


// legacy support for global definitions

#ifdef MCUTILS_ALLOW_LEGACY_GLOBAL
using mcutils::sqr;
using mcutils::KroneckerDelta;
#endif

#endif
