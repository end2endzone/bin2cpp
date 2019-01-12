set(GENERATED_TEST_FILES_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated_files)
include_directories(${GENERATED_TEST_FILES_DIR})
include_directories(${GENERATED_TEST_FILES_DIR}/..)

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

#  https://stackoverflow.com/questions/18427877/add-custom-build-step-in-cmake
if (WIN32)
  # Generate test file for 'Debug' configuration.
  set(TESTFILEGENERATOR_TARGET_PATH testfilegenerator-d.exe)
  set(BIN2CPP_TARGET_PATH bin2cpp-d.exe)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/generate_test_files.bat.in ${CMAKE_CURRENT_BINARY_DIR}/generate_test_files_Debug.bat @ONLY)

  # Generate test file for 'Release' configuration.
  set(TESTFILEGENERATOR_TARGET_PATH testfilegenerator.exe)
  set(BIN2CPP_TARGET_PATH bin2cpp.exe)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/generate_test_files.bat.in ${CMAKE_CURRENT_BINARY_DIR}/generate_test_files_Release.bat @ONLY)

  add_custom_command( OUTPUT ${GENERATED_TEST_FILES}
                      # Copy generator script of the current configuration to the final output file. This file will be the one used when installing.
                      COMMAND ${CMAKE_COMMAND} -E copy generate_test_files_$(Configuration).bat generate_test_files.bat

                      # Copy generator script to $(OutDir) for running unit tests on AppVeyor.
                      COMMAND ${CMAKE_COMMAND} -E copy generate_test_files.bat ${CMAKE_BINARY_DIR}/bin/$(Configuration)

                      # Execute generator script
                      COMMAND set PATH=${CMAKE_BINARY_DIR}/bin/$(Configuration)\;%PATH%
                      COMMAND call generate_test_files.bat
  )

  # Show all generated files in a common folder
  source_group("Generated Files" FILES ${GENERATED_TEST_FILES})
endif()

add_executable(bin2cpp_unittest
  ${BIN2CPP_EXPORT_HEADER}
  ${BIN2CPP_VERSION_HEADER}
  ${BIN2CPP_CONFIG_HEADER}
  application.cpp
  application.h
  CMakeLists.txt
  generate_test_files.bat.in
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
if (WIN32)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/generate_test_files.bat DESTINATION ${BIN2CPP_INSTALL_BIN_DIR})
endif()
