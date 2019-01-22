# CMakeLists.txt
cmake_minimum_required(VERSION 3.4.3 FATAL_ERROR)
project(bin2cpp)

##############################################################################################################################################
# Standard CMake variables
##############################################################################################################################################

# Set a default build type if none was specified.
# See https://blog.kitware.com/cmake-and-the-default-build-type/
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  mark_as_advanced(CMAKE_BUILD_TYPE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Set the output folder where your program will be created
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(   LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

##############################################################################################################################################
# CMake properties
##############################################################################################################################################
MESSAGE( STATUS "PROJECT_NAME:             " ${PROJECT_NAME} )
MESSAGE( STATUS "CMAKE_BINARY_DIR:         " ${CMAKE_BINARY_DIR} )
MESSAGE( STATUS "CMAKE_CURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR} )
MESSAGE( STATUS "CMAKE_SOURCE_DIR:         " ${CMAKE_SOURCE_DIR} )
MESSAGE( STATUS "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR} )
MESSAGE( STATUS "PROJECT_BINARY_DIR:       " ${PROJECT_BINARY_DIR} )
MESSAGE( STATUS "PROJECT_SOURCE_DIR:       " ${PROJECT_SOURCE_DIR} )
MESSAGE( STATUS "EXECUTABLE_OUTPUT_PATH:   " ${EXECUTABLE_OUTPUT_PATH} )
MESSAGE( STATUS "LIBRARY_OUTPUT_PATH:      " ${LIBRARY_OUTPUT_PATH} )
MESSAGE( STATUS "CMAKE_MODULE_PATH:        " ${CMAKE_MODULE_PATH} )
MESSAGE( STATUS "CMAKE_COMMAND:            " ${CMAKE_COMMAND} )
MESSAGE( STATUS "CMAKE_ROOT:               " ${CMAKE_ROOT} )
MESSAGE( STATUS "CMAKE_CURRENT_LIST_FILE:  " ${CMAKE_CURRENT_LIST_FILE} )
MESSAGE( STATUS "CMAKE_CURRENT_LIST_LINE:  " ${CMAKE_CURRENT_LIST_LINE} )
MESSAGE( STATUS "CMAKE_INCLUDE_PATH:       " ${CMAKE_INCLUDE_PATH} )
MESSAGE( STATUS "CMAKE_LIBRARY_PATH:       " ${CMAKE_LIBRARY_PATH} )
MESSAGE( STATUS "CMAKE_SYSTEM:             " ${CMAKE_SYSTEM} )
MESSAGE( STATUS "CMAKE_SYSTEM_NAME:        " ${CMAKE_SYSTEM_NAME} )
MESSAGE( STATUS "CMAKE_SYSTEM_VERSION:     " ${CMAKE_SYSTEM_VERSION} )
MESSAGE( STATUS "CMAKE_SYSTEM_PROCESSOR:   " ${CMAKE_SYSTEM_PROCESSOR} )

##############################################################################################################################################
# Global settings
##############################################################################################################################################

# Product version according to Semantic Versioning v2.0.0 https://semver.org/
SET(BIN2CPP_VERSION_MAJOR 2)
SET(BIN2CPP_VERSION_MINOR 2)
SET(BIN2CPP_VERSION_PATCH 0)
set(BIN2CPP_VERSION ${BIN2CPP_VERSION_MAJOR}.${BIN2CPP_VERSION_MINOR}.${BIN2CPP_VERSION_PATCH})

# read license file
file(READ ${CMAKE_SOURCE_DIR}/LICENSE.h LICENSE)

# version.h file
set(BIN2CPP_VERSION_HEADER ${CMAKE_BINARY_DIR}/include/bin2cpp/version.h)
message("Generating ${BIN2CPP_VERSION_HEADER}...")
configure_file( ${CMAKE_SOURCE_DIR}/src/bin2cpp/version.h.in ${BIN2CPP_VERSION_HEADER} )

# config.h file
set(BIN2CPP_CONFIG_HEADER ${CMAKE_BINARY_DIR}/include/bin2cpp/config.h)
message("Generating ${BIN2CPP_CONFIG_HEADER}...")
if (BUILD_SHARED_LIBS)
  set(BIN2CPP_BUILD_TYPE_CPP_DEFINE "#define BIN2CPP_BUILT_AS_SHARED")
else()
  set(BIN2CPP_BUILD_TYPE_CPP_DEFINE "#define BIN2CPP_BUILT_AS_STATIC")
endif()
configure_file( ${CMAKE_SOURCE_DIR}/src/bin2cpp/config.h.in ${BIN2CPP_CONFIG_HEADER} )
set(BIN2CPP_BUILD_TYPE_CPP_DEFINE)

# Define installation directories
set(BIN2CPP_INSTALL_BIN_DIR      "bin")
set(BIN2CPP_INSTALL_LIB_DIR      "lib/bin2cpp-${BIN2CPP_VERSION}")
set(BIN2CPP_INSTALL_INCLUDE_DIR  "include/bin2cpp-${BIN2CPP_VERSION}")
set(BIN2CPP_INSTALL_CMAKE_DIR    ${BIN2CPP_INSTALL_LIB_DIR}) # CMake files (*.cmake) should have the same destination as the library files. Some also prefers to use "cmake".

##############################################################################################################################################
# Project settings
##############################################################################################################################################

# Build options
option(BIN2CPP_BUILD_TEST "Build all bin2cpp's unit tests" OFF)
option(BIN2CPP_BUILD_DOC "Build documentation" OFF)

# Force a debug postfix if none specified.
# This allows publishing both release and debug binaries to the same location
# and it helps to prevent linking with the wrong library on Windows.
if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX "-d")
endif()

# Prevents annoying warnings on MSVC
if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Define include directories for source code.
# The specified values will not be exported.
include_directories(${CMAKE_BINARY_DIR}/include     # for ${BIN2CPP_VERSION_HEADER} and ${BIN2CPP_CONFIG_HEADER} generated files.
                    ${CMAKE_SOURCE_DIR}/src/common  # for common files shared by bin2cpp and unit tests.
)

##############################################################################################################################################
# Dependencies
##############################################################################################################################################
find_package(GTest REQUIRED) #rapidassist requires GTest
find_package(rapidassist 0.5.0 REQUIRED)

##############################################################################################################################################
# Subprojects
##############################################################################################################################################

add_subdirectory(src/bin2cpp)

# unit tests
if(BIN2CPP_BUILD_TEST)
  add_subdirectory(test/testfilegenerator)
  add_subdirectory(test/bin2cpp_unittest)
endif()

##############################################################################################################################################
# Generate doxygen documentation
# See https://vicrucann.github.io/tutorials/quick-cmake-doxygen/
##############################################################################################################################################
if (BIN2CPP_BUILD_DOC)
  # check if Doxygen is installed
  find_package(Doxygen)
  if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
 
    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")
 
    # note the option ALL which allows to build the docs together with the application
    add_custom_target( bin2cpp_doc ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM )
  else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
  endif (DOXYGEN_FOUND)
endif()

##############################################################################################################################################
# Install
##############################################################################################################################################

# Install locations:   See https://unix.stackexchange.com/a/36874
#   On UNIX, installs to "/usr/local".
#   On Windows, installs to "C:\Program Files (x86)\${PROJECT_NAME}" or to "C:\Program Files\${PROJECT_NAME}" for 64 bit binaries

# Target config version verification file
configure_file(${CMAKE_SOURCE_DIR}/cmake/bin2cpp-config-version.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/cmake/bin2cpp-config-version.cmake @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cmake/bin2cpp-config-version.cmake DESTINATION ${BIN2CPP_INSTALL_CMAKE_DIR})

# Target config file
configure_file(${CMAKE_SOURCE_DIR}/cmake/bin2cpp-config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/cmake/bin2cpp-config.cmake @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cmake/bin2cpp-config.cmake DESTINATION ${BIN2CPP_INSTALL_CMAKE_DIR})

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${BIN2CPP_INSTALL_INCLUDE_DIR})
install(FILES ${BIN2CPP_VERSION_HEADER}
              ${BIN2CPP_CONFIG_HEADER}
              DESTINATION ${BIN2CPP_INSTALL_INCLUDE_DIR}/bin2cpp)
install(EXPORT bin2cpp-targets DESTINATION ${BIN2CPP_INSTALL_CMAKE_DIR})

##############################################################################################################################################
# Packaging
##############################################################################################################################################

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${BIN2CPP_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR "${BIN2CPP_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${BIN2CPP_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${BIN2CPP_VERSION_PATCH}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "bin2cpp: The easiest way to embed small files into a c++ executable. bin2cpp converts text or binary files to C++ files (*.h, *.cpp) for easy access within the code.")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

# we don't want to split our program up into several things
set(CPACK_MONOLITHIC_INSTALL 1)

# This must be last
include(CPack)
