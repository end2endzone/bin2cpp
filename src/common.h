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
  ///Converts a single byte character to the two characters representing this byte in hexadicimal.
  ///For instance, toHexString(64, c1, c2) assigns c1='4' and c2='0'
  ///</summary>
  ///<param name="c">The input byte value</param>
  ///<param name="c1">The first  character of the hexadecimal string.</param>
  ///<param name="c2">The second character of the hexadecimal string.</param>
  ///<return>The the two characters representing the hexadecimal value of c.<return>
  void toHexString(unsigned char c, unsigned char & c1, unsigned char & c2);

  ///<summary>
  ///Converts a single byte character to the 3 characters representing this byte in octal.
  ///For instance, toOctString(83, c1, c2, c3) assigns c1='1', c2='2' and c3='3'
  ///</summary>
  ///<param name="c">The input byte value</param>
  ///<param name="c1">The first  character of the octal string.</param>
  ///<param name="c2">The second character of the octal string.</param>
  ///<param name="c3">The third  character of the octal string.</param>
  ///<return>The the two characters representing the hexadecimal value of c.<return>
  void toOctString(unsigned char c, unsigned char & c1, unsigned char & c2, unsigned char & c3);

  ///<summary>
  ///Returns true if the given character is printable to a console.
  ///</summary>
  ///<param name="c">The input character value</param>
  ///<return>Returns true if the given character is printable to a console.<return>
  bool isPrintableCharacter(const char c);

}; //bin2cpp