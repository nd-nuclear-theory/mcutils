$(eval $(begin-module))

################################################################
# unit definitions
################################################################

# units
module_units_h += arithmetic deprecated vector_tuple memoizer profiling eigen gsl
module_units_cpp-h += parsing io

# programs
module_programs_cpp +=

# test programs
module_programs_cpp_test += arithmetic_test eigen_test gsl_test io_test memoizer_test profiling_test vector_tuple_test

################################################################
# library creation flag
################################################################

$(eval $(library))

$(eval $(end-module))
