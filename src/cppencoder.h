#pragma once

#include <string>

namespace cppencoder
{
  ///<summary>
  ///Returns true if the given character is printable to a console.
  ///</summary>
  ///<param name="c">The input character value</param>
  ///<return>Returns true if the given character is printable to a console.<return>
  bool isPrintableCharacter(const char c);

  ///<summary>
  ///Returns true if the given character is a string literal control character.
  ///A control character must be escaped with \ to be inserted as a literal character.
  ///</summary>
  ///<param name="c">The given character</param>
  ///<return>Returns true if the given character is a string literal control character. Returns false otherwise.<return>
  bool isControlCharacter(char c);

  ///<summary>
  ///Returns a string literal that contains the control character.
  ///Returns NULL if the given character is not a control character.
  ///See isControlCharacter() for details.
  ///</summary>
  ///<param name="c">The given character</param>
  ///<return>Returns a string literal that contains the control character. Returns NULL if the given character is not a control character.<return>
  const char * getControlCharacterEscapeString(char c);

  ///<summary>
  ///Returns true if the given character is an hexadecimal character.
  ///A hexadecimal character is the number '0' to '9' or the letter 'A' to 'F' (including 'a' to 'f')
  ///</summary>
  ///<param name="c">The given character</param>
  ///<return>Returns true if the given character is an hexadecimal character. Returns false otherwise.<return>
  bool isHexCharacter(char c);

  ///<summary>
  ///Returns true if the given character is a digit character.
  ///A digit character is the number '0' to '9'.
  ///</summary>
  ///<param name="c">The given character</param>
  ///<return>Returns true if the given character is a digit character. Returns false otherwise.<return>
  bool isDigitCharacter(char c);

  ///<summary>
  ///Returns a string literal that contains an octal escape sequence of the given character.
  ///The returned buffer is private to the function and must be copied to a local variable before the next call to the function.
  ///</summary>
  ///<param name="c">The given character</param>
  ///<return>Returns a string literal that contains an octal escape sequence of the given character.<return>
  const char * toOctString(unsigned char c);

  ///<summary>
  ///Returns a string literal that contains an hexadecimal escape sequence of the given character.
  ///The returned buffer is private to the function and must be copied to a local variable before the next call to the function.
  ///</summary>
  ///<param name="c">The given character</param>
  ///<return>Returns a string literal that contains an hexadecimal escape sequence of the given character.<return>
  const char * toHexString(unsigned char c);

  ///<summary>
  ///Converts the given buffer to a c++ string literals.
  ///All printable characters are printed in the string as is.
  ///Characters that matches a known escaped sequence are properly escaped.
  ///Other characters are printed using their octal representation.
  ///</summary>
  ///<remarks>For a shorter string literal, see other toOctString() API with iDisableWarningC4125 set to false.</remarks>
  ///<param name="iBuffer">The input buffer to convert</param>
  ///<param name="iSize">The size in bytes of the given buffer</param>
  ///<return>Returns the string literals of the given buffer<return>
  std::string toOctString(const unsigned char * iBuffer, size_t iSize);

  ///<summary>
  ///Converts the given buffer to a c++ string literals.
  ///All printable characters are printed in the string as is.
  ///Characters that matches a known escaped sequence are properly escaped.
  ///Other characters are printed using their octal representation.
  ///</summary>
  ///<param name="iBuffer">The input buffer to convert</param>
  ///<param name="iSize">The size in bytes of the given buffer</param>
  ///<param name="iDisableWarningC4125">
  ///Disable generation of code which could generate compilation warning C4125 (decimal digit terminates octal escape sequence)
  ///The following string would generate warning C4125: const char * buffer = "foo\12345bar";
  ///Allowing C4125 warning result in a shorter string literal.
  ///</param>
  ///<return>Returns the string literals of the given buffer<return>
  std::string toOctString(const unsigned char * iBuffer, size_t iSize, bool iDisableWarningC4125);

  ///<summary>
  ///Converts the given buffer to a c++ string literals.
  ///All printable characters are printed in the string as is.
  ///Characters that matches a known escaped sequence are properly escaped.
  ///Other characters are printed using their hexadecimal representation.
  ///</summary>
  ///<param name="iBuffer">The input buffer to convert</param>
  ///<param name="iSize">The size in bytes of the given buffer</param>
  ///<return>Returns the string literals of the given buffer<return>
  std::string toHexString(const unsigned char * iBuffer, size_t iSize);

  ///<summary>
  ///Converts the given buffer to an array of characters.
  ///All printable characters are printed in the array as characters (ie 'a').
  ///Other characters are printed using their hexadecimal representation.
  ///</summary>
  ///<param name="iBuffer">The input buffer to convert</param>
  ///<param name="iSize">The size in bytes of the given buffer</param>
  ///<return>Returns a string representating an array of bytes which matches the given buffer<return>
  std::string toCppCharactersArray(const unsigned char * iBuffer, size_t iSize);

}; //cppencoder
