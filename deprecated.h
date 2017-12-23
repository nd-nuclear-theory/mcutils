/****************************************************************
  deprecated.h

  Define deprecation macro.

  Patrick J. Fasano
  University of Notre Dame

  - 08/11/17 (pjf): Created.

****************************************************************/
#ifndef MCUTILS_DEPRECATED_H_
#define MCUTILS_DEPRECATED_H_

// borrowed from http://en.cppreference.com/w/cpp/experimental/feature_test
#ifdef __has_cpp_attribute                      // Check if __has_cpp_attribute is present
#  if __has_cpp_attribute(deprecated)           // Check for an attribute
#    define DEPRECATED(msg) [[deprecated(msg)]]
#  endif
#endif
#ifndef DEPRECATED
#    define DEPRECATED(msg)
#endif


#endif  // MCUTILS_DEPRECATED_H_
