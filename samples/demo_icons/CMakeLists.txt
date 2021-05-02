set(GENERATED_TEST_FILES 
  ${CMAKE_CURRENT_BINARY_DIR}/cat-solid.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/cat-solid.h
  ${CMAKE_CURRENT_BINARY_DIR}/crow-solid.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/crow-solid.h
  ${CMAKE_CURRENT_BINARY_DIR}/dog-solid.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/dog-solid.h
  ${CMAKE_CURRENT_BINARY_DIR}/dove-solid.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/dove-solid.h
  ${CMAKE_CURRENT_BINARY_DIR}/dragon-solid.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/dragon-solid.h
  ${CMAKE_CURRENT_BINARY_DIR}/feather-alt-solid.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/feather-alt-solid.h
  ${CMAKE_CURRENT_BINARY_DIR}/feather-solid.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/feather-solid.h
  ${CMAKE_CURRENT_BINARY_DIR}/fish-solid.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/fish-solid.h
  ${CMAKE_CURRENT_BINARY_DIR}/frog-solid.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/frog-solid.h
  ${CMAKE_CURRENT_BINARY_DIR}/hippo-solid.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/hippo-solid.h
  ${CMAKE_CURRENT_BINARY_DIR}/horse-head-solid.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/horse-head-solid.h
  ${CMAKE_CURRENT_BINARY_DIR}/horse-solid.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/horse-solid.h
  ${CMAKE_CURRENT_BINARY_DIR}/kiwi-bird-solid.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/kiwi-bird-solid.h
  ${CMAKE_CURRENT_BINARY_DIR}/otter-solid.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/otter-solid.h
  ${CMAKE_CURRENT_BINARY_DIR}/paw-solid.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/paw-solid.h
  ${CMAKE_CURRENT_BINARY_DIR}/spider-solid.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/spider-solid.h
  ${CMAKE_CURRENT_BINARY_DIR}/IconsFileManager.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/IconsFileManager.h
)

add_custom_command( OUTPUT ${GENERATED_TEST_FILES}
                    # Execute bin2cpp generator
                    COMMAND $<TARGET_FILE:bin2cpp> 
                      --dir=${CMAKE_CURRENT_SOURCE_DIR}/icons
                      --managerfile=IconsFileManager.h
                      --output=${CMAKE_CURRENT_BINARY_DIR}
                      --chunksize=50
)

# Show all generated files in a common folder
source_group("Generated Files" FILES ${GENERATED_TEST_FILES})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(demo_icons
  demo_icons.cpp
  ${GENERATED_TEST_FILES}
)

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(demo_icons PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# This is normally not required if bin2cpp is installed and in PATH.
add_dependencies(demo_icons bin2cpp)
