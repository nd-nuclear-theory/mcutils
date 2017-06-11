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
#include <string>

namespace mcutils
{
  ////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////


  template <typename tDataType>
    void WriteBinary(std::ostream& os, tDataType data)
    // Write binary data item to stream.
    //
    // Note that, if the template parameter is omitted, the data type
    // of the value given for data will determine the output type, but
    // explicitly giving the template parameter casts the data to the
    // given data type tDataType.  Explicitly giving the template
    // parameter is recommended both to document the data format in
    // the output file and to avoid any ambiguity of the output type.
    //
    // Arguments:
    //   os (input): binary stream for output
    //   data (input): data value to output
    //
    // Ex:
    //   mcutils::WriteBinary<float>(out_stream,value);

    {
      os.write(reinterpret_cast<const char*> (&data),sizeof(data));
    }

  template <typename tDataType>
    void ReadBinary(std::istream& is, tDataType &data)
    // Read binary data item from stream.
    //
    // Arguments:
    //   is (input): binary stream for input
    //   data (output): data value read from stream
    //
    // Ex:
    //   mcutils::ReadBinary<float>(in_stream,value);
    {
      is.read(reinterpret_cast<char*> (&data),sizeof(data));
    }

  template <typename tDataType>
    void VerifyBinary(
        std::istream& is, tDataType benchmark_data,
        const std::string& message, const std::string& item_name
      )
    // Read binary data item from stream and verify against benchmark
    // value.
    //
    // Arguments:
    //   is (input): binary stream for input
    //   benchmark_data (input): expected data value
    //   message (input): text for general error message describing context of error
    //   item_name (input): text describing meaning of value being checked
    //
    // Ex:
    //   mcutils::VerifyBinary<int>(
    //       in_stream,bytes,
    //       "Encountered unexpected value in data file","record delimiter"
    //     );

    {
      tDataType data;
      ReadBinary<tDataType>(is,data);
      if (data!=benchmark_data)
        {
          std::cerr << std::endl;
          std::cerr << message << std::endl;
          std::cerr
            << "Encountered input value " << data << " for " << item_name
            << " when expecting " << benchmark_data << std::endl;
          std::exit(EXIT_FAILURE);
        }

    }

}  // namespace

#endif
