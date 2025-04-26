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

#include "ArrayGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>

#include "rapidassist/code_cpp.h"
#include "rapidassist/strings.h"
#include "rapidassist/filesystem.h"

namespace bin2cpp
{
  ArrayGenerator::ArrayGenerator()
  {
  }

  ArrayGenerator::~ArrayGenerator()
  {
  }

  const char * ArrayGenerator::getName() const
  {
    return "array";
  }

  bool ArrayGenerator::createCppSourceFile(const char * cpp_file_path)
  {
    //check if input file exists
    FILE * input = fopen(mContext.inputFilePath.c_str(), "rb");
    if (!input)
      return false;

    //Uppercase function identifier
    std::string functionIdentifier = ra::strings::CapitalizeFirstCharacter(getContext().functionIdentifier);

    //Build header and cpp file path
    std::string headerPath = getHeaderFilePath(cpp_file_path);
    std::string cppPath = cpp_file_path;

    //create cpp file
    FILE * cpp = fopen(cppPath.c_str(), "w");
    if (!cpp)
    {
      fclose(input);
      return false;
    }

    //determine file properties
    uint32_t fileSize = ra::filesystem::GetFileSize(input);
    std::string filename = ra::filesystem::GetFilename(mContext.inputFilePath.c_str());

    //Build class name
    std::string className = getClassName();

    //Build function 
    std::string getterFunctionName = getGetterFunctionName();

    //write cpp file heading
    fprintf(cpp, "%s", getHeaderTemplate().c_str());
    fprintf(cpp, "#include \"%s\"\n", mContext.headerFilename.c_str() );
    fprintf(cpp, "#include <iostream>\n");
    fprintf(cpp, "#include <fstream>  //for ofstream\n");
    fprintf(cpp, "namespace %s\n", getContext().codeNamespace.c_str());
    fprintf(cpp, "{\n");
    fprintf(cpp, "  class %s : public virtual %s::%s\n", className.c_str(), getContext().codeNamespace.c_str(), getContext().baseClass.c_str());
    fprintf(cpp, "  {\n");
    fprintf(cpp, "  public:\n");
    fprintf(cpp, "    %s() {}\n", className.c_str());
    fprintf(cpp, "    virtual ~%s() {}\n", className.c_str());
    fprintf(cpp, "    virtual size_t getSize() const { return %u; }\n", fileSize);
    fprintf(cpp, "    virtual const char * getFileName() const { %s }\n", getImplOfGetFileName().c_str());
    fprintf(cpp, "    virtual const char * getFilePath() const { %s }\n", getImplOfGetFilePath().c_str());
    fprintf(cpp, "    virtual const char * getBuffer() const\n");
    fprintf(cpp, "    {\n");
    fprintf(cpp, "      static const unsigned char buffer[] = {\n");

    //create buffer for each chunks from input buffer
    int numLinePrinted = 0;
    unsigned char * buffer = new unsigned char[getContext().chunkSize];
    while(!feof(input))
    {
      //read a chunk of the file
      size_t readSize = fread(buffer, 1, getContext().chunkSize, input);

      bool isLastChunk = !(readSize == getContext().chunkSize);

      if (readSize > 0)
      {
        if (numLinePrinted > 0)
        {
          //end previous line
          fprintf(cpp, ",\n");
        }

        //output
        fprintf(cpp, "        %s", ra::code::cpp::ToCppCharactersArray(buffer, readSize).c_str());
        numLinePrinted++;
      }

      //end the array. all the file content is printed
      if (isLastChunk)
      {
        fprintf(cpp, "\n");
        fprintf(cpp, "      };\n");
      }
    }
    delete[] buffer;
    buffer = NULL;

    //write cpp file footer
    fprintf(cpp, "      return (const char *)buffer;\n");
    fprintf(cpp, "    }\n");
    fprintf(cpp, "%s", getSaveMethodTemplate().c_str());
    fprintf(cpp, "  };\n");
    fprintf(cpp, "  const %s & %s() { static %s _instance; return _instance; }\n", getContext().baseClass.c_str(), getterFunctionName.c_str(), className.c_str());
    if (mContext.registerFiles)
    {
      std::string fileManagerTemplate = getFileManagerRegistrationTemplate();
      fprintf(cpp, "%s", fileManagerTemplate.c_str());
    }
    fprintf(cpp, "}; //%s\n", getContext().codeNamespace.c_str());

    fclose(input);
    fclose(cpp);

    return true;
  }

  bool ArrayGenerator::printFileContent()
  {
    //check if input file exists
    FILE * input = fopen(mContext.inputFilePath.c_str(), "rb");
    if (!input)
      return false;

    //determine file properties
    uint32_t fileSize = ra::filesystem::GetFileSize(input);

    //create buffer for each chunks from input buffer
    int numLinePrinted = 0;
    unsigned char * buffer = new unsigned char[getContext().chunkSize];
    while(!feof(input))
    {
      //read a chunk of the file
      size_t readSize = fread(buffer, 1, getContext().chunkSize, input);

      bool isLastChunk = !(readSize == getContext().chunkSize);

      if (readSize > 0)
      {
        if (numLinePrinted > 0)
        {
          //end previous line
          printf("\n");
        }

        //output
        std::string text = ra::code::cpp::ToCppCharactersArray(buffer, readSize);
        printf("\"%s\"", text.c_str());
        numLinePrinted++;
      }
    }
    delete[] buffer;
    buffer = NULL;

    fclose(input);

    return true;
  }

}; //bin2cpp