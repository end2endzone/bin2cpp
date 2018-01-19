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

  ///<summary>
  ///Returns value of an environment variable.
  ///</summary>
  ///<param name="iName">Name of the variable</param>
  ///<return>Returns the value of the given environment variable.<return>
  std::string getEnvironmentVariable(const char * iName);

  ///<summary>
  ///Returns the file name of a tempporary file.
  ///</summary>
  ///<return>Returns the file name of a tempporary file.<return>
  std::string getTemporaryFileName();

  ///<summary>
  ///Returns the path of a tempporary file.
  ///</summary>
  ///<return>Returns the path of a tempporary file.<return>
  std::string getTemporaryFilePath();

  /*
  Description:
    Returns the application version number.
  */
  const char * getVersionString();

  ///<summary>
  ///Returns the parent element of a path. For files, returns the file's directory. For folders, returns the parent path
  ///</summary>
  ///<param name="iPath">The input path to split.</param>
  ///<return>Returns the parent element of a path.<return>
  std::string getParentPath(const std::string & iPath);

  ///<summary>
  ///Splits a path into a folder and a filename.
  ///</summary>
  ///<param name="iPath">The input path to split.</param>
  ///<param name="oFolder">The output folder of the given path.</param>
  ///<param name="oFile">The output file of the given path.</param>
  ///<return>Returns a the folder and the filename of the given path.<return>
  void splitPath(const std::string & iPath, std::string & oFolder, std::string & oFilename);

  ///<summary>
  ///Gets the character that represents the path separator.
  ///</summary>
  ///<return>Returns the character that represents the path separator.<return>
  char getPathSeparator();

  ///<summary>
  ///Returns the current folder
  ///</summary>
  ///<return>Returns the current folder<return>
  std::string getCurrentFolder();

  ///<summary>
  ///Returns the extension of a file.
  ///</summary>
  ///<param name="iPath">The valid path to a file.</param>
  ///<return>Returns the extension of a file.<return>
  std::string getFileExtention(const std::string & iPath);
  
  enum FileSizeEnum {Bytes, Kilobytes, Megabytes, Gigabytes, Terabytes};

  ///<summary>
  ///Returns a given size in a user friendly format and units.
  ///</summary>
  ///<param name="iPath">The size in bytes</param>
  ///<return>Returns a given size in a user friendly format and units.<return>
  std::string getUserFriendlySize(uint64_t iBytesSize);

  ///<summary>
  ///Returns a given size in a user friendly format and units.
  ///</summary>
  ///<param name="iPath">The size in bytes</param>
  ///<return>Returns a given size in a user friendly format and units.<return>
  std::string getUserFriendlySize(uint64_t iBytesSize, FileSizeEnum iUnit);

  ///<summary>
  ///Returns the modified date of the given file.
  ///Note that the function returns the number of seconds elapsed since epoch since Jan 1st 1970.
  ///</summary>
  ///<param name="iPath">The valid path to a file.</param>
  ///<return>Returns the modified date of the given file.<return>
  uint64_t getFileModifiedDate(const std::string & iPath);

  ///<summary>
  ///Returns the modified date from an embedded file's c++ header/source file.
  ///Note that the function returns the number of seconds elapsed since epoch since Jan 1st 1970.
  ///</summary>
  ///<param name="iPath">The valid path to a c++ header/source file.</param>
  ///<return>Returns the modified date from an embedded file's c++ header/source file.<return>
  uint64_t getOutputFileModifiedDate(const std::string & iPath);

  ///<summary>
  ///Converts the given value to string.
  ///</summary>
  ///<param name="value">The numeric value.</param>
  ///<return>Converts the given value to string.<return>
  std::string toString(const uint64_t & value);

  ///<summary>
  ///Streams a value to an existing string.
  ///</summary>
  ///<param name="value">The value to append to the given string.</param>
  ///<return>Returns the given string.<return>
  std::string& operator<<(std::string& str, const std::string & value);

  ///<summary>
  ///Streams a value to an existing string.
  ///</summary>
  ///<param name="value">The value to append to the given string.</param>
  ///<return>Returns the given string.<return>
  std::string& operator<<(std::string& str, const int & value);

  ///<summary>
  ///Streams a value to an existing string.
  ///</summary>
  ///<param name="value">The value to append to the given string.</param>
  ///<return>Returns the given string.<return>
  std::string& operator<<(std::string& str, const size_t & value);

  ///<summary>
  ///Streams a value to an existing string.
  ///</summary>
  ///<param name="value">The value to append to the given string.</param>
  ///<return>Returns the given string.<return>
  std::string& operator<<(std::string& str, const uint64_t & value);

  ///<summary>
  ///Parse the given string into the given numeric variable.
  ///</summary>
  ///<param name="str">The input string which contains a numeric value.</param>
  ///<param name="value">The output numeric value.</param>
  ///<return>Returns true when the parsing is successful.<return>
  bool parseValue(const std::string& str, uint64_t & oValue);

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

  ///<summary>
  ///Sleep the current thread by iTimeMs milliseconds.
  ///</summary>
  ///<param name="iTimeMs">The number of milliseconds to sleep.</param>
  void sleep(const size_t & iTimeMs);

}; //bin2cpp
