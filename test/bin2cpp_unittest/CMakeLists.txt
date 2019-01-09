add_executable(bin2cpp_unittest
  ${BIN2CPP_EXPORT_HEADER}
  ${BIN2CPP_VERSION_HEADER}
  ${BIN2CPP_CONFIG_HEADER}
  application.cpp
  application.h
  CMakeLists.txt
  generate_test_files.bat
  main.cpp
  TestCLI.cpp
  TestCLI.h
  TestCommon.cpp
  TestCommon.h
  TestExtraction.cpp
  TestExtraction.h
)

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
target_link_libraries(bin2cpp_unittest PRIVATE rapidassist bin2cpp ${PTHREAD_LIBRARIES} ${GTEST_LIBRARIES} )

#     # Copy test files database to target dir
#     add_custom_command( TARGET bin2cpp_unittest POST_BUILD
#                         COMMAND ${CMAKE_COMMAND} -E copy_directory
#                         ${CMAKE_CURRENT_SOURCE_DIR}/test_files $<TARGET_FILE_DIR:bin2cpp_unittest>/test_files
#                         COMMENT "Copying test_files database.")

#     # Copy test files to build dir for local execution (from within the IDE)
#     file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_files DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS bin2cpp_unittest
        EXPORT bin2cpp-targets
        ARCHIVE DESTINATION ${BIN2CPP_INSTALL_LIB_DIR}
        LIBRARY DESTINATION ${BIN2CPP_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${BIN2CPP_INSTALL_BIN_DIR}
)
#     install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test_files
#             DESTINATION ${BIN2CPP_INSTALL_BIN_DIR}
#     )
