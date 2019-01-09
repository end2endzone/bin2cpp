#pragma once

#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "rapidassist/strings.h"
#include "rapidassist/filesystem.h"

#include "bin2cpp/version.h"

namespace bin2cpp
{
  int getCopyrightYear()
  {
    static const int DEFAULT_YEAR = 2016;
    std::string compilationDate = __DATE__;
    size_t lastSpace = compilationDate.find_last_of(" ");
    if (lastSpace == std::string::npos)
      return DEFAULT_YEAR;
    const char * yearStr = &compilationDate[lastSpace+1];
    int year = atoi(yearStr);
    return year;
  }

  const char * getVersionString()
  {
    return BIN2CPP_VERSION;
  }

  uint64_t getOutputFileModifiedDate(const std::string & iPath)
  {
    uint64_t mod_time = 0;

    FILE * f = fopen(iPath.c_str(), "r");
    if (!f)
      return mod_time;

    //create buffer for each chunks from input buffer
    static const size_t BUFFER_SIZE = 10240;
    char buffer[BUFFER_SIZE];
    while(/*!feof(f)*/ fgets(buffer, BUFFER_SIZE, f) != NULL )
    {
      //read a text line of the file
      std::string text = buffer;

      static const char * lastModifiedTag = "last modified";
      size_t lastModifiedIndex = text.find(lastModifiedTag);
      if (lastModifiedIndex != std::string::npos)
      {
        std::string date = text.substr(lastModifiedIndex);
        ra::strings::replace(date, lastModifiedTag, "");
        ra::strings::replace(date, " ", "");
        ra::strings::replace(date, ".", "");

        //parse date into mod_time
        bool parseOK = ra::strings::parse(date, mod_time);
        if (parseOK)
          fclose(f); //force existing while loop
      }
    }

    fclose(f);

    return mod_time;
  }

  bool isCppHeaderFile(const std::string & iPath)
  {
    std::string extension = ra::strings::uppercase(ra::filesystem::getFileExtention(iPath));
    if (extension == "H" || extension == "HPP")
      return true;
    return false;
  }

  bool isCppSourceFile(const std::string & iPath)
  {
    std::string extension = ra::strings::uppercase(ra::filesystem::getFileExtention(iPath));
    if (extension == "CPP" || extension == "CXX")
      return true;
    return false;
  }

}; //bin2cpp
