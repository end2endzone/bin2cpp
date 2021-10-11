set(GENERATED_TEST_FILES_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated_files)
file(MAKE_DIRECTORY ${GENERATED_TEST_FILES_DIR})

set(GENERATED_TEST_FILES 
  ${GENERATED_TEST_FILES_DIR}/index.h
  ${GENERATED_TEST_FILES_DIR}/index.cpp
  ${GENERATED_TEST_FILES_DIR}/blog/index.h
  ${GENERATED_TEST_FILES_DIR}/blog/index.cpp
  ${GENERATED_TEST_FILES_DIR}/blog/using-bin2cpp/index.h
  ${GENERATED_TEST_FILES_DIR}/blog/using-bin2cpp/index.cpp
  ${GENERATED_TEST_FILES_DIR}/blog/how-to-create-a-web-site/index.h
  ${GENERATED_TEST_FILES_DIR}/blog/how-to-create-a-web-site/index.cpp
  ${GENERATED_TEST_FILES_DIR}/contact/index.h
  ${GENERATED_TEST_FILES_DIR}/contact/index.cpp
  ${GENERATED_TEST_FILES_DIR}/home/index.h
  ${GENERATED_TEST_FILES_DIR}/home/index.cpp
  
  ${GENERATED_TEST_FILES_DIR}/PagesFileManager.h
  ${GENERATED_TEST_FILES_DIR}/PagesFileManager.cpp
)


add_custom_command( OUTPUT ${GENERATED_TEST_FILES}
                    # Execute bin2cpp generator
                    COMMAND $<TARGET_FILE:bin2cpp> 
                      --dir=${CMAKE_CURRENT_SOURCE_DIR}/www
                      --managerfile=PagesFileManager.h
                      --output=${GENERATED_TEST_FILES_DIR}
                      --chunksize=50
                      --namespace=www
                      --keepdirs
)

# Show all generated files in a common folder
source_group("Generated Files" FILES ${GENERATED_TEST_FILES})

include_directories(${GENERATED_TEST_FILES_DIR})

add_executable(demo_website
  demo_website.cpp
  ${GENERATED_TEST_FILES}
)

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(demo_website PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# This is normally not required if bin2cpp is installed and in PATH.
add_dependencies(demo_website bin2cpp)
