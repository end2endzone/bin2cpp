set(GENERATED_TEST_FILES 
  ${CMAKE_CURRENT_BINARY_DIR}/include/bin2cpp/file_icon.h
  ${CMAKE_CURRENT_BINARY_DIR}/include/bin2cpp/file_icon.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/include/bin2cpp/file_sinus.h
  ${CMAKE_CURRENT_BINARY_DIR}/include/bin2cpp/file_sinus.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/include/bin2cpp/file_splashscreen.h
  ${CMAKE_CURRENT_BINARY_DIR}/include/bin2cpp/file_splashscreen.cpp
)


file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/bin2cpp)

add_custom_command( OUTPUT ${GENERATED_TEST_FILES}
                    # Execute bin2cpp generator
                    COMMAND $<TARGET_FILE:bin2cpp> 
                      --file=${CMAKE_CURRENT_SOURCE_DIR}/res/icon.ico
                      --output=${CMAKE_CURRENT_BINARY_DIR}/include
                      --headerfile=bin2cpp/file_icon.h
                      --identifier=icon
                      --chunksize=50
                    COMMAND $<TARGET_FILE:bin2cpp> 
                      --file=${CMAKE_CURRENT_SOURCE_DIR}/res/sinus.dat
                      --output=${CMAKE_CURRENT_BINARY_DIR}/include
                      --headerfile=bin2cpp/file_sinus.h
                      --identifier=SinusTable
                      --chunksize=50
                    COMMAND $<TARGET_FILE:bin2cpp> 
                      --file=${CMAKE_CURRENT_SOURCE_DIR}/res/splashscreen.png
                      --output=${CMAKE_CURRENT_BINARY_DIR}/include
                      --headerfile=bin2cpp/file_splashscreen.h
                      --identifier=splashscreen
                      --chunksize=50
)

# Show all generated files in a common folder
source_group("Generated Files" FILES ${GENERATED_TEST_FILES})

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}/include
)

add_executable(demo_relative_dir
  include/libtrigo/sincos.h
  src/sincos.cpp
  src/main.cpp
  # src/generate_sinus_table.cpp
  ${GENERATED_TEST_FILES}
)

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(demo_relative_dir PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# This is normally not required if bin2cpp is installed and in PATH.
add_dependencies(demo_relative_dir bin2cpp)
