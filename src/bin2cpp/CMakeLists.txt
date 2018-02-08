include_directories(${CMAKE_SOURCE_DIR}/common)

link_directories(${LIBRARY_OUTPUT_PATH})

add_executable(bin2cpp ArrayGenerator.cpp ArrayGenerator.h BaseGenerator.cpp BaseGenerator.h IGenerator.h SegmentGenerator.cpp SegmentGenerator.h StringGenerator.cpp StringGenerator.h targetver.h main.cpp Win32ResourceGenerator.cpp Win32ResourceGenerator.h)

if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

target_link_libraries(bin2cpp debug common.lib)
target_link_libraries(bin2cpp optimized common.lib)
