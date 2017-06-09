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

****************************************************************/

#ifndef PARSING_H_
#define PARSING_H_

#include <iostream>
#include <sstream>
#include <string>

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
  //   StreamCheck(bool(in_stream),in_stream_name,"Failed to open file");
  //   StreamCheck(bool(in_stream),in_stream_name,"Failure while writing header");

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
  //      ParsingCheck(line_stream, line_count, line);
  //  
  //      // do stuff with input
  //      ...
  //    }

}  // namespace


// legacy support for global definitions

#ifdef MCUTILS_ALLOW_LEGACY_GLOBAL
using mcutils::StreamCheck;
using mcutils::ParsingError;
using mcutils::ParsingCheck;
#endif

#endif
