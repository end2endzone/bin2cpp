/**********************************************************************************
 * MIT License
 * 
 * Copyright (c) 2018 Antoine Beauchamp
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *********************************************************************************/

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

  std::string getCppIncludeGuardMacroName(const std::string & iPath)
  {
    static const std::string EMPTY_STRING;
    if (iPath.empty())
      return EMPTY_STRING;

    std::string filename = ra::filesystem::getFilename(iPath.c_str());

    //replace
    int numDots = ra::strings::replace(filename, ".", "_");
    
    //uppercase
    filename = ra::strings::uppercase(filename);

    return filename;
  }


}; //bin2cpp
