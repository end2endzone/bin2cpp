message("${CMAKE_CURRENT_LIST_FILE} - START")

# Valid input arguments
if(NOT TESTFILEGENERATOR_TARGET_FILE)
  message( FATAL_ERROR "Please define TESTFILEGENERATOR_TARGET_FILE." )
endif()
if(NOT BIN2CPP_TARGET_FILE)
  message( FATAL_ERROR "Please define BIN2CPP_TARGET_FILE." )
endif()

# Show current environment
message(STATUS "TESTFILEGENERATOR_TARGET_FILE=" ${TESTFILEGENERATOR_TARGET_FILE})
message(STATUS "BIN2CPP_TARGET_FILE=          " ${BIN2CPP_TARGET_FILE})
message(STATUS "BIN2CPP_UNITTEST_SOURCE_DIR=  " ${BIN2CPP_UNITTEST_SOURCE_DIR})
message(STATUS "CMAKE_CURRENT_SOURCE_DIR=     " ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "CMAKE_CURRENT_BINARY_DIR=     " ${CMAKE_CURRENT_BINARY_DIR})
message(STATUS "CMAKE_CURRENT_LIST_DIR=       " ${CMAKE_CURRENT_LIST_DIR})
message(STATUS "CMAKE_SOURCE_DIR=             " ${CMAKE_SOURCE_DIR})
message(STATUS "CMAKE_BINARY_DIR=             " ${CMAKE_BINARY_DIR})

# Prebuild script for building unit tests...
# Note this script is also the one used by the install package.
message("Configuring prebuild script file '${CMAKE_CURRENT_BINARY_DIR}/generate_test_files.sh' for building unit tests...")
configure_file(${BIN2CPP_UNITTEST_SOURCE_DIR}/generate_test_files.sh.in ${CMAKE_CURRENT_BINARY_DIR}/generate_test_files.sh @ONLY)

# Script for running unit tests on Travis CI...
message("Configuring script file '${CMAKE_BINARY_DIR}/generate_test_files.sh' for running unit tests...")
configure_file(${BIN2CPP_UNITTEST_SOURCE_DIR}/generate_test_files.sh.in ${CMAKE_BINARY_DIR}/generate_test_files.sh @ONLY)

message("${CMAKE_CURRENT_LIST_FILE} - END")
