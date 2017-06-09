/****************************************************************
  parsing.cpp

  Mark A. Caprio
  University of Notre Dame

****************************************************************/

#include "parsing.h"

#include <cstdlib>

namespace mcutils
{
  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////

  void StreamCheck(bool success, const std::string& filename, const std::string& message)
  {
    if (!success)
      {
        std::cerr << std::endl;
        std::cerr << "File access failure: " << filename << std::endl;
        std::cerr << message << std::endl;
        std::exit(EXIT_FAILURE);
      }
  }

  void OpenCheck(bool success, const std::string& filename)
  {
    // if (!success)
    //   {
    //     std::cerr << std::endl;
    //     std::cerr << "Failed to open file: " << filename << std::endl;
    //     std::exit(EXIT_FAILURE);
    //   }
    StreamCheck(success,filename, "Failed to open file");
  }

  void ParsingError(int line_count, const std::string& line, const std::string& message)
  {
    std::cerr << std::endl;
    std::cerr << message << std::endl;
    std::cerr << "Input line " << line_count << ": " << line  << std::endl;
    std::exit(EXIT_FAILURE);
  }

  void ParsingCheck(std::istringstream& line_stream, int line_count, const std::string& line)
  {
    if (!line_stream)
      ParsingError(line_count,line,"Failed parsing line (missing or incorrect arguments)");
  }

}  // namespace
