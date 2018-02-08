#pragma once

#include "logger.h"

#include <sstream>
#include <stdarg.h>
#include <shlobj.h>

namespace logger
{
  bool gQuietMode = false;

  void setQuietMode(bool iQuiet)
  {
    gQuietMode = iQuiet;
  }

  bool isQuietModeEnabled()
  {
    return gQuietMode;
  }

  void log(LOGGER_LEVEL iLevel, const char * iFormat, ...)
  {
    std::string logstring;

    //convert arguments to a single string
    va_list args;
    va_start(args, iFormat);
    static const int BUFFER_SIZE = 10240;
    char buffer[BUFFER_SIZE];
    buffer[0] = '\0';
    vsprintf_s(buffer, BUFFER_SIZE, iFormat, args);
    logstring = buffer;
    va_end (args);

    //print the single string to the console
    if (iLevel == LOG_INFO && gQuietMode)
      return; //silence the output

    if (iLevel == LOG_ERROR)
    {
      printf("Error: %s\n", logstring.c_str());
    }
    else if (iLevel == LOG_WARNING)
    {
      printf("Warning: %s\n", logstring.c_str());
    }
    else
    {
      printf("%s\n", logstring.c_str());
    }
  }

}; //logger
