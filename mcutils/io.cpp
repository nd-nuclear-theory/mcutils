/****************************************************************
  io.cpp

  Patrick J. Fasano
  University of Notre Dame

****************************************************************/

#include "io.h"

#include <iostream>

namespace mcutils
{
////////////////////////////////////////////////////////////////
// I/O mode deduction
////////////////////////////////////////////////////////////////

IOMode DeducedIOMode(const std::string& filename)
{
  if (filename.length() < 4)
  {
    // prevent compare on underlength string
    std::cerr << "File I/O: No extension found (too short) in filename "
              << filename << std::endl;
    exit(EXIT_FAILURE);
  }
  else if (!filename.compare(filename.length() - 4, 4, ".dat"))
    return IOMode::kText;
  else if (!filename.compare(filename.length() - 4, 4, ".bin"))
    return IOMode::kBinary;
  else
  {
    std::cerr << "File I/O: Extension unrecognized in filename " << filename
              << std::endl;
    exit(EXIT_FAILURE);
  }
}
};  // namespace mcutils
