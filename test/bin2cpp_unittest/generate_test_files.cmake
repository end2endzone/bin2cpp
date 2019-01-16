message("${CMAKE_CURRENT_LIST_FILE} - START")

# Valid input arguments
if(NOT TESTFILEGENERATOR_TARGET_FILE)
  message( FATAL_ERROR "Please define TESTFILEGENERATOR_TARGET_FILE." )
endif()
if(NOT BIN2CPP_TARGET_FILE)
  message( FATAL_ERROR "Please define BIN2CPP_TARGET_FILE." )
endif()
if(NOT BIN2CPP_UNITTEST_SOURCE_DIR)
  message( FATAL_ERROR "Please define BIN2CPP_UNITTEST_SOURCE_DIR." )
endif()
if(NOT BIN2CPP_UNITTEST_PROJECT_DIR)
  message( FATAL_ERROR "Please define BIN2CPP_UNITTEST_PROJECT_DIR." )
endif()
if(NOT BIN2CPP_UNITTEST_OUTPUT_DIR)
  message( FATAL_ERROR "Please define BIN2CPP_UNITTEST_OUTPUT_DIR." )
endif()

# Show current environment
message(STATUS "TESTFILEGENERATOR_TARGET_FILE=" ${TESTFILEGENERATOR_TARGET_FILE})
message(STATUS "BIN2CPP_TARGET_FILE=          " ${BIN2CPP_TARGET_FILE})
message(STATUS "BIN2CPP_UNITTEST_SOURCE_DIR=  " ${BIN2CPP_UNITTEST_SOURCE_DIR})
message(STATUS "BIN2CPP_UNITTEST_PROJECT_DIR= " ${BIN2CPP_UNITTEST_PROJECT_DIR})
message(STATUS "BIN2CPP_UNITTEST_OUTPUT_DIR=  " ${BIN2CPP_UNITTEST_OUTPUT_DIR})

# message(STATUS "CMAKE_CURRENT_SOURCE_DIR=     " ${CMAKE_CURRENT_SOURCE_DIR})
# message(STATUS "CMAKE_CURRENT_BINARY_DIR=     " ${CMAKE_CURRENT_BINARY_DIR})
# message(STATUS "CMAKE_CURRENT_LIST_DIR=       " ${CMAKE_CURRENT_LIST_DIR})
# message(STATUS "CMAKE_SOURCE_DIR=             " ${CMAKE_SOURCE_DIR})
# message(STATUS "CMAKE_BINARY_DIR=             " ${CMAKE_BINARY_DIR})

# Define the file extension for bash/batch files on the target platform
if (WIN32)
  set(SCRIPT_FILE_EXTENSION bat)
else()
  set(SCRIPT_FILE_EXTENSION sh)
endif()

# Configure generator script template for the current configuration. This file will be the one used by the install package.
message("Configuring prebuild script file '${BIN2CPP_UNITTEST_PROJECT_DIR}/generate_test_files.${SCRIPT_FILE_EXTENSION}' for compiling unit tests...")
configure_file(${BIN2CPP_UNITTEST_SOURCE_DIR}/generate_test_files.${SCRIPT_FILE_EXTENSION}.in ${BIN2CPP_UNITTEST_PROJECT_DIR}/generate_test_files.${SCRIPT_FILE_EXTENSION} @ONLY)

# Also copy the generator script to $(OutDir).
message("Copy prebuild script file to '${BIN2CPP_UNITTEST_OUTPUT_DIR}/generate_test_files.${SCRIPT_FILE_EXTENSION}' for executing unit tests on AppVeyor/Travis CI build servers.")
configure_file(${BIN2CPP_UNITTEST_SOURCE_DIR}/generate_test_files.${SCRIPT_FILE_EXTENSION}.in ${BIN2CPP_UNITTEST_OUTPUT_DIR}/generate_test_files.${SCRIPT_FILE_EXTENSION} @ONLY)

message("${CMAKE_CURRENT_LIST_FILE} - END")
