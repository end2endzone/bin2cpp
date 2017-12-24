#pragma once

#include "md5support.h"
#include "md5.h"
#include "md5Ex.h"

namespace bin2cpp
{

  std::string getFileHexDigest(const char * iFilePath)
  {
    FILE * f = fopen(iFilePath, "rb");
    if (!f)
      return std::string();

    //Compute MD5 while generating cpp code
    MD5_CTX context;
    MD5Init(&context);

    //create buffer for each chunks from input buffer
    static const size_t BUFFER_SIZE = 1024;
    unsigned char buffer[BUFFER_SIZE];
    while(!feof(f))
    {
      //read a chunk of the file
      size_t readSize = fread(buffer, 1, BUFFER_SIZE, f);

      //send to MD5 for analysist
      MD5Update(&context, buffer, readSize);
    }

    //compute final digest
    MD5DIGEST digest;
    MD5Final(digest.bytes, &context);
    std::string md5String = toString(digest);

    fclose(f);

    return md5String;
  }

  std::string getGeneratedFileHexDigest(const char * iFilePath)
  {
    FILE * f = fopen(iFilePath, "r");
    if (!f)
      return std::string();

    std::string md5;

    //create buffer for each chunks from input buffer
    static const size_t BUFFER_SIZE = 10240;
    char buffer[BUFFER_SIZE];
    while(/*!feof(f)*/ fgets(buffer, BUFFER_SIZE, f) != NULL )
    {
      //read a text line of the file
      std::string text = buffer;

      size_t md5FunctionIndex = text.find("getMd5()");
      if (md5FunctionIndex != std::string::npos)
      {
        size_t startIndex = text.find("\"", md5FunctionIndex+1);
        if (startIndex != std::string::npos)
        {
          size_t endIndex = text.find("\"", startIndex+1);
          if (endIndex != std::string::npos)
          {
            startIndex++;
            endIndex--;
            size_t length = endIndex-startIndex+1;
            md5 = text.substr(startIndex, length);
          }
        }
      }
    }

    fclose(f);

    return md5;
  }

}; //bin2cpp
