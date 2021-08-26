cmake_minimum_required(VERSION 3.20)

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

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

# add headers and sources
set(mcutils_headers
    arithmetic.h
    deprecated.h
    vector_tuple.h
    memoizer.h
    profiling.h
    eigen.h
    gsl.h
    io.h
    parsing.h
)
set(mcutils_sources parsing.cpp)

# define am library and add source files
add_library(mcutils ${mcutils_sources})
add_library(mcutils::mcutils ALIAS mcutils)

# ##############################################################################
# find external projects/dependencies
# ##############################################################################

if(NOT TARGET fmt::fmt)
  find_package(fmt REQUIRED)
endif()

if(NOT TARGET am::am)
  find_package(am REQUIRED)
endif()

# ##############################################################################
# link dependencies
# ##############################################################################

target_link_libraries(mcutils am::am fmt::fmt)

# ##############################################################################
# define include directory
# ##############################################################################
cmake_path(GET mcutils_SOURCE_DIR PARENT_PATH mcutils_PARENT_DIR)
target_include_directories(
  mcutils INTERFACE $<BUILD_INTERFACE:${mcutils_PARENT_DIR}>
                    $<INSTALL_INTERFACE:include>
)

# ##############################################################################
# define installation rules
# ##############################################################################
install(
  TARGETS mcutils
  DESTINATION lib
  EXPORT mcutilsTargets
)
install(FILES ${mcutils_headers} DESTINATION include/mcutils)

install(
  EXPORT mcutilsTargets
  NAMESPACE mcutils::
  FILE mcutilsTargets.cmake
  DESTINATION lib/cmake/mcutils
)

include(CMakePackageConfigHelpers)
# generate the config file that is includes the exports
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/mcutilsConfig.cmake"
  INSTALL_DESTINATION "lib/cmake/mcutils"
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
        DESTINATION lib/cmake/mcutils
)

export(
  EXPORT mcutilsTargets
  NAMESPACE mcutils::
  FILE "${CMAKE_CURRENT_BINARY_DIR}/mcutilsTargets.cmake"
)