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

    //remove consecutive dash
    pattern = "--";
    while(filename.find(pattern) != std::string::npos)
    {
      ra::strings::Replace(filename, pattern, pattern.substr(0, 1));
    }

    //replace
    int numSpaces = ra::strings::Replace(filename, " ", "_");
    int numColon = ra::strings::Replace(filename, ":", "_");
    int numDots = ra::strings::Replace(filename, ".", "_");
    int numDash = ra::strings::Replace(filename, "-", "_");
    
    //uppercase
    filename = ra::strings::Uppercase(filename);

    return filename;
  }

  std::string filter(std::string str, const std::string & valid_characters)
  {
    std::string output;
  
    //reserve as many characters as in input string
    output.reserve(str.size());

    //for each characters in input string
    for(size_t i=0; i < str.size(); i++)
    {
      //is the current character is found in valid characters?
      size_t pos = valid_characters.find(str[i], 0);
      if (pos != std::string::npos)
        output.append(1, str[i]);
    }

    return output;
  }

  std::string getFunctionIdentifierFromPath(const std::string & path)
  {
    std::string id;

    //get filename of the given path
    id = ra::filesystem::GetFilename(path.c_str());

    //filter out characters which are not alphanumeric characters or '_'.
    static const std::string validCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
    id = filter(id, validCharacters);

    return id;
  }

  std::string getUniqueFunctionIdentifierFromPath(const std::string & path, Dictionary & dict) {
    std::string id = getFunctionIdentifierFromPath(path);

    //find an unused identifier
    bool exists = dict.find(id) != dict.end();
    if (exists) {
      std::string base_id = id + "_";

      //increase a counter until an identifier does not already exists
      size_t counter = 0;
      while(exists) {
        //duplicate id
      
        //increase counter and generate a new id
        counter++;
        id = base_id + ra::strings::ToString(counter);

        //check again
        exists = dict.find(id) != dict.end();
      }
    }

    //this identifier is not already used.
    //register this identifier in the dictionary.
    dict.insert(id);

    return id;
  }

}; //bin2cpp
