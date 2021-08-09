/****************************************************************
  parsing.cpp

  Mark A. Caprio
  University of Notre Dame

****************************************************************/

#include "parsing.h"

#include <sys/stat.h>
#include <cstdlib>
#include <utility>

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
    std::cerr << "ERROR: " << message << std::endl;
    std::cerr << "Input line " << line_count << ": " << line  << std::endl;
    std::exit(EXIT_FAILURE);
  }

  void ParsingCheck(std::istringstream& line_stream, int line_count, const std::string& line)
  {
    if (!line_stream)
      ParsingError(line_count,line,"Failed parsing line (missing or incorrect arguments)");
  }

  bool FileExistCheck(const std::string& filename, bool exit_on_nonexist, bool warn_on_overwrite)
  {
    struct stat st;
    bool file_exists = (stat(filename.c_str(), &st) == 0);
    if (!file_exists && exit_on_nonexist) {
        std::cerr << "ERROR: file " << filename << " does not exist!" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if (file_exists && warn_on_overwrite) {
      std::cerr << "WARN: overwriting file " << filename << std::endl;
    }

    return file_exists;
  }

  std::vector<std::string> TokenizeString(const std::string& str)
  {
    std::vector<std::string> tokens;
    std::istringstream str_stream(str);
    while (str_stream.good())
    {
      std::string temp_str;
      str_stream >> temp_str;
      if ((temp_str.size()==0) || (temp_str.at(0)=='#')) break;
      tokens.push_back(std::move(temp_str));
    }
    return std::move(tokens);
  }

}  // namespace mcutils
