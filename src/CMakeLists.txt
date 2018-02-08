cmake_minimum_required(VERSION 3.4.3)
project(bin2cpp)

message(STATUS "CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")
message(STATUS "EXECUTABLE_OUTPUT_PATH=${EXECUTABLE_OUTPUT_PATH}")
message(STATUS "LIBRARY_OUTPUT_PATH=${LIBRARY_OUTPUT_PATH}")
message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/../build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(bin2cpp)
add_subdirectory(bin2cpp_unittest)
add_subdirectory(common)
add_subdirectory(testfilegenerator)

add_dependencies(bin2cpp common)
add_dependencies(testfilegenerator common)
add_dependencies(bin2cpp_unittest common)
add_dependencies(bin2cpp_unittest bin2cpp)
add_dependencies(bin2cpp_unittest testfilegenerator)
