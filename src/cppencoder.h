#pragma once

#include <string>

namespace bin2cpp
{
  ///<summary>
  ///Converts the given buffer to a c++ string literals.
  ///All printable characters are printed in the string as is.
  ///Characters that matches a known escaped sequence are properly escaped.
  ///Other characters are printed using their octal representation.
  ///</summary>
  ///<param name="iBuffer">The input buffer to convert</param>
  ///<param name="iSize">The size in bytes of the given buffer</param>
  ///<return>Returns the string literals of the given buffer<return>
  std::string toCppString(const unsigned char * iBuffer, size_t iSize);

  ///<summary>
  ///Converts the given buffer to an array of characters.
  ///All printable characters are printed in the array as characters (ie 'a').
  ///Other characters are printed using their hexadecimal representation.
  ///</summary>
  ///<param name="iBuffer">The input buffer to convert</param>
  ///<param name="iSize">The size in bytes of the given buffer</param>
  ///<return>Returns a string representating an array of bytes which matches the given buffer<return>
  std::string toCppCharactersArray(const unsigned char * iBuffer, size_t iSize);

}; //bin2cpp
