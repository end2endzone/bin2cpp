#pragma once

#include "cppencoder.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>
#include <sstream>

namespace bin2cpp
{
  std::string gHexSymbols[256];
  std::string gSmallestSymbols[256];
  std::string gOctSymbols[256];

  void initCppSymbols()
  {
    //Build default symbols
    static const int BUFFER_SIZE = 10;
    unsigned char buffer[BUFFER_SIZE];
    for(unsigned int i=0; i<=255; i++)
    {
      //hex
      buffer[0] = '\\';
      buffer[1] = 'x';
      toHexString((unsigned char)i, buffer[2], buffer[3]);
      buffer[4] = 0;
      //sprintf((char*)buffer, "\\x%02x", (unsigned char)iBuffer[i]);
      gHexSymbols[i] = (char*)buffer;

      //oct
      buffer[0] = '\\';
      toOctString((unsigned char)i, buffer[1], buffer[2], buffer[3]);
      buffer[4] = 0;
      gOctSymbols[i] = (char*)buffer;

      //smallest (uses HEX format as must as possible)
      gSmallestSymbols[i] = gHexSymbols[i];
    }

    //http://stackoverflow.com/questions/5784969/when-did-c-compilers-start-considering-more-than-two-hex-digits-in-string-lite
    //http://stackoverflow.com/questions/10057258/how-does-x-work-in-a-string
    //http://en.cppreference.com/w/cpp/language/escape

    //Build smallest symbol optimizations (printable characters)
    for(unsigned int i=32; i<=126; i++)
    {
      buffer[0] = i;
      buffer[1] = '\0';
      gSmallestSymbols[i] = (char*)buffer;
    }

    //Control characters:
    //https://stackoverflow.com/questions/10220401/rules-for-c-string-literals-escape-character
    gSmallestSymbols[0x00] = "\\0";
    gSmallestSymbols[0x07] = "\\a";   //allert (bell)
    gSmallestSymbols[0x08] = "\\b";   //backspace
    gSmallestSymbols[0x09] = "\\t";   //horizonal tab
    gSmallestSymbols[0x0A] = "\\n";   //newline (or line feed)
    gSmallestSymbols[0x0B] = "\\v";   //vertical tab
    gSmallestSymbols[0x0C] = "\\f";   //form feed
    gSmallestSymbols[0x0D] = "\\r";   //carriage return
    //gSmallestSymbols[0x1B] = "\\e";   //escape.  VS2010: warning C4129: 'e' : unrecognized character escape sequence

    gSmallestSymbols['\"'] = "\\\"";
    gSmallestSymbols['\''] = "\\\'";
    gSmallestSymbols['\?'] = "\\\?";
    gSmallestSymbols['\\'] = "\\\\";
  }

  std::string toCppString(const unsigned char * iBuffer, size_t iSize)
  {
    static bool firstPass = true;
    if (firstPass)
    {
      initCppSymbols();
      firstPass = false;
    }

    //Build an array of choices between HEX, OCT and BEST format
    enum Format { FormatSmallest, FormatHex, FormatOct };
    Format * formats = new Format[iSize];

    //default all buffer character to use the smallest format
    for(size_t i=0; i<iSize; i++)
    {
      const std::string * smallestRepresentation = &gSmallestSymbols[iBuffer[i]];
      const std::string *      hexRepresentation =      &gHexSymbols[iBuffer[i]];
      const std::string *      octRepresentation =      &gOctSymbols[iBuffer[i]];
      formats[i] = FormatSmallest;
      if ( *smallestRepresentation == *octRepresentation)
        formats[i] = FormatOct;
      if ( *smallestRepresentation == *hexRepresentation)
        formats[i] = FormatHex;
    }

    //a single character representation cannot follow an HEX representation
    for(size_t i=1; i<iSize; i++)
    {
      //get this character representation
      const std::string * charRepresentation = NULL;
      switch(formats[i])
      {
      case FormatSmallest:
        charRepresentation = &gSmallestSymbols[iBuffer[i]];
        break;
      case FormatOct:
        charRepresentation =      &gOctSymbols[iBuffer[i]];
        break;
      case FormatHex:
        charRepresentation =      &gHexSymbols[iBuffer[i]];
        break;
      };

      //if current element is a single character representation
      if (charRepresentation->size() == 1)
      {
        //the preceding representation must be an oct representation or another single character
        if (
          formats[i-1] == FormatOct || 
          (formats[i-1] == FormatSmallest && gSmallestSymbols[iBuffer[i]].size() == 1)
          )
        {
          //fine
        }
        else
        {
          //force OCT format
          formats[i-1] = FormatOct;
        }
      }
    }

    //a NULL character cannot be followed by a single character if it is between 0 and 9
    for(size_t i=0; i<iSize-1; i++)
    {
      unsigned char c0 = iBuffer[i];
      unsigned char c1 = iBuffer[i+1];
      if (c0 == '\0' && (c1 >= '0' && c1 <= '9'))
        formats[i] = FormatOct;
    }

    std::string output;
    for(size_t i=0; i<iSize; i++)
    {
      //get this character representation
      const std::string * charRepresentation = NULL;
      switch(formats[i])
      {
      case FormatSmallest:
        charRepresentation = &gSmallestSymbols[iBuffer[i]];
        break;
      case FormatOct:
        charRepresentation =      &gOctSymbols[iBuffer[i]];
        break;
      case FormatHex:
        charRepresentation =      &gHexSymbols[iBuffer[i]];
        break;
      };

      //append
      output.append(charRepresentation->c_str());
    }

    delete[] formats;

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


}; //bin2cpp
