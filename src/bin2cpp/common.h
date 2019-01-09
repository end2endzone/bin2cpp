#pragma once

#include <stdint.h>
#include <string>
#include <stdio.h>

namespace bin2cpp
{
  ///<summary>
  ///Returns the year when the code was build.
  ///Usefull for copyright messages.
  ///</summary>
  ///<return>The year when the function was compiled.<return>
  int getCopyrightYear();

  /*
  Description:
    Returns the application version number.
  */
  const char * getVersionString();

  ///<summary>
  ///Returns the modified date from an embedded file's c++ header/source file.
  ///Note that the function returns the number of seconds elapsed since epoch since Jan 1st 1970.
  ///</summary>
  ///<param name="iPath">The valid path to a c++ header/source file.</param>
  ///<return>Returns the modified date from an embedded file's c++ header/source file.<return>
  uint64_t getOutputFileModifiedDate(const std::string & iPath);

  ///<summary>
  ///Determine if a given path is a c++ header file.
  ///</summary>
  ///<param name="iPath">An valid file path.</param>
  ///<return>Returns true if iPath is a c++ header file. Returns false otherwise.<return>
  bool isCppHeaderFile(const std::string & iPath);

  ///<summary>
  ///Determine if a given path is a c++ source file.
  ///</summary>
  ///<param name="iPath">An valid file path.</param>
  ///<return>Returns true if iPath is a c++ source file. Returns false otherwise.<return>
  bool isCppSourceFile(const std::string & iPath);

}; //bin2cpp
