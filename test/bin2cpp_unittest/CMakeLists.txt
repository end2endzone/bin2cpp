include_directories(${CMAKE_SOURCE_DIR}/src/bin2cpp)    # for External Files

set(GENERATED_TEST_FILES_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated_files)
include_directories(${GENERATED_TEST_FILES_DIR})    # for TestExtraction.cpp

# Include directories for each generated files.
# These are required because each generated cpp file is including its header file
# without using the parent directory.
include_directories(${GENERATED_TEST_FILES_DIR}/testBaseClass)
include_directories(${GENERATED_TEST_FILES_DIR}/testDir01/sources)
include_directories(${GENERATED_TEST_FILES_DIR}/testEncodingHex)
include_directories(${GENERATED_TEST_FILES_DIR}/testEncodingOct)
include_directories(${GENERATED_TEST_FILES_DIR}/testFileManager)
include_directories(${GENERATED_TEST_FILES_DIR}/testGeneratorArray10000)
include_directories(${GENERATED_TEST_FILES_DIR}/testGeneratorSegment10000)
include_directories(${GENERATED_TEST_FILES_DIR}/testGeneratorString10000)
if (WIN32)
  include_directories(${GENERATED_TEST_FILES_DIR}/testGeneratorWin32)
endif()
include_directories(${GENERATED_TEST_FILES_DIR}/testHtml100000)
include_directories(${GENERATED_TEST_FILES_DIR}/testIssue12)
include_directories(${GENERATED_TEST_FILES_DIR}/testIssue12Mini)
include_directories(${GENERATED_TEST_FILES_DIR}/testIssue13)
include_directories(${GENERATED_TEST_FILES_DIR}/testIssue47)
include_directories(${GENERATED_TEST_FILES_DIR}/testIssue50)
include_directories(${GENERATED_TEST_FILES_DIR}/testIssue56a)
include_directories(${GENERATED_TEST_FILES_DIR}/testIssue56b/generated_sources)
include_directories(${GENERATED_TEST_FILES_DIR}/testIssue56c/generated_sources)
include_directories(${GENERATED_TEST_FILES_DIR}/testKeepDirectories)
include_directories(${GENERATED_TEST_FILES_DIR}/testNamespace)
include_directories(${GENERATED_TEST_FILES_DIR}/testRandom1)
include_directories(${GENERATED_TEST_FILES_DIR}/testRandom2)
include_directories(${GENERATED_TEST_FILES_DIR}/testRandom3)
include_directories(${GENERATED_TEST_FILES_DIR}/testRandom4)
include_directories(${GENERATED_TEST_FILES_DIR}/testRandom5)
include_directories(${GENERATED_TEST_FILES_DIR}/testRandom6)
include_directories(${GENERATED_TEST_FILES_DIR}/testRandom7)
include_directories(${GENERATED_TEST_FILES_DIR}/testRandom8)
include_directories(${GENERATED_TEST_FILES_DIR}/testReportedPathDir/generated_sources)
include_directories(${GENERATED_TEST_FILES_DIR}/testReportedPathFile1)
include_directories(${GENERATED_TEST_FILES_DIR}/testReportedPathFile2)
include_directories(${GENERATED_TEST_FILES_DIR}/testSequential1000)
include_directories(${GENERATED_TEST_FILES_DIR}/testSequential10MB)
include_directories(${GENERATED_TEST_FILES_DIR}/testText10)
include_directories(${GENERATED_TEST_FILES_DIR}/testText1000)
include_directories(${GENERATED_TEST_FILES_DIR}/testText100000)

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
  ${GENERATED_TEST_FILES_DIR}/testHtml100000/_testHtml100000.h
  ${GENERATED_TEST_FILES_DIR}/testHtml100000/_testHtml100000.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue12/_testIssue12.h
  ${GENERATED_TEST_FILES_DIR}/testIssue12/_testIssue12.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue12Mini/_testIssue12Mini.h
  ${GENERATED_TEST_FILES_DIR}/testIssue12Mini/_testIssue12Mini.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue13/_testIssue13.h
  ${GENERATED_TEST_FILES_DIR}/testIssue13/_testIssue13.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue47/generated/_testIssue47.h
  ${GENERATED_TEST_FILES_DIR}/testIssue47/generated/_testIssue47.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue47/generated/FileManager47.h
  ${GENERATED_TEST_FILES_DIR}/testIssue47/generated/FileManager47.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue50/_testIssue50.h
  ${GENERATED_TEST_FILES_DIR}/testIssue50/_testIssue50.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue50/FileManager50.h
  ${GENERATED_TEST_FILES_DIR}/testIssue50/FileManager50.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue56a/compiled_sources/_testIssue56a.index.4321.h
  ${GENERATED_TEST_FILES_DIR}/testIssue56a/compiled_sources/_testIssue56a.index.1234.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue56a/compiled_sources/_testIssue56a.index.1234.h
  ${GENERATED_TEST_FILES_DIR}/testIssue56a/compiled_sources/_testIssue56a.index.4321.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue56a/compiled_sources/FileManager56a.h
  ${GENERATED_TEST_FILES_DIR}/testIssue56a/compiled_sources/FileManager56a.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue56b/generated_sources/FileManager56b.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue56b/generated_sources/FileManager56b.h
  ${GENERATED_TEST_FILES_DIR}/testIssue56b/generated_sources/www/index.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue56b/generated_sources/www/index.h
  ${GENERATED_TEST_FILES_DIR}/testIssue56b/generated_sources/www/blog/index.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue56b/generated_sources/www/blog/index.h
  ${GENERATED_TEST_FILES_DIR}/testIssue56b/generated_sources/www/blog/how-to-create-a-web-site/index.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue56b/generated_sources/www/blog/how-to-create-a-web-site/index.h
  ${GENERATED_TEST_FILES_DIR}/testIssue56b/generated_sources/www/blog/using-bin2cpp/index.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue56b/generated_sources/www/blog/using-bin2cpp/index.h
  ${GENERATED_TEST_FILES_DIR}/testIssue56b/generated_sources/www/contact/index.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue56b/generated_sources/www/contact/index.h
  ${GENERATED_TEST_FILES_DIR}/testIssue56b/generated_sources/www/home/index.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue56b/generated_sources/www/home/index.h
  ${GENERATED_TEST_FILES_DIR}/testIssue56c/generated_sources/FileManager56c.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue56c/generated_sources/FileManager56c.h
  ${GENERATED_TEST_FILES_DIR}/testIssue56c/generated_sources/index.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue56c/generated_sources/index.h
  ${GENERATED_TEST_FILES_DIR}/testIssue56c/generated_sources/index_1.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue56c/generated_sources/index_1.h
  ${GENERATED_TEST_FILES_DIR}/testIssue56c/generated_sources/index_2.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue56c/generated_sources/index_2.h
  ${GENERATED_TEST_FILES_DIR}/testIssue56c/generated_sources/index_3.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue56c/generated_sources/index_3.h
  ${GENERATED_TEST_FILES_DIR}/testIssue56c/generated_sources/index_4.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue56c/generated_sources/index_4.h
  ${GENERATED_TEST_FILES_DIR}/testIssue56c/generated_sources/index_5.cpp
  ${GENERATED_TEST_FILES_DIR}/testIssue56c/generated_sources/index_5.h
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
  ${GENERATED_TEST_FILES_DIR}/testFileManager/_testFileManager.1.h
  ${GENERATED_TEST_FILES_DIR}/testFileManager/_testFileManager.1.cpp
  ${GENERATED_TEST_FILES_DIR}/testFileManager/_testFileManager.2.h
  ${GENERATED_TEST_FILES_DIR}/testFileManager/_testFileManager.2.cpp
  ${GENERATED_TEST_FILES_DIR}/testFileManager/FileManager.h
  ${GENERATED_TEST_FILES_DIR}/testFileManager/FileManager.cpp
  ${GENERATED_TEST_FILES_DIR}/testDir01/sources/_img0001.h
  ${GENERATED_TEST_FILES_DIR}/testDir01/sources/_img0002.h
  ${GENERATED_TEST_FILES_DIR}/testDir01/sources/_img0003.h
  ${GENERATED_TEST_FILES_DIR}/testDir01/sources/_img0004.h
  ${GENERATED_TEST_FILES_DIR}/testDir01/sources/_img0005.h
  ${GENERATED_TEST_FILES_DIR}/testDir01/sources/_img0001.cpp
  ${GENERATED_TEST_FILES_DIR}/testDir01/sources/_img0002.cpp
  ${GENERATED_TEST_FILES_DIR}/testDir01/sources/_img0003.cpp
  ${GENERATED_TEST_FILES_DIR}/testDir01/sources/_img0004.cpp
  ${GENERATED_TEST_FILES_DIR}/testDir01/sources/_img0005.cpp
  ${GENERATED_TEST_FILES_DIR}/testReportedPathDir/generated_sources/index.h
  ${GENERATED_TEST_FILES_DIR}/testReportedPathDir/generated_sources/index.cpp
  ${GENERATED_TEST_FILES_DIR}/testReportedPathDir/generated_sources/index_1.h
  ${GENERATED_TEST_FILES_DIR}/testReportedPathDir/generated_sources/index_1.cpp
  ${GENERATED_TEST_FILES_DIR}/testReportedPathDir/generated_sources/index_2.h
  ${GENERATED_TEST_FILES_DIR}/testReportedPathDir/generated_sources/index_2.cpp
  ${GENERATED_TEST_FILES_DIR}/testReportedPathDir/generated_sources/index_3.h
  ${GENERATED_TEST_FILES_DIR}/testReportedPathDir/generated_sources/index_3.cpp
  ${GENERATED_TEST_FILES_DIR}/testReportedPathDir/generated_sources/index_4.h
  ${GENERATED_TEST_FILES_DIR}/testReportedPathDir/generated_sources/index_4.cpp
  ${GENERATED_TEST_FILES_DIR}/testReportedPathDir/generated_sources/index_5.h
  ${GENERATED_TEST_FILES_DIR}/testReportedPathDir/generated_sources/index_5.cpp
  ${GENERATED_TEST_FILES_DIR}/testReportedPathDir/generated_sources/FileManagerReportedPathDir.h
  ${GENERATED_TEST_FILES_DIR}/testReportedPathDir/generated_sources/FileManagerReportedPathDir.cpp
  ${GENERATED_TEST_FILES_DIR}/testReportedPathFile1/_testReportedPathFile1.h
  ${GENERATED_TEST_FILES_DIR}/testReportedPathFile1/_testReportedPathFile1.cpp
  ${GENERATED_TEST_FILES_DIR}/testReportedPathFile2/_testReportedPathFile2.h
  ${GENERATED_TEST_FILES_DIR}/testReportedPathFile2/_testReportedPathFile2.cpp
)
if (WIN32)
  # Include win32 generated test files only on Windows platform.
  set(GENERATED_TEST_FILES ${GENERATED_TEST_FILES} 
      ${GENERATED_TEST_FILES_DIR}/testGeneratorWin32/_testGeneratorWin32.h
      ${GENERATED_TEST_FILES_DIR}/testGeneratorWin32/_testGeneratorWin32.cpp
      ${GENERATED_TEST_FILES_DIR}/testGeneratorWin32/_testGeneratorWin32.rc
  )
endif()

set(TEMPLATE_SCRIPT_FILE ${BIN2CPP_UNITTEST_SOURCE_DIR}/generate_test_files.${SCRIPT_FILE_EXTENSION}.in)

#  https://stackoverflow.com/questions/18427877/add-custom-build-step-in-cmake
if (WIN32)
  add_custom_command( OUTPUT ${GENERATED_TEST_FILES}
                      # Execute prebuild copy
                      COMMAND ${CMAKE_COMMAND}
                        -DBIN2CPP_TARGET_FILE=$<TARGET_FILE:bin2cpp>
                        -DTESTFILEGENERATOR_TARGET_FILE=$<TARGET_FILE:testfilegenerator>
                        -DBIN2CPP_UNITTEST_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
                        -DBIN2CPP_UNITTEST_PROJECT_DIR=${CMAKE_CURRENT_BINARY_DIR}
                        -DBIN2CPP_UNITTEST_OUTPUT_DIR=${CMAKE_BINARY_DIR}/bin/$(Configuration)
                        -P ${CMAKE_CURRENT_SOURCE_DIR}/generate_test_files.cmake

                      # Execute generator script
                      COMMAND echo Calling 'generate_test_files.bat' script...
                      COMMAND cd /d ${CMAKE_CURRENT_BINARY_DIR} && call generate_test_files.bat
  )
else()
  # Linux build
  add_custom_command( OUTPUT ${GENERATED_TEST_FILES}
                      # Execute prebuild copy
                      COMMAND ${CMAKE_COMMAND}
                        -DBIN2CPP_TARGET_FILE=$<TARGET_FILE:bin2cpp>
                        -DTESTFILEGENERATOR_TARGET_FILE=$<TARGET_FILE:testfilegenerator>
                        -DBIN2CPP_UNITTEST_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
                        -DBIN2CPP_UNITTEST_PROJECT_DIR=${CMAKE_CURRENT_BINARY_DIR}
                        -DBIN2CPP_UNITTEST_OUTPUT_DIR=${CMAKE_BINARY_DIR}/bin
                        -P ${CMAKE_CURRENT_SOURCE_DIR}/generate_test_files.cmake
                      
                      # Execute generator script
                      COMMAND echo Calling 'generate_test_files.sh' script...
                      COMMAND cd ${CMAKE_CURRENT_BINARY_DIR} && ./generate_test_files.sh
  )
endif()

# target build_test_files is always built
add_custom_target(build_test_files ALL
  COMMAND echo "Building unit test files..."
  
  # If the files exists, then commands related to these files won't be executed
  # DONOT let other target depends on the same OUTPUT as current target,
  #   or it may be bad when doing parallel make
  DEPENDS ${GENERATED_TEST_FILES}
)

# Show all generated files in a common folder
source_group("Generated Files" FILES ${GENERATED_TEST_FILES})
source_group("External Files" FILES ${CMAKE_SOURCE_DIR}/src/bin2cpp/common.cpp ${CMAKE_SOURCE_DIR}/src/bin2cpp/common.h)

add_executable(bin2cpp_unittest
  ${BIN2CPP_VERSION_HEADER}
  ${BIN2CPP_CONFIG_HEADER}
  ${CMAKE_SOURCE_DIR}/src/bin2cpp/common.cpp
  ${CMAKE_SOURCE_DIR}/src/bin2cpp/common.h
  application.cpp
  application.h
  CMakeLists.txt
  generate_test_files.bat.in
  generate_test_files.sh.in
  generate_test_files.cmake
  main.cpp
  TestCLI.cpp
  TestCLI.h
  TestCommon.cpp
  TestCommon.h
  TestExtraction.cpp
  TestExtraction.h
  ${GENERATED_TEST_FILES}
)

add_dependencies(bin2cpp_unittest bin2cpp)
add_dependencies(bin2cpp_unittest testfilegenerator)
add_dependencies(bin2cpp_unittest build_test_files)

# Unit test projects requires to link with pthread if also linking with gtest
if(NOT WIN32)
  set(PTHREAD_LIBRARIES -pthread)
endif()

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(bin2cpp_unittest PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_include_directories(bin2cpp_unittest 
  PRIVATE
    ${GTEST_INCLUDE_DIR}
)
add_dependencies(bin2cpp_unittest bin2cpp)
target_link_libraries(bin2cpp_unittest PRIVATE rapidassist ${PTHREAD_LIBRARIES} ${GTEST_LIBRARIES} )

install(TARGETS bin2cpp_unittest
        EXPORT bin2cpp-targets
        ARCHIVE DESTINATION ${BIN2CPP_INSTALL_LIB_DIR}
        LIBRARY DESTINATION ${BIN2CPP_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${BIN2CPP_INSTALL_BIN_DIR}
)
# Add the test file generator script created by 'generate_test_files.cmake'.
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/package/generate_test_files.${SCRIPT_FILE_EXTENSION} DESTINATION ${BIN2CPP_INSTALL_BIN_DIR})
