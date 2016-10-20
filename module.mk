$(eval $(begin-module))

################################################################
# unit definitions
################################################################

module_units_h := arithmetic vector_tuple memoizer profiling
module_units_cpp-h := parsing
# module_units_f := 
module_programs_cpp := arithmetic_test vector_tuple_test memoizer_test
# module_programs_f :=
# module_generated :=

################################################################
# library creation flag
################################################################

$(eval $(library))

$(eval $(end-module))
