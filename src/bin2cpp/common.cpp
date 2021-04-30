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
  const char * getVersionString()
  {
    return BIN2CPP_VERSION;
  }

  uint64_t getOutputFileModifiedDate(const std::string & path)
  {
    uint64_t mod_time = 0;

    FILE * f = fopen(path.c_str(), "r");
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
        ra::strings::Replace(date, lastModifiedTag, "");
        ra::strings::Replace(date, " ", "");
        ra::strings::Replace(date, ".", "");

        //parse date into mod_time
        bool parseOK = ra::strings::Parse(date, mod_time);
        if (parseOK)
          fclose(f); //force existing while loop
      }
    }

    fclose(f);

    return mod_time;
  }

  bool isCppHeaderFile(const std::string & path)
  {
    std::string extension = ra::strings::Uppercase(ra::filesystem::GetFileExtention(path));
    if (extension == "H" || extension == "HPP")
      return true;
    return false;
  }

  bool isCppSourceFile(const std::string & path)
  {
    std::string extension = ra::strings::Uppercase(ra::filesystem::GetFileExtention(path));
    if (extension == "CPP" || extension == "CXX")
      return true;
    return false;
  }

  std::string getCppIncludeGuardMacroName(const std::string & path)
  {
    static const std::string EMPTY_STRING;
    if (path.empty())
      return EMPTY_STRING;

    std::string filename = ra::filesystem::GetFilename(path.c_str());

    //remove consecutive spaces
    std::string pattern = "  ";
    while(filename.find(pattern) != std::string::npos)
    {
      ra::strings::Replace(filename, pattern, pattern.substr(0, 1));
    }

    //remove consecutive colon (for handling namespaces)
    pattern = "::";
    while(filename.find(pattern) != std::string::npos)
    {
      ra::strings::Replace(filename, pattern, pattern.substr(0, 1));
    }

    //replace
    int numSpaces = ra::strings::Replace(filename, " ", "_");
    int numColon = ra::strings::Replace(filename, ":", "_");
    int numDots = ra::strings::Replace(filename, ".", "_");
    
    //uppercase
    filename = ra::strings::Uppercase(filename);

    return filename;
  }


}; //bin2cpp
