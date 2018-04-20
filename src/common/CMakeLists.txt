add_library(common STATIC
  argumentparser.cpp
  argumentparser.h
  common.cpp
  common.h
  cppencoder.cpp
  cppencoder.h
  environmentfunc.cpp
  environmentfunc.h
  filesystemfunc.cpp
  filesystemfunc.h
  logger.cpp
  logger.h
  nativefunc.cpp
  nativefunc.h
  stringfunc.cpp
  stringfunc.h
)

if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
