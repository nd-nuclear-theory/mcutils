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
# define headers and sources
# ##############################################################################

# define units
set(${PROJECT_NAME}_UNITS_H
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
set(${PROJECT_NAME}_UNITS_H_CPP parsing io)

if(TARGET Eigen3::Eigen)
  list(APPEND ${PROJECT_NAME}_UNITS_H eigen)
  message(STATUS "building mcutils with Eigen support")
endif()
if(TARGET GSL::gsl)
  list(APPEND ${PROJECT_NAME}_UNITS_H gsl)
  message(STATUS "building mcutils with GSL support")
endif()

# construct lists of headers and sources
set(${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_UNITS_H} ${${PROJECT_NAME}_UNITS_H_CPP})
list(TRANSFORM ${PROJECT_NAME}_HEADERS PREPEND "${PROJECT_NAME}/")
list(TRANSFORM ${PROJECT_NAME}_HEADERS APPEND .h)
set(${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_UNITS_H_CPP})
list(TRANSFORM ${PROJECT_NAME}_SOURCES PREPEND "${PROJECT_NAME}/")
list(TRANSFORM ${PROJECT_NAME}_SOURCES APPEND .cpp)

# define mcutils library and add source files
add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})
target_sources(
  ${PROJECT_NAME} PUBLIC
  FILE_SET HEADERS
  TYPE HEADERS
  FILES ${${PROJECT_NAME}_HEADERS}
)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# ##############################################################################
# link dependencies
# ##############################################################################

target_link_libraries(${PROJECT_NAME} PUBLIC am::am)

if(TARGET Eigen3::Eigen AND TARGET fmt::fmt)
  target_link_libraries(${PROJECT_NAME} INTERFACE Eigen3::Eigen fmt::fmt)
endif()

if(TARGET GSL::gsl)
  target_link_libraries(${PROJECT_NAME} INTERFACE GSL::gsl)
endif()

# ##############################################################################
# define installation rules
# ##############################################################################
include(GNUInstallDirs)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  FILE_SET HEADERS
)

install(
  EXPORT ${PROJECT_NAME}Targets
  NAMESPACE ${PROJECT_NAME}::
  FILE ${PROJECT_NAME}Targets.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)
# generate the config file that is includes the exports
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  NO_SET_AND_CHECK_MACRO NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
# generate the version file for the config file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

# install the configuration file
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

export(
  EXPORT ${PROJECT_NAME}Targets
  NAMESPACE ${PROJECT_NAME}::
  FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake"
)

# ##############################################################################
# define tests
# ##############################################################################

set(${PROJECT_NAME}_UNITS_TEST
    arithmetic_test
    eigen_test
    gsl_test
    io_test
    memoizer_test
    profiling_test
    vector_tuple_test
)

add_custom_target(${PROJECT_NAME}_tests)
foreach(test_name IN LISTS ${PROJECT_NAME}_UNITS_TEST)
  add_executable(${test_name} EXCLUDE_FROM_ALL test/${test_name}.cpp)
  target_link_libraries(${test_name} ${PROJECT_NAME}::${PROJECT_NAME})
  add_dependencies(${PROJECT_NAME}_tests ${test_name})
endforeach()

if(MCUTILS_MASTER_PROJECT)
  add_custom_target(tests)
  add_dependencies(tests ${PROJECT_NAME}_tests)
endif()
