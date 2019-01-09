set(BIN2CPP_HEADER_FILES ""
#         ${BIN2CPP_INCLUDE_DIR}/bin2cpp/bin2cpp.h
#         ${BIN2CPP_INCLUDE_DIR}/bin2cpp/notes.h
#         ${BIN2CPP_INCLUDE_DIR}/bin2cpp/pitches.h
#         ${BIN2CPP_INCLUDE_DIR}/bin2cpp/instruments.h
)

add_library(bin2cpp
  ${BIN2CPP_HEADER_FILES}
  ${BIN2CPP_EXPORT_HEADER}
  ${BIN2CPP_VERSION_HEADER}
  ${BIN2CPP_CONFIG_HEADER}
  ArrayGenerator.cpp
  ArrayGenerator.h
  BaseGenerator.cpp
  BaseGenerator.h
  bin2cpp.samples.txt
  common.cpp
  common.h
  IGenerator.h
  main.cpp
  SegmentGenerator.cpp
  SegmentGenerator.h
  StringGenerator.cpp
  StringGenerator.h
  targetver.h
  Win32ResourceGenerator.cpp
  Win32ResourceGenerator.h
)

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(bin2cpp PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# Define include directories for exported code.
target_include_directories(bin2cpp
  PUBLIC
    $<INSTALL_INTERFACE:${BIN2CPP_INSTALL_INCLUDE_DIR}>  # for clients using the installed library.
)
target_link_libraries(bin2cpp PUBLIC rapidassist)

install(TARGETS bin2cpp
        EXPORT bin2cpp-targets
        ARCHIVE DESTINATION ${BIN2CPP_INSTALL_LIB_DIR}
        LIBRARY DESTINATION ${BIN2CPP_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${BIN2CPP_INSTALL_BIN_DIR}
)
