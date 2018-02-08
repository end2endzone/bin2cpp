include_directories(${CMAKE_SOURCE_DIR}/common)

link_directories(${LIBRARY_OUTPUT_PATH})

add_executable(testfilegenerator testfilegenerator.cpp)

if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

target_link_libraries(testfilegenerator debug common.lib)
target_link_libraries(testfilegenerator optimized common.lib)
