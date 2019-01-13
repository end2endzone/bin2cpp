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

# Generate test file for current configuration.
configure_file(${BIN2CPP_UNITTEST_SOURCE_DIR}/generate_test_files.sh.in ${CMAKE_CURRENT_BINARY_DIR}/generate_test_files.sh @ONLY)

message("${CMAKE_CURRENT_LIST_FILE} - END")
