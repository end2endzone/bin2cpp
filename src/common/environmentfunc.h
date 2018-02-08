#pragma once

#include <string>

namespace environment
{

  ///<summary>
  ///Returns value of an environment variable.
  ///</summary>
  ///<param name="iName">Name of the variable</param>
  ///<return>Returns the value of the given environment variable.<return>
  std::string getEnvironmentVariable(const char * iName);

}; //environment
