#pragma once

#include "environmentfunc.h"

namespace environment
{

  std::string getEnvironmentVariable(const char * iName)
  {
    if (iName == NULL)
      return std::string();
    const char * value = getenv(iName);
    if (value == NULL)
      return std::string();
    else
      return std::string(value);
  }

}; //environment
