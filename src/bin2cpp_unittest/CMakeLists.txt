include_directories(${CMAKE_SOURCE_DIR}/common)
include_directories(${CMAKE_SOURCE_DIR}/../third_party/googletest/include)

set(GENERATED_TEST_FILES_DIR ${CMAKE_BINARY_DIR}/bin2cpp_unittest/generated_files)
include_directories(${GENERATED_TEST_FILES_DIR})
include_directories(${GENERATED_TEST_FILES_DIR}/..)

link_directories(${LIBRARY_OUTPUT_PATH})
link_directories(${CMAKE_SOURCE_DIR}/../third_party/googletest/msvc2010)

set(GENERATED_TEST_FILES 
  ${GENERATED_TEST_FILES_DIR}/testBaseClass/_testBaseClass.h
  ${GENERATED_TEST_FILES_DIR}/testBaseClass/_testBaseClass.cpp
  ${GENERATED_TEST_FILES_DIR}/testEncodingHex/_testEncodingHex.h
  ${GENERATED_TEST_FILES_DIR}/testEncodingHex/_testEncodingHex.cpp
  ${GENERATED_TEST_FILES_DIR}/testEncodingOct/_testEncodingOct.h
  ${GENERATED_TEST_FILES_DIR}/testEncodingOct/_testEncodingOct.cpp
  ${GENERATED_TEST_FILES_DIR}/testGeneratorArray10000/_testGeneratorArray10000.h
  ${GENERATED_TEST_FILES_DIR}/testGeneratorArray10000/_testGeneratorArray10000.cpp
  ${GENERATED_TEST_FILES_DIR}/testGeneratorSegment10000/_testGeneratorSegment10000.h
  ${GENERATED_TEST_FILES_DIR}/testGeneratorSegment10000/_testGeneratorSegment10000.cpp
  ${GENERATED_TEST_FILES_DIR}/testGeneratorString10000/_testGeneratorString10000.h
  ${GENERATED_TEST_FILES_DIR}/testGeneratorString10000/_testGeneratorString10000.cpp
  ${GENERATED_TEST_FILES_DIR}/testGeneratorWin32/_testGeneratorWin32.h
  ${GENERATED_TEST_FILES_DIR}/testGeneratorWin32/_testGeneratorWin32.cpp
  ${GENERATED_TEST_FILES_DIR}/testHtml100000/_testHtml100000.h
  ${GENERATED_TEST_FILES_DIR}/testHtml100000/_testHtml100000.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue12/_testIssue12.h
  ${GENERATED_TEST_FILES_DIR}/testIssue12/_testIssue12.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue12Mini/_testIssue12Mini.h
  ${GENERATED_TEST_FILES_DIR}/testIssue12Mini/_testIssue12Mini.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue13/_testIssue13.h
  ${GENERATED_TEST_FILES_DIR}/testIssue13/_testIssue13.cpp
  ${GENERATED_TEST_FILES_DIR}/testNamespace/_testNamespace.h
  ${GENERATED_TEST_FILES_DIR}/testNamespace/_testNamespace.cpp
  ${GENERATED_TEST_FILES_DIR}/testRandom1/_testRandom1.h
  ${GENERATED_TEST_FILES_DIR}/testRandom1/_testRandom1.cpp
  ${GENERATED_TEST_FILES_DIR}/testRandom2/_testRandom2.h
  ${GENERATED_TEST_FILES_DIR}/testRandom2/_testRandom2.cpp
  ${GENERATED_TEST_FILES_DIR}/testRandom3/_testRandom3.h
  ${GENERATED_TEST_FILES_DIR}/testRandom3/_testRandom3.cpp
  ${GENERATED_TEST_FILES_DIR}/testRandom4/_testRandom4.h
  ${GENERATED_TEST_FILES_DIR}/testRandom4/_testRandom4.cpp
  ${GENERATED_TEST_FILES_DIR}/testRandom5/_testRandom5.h
  ${GENERATED_TEST_FILES_DIR}/testRandom5/_testRandom5.cpp
  ${GENERATED_TEST_FILES_DIR}/testRandom6/_testRandom6.h
  ${GENERATED_TEST_FILES_DIR}/testRandom6/_testRandom6.cpp
  ${GENERATED_TEST_FILES_DIR}/testRandom7/_testRandom7.h
  ${GENERATED_TEST_FILES_DIR}/testRandom7/_testRandom7.cpp
  ${GENERATED_TEST_FILES_DIR}/testRandom8/_testRandom8.h
  ${GENERATED_TEST_FILES_DIR}/testRandom8/_testRandom8.cpp
  ${GENERATED_TEST_FILES_DIR}/testSequential1000/_testSequential1000.h
  ${GENERATED_TEST_FILES_DIR}/testSequential1000/_testSequential1000.cpp
  ${GENERATED_TEST_FILES_DIR}/testSequential10MB/_testSequential10MB.h
  ${GENERATED_TEST_FILES_DIR}/testSequential10MB/_testSequential10MB.cpp
  ${GENERATED_TEST_FILES_DIR}/testText10/_testText10.h
  ${GENERATED_TEST_FILES_DIR}/testText10/_testText10.cpp
  ${GENERATED_TEST_FILES_DIR}/testText1000/_testText1000.h
  ${GENERATED_TEST_FILES_DIR}/testText1000/_testText1000.cpp
  ${GENERATED_TEST_FILES_DIR}/testText100000/_testText100000.h
  ${GENERATED_TEST_FILES_DIR}/testText100000/_testText100000.cpp
)

if (WIN32)
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/generate_test_files.bat DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  add_custom_command( OUTPUT ${GENERATED_TEST_FILES}
                      COMMAND set PATH=${CMAKE_BINARY_DIR}/bin/$(Configuration)\;%PATH%
                      COMMAND call generate_test_files.bat
  )
endif()

add_executable(bin2cpp_unittest
  application.cpp
  application.h
  gtesthelper.cpp
  gtesthelper.h
  TestCLI.cpp
  TestCLI.h
  TestCommon.cpp
  TestCommon.h
  TestExtraction.cpp
  TestExtraction.h
  TestEnvironmentFunc.cpp
  TestEnvironmentFunc.h
  TestFilesystemFunc.cpp
  TestFilesystemFunc.h
  TestStringFunc.cpp
  TestStringFunc.h
  main.cpp
  ${GENERATED_TEST_FILES}
)

if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

target_link_libraries(bin2cpp_unittest debug common.lib gtest.lib)
target_link_libraries(bin2cpp_unittest optimized common.lib)
