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

#include "SegmentGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>

#include "rapidassist/code_cpp.h"
#include "rapidassist/strings.h"
#include "rapidassist/filesystem.h"

namespace bin2cpp
{
  SegmentGenerator::SegmentGenerator()
  {
  }

  SegmentGenerator::~SegmentGenerator()
  {
  }

  const char * SegmentGenerator::getName() const
  {
    return "segment";
  }

  bool SegmentGenerator::createCppSourceFile(const char * cpp_file_path)
  {
    //check if input file exists
    FILE * input = fopen(getInputFilePath(), "rb");
    if (!input)
      return false;

    //Uppercase function identifier
    std::string functionIdentifier = ra::strings::CapitalizeFirstCharacter(mFunctionIdentifier);

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
    std::string filename = ra::filesystem::GetFilename(getInputFilePath());
    //long lastSegmentSize = fileSize%chunk_size;
    //size_t numSegments = fileSize/chunk_size + (lastSegmentSize == 0 ? 0 : 1);

    //Build class name
    std::string className = getClassName();

    //Build function 
    std::string getterFunctionName = getGetterFunctionName();

    //Build FileManager class template
    std::string manager = getManagerHeaderFilename();

    //write cpp file heading
    fprintf(cpp, "%s", getHeaderTemplate().c_str());
    fprintf(cpp, "#if defined(_WIN32) && !defined(_CRT_SECURE_NO_WARNINGS)\n");
    fprintf(cpp, "#define _CRT_SECURE_NO_WARNINGS\n");
    fprintf(cpp, "#endif\n");
    fprintf(cpp, "#include \"%s\"\n", getHeaderFilename() );
    fprintf(cpp, "#include <string> //for std::string\n");
    fprintf(cpp, "#include <iostream>\n");
    fprintf(cpp, "#include <fstream>  //for ofstream\n");
    fprintf(cpp, "namespace %s\n", mNamespace.c_str());
    fprintf(cpp, "{\n");
    fprintf(cpp, "  class %s : public virtual %s::%s\n", className.c_str(), mNamespace.c_str(), mBaseClass.c_str());
    fprintf(cpp, "  {\n");
    fprintf(cpp, "  public:\n");
    fprintf(cpp, "    %s() { build(); }\n", className.c_str());
    fprintf(cpp, "    virtual ~%s() {}\n", className.c_str());
    fprintf(cpp, "    virtual size_t getSize() const { return %u; }\n", fileSize);
    fprintf(cpp, "    virtual const char * getFileName() const { %s }\n", getImplOfGetFileName().c_str());
    fprintf(cpp, "    virtual const char * getFilePath() const { %s }\n", getImplOfGetFilePath().c_str());
    fprintf(cpp, "    virtual const char * getBuffer() const { return mBuffer.c_str(); }\n");
    fprintf(cpp, "    void build()\n");
    fprintf(cpp, "    {\n");
    fprintf(cpp, "      mBuffer.clear();\n");
    fprintf(cpp, "      mBuffer.reserve(getSize()); //allocate all required memory at once to prevent reallocations\n");

    //create buffer for each chunks from input buffer
    unsigned char * buffer = new unsigned char[mChunkSize];
    while(!feof(input))
    {
      //read a chunk of the file
      size_t readSize = fread(buffer, 1, mChunkSize, input);

      //bool isLastChunk = !(readSize == chunk_size);

      if (readSize == 0)
        continue; //nothing to output if nothing was read

      //convert to cpp string
      std::string cppEncoder;
      switch(mCppEncoder)
      {
      case IGenerator::CPP_ENCODER_HEX:
        cppEncoder = ra::code::cpp::ToHexString(buffer, readSize);
        break;
      case IGenerator::CPP_ENCODER_OCT:
      default:
        cppEncoder = ra::code::cpp::ToOctString(buffer, readSize, false);
        break;
      };

      //output
      fprintf(cpp, "      mBuffer.append(\"%s\", %s);\n", cppEncoder.c_str(), ra::strings::ToString(readSize).c_str());
    }
    delete[] buffer;
    buffer = NULL;

    //write cpp file footer
    fprintf(cpp, "    }\n");
    fprintf(cpp, "%s", getSaveMethodTemplate().c_str());
    fprintf(cpp, "  private:\n");
    fprintf(cpp, "    std::string mBuffer;\n");
    fprintf(cpp, "  };\n");
    fprintf(cpp, "  const %s & %s() { static %s _instance; return _instance; }\n", mBaseClass.c_str(), getterFunctionName.c_str(), className.c_str());
    if (isRegisterFileEnabled())
    {
      std::string fileManagerTemplate = getFileManagerRegistrationTemplate();
      fprintf(cpp, "%s", fileManagerTemplate.c_str());
    }
    fprintf(cpp, "}; //%s\n", mNamespace.c_str());

    fclose(input);
    fclose(cpp);

    return true;
  }

}; //bin2cpp