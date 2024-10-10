$(eval $(begin-module))

################################################################
# unit definitions
################################################################

# units
module_units_h += arithmetic deprecated vector_tuple memoizer profiling eigen gsl
module_units_h += fortran_io
module_units_cpp-h += parsing io

# programs
module_programs_cpp +=

################################################################
# library creation flag
################################################################

$(eval $(library))

$(eval $(end-module))
