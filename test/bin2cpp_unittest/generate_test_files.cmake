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
set(TEMPLATE_SCRIPT_FILE ${BIN2CPP_UNITTEST_SOURCE_DIR}/generate_test_files.${SCRIPT_FILE_EXTENSION}.in)

############################################################################################################################################################
# Generate scripts for creating the required test files and source code for be able to compile and execute unit tests.
############################################################################################################################################################

# This file is executed as a pre-build step of project bin2cpp_unittest. It uses absolute path to executables.
set(SCRIPT_DESTINATION_FILE ${BIN2CPP_UNITTEST_PROJECT_DIR}/generate_test_files.${SCRIPT_FILE_EXTENSION})
message("Configure script file '${SCRIPT_DESTINATION_FILE}' as bin2cpp_unittest prebuild for compiling unit tests...")
configure_file(${TEMPLATE_SCRIPT_FILE} ${SCRIPT_DESTINATION_FILE} @ONLY)

# Convert from absolute path to local path (execute from current directory)
get_filename_component(TESTFILEGENERATOR_TARGET_FILE ${TESTFILEGENERATOR_TARGET_FILE} NAME)
get_filename_component(BIN2CPP_TARGET_FILE ${BIN2CPP_TARGET_FILE} NAME)
if (NOT WIN32)
  set(TESTFILEGENERATOR_TARGET_FILE "./${TESTFILEGENERATOR_TARGET_FILE}")
  set(BIN2CPP_TARGET_FILE "./${BIN2CPP_TARGET_FILE}")
endif()

# Create directory for installer package
file(MAKE_DIRECTORY ${BIN2CPP_UNITTEST_PROJECT_DIR}/package)

# Generate script for installation package. It uses local path to executables.
set(SCRIPT_DESTINATION_FILE ${BIN2CPP_UNITTEST_PROJECT_DIR}/package/generate_test_files.${SCRIPT_FILE_EXTENSION})
message("Configure script file '${SCRIPT_DESTINATION_FILE}' for installation package.")
configure_file(${TEMPLATE_SCRIPT_FILE} ${SCRIPT_DESTINATION_FILE} @ONLY)

# Generate script for execution from $(OutDir) aka BIN2CPP_UNITTEST_OUTPUT_DIR. It uses local path to executables.
set(SCRIPT_DESTINATION_FILE ${BIN2CPP_UNITTEST_OUTPUT_DIR}/generate_test_files.${SCRIPT_FILE_EXTENSION})
message("Configure script file '${SCRIPT_DESTINATION_FILE}' for executing unit tests on AppVeyor/Travis CI build servers.")
configure_file(${TEMPLATE_SCRIPT_FILE} ${SCRIPT_DESTINATION_FILE} @ONLY)

message("${CMAKE_CURRENT_LIST_FILE} - END")
