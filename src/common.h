#pragma once

#include <string>

namespace bin2cpp
{
  ///<summary>
  ///Returns the year when the code was build.
  ///Usefull for copyright messages.
  ///</summary>
  ///<return>The year when the function was compiled.<return>
  int getCopyrightYear();

  ///<summary>
  ///Defines if a string value is a numeric value.
  ///</summary>
  ///<return>True when iValue is numeric. False otherwise.<return>
  bool isNumeric(const char * iValue);

  ///<summary>
  ///Replace an occurance of a string by another.
  ///</summary>
  ///<param name="iString">The given string that need to be searched.</param>
  ///<param name="iOldValue">The old value to replace.</param>
  ///<param name="iNewValue">The new value to replace.</param>
  ///<return>Returns the number of token that was replaced.<return>
  int strReplace(std::string & iString, const char * iOldValue, const char * iNewValue);

  ///<summary>
  ///Returns the size of the given FILE* in bytes.
  ///</summary>
  ///<param name="f">An valid FILE pointer.</param>
  ///<return>Returns the size of the given FILE* in bytes.<return>
  long getFileSize(FILE * f);

  ///<summary>
  ///Returns the filename of the given path.
  ///</summary>
  ///<param name="iPath">An valid file path.</param>
  ///<return>Returns the filename of the given path.<return>
  std::string getFilename(const char * iPath);

  ///<summary>
  ///Determine if a file exists.
  ///</summary>
  ///<param name="iFilePath">An valid file path.</param>
  ///<return>Returns true when the file exists. Returns false otherwise.<return>
  bool fileExists(const char * iFilePath);

}; //bin2cpp
