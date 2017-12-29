#include "SegmentGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>

#include "common.h"
#include "md5support.h"
#include "md5.h"
#include "md5Ex.h"

namespace bin2cpp
{
  namespace SegmentGeneratorUtils
  {
    std::string gHexSymbols[256];
    std::string gSmallestSymbols[256];
    std::string gOctSymbols[256];

    std::string getGetterFunctionName(const char * iFunctionIdentifier)
    {
      std::string getter;
      getter.append("get");
      getter.append(iFunctionIdentifier);
      getter.append("File");
      return getter;
    }

    void toHexString(unsigned char c, unsigned char & c1, unsigned char & c2)
    {
      static const char * hexCharacters = "0123456789abcdef";

      c1 = hexCharacters[c/16];
      c2 = hexCharacters[c%16];
    }

    void toOctString(unsigned char c, unsigned char & c1, unsigned char & c2, unsigned char & c3)
    {
      static const char * octCharacters = "01234567";
      c3 = octCharacters[c%8];
      c /= 8;
      c2 = octCharacters[c%8];
      c /= 8;
      c1 = octCharacters[c];
    }

    std::string toCppString(const char * iValue)
    {
      std::string tmp = iValue;
      strReplace(tmp, "\\", "\\\\");
      strReplace(tmp, "\"", "\\\"");
      return tmp;
    }

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

    bin2cpp::ErrorCodes createHeaderEmbededFile(const char * iOutputHeaderFilename, const char * iFunctionIdentifier)
    {
      FILE * header = fopen(iOutputHeaderFilename, "w");
      if (!header)
        return bin2cpp::ErrorCodes::UnableToCreateOutputFiles;

      fprintf(header, "/**\n");
      fprintf(header, " * This file was generated by bin2cpp v%s.\n", getVersionString() );
      fprintf(header, " * Copyright (C) 2013-%d end2endzone.com. All rights reserved.\n", bin2cpp::getCopyrightYear());
      fprintf(header, " * Do not modify this file.\n");
      fprintf(header, " */\n");
      fprintf(header, "#pragma once\n");
      fprintf(header, "#include <stddef.h>\n");
      fprintf(header, "namespace bin2cpp\n");
      fprintf(header, "{\n");
      fprintf(header, "  #ifndef BIN2CPP_EMBEDDEDFILE_CLASS\n");
      fprintf(header, "  #define BIN2CPP_EMBEDDEDFILE_CLASS\n");
      fprintf(header, "  class File\n");
      fprintf(header, "  {\n");
      fprintf(header, "  public:\n");
      fprintf(header, "    virtual size_t getSize() const = 0;\n");
      fprintf(header, "    virtual size_t getSegmentSize() const = 0;\n");
      fprintf(header, "    virtual size_t getNumSegments() const = 0;\n");
      fprintf(header, "    virtual const char * getFilename() const = 0;\n");
      fprintf(header, "    virtual const char * getSegment(size_t iIndex, size_t & oLength) const = 0;\n");
      fprintf(header, "    virtual const char * getMd5() const = 0;\n");
      fprintf(header, "    virtual char * newBuffer() const = 0;\n");
      fprintf(header, "    virtual bool save(const char * iFilename) const = 0;\n");
      fprintf(header, "  };\n");
      fprintf(header, "  #endif\n");
      fprintf(header, "  const File & %s();\n", getGetterFunctionName(iFunctionIdentifier).c_str());
      fprintf(header, "}; //bin2cpp\n");

      fclose(header);

      return bin2cpp::ErrorCodes::Success;
    }

  }; //SegmentGeneratorUtils

  SegmentGenerator::SegmentGenerator()
  {
  }

  SegmentGenerator::~SegmentGenerator()
  {
  }

  bin2cpp::ErrorCodes SegmentGenerator::createCppEmbeddedFile(const char * iInputFilename, const char * iOutputFolder, const char * iHeaderFilename, const char * iFunctionIdentifier, size_t iChunkSize, bool iOverrideExisting)
  {
    //check if input file exists
    FILE * input = fopen(iInputFilename, "rb");
    if (!input)
      return bin2cpp::ErrorCodes::InputFileNotFound;

    //Uppercase function identifier
    std::string functionIdentifier = iFunctionIdentifier;
    functionIdentifier[0] = toupper(functionIdentifier[0]);

    //Build header file path
    std::string headerPath;
    headerPath.append(iOutputFolder);
    if (headerPath[headerPath.length()-1] != '\\')
      headerPath.append("\\");
    headerPath.append(iHeaderFilename);

    //Build cpp file file
    std::string outputCppFilename = headerPath;
    strReplace(outputCppFilename, ".h", ".cpp");

    //check if output files already exists
    if (fileExists(headerPath.c_str()) || fileExists(outputCppFilename.c_str()))
    {
      //compute input/output file md5
      std::string inputFileHexDigest = getFileHexDigest(iInputFilename);
      std::string sourceFileHexDigest = getGeneratedFileHexDigest(outputCppFilename.c_str());
      if (inputFileHexDigest == sourceFileHexDigest)
      {
        //md5 identical
        return bin2cpp::ErrorCodes::OutputFilesSkipped;
      }

      //md5 not identical
      if (!iOverrideExisting)
        return bin2cpp::ErrorCodes::OutputFilesAlreadyExist;
    }

    //create Header file
    bin2cpp::ErrorCodes headerResult = bin2cpp::SegmentGeneratorUtils::createHeaderEmbededFile(headerPath.c_str(), functionIdentifier.c_str());
    if (headerResult != bin2cpp::ErrorCodes::Success)
    {
      fclose(input);
      return headerResult;
    }

    //create cpp file
    FILE * cpp = fopen(outputCppFilename.c_str(), "w");
    if (!cpp)
    {
      fclose(input);
      return bin2cpp::ErrorCodes::UnableToCreateOutputFiles;
    }

    //determine segment size
    long fileSize = getFileSize(input);
    long lastSegmentSize = fileSize%iChunkSize;
    size_t numSegments = fileSize/iChunkSize + (lastSegmentSize == 0 ? 0 : 1);

    //Build class name
    std::string className;
    className.append(functionIdentifier.c_str());
    className.append("File");

    //Build function 
    std::string getterFunctionName = bin2cpp::SegmentGeneratorUtils::getGetterFunctionName(functionIdentifier.c_str());

    //write cpp file heading
    fprintf(cpp, "/**\n");
    fprintf(cpp, " * This file was generated by bin2cpp v%s.\n", getVersionString() );
    fprintf(cpp, " * Copyright (C) 2013-%d end2endzone.com. All rights reserved.\n", bin2cpp::getCopyrightYear());
    fprintf(cpp, " * Do not modify this file.\n");
    fprintf(cpp, " */\n");
    fprintf(cpp, "#include \"%s\"\n", iHeaderFilename );
    fprintf(cpp, "#include <stdio.h> //for FILE\n");
    fprintf(cpp, "#include <string> //for memcpy\n");
    fprintf(cpp, "namespace bin2cpp\n");
    fprintf(cpp, "{\n");
    fprintf(cpp, "  class %s : public virtual bin2cpp::File\n", className.c_str());
    fprintf(cpp, "  {\n");
    fprintf(cpp, "  public:\n");
    fprintf(cpp, "    %s() {}\n", className.c_str());
    fprintf(cpp, "    ~%s() {}\n", className.c_str());
    fprintf(cpp, "    virtual size_t getSize() const { return %d; }\n", fileSize);
    fprintf(cpp, "    virtual size_t getSegmentSize() const { return %d; }\n", iChunkSize);
    fprintf(cpp, "    virtual size_t getNumSegments() const { return %d; }\n", numSegments);
    fprintf(cpp, "    virtual const char * getFilename() const { return \"%s\"; }\n", getFilename(iInputFilename).c_str());
    fprintf(cpp, "    virtual const char * getSegment(size_t iIndex, size_t & oLength) const\n");
    fprintf(cpp, "    {\n");
    fprintf(cpp, "      oLength = 0;\n");
    fprintf(cpp, "      if (iIndex >= getNumSegments())\n");
    fprintf(cpp, "        return NULL;\n");
    fprintf(cpp, "      const char * buffer = NULL;\n");
    fprintf(cpp, "      size_t index = 0;\n");
    fprintf(cpp, "      oLength = getSegmentSize();\n");

    //Compute MD5 while generating cpp code
    MD5_CTX context;
    MD5Init(&context);

    //create buffer for each chunks from input buffer
    unsigned char * buffer = new unsigned char[iChunkSize];
    while(!feof(input))
    {
      //read a chunk of the file
      size_t readSize = fread(buffer, 1, iChunkSize, input);

      bool isLastChunk = !(readSize == iChunkSize);

      if (isLastChunk || readSize == 0)
        fprintf(cpp, "      oLength = %d;\n", lastSegmentSize);

      if (readSize == 0)
        continue; //nothing to output if nothing was read

      //send to MD5 for analysist
      MD5Update(&context, buffer, readSize);

      //output mode 1
      fprintf(cpp, "      buffer = \"%s\"; if (iIndex == index) return buffer; index++;\n", bin2cpp::SegmentGeneratorUtils::toCppString(buffer, readSize).c_str(), readSize);

      //output mode 2. DOES NOT COMPILE!
      //fprintf(cpp, "  { const unsigned char buffer[] = {%s}; fwrite(buffer, 1, %d, f); }\n", arrayToCppArray((unsigned char *)buffer, readSize).c_str(), readSize);

      ////output mode 3 (slower than mode 1)
      //static bool firstPass = true;
      //if (firstPass)
      //    fprintf(cpp, "  unsigned char buffer[%d];\n", iChunkSize);
      //firstPass = false;
      //for(size_t i=0; i<readSize; i++)
      //{
      //  fprintf(cpp, "  buffer[%d] = %s;\n", i, arrayToCppArray((unsigned char *)&buffer[i], 1).c_str());
      //}
      //fprintf(cpp, "  fwrite(buffer, 1, %d, f);\n", readSize);
    }
    delete[] buffer;
    buffer = NULL;

    //compute final digest
    MD5DIGEST digest;
    MD5Final(digest.bytes, &context);
    std::string md5String = toString(digest);

    //write cpp file footer
    fprintf(cpp, "      oLength = 0;\n");
    fprintf(cpp, "      return NULL;\n");
    fprintf(cpp, "    }\n");
    fprintf(cpp, "    virtual const char * getMd5() const { return \"%s\"; }\n", md5String.c_str() );
    fprintf(cpp, "    virtual char * newBuffer() const\n");
    fprintf(cpp, "    {\n");
    fprintf(cpp, "      size_t size = getSize();\n");
    fprintf(cpp, "      char * buffer = new char[size];\n");
    fprintf(cpp, "      if (buffer == NULL)\n");
    fprintf(cpp, "        return NULL;\n");
    fprintf(cpp, "      size_t numSegments = getNumSegments();\n");
    fprintf(cpp, "      size_t segmentLength = 0;\n");
    fprintf(cpp, "      size_t index = 0;\n");
    fprintf(cpp, "      for(size_t i=0; i<numSegments; i++)\n");
    fprintf(cpp, "      {\n");
    fprintf(cpp, "        const char * segmentBuffer = getSegment(i, segmentLength);\n");
    fprintf(cpp, "        memcpy(&buffer[index], segmentBuffer, segmentLength);\n");
    fprintf(cpp, "        index += segmentLength;\n");
    fprintf(cpp, "      }\n");
    fprintf(cpp, "      return buffer;\n");
    fprintf(cpp, "    }\n");
    fprintf(cpp, "    virtual bool save(const char * iFilename) const\n");
    fprintf(cpp, "    {\n");
    fprintf(cpp, "      FILE * f = fopen(iFilename, \"wb\");\n");
    fprintf(cpp, "      if (!f) return false;\n");
    fprintf(cpp, "      size_t numSegments = getNumSegments();\n");
    fprintf(cpp, "      size_t segmentLength = 0;\n");
    fprintf(cpp, "      const char * buffer = NULL;\n");
    fprintf(cpp, "      for(size_t i=0; i<numSegments; i++)\n");
    fprintf(cpp, "      {\n");
    fprintf(cpp, "        buffer = getSegment(i, segmentLength);\n");
    fprintf(cpp, "        fwrite(buffer, 1, segmentLength, f);\n");
    fprintf(cpp, "      }\n");
    fprintf(cpp, "      fclose(f);\n");
    fprintf(cpp, "      return true;\n");
    fprintf(cpp, "    }\n");
    fprintf(cpp, "  };\n");
    fprintf(cpp, "  const File & %s() { static %s _instance; return _instance; }\n", getterFunctionName.c_str(), className.c_str());
    fprintf(cpp, "}; //bin2cpp\n");

    fclose(input);

    return bin2cpp::ErrorCodes::Success;
  }

}; //bin2cpp