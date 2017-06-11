$(eval $(begin-module))

################################################################
# unit definitions
################################################################

module_units_h := arithmetic vector_tuple memoizer profiling eigen gsl io
module_units_cpp-h := parsing
# module_units_f := 
module_programs_cpp := arithmetic_test eigen_test gsl_test io_test memoizer_test profiling_test vector_tuple_test
# module_programs_f :=
# module_generated :=

################################################################
# library creation flag
################################################################

$(eval $(library))

$(eval $(end-module))
