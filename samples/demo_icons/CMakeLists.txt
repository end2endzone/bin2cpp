set(GENERATED_TEST_FILES 
  ${CMAKE_CURRENT_BINARY_DIR}/about.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/about.h
  ${CMAKE_CURRENT_BINARY_DIR}/address_book.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/address_book.h
  ${CMAKE_CURRENT_BINARY_DIR}/alarm_clock.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/alarm_clock.h
  ${CMAKE_CURRENT_BINARY_DIR}/audio_file.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/audio_file.h
  ${CMAKE_CURRENT_BINARY_DIR}/binoculars.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/binoculars.h
  ${CMAKE_CURRENT_BINARY_DIR}/bookmark.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/bookmark.h
  ${CMAKE_CURRENT_BINARY_DIR}/broken_link.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/broken_link.h
  ${CMAKE_CURRENT_BINARY_DIR}/calculator.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/calculator.h
  ${CMAKE_CURRENT_BINARY_DIR}/calendar.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/calendar.h
  ${CMAKE_CURRENT_BINARY_DIR}/cancel.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/cancel.h
  ${CMAKE_CURRENT_BINARY_DIR}/checkmark.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/checkmark.h
  ${CMAKE_CURRENT_BINARY_DIR}/clock.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/clock.h
  ${CMAKE_CURRENT_BINARY_DIR}/contacts.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/contacts.h
  ${CMAKE_CURRENT_BINARY_DIR}/cursor.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/cursor.h
  ${CMAKE_CURRENT_BINARY_DIR}/dislike.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/dislike.h
  ${CMAKE_CURRENT_BINARY_DIR}/document.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/document.h
  ${CMAKE_CURRENT_BINARY_DIR}/download.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/download.h
  ${CMAKE_CURRENT_BINARY_DIR}/empty_trash.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/empty_trash.h
  ${CMAKE_CURRENT_BINARY_DIR}/file.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/file.h
  ${CMAKE_CURRENT_BINARY_DIR}/folder.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/folder.h
  ${CMAKE_CURRENT_BINARY_DIR}/frame.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/frame.h
  ${CMAKE_CURRENT_BINARY_DIR}/full_trash.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/full_trash.h
  ${CMAKE_CURRENT_BINARY_DIR}/globe.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/globe.h
  ${CMAKE_CURRENT_BINARY_DIR}/home.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/home.h
  ${CMAKE_CURRENT_BINARY_DIR}/icons8_cup.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/icons8_cup.h
  ${CMAKE_CURRENT_BINARY_DIR}/idea.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/idea.h
  ${CMAKE_CURRENT_BINARY_DIR}/image_file.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/image_file.h
  ${CMAKE_CURRENT_BINARY_DIR}/info.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/info.h
  ${CMAKE_CURRENT_BINARY_DIR}/key.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/key.h
  ${CMAKE_CURRENT_BINARY_DIR}/like.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/like.h
  ${CMAKE_CURRENT_BINARY_DIR}/like_placeholder.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/like_placeholder.h
  ${CMAKE_CURRENT_BINARY_DIR}/link.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/link.h
  ${CMAKE_CURRENT_BINARY_DIR}/lock.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/lock.h
  ${CMAKE_CURRENT_BINARY_DIR}/menu.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/menu.h
  ${CMAKE_CURRENT_BINARY_DIR}/minus.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/minus.h
  ${CMAKE_CURRENT_BINARY_DIR}/music.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/music.h
  ${CMAKE_CURRENT_BINARY_DIR}/news.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/news.h
  ${CMAKE_CURRENT_BINARY_DIR}/no_idea.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/no_idea.h
  ${CMAKE_CURRENT_BINARY_DIR}/ok.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/ok.h
  ${CMAKE_CURRENT_BINARY_DIR}/opened_folder.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/opened_folder.h
  ${CMAKE_CURRENT_BINARY_DIR}/picture.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/picture.h
  ${CMAKE_CURRENT_BINARY_DIR}/plus.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/plus.h
  ${CMAKE_CURRENT_BINARY_DIR}/print.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/print.h
  ${CMAKE_CURRENT_BINARY_DIR}/puzzle.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/puzzle.h
  ${CMAKE_CURRENT_BINARY_DIR}/rating.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/rating.h
  ${CMAKE_CURRENT_BINARY_DIR}/refresh.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/refresh.h
  ${CMAKE_CURRENT_BINARY_DIR}/ruler.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/ruler.h
  ${CMAKE_CURRENT_BINARY_DIR}/search.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/search.h
  ${CMAKE_CURRENT_BINARY_DIR}/services.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/services.h
  ${CMAKE_CURRENT_BINARY_DIR}/settings.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/settings.h
  ${CMAKE_CURRENT_BINARY_DIR}/share_.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/share_.h
  ${CMAKE_CURRENT_BINARY_DIR}/support.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/support.h
  ${CMAKE_CURRENT_BINARY_DIR}/synchronize.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/synchronize.h
  ${CMAKE_CURRENT_BINARY_DIR}/unlock.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/unlock.h
  ${CMAKE_CURRENT_BINARY_DIR}/upload.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/upload.h
  ${CMAKE_CURRENT_BINARY_DIR}/video_file.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/video_file.h
  
  ${CMAKE_CURRENT_BINARY_DIR}/IconsFileManager.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/IconsFileManager.h
)

add_custom_command( OUTPUT ${GENERATED_TEST_FILES}
                    # Execute bin2cpp generator
                    COMMAND $<TARGET_FILE:bin2cpp> 
                      --dir=${CMAKE_CURRENT_SOURCE_DIR}/flat-color-icons
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
