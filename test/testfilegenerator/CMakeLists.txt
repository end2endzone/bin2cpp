add_executable(testfilegenerator
  ${BIN2CPP_VERSION_HEADER}
  ${BIN2CPP_CONFIG_HEADER}
  samples.txt
  testfilegenerator.cpp
)

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(testfilegenerator PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_link_libraries(testfilegenerator PRIVATE rapidassist)

install(TARGETS testfilegenerator
        EXPORT bin2cpp-targets
        ARCHIVE DESTINATION ${BIN2CPP_INSTALL_LIB_DIR}
        LIBRARY DESTINATION ${BIN2CPP_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${BIN2CPP_INSTALL_BIN_DIR}
)
