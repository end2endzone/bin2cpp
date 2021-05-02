set(GENERATED_TEST_FILES 
  ${CMAKE_CURRENT_BINARY_DIR}/generated_helloworld.h
  ${CMAKE_CURRENT_BINARY_DIR}/generated_helloworld.cpp
)

add_custom_command( OUTPUT ${GENERATED_TEST_FILES}
                    # Execute bin2cpp generator
                    COMMAND $<TARGET_FILE:bin2cpp> 
                      --file=${CMAKE_CURRENT_SOURCE_DIR}/helloworld.html
                      --output=${CMAKE_CURRENT_BINARY_DIR}
                      --headerfile=generated_helloworld.h
                      --identifier=HelloWorldHtml
                      --chunksize=50
)

# Show all generated files in a common folder
source_group("Generated Files" FILES ${GENERATED_TEST_FILES})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(demo_helloworld
  helloworld.cpp
  ${GENERATED_TEST_FILES}
)

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(demo_helloworld PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# This is normally not required if bin2cpp is installed and in PATH.
add_dependencies(demo_helloworld bin2cpp)
