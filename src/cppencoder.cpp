#pragma once

#include "cppencoder.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>
#include <sstream>

//http://stackoverflow.com/questions/10057258/how-does-x-work-in-a-string
//http://en.cppreference.com/w/cpp/language/escape

namespace cppencoder
{
  struct CONTROL_CHARACTER
  {
    char c;
    const char * escapeStr;
  };
  //https://stackoverflow.com/questions/10220401/rules-for-c-string-literals-escape-character
  static const CONTROL_CHARACTER gCtrlChars[] = {
    {'\0',"\\0"},   //0x00, null
    {'\a',"\\a"},   //0x07, alert (bell)
    {'\b',"\\b"},   //0x08, backspace
    {'\t',"\\t"},   //0x09, horizontal tab
    {'\n',"\\n"},   //0x0A, newline (or line feed)
    {'\v',"\\v"},   //0x0B, vertical tab
    {'\f',"\\f"},   //0x0C, form feed
    {'\r',"\\r"},   //0x0D, carriage return
    //{'\e',"\\e"}, //0x1B, //escape.  VS2010: warning C4129: 'e' : unrecognized character escape sequence
    {'\"',"\\\""},  //0x22
    {'\'',"\\\'"},  //0x27
    {'\?',"\\\?"},  //0x3F
    {'\\',"\\\\"},  //0x5C
  };
  static const size_t gNumCtrlChars = sizeof(gCtrlChars)/sizeof(gCtrlChars[0]);

  bool isPrintableCharacter(const char c)
  {
    if (c == 39) // character ' must be escaped with \' which is not supported right now
      return false;
    if (c == 92) // character \ must be escaped with \\ which is not supported right now
      return false;
    if (c >= 32 && c<= 126)
      return true;
    return false;
  }

  bool isControlCharacter(char c)
  {
    for(size_t i=0; i<gNumCtrlChars; i++)
    {
      const CONTROL_CHARACTER & ctrl = gCtrlChars[i];
      if (ctrl.c == c)
        return true;
    }
    return false;
  }

  bool isHexCharacter(char c)
  {
    if (c >= '0' && c <= '9')
      return true;
    if (c >= 'a' && c <= 'f')
      return true;
    if (c >= 'A' && c <= 'F')
      return true;
    return false;
  }

  bool isDigitCharacter(char c)
  {
    if (c >= '0' && c <= '9')
      return true;
    return false;
  }

  const char * getControlCharacterEscapeString(char c)
  {
    for(size_t i=0; i<gNumCtrlChars; i++)
    {
      const CONTROL_CHARACTER & ctrl = gCtrlChars[i];
      if (ctrl.c == c)
        return ctrl.escapeStr;
    }
    return NULL;
  }

  const char * toOctString(unsigned char c)
  {
    static char buffer[] = {'\\', '9', '9', '9', '\0'};
    static const char * octCharacters = "01234567";
    buffer[3] = octCharacters[c%8];
    c /= 8;
    buffer[2] = octCharacters[c%8];
    c /= 8;
    buffer[1] = octCharacters[c];
    return buffer;
  }

  const char * toHexString(unsigned char c)
  {
    static char buffer[] = {'\\', 'x', 'f', 'f', '\0'};
    static const char * hexCharacters = "0123456789abcdef";
    buffer[3] = hexCharacters[c%16];
    c /= 16;
    buffer[2] = hexCharacters[c];
    return buffer;
  }

  std::string toOctString(const unsigned char * iBuffer, size_t iSize)
  {
    return toOctString(iBuffer, iSize, true);
  }

  std::string toOctString(const unsigned char * iBuffer, size_t iSize, bool iDisableWarningC4125)
  {
    std::string output;

    //estimate the size of the output string to prevent memory copy
    //assume 50% of buffer is *NOT* printable
    size_t nonPrintableSize = ((iSize*50)/100);
    size_t estimatedStringSize = iSize - nonPrintableSize + nonPrintableSize*4; //4 bytes per octal characters
    output.reserve(estimatedStringSize);

    enum CHARACTER_TYPE
    {
      OCTAL,
      CONTROL,
      PRINTABLE,
    };

    CHARACTER_TYPE previous = PRINTABLE;
    for(size_t i=0; i<iSize; i++)
    {
      unsigned char c = iBuffer[i];
      unsigned char next = iBuffer[i+1];
      if (i+1 == iSize) //if out of scope
        next = '\0';

      if (c == 0 && !isDigitCharacter(next))
      {
        //safe to encode NULL character as '\0' instead of '\000'
        output.append(getControlCharacterEscapeString(c));
        previous = OCTAL;
      }
      else if (c == 0)
      {
        output.append(toOctString(c));
        previous = OCTAL;
      }
      else if (isControlCharacter(c))
      {
        output.append(getControlCharacterEscapeString(c));
        previous = CONTROL;
      }
      else if (iDisableWarningC4125 && previous == OCTAL && isDigitCharacter(c) ) //prevent warning C4125: decimal digit terminates octal escape sequence
      {
        //character must be encoded as octal instead of printable
        output.append(toOctString(c));
        previous = OCTAL;
      }
      else if (isPrintableCharacter(c))
      {
        output.append(1, c);
        previous = PRINTABLE;
      }
      else
      {
        output.append(toOctString(c));
        previous = OCTAL;
      }
    }

    return output;
  }

  std::string toHexString(const unsigned char * iBuffer, size_t iSize)
  {
    std::string output;

    //estimate the size of the output string to prevent memory copy
    //assume 50% of buffer is *NOT* printable
    size_t nonPrintableSize = ((iSize*50)/100);
    size_t estimatedStringSize = iSize - nonPrintableSize + nonPrintableSize*4; //4 bytes per hex characters
    output.reserve(estimatedStringSize);

    enum CHARACTER_TYPE
    {
      HEX,
      CONTROL,
      PRINTABLE,
    };

    CHARACTER_TYPE previous = PRINTABLE;
    for(size_t i=0; i<iSize; i++)
    {
      unsigned char c = iBuffer[i];
      unsigned char next = iBuffer[i+1];
      if (i+1 == iSize) //if out of scope
        next = '\0';

      if (c == 0 && !isDigitCharacter(next))
      {
        //safe to encode NULL character as '\0' instead of '\000'
        output.append(getControlCharacterEscapeString(c));
        previous = CONTROL;
      }
      else if (c == 0)
      {
        output.append(toHexString(c));
        previous = HEX;
      }
      else if (isControlCharacter(c))
      {
        output.append(getControlCharacterEscapeString(c));
        previous = CONTROL;
      }
      else if (previous == HEX && isHexCharacter(c)) //an hexadecimal letter cannot follow an hexadecimal escape sequence.
      {
        //must also be printed as an hexadecimal escape sequence
        //http://stackoverflow.com/questions/5784969/when-did-c-compilers-start-considering-more-than-two-hex-digits-in-string-lite
        output.append(toHexString(c));
        previous = HEX;
      }
      else if (isPrintableCharacter(c))
      {
        output.append(1, c);
        previous = PRINTABLE;
      }
      else
      {
        output.append(toHexString(c));
        previous = HEX;
      }
    }

    return output;
  }

  std::string toCppCharactersArray(const unsigned char * iBuffer, size_t iSize)
  {
    std::ostringstream oss;

    for(size_t i=0; i<iSize; i++)
    {
      unsigned char c = iBuffer[i];

      if (isPrintableCharacter((char)c))
        oss << '\'' << (char)c << '\'';
      else
        oss << (int)c; //print as decimal value

      size_t lastByteIndex = iSize-1;

      if (i != lastByteIndex)
        oss << ",";
    }

    return oss.str();
  }

}; //cppencoder
