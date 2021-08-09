/****************************************************************
  parsing.h

  Generic input parsing and error message utilities.
`
  Mark A. Caprio
  University of Notre Dame

  3/10/16 (mac): Extracted from mfdn_h2.
  7/26/16 (mac): Add function StreamCheck.
  10/11/16 (mac): Introduce prototype for ParsingError to header file.
  10/19/16 (mac):
    - OpenCheck: Deprecated in favor of StreamCheck.
    - StreamCheck: Add user-provided error message.
    - ParsingError: Reorder arguments for consistency.
  6/9/17 (mac):
    - Remove deprecated function OpenCheck.
    - Move into namespace mcutils.
  08/11/18 (pjf):
    - Add FileExistCheck.
    - Add GetLine.
  04/03/19 (pjf): Add TokenizeString.

****************************************************************/

#ifndef MCUTILS_PARSING_H_
#define MCUTILS_PARSING_H_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace mcutils
{
  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////


  void StreamCheck(bool success, const std::string& filename, const std::string& message);
  // Check stream status and, if in failure state, terminate with error
  // message indicating stream access failure.
  //
  // Arguments:
  //   success (bool) : access status
  //   filename (string) : file name to use in error messasge
  //   message (string) : general error message
  //
  // The success parameter was meant to be used with automatic
  // conversion from stream to bool, but explicit conversion
  // bool(stream) may be needed (e.g., C++11 under gcc 5) and is thus
  // more portable.
  //
  // Example:
  //   mcutils::StreamCheck(bool(in_stream),in_stream_name,"Failed to open file");
  //   mcutils::StreamCheck(bool(in_stream),in_stream_name,"Failure while writing header");

  void ParsingError(int line_count, const std::string& line, const std::string& message);
  // Generate error message indicating line of input.
  //
  // Limitations: Would ideally also support arguments to give filename
  // and optional supplementary information on expected content.
  //
  // Arguments:
  //   line_count (int) : line count for error message
  //   line (string) : line text for error message
  //   message (string) : general error message


  void ParsingCheck(std::istringstream& line_stream, int line_count, const std::string& line);
  // Provide error message upon stream extraction failure for line of input.
  //
  // Limitations: Would ideally also support arguments to give filename
  // and optional supplementary information on expected content.
  //
  // Arguments:
  //   line_stream (istringstream) : string stream from which parsing was attempted
  //   line_count (int) : line count for error message
  //   line (string) : line text for error message
  //
  // Example:
  //
  //    // scan input file
  //    std::string line;
  //    int line_count = 0;
  //    while ( std::getline(in_stream, line) )
  //    {
  //      // count line
  //      ++line_count;
  //
  //      // set up for parsing
  //      std::istringstream line_stream(line);
  //
  //      // parse line
  //      int a, b, ...;
  //      line_stream >> a >> b >> ...;
  //      mcutils::ParsingCheck(line_stream, line_count, line);
  //
  //      // do stuff with input
  //      ...
  //    }

  bool FileExistCheck(const std::string& filename, bool exit_on_nonexist, bool warn_on_overwrite);
  // Check if file exists, and optionally exit on nonexistence or warn on existence
  //
  // Arguments:
  //   filename (std::string): filename to check existence
  //   exit_on_nonexist (bool): exit with error message if file does not exist
  //   warn_on_overwrite (bool): emit warning message if file will be overwritten

  template<typename CharT, typename Traits, typename Allocator>
  inline std::basic_istream<CharT,Traits>& GetLine(
      std::basic_istream<CharT,Traits>& stream,
      std::basic_string<CharT,Traits,Allocator>& line,
      int& line_count
    )
  // Get next non-blank, non-comment line.
  //
  // Note: This is an almost drop-in replacement for std::getline(), with
  // additional line_count argument.
  //
  // Arguments:
  //   stream (std::ifstream, input): stream to read line from
  //   line (std::string, output): buffer to store into
  //   line_count (int): line number counter
  {
    const std::string whitespace = " \t\r\n\f";
    bool found_line = false;
    while (!found_line && stream.good())
    {
      std::getline(stream, line);
      ++line_count;
      auto start_pos = line.find_first_not_of(whitespace);
      if (!(start_pos == std::string::npos || line[start_pos] == '#'))
      {
        found_line = true;
      }
    }

    return stream;
  }

#if __cplusplus >= 201103L
  /// Read a line from an rvalue stream into a string.
  template<typename CharT, typename Traits, typename Allocator>
  inline std::basic_istream<CharT, Traits>& GetLine(
      std::basic_istream<CharT, Traits>&& stream,
      std::basic_string<CharT, Traits, Allocator>& line,
      int& line_count
    )
  { return GetLine(stream, line, line_count); }
#endif

  std::vector<std::string> TokenizeString(const std::string& str);
  // Break a string of space-separated keywords into separate strings.
  //
  // Ignores any tokens following the comment character '#'.
  //
  // Arguments:
  //   str (std::string): input string
  // Returns:
  //   (std::vector<std::string>): vector of token strings

}  // namespace mcutils


// legacy support for global definitions

#ifdef MCUTILS_ALLOW_LEGACY_GLOBAL
using mcutils::StreamCheck;
using mcutils::ParsingError;
using mcutils::ParsingCheck;
#endif

#endif
