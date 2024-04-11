cmake_minimum_required(VERSION 3.23)

# Determine if mcutils is built as a subproject (using add_subdirectory) or if
# it is the master project.
if(NOT DEFINED MCUTILS_MASTER_PROJECT)
  set(MCUTILS_MASTER_PROJECT OFF)
  if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MCUTILS_MASTER_PROJECT ON)
    message(STATUS "CMake version: ${CMAKE_VERSION}")
  endif()
endif()

project(mcutils CXX)

if(MCUTILS_MASTER_PROJECT AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

# ##############################################################################
# find external projects/dependencies
# ##############################################################################

if(NOT TARGET am::am)
  find_package(am REQUIRED)
endif()

if(NOT TARGET Eigen3::Eigen)
  find_package(Eigen3 NO_MODULE)
endif()

if(TARGET Eigen3::Eigen)
  # fmt is only required for Eigen3 support
  if(NOT TARGET fmt::fmt)
    find_package(fmt REQUIRED)
  endif()
endif()

if(NOT TARGET GSL::gsl)
  find_package(GSL)
endif()

# ##############################################################################
# define mcutils headers and sources
# ##############################################################################

# define units
set(mcutils_UNITS_H
    arithmetic
    deprecated
    vector_tuple
    memoizer
    meta
    profiling
    fortran_io
    # eigen  # optional -- see below
    # gsl  # optional -- see below
)
set(mcutils_UNITS_H_CPP parsing io)

if(TARGET Eigen3::Eigen)
  list(APPEND mcutils_UNITS_H eigen)
  message(STATUS "building mcutils with Eigen support")
endif()
if(TARGET GSL::gsl)
  list(APPEND mcutils_UNITS_H gsl)
  message(STATUS "building mcutils with GSL support")
endif()

# construct lists of headers and sources
set(mcutils_HEADERS ${mcutils_UNITS_H} ${mcutils_UNITS_H_CPP})
list(TRANSFORM mcutils_HEADERS APPEND .h)
set(mcutils_SOURCES ${mcutils_UNITS_H_CPP})
list(TRANSFORM mcutils_SOURCES APPEND .cpp)

# define mcutils library and add source files
add_library(mcutils ${mcutils_SOURCES})
target_sources(
  mcutils PUBLIC
  FILE_SET HEADERS
  TYPE HEADERS
  FILES ${mcutils_HEADERS}
)
add_library(mcutils::mcutils ALIAS mcutils)

# ##############################################################################
# link dependencies
# ##############################################################################

target_link_libraries(mcutils PUBLIC am::am)

if(TARGET Eigen3::Eigen AND TARGET fmt::fmt)
  target_link_libraries(mcutils INTERFACE Eigen3::Eigen fmt::fmt)
endif()

if(TARGET GSL::gsl)
  target_link_libraries(mcutils INTERFACE GSL::gsl)
endif()

# ##############################################################################
# define installation rules
# ##############################################################################
include(GNUInstallDirs)

install(
  TARGETS mcutils
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  EXPORT mcutilsTargets
  FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mcutils
)

install(
  EXPORT mcutilsTargets
  NAMESPACE mcutils::
  FILE mcutilsTargets.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mcutils
)

include(CMakePackageConfigHelpers)
# generate the config file that is includes the exports
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/mcutilsConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/mcutils"
  NO_SET_AND_CHECK_MACRO NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
# generate the version file for the config file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/mcutilsConfigVersion.cmake"
  VERSION "${mcutils_VERSION_MAJOR}.${mcutils_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

# install the configuration file
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/mcutilsConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/mcutilsConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mcutils
)

export(
  EXPORT mcutilsTargets
  NAMESPACE mcutils::
  FILE "${CMAKE_CURRENT_BINARY_DIR}/mcutilsTargets.cmake"
)

# ##############################################################################
# define tests
# ##############################################################################

set(mcutils_UNITS_TEST
    arithmetic_test
    eigen_test
    gsl_test
    io_test
    memoizer_test
    profiling_test
    vector_tuple_test
)

add_custom_target(mcutils_tests)
foreach(test_name IN LISTS mcutils_UNITS_TEST)
  add_executable(${test_name} EXCLUDE_FROM_ALL ${test_name}.cpp)
  target_link_libraries(${test_name} mcutils::mcutils)
  add_dependencies(mcutils_tests ${test_name})
endforeach()

if(MCUTILS_MASTER_PROJECT)
  add_custom_target(tests)
  add_dependencies(tests mcutils_tests)
endif()
