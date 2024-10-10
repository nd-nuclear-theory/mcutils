/****************************************************************
  io_test.cpp

  Mark A. Caprio
  University of Notre Dame

****************************************************************/

#include <iostream>
#include <sstream>

#include "mcutils/io.h"

int main(int argc, char **argv)
{
  // std::ofstream out_stream("io_test.dat",std::ios_base::out|std::ios_base::binary);
  // mcutils::WriteBinary<int>(out_stream,72);
  // mcutils::WriteBinary<int>(out_stream,69);
  // mcutils::WriteBinary<int>(out_stream,76);
  // mcutils::WriteBinary<int>(out_stream,76);
  // mcutils::WriteBinary<int>(out_stream,79);
  // out_stream.close();

  std::stringstream out_stream(std::ios_base::out|std::ios_base::binary);
  mcutils::WriteBinary<int>(out_stream,72);
  mcutils::WriteBinary<int>(out_stream,69);
  mcutils::WriteBinary<int>(out_stream,76);
  mcutils::WriteBinary<int>(out_stream,76);
  mcutils::WriteBinary<int>(out_stream,79);

  std::cout << out_stream.str() << std::endl;


  // termination
  return EXIT_SUCCESS;
}
