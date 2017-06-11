/****************************************************************
  io.h

  Generic binary I/O helper functions.
`
  Mark A. Caprio
  University of Notre Dame

  6/9/17 (mac): Created.

****************************************************************/

#ifndef MCUTILS_IO_H_
#define MCUTILS_IO_H_

#include <iostream>

namespace mcutils
{
  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////


  template <typename T>
  void WriteBinary(std::ostream& os, T data)
  // Write binary data item to stream.
  {
    os.write(reinterpret_cast<const char*> (&data),sizeof(data));
  }

  template <typename T>
  void ReadBinary(std::istream& is, T &data)
  // Read binary data item from stream.
  {
    is.read(reinterpret_cast<char*> (&data),sizeof(data));
  }

  template <typename T>
  bool VerifyBinary(std::istream& is, T benchmark_data)
  // Read binary data item from stream and verify against benchmark
  // value.
  {
    T data;
    ReadBinary<T>(is,data);
    return (data==benchmark_data);
  }

}  // namespace

#endif
