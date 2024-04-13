/****************************************************************
  fortran_io.h

  Fortran unformatted binary I/O helper functions.
`
  Patrick J. Fasano
  University of Notre Dame

  + 07/18/22 (pjf): Created.

****************************************************************/

#ifndef MCUTILS_FORTRAN_IO_H_
#define MCUTILS_FORTRAN_IO_H_

#include <iostream>
#include <string>
#include <vector>

#include "io.h"

namespace mcutils
{
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

// code assumes int and float are both 4-byte
constexpr int kIntegerSize = 4;
static_assert(sizeof(int) == kIntegerSize, "Integers are not 4 bytes.");
// Fortran allows a maximum record length of (2**31-1) minus the bytes
// for record overhead
constexpr std::size_t kMaxRecordLength =
    (std::size_t)2147483647 - 2 * kIntegerSize;

template<
    typename T,
    typename tDataType = typename T::value_type,
    decltype(std::declval<T>().size())* = nullptr,
    decltype(std::declval<T>().data())* = nullptr
  >
void WriteFortranRecord(std::ostream& os, const T& data)
// Write unformatted Fortran record to stream.
//
// The `data` argument can be any `std::vector`-like object which provides
// `size()` and `data()` accessors, and a `::value_type` typedef.
//
// Arguments:
//   os (input): binary stream for output
//   data (input): data value to output
//
// Ex:
//   mcutils::WriteFortranRecord(out_stream,vec);
{
  // check for too much data
  if (static_cast<std::size_t>(data.size()) * sizeof(tDataType) > kMaxRecordLength)
    throw std::length_error("tried to write too-long Fortran record");

  // write beginning delimiter
  WriteBinary<int32_t>(os, data.size() * sizeof(tDataType));
  // write actual data
  WriteBinary<tDataType>(os, data.data(), data.size());
  // write ending delimiter
  WriteBinary<int32_t>(os, data.size() * sizeof(tDataType));
}

template<typename tDataType>
std::vector<tDataType> ReadFortranRecord(std::istream& is)
// Read unformatted Fortran record from stream.
//
// Arguments:
//   is (input): binary stream for input
//   data (output): data value read from stream
//
// Ex:
//   mcutils::ReadFortranRecord(in_stream,vec);
{
  // throw exceptions on read errors
  auto exceptions = is.exceptions();
  is.exceptions(exceptions | std::istream::failbit);

  // get record size
  int32_t record_size;
  ReadBinary<int32_t>(is, record_size);
  if (record_size < 0)
    throw std::length_error("cannot read Fortran subrecords");
  if (record_size%sizeof(tDataType) != 0)
    throw std::runtime_error("record size is not integer multiple of data type size");

  // convert record size to number of elements, and resize storage
  std::size_t count = record_size / sizeof(tDataType);
  std::vector<tDataType> data(count);

  // read record data into output
  ReadBinary<tDataType>(is, data.data(), count);

  // verify ending delimiter
  VerifyBinary<int32_t>(
      is, record_size, "Unmatched Fortran record closing delimiter", "record delimiter"
    );

  // return stream exception flags to original state
  is.exceptions(exceptions);

  return data;
}

void SkipFortranRecord(std::istream& is)
// Skip unformatted Fortran record in stream.
//
// Arguments:
//   is (input): binary stream for input
//
// Example:
//   mcutils::SkipFortranRecord(in_stream);
{
  // throw exceptions on read errors
  auto exceptions = is.exceptions();
  is.exceptions(exceptions | std::istream::failbit);

  // get record size
  int32_t record_size;
  ReadBinary<int32_t>(is, record_size);
  if (record_size < 0)
    throw std::length_error("cannot read Fortran subrecords");

  // move file pointer ahead
  is.seekg(record_size, std::ios_base::cur);

  // verify ending delimiter
  VerifyBinary<int32_t>(
      is, record_size, "Unmatched Fortran record closing delimiter", "record delimiter"
    );

  // return stream exception flags to original state
  is.exceptions(exceptions);
}

}  // namespace mcutils

#endif  // MCUTILS_FORTRAN_IO_H_
