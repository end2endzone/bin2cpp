#ifndef STRINGFUNC_H
#define STRINGFUNC_H

#include <stdint.h>
#include <string>
#include <stdio.h>

namespace stringfunc
{

  ///<summary>
  ///Defines if a string value is a numeric value.
  ///A numeric value can be positive or negative.
  ///A numeric value can be an integer or a floating point value.
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
  ///Converts the given value to string.
  ///</summary>
  ///<param name="value">The numeric value.</param>
  ///<return>Converts the given value to string.<return>
  std::string toString(const uint64_t & value);

  ///<summary>
  ///Parse the given string into the given numeric variable.
  ///</summary>
  ///<param name="str">The input string which contains a numeric value.</param>
  ///<param name="value">The output numeric value.</param>
  ///<return>Returns true when the parsing is successful.<return>
  bool parseValue(const std::string& str, uint64_t & oValue);

  ///<summary>
  ///Capitalize the first character of the given string.
  ///</summary>
  ///<param name="iValue">The string value to uppercase the first character.</param>
  std::string capitalizeFirstCharacter(const std::string & iValue);

  ///<summary>
  ///Upper case all characters of the given string.
  ///</summary>
  ///<param name="iValue">The string value to uppercase.</param>
  std::string uppercase(const std::string & value);

  ///<summary>
  ///Lower case all characters of the given string.
  ///</summary>
  ///<param name="iValue">The string value to lowercase.</param>
  std::string lowercase(const std::string & value);

}; //stringfunc

///<summary>
///Streams a value to an existing string.
///</summary>
///<param name="value">The value to append to the given string.</param>
///<return>Returns the given string.<return>
std::string& operator<<(std::string& str, const void * value);

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
std::string& operator<<(std::string& str, const char * value);

///<summary>
///Streams a value to an existing string.
///</summary>
///<param name="value">The value to append to the given string.</param>
///<return>Returns the given string.<return>
std::string& operator<<(std::string& str, const int8_t & value);

///<summary>
///Streams a value to an existing string.
///</summary>
///<param name="value">The value to append to the given string.</param>
///<return>Returns the given string.<return>
std::string& operator<<(std::string& str, const uint8_t & value);

///<summary>
///Streams a value to an existing string.
///</summary>
///<param name="value">The value to append to the given string.</param>
///<return>Returns the given string.<return>
std::string& operator<<(std::string& str, const int16_t & value);

///<summary>
///Streams a value to an existing string.
///</summary>
///<param name="value">The value to append to the given string.</param>
///<return>Returns the given string.<return>
std::string& operator<<(std::string& str, const uint16_t & value);

///<summary>
///Streams a value to an existing string.
///</summary>
///<param name="value">The value to append to the given string.</param>
///<return>Returns the given string.<return>
std::string& operator<<(std::string& str, const int32_t & value);

///<summary>
///Streams a value to an existing string.
///</summary>
///<param name="value">The value to append to the given string.</param>
///<return>Returns the given string.<return>
std::string& operator<<(std::string& str, const uint32_t & value);

///<summary>
///Streams a value to an existing string.
///</summary>
///<param name="value">The value to append to the given string.</param>
///<return>Returns the given string.<return>
std::string& operator<<(std::string& str, const int64_t & value);

///<summary>
///Streams a value to an existing string.
///</summary>
///<param name="value">The value to append to the given string.</param>
///<return>Returns the given string.<return>
std::string& operator<<(std::string& str, const uint64_t & value);

#endif //STRINGFUNC_H