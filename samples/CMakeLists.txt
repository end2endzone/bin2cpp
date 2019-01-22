set(GENERATED_TEST_FILES 
  ${CMAKE_CURRENT_BINARY_DIR}/file_html5skeleton.h
  ${CMAKE_CURRENT_BINARY_DIR}/file_html5skeleton.cpp
)

add_custom_command( OUTPUT ${GENERATED_TEST_FILES}
                    # Execute bin2cpp generator
                    COMMAND $<TARGET_FILE:bin2cpp> 
                      --file=${CMAKE_CURRENT_SOURCE_DIR}/html5skeleton.html
                      --output=${CMAKE_CURRENT_BINARY_DIR}
                      --headerfile=file_html5skeleton.h
                      --identifier=HtmlSample
                      --chunksize=50
)

# Show all generated files in a common folder
source_group("Generated Files" FILES ${GENERATED_TEST_FILES})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(demo_sample
  demo_sample.cpp
  ${GENERATED_TEST_FILES}
)

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(demo_sample PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# This is normally not required if bin2cpp is installed and in PATH.
add_dependencies(demo_sample bin2cpp)
