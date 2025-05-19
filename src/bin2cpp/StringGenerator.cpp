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

#include "StringGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>

#include "rapidassist/code_cpp.h"
#include "rapidassist/strings.h"
#include "rapidassist/filesystem.h"

namespace bin2cpp
{
  StringGenerator::StringGenerator()
  {
  }

  StringGenerator::~StringGenerator()
  {
  }

  const char * StringGenerator::getName() const
  {
    return "string";
  }

  bool StringGenerator::createCppSourceFile(const char * cpp_file_path)
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
    fprintf(cpp, "      const char * buffer = ""\n");

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
          fprintf(cpp, "\n");
        }

        //convert to cpp string
        std::string cppEncoder;
        switch(getContext().cppEncoder)
        {
        case CPP_ENCODER_HEX:
          cppEncoder = ra::code::cpp::ToHexString(buffer, readSize);
          break;
        case CPP_ENCODER_OCT:
        default:
          cppEncoder = ra::code::cpp::ToOctString(buffer, readSize, false);
          break;
        };

        //output
        fprintf(cpp, "        \"%s\"", cppEncoder.c_str());
        numLinePrinted++;
      }

      //end the string if last chunk printed
      if (isLastChunk)
      {
        fprintf(cpp, ";\n");
      }
    }
    delete[] buffer;
    buffer = NULL;

    //write cpp source file footer
    fprintf(cpp, "      return buffer;\n");
    fprintf(cpp, "    }\n");
    fprintf(cpp, "%s", getSaveMethodTemplate().c_str());
    fprintf(cpp, "  };\n");
    fprintf(cpp, "  const %s & %s() { static %s _instance; return _instance; }\n", getContext().baseClass.c_str(), getterFunctionName.c_str(), className.c_str());
    if (mContext.registerFiles)
    {
      std::string fileManagerTemplate = getCppFileManagerRegistrationImplementationTemplate();
      fprintf(cpp, "%s", fileManagerTemplate.c_str());
    }
    fprintf(cpp, "}; //%s\n", getContext().codeNamespace.c_str());

    fclose(input);
    fclose(cpp);

    return true;
  }

  bool StringGenerator::createCSourceFile(const char* file_path)
  {
    //check if input file exists
    FILE* input = fopen(mContext.inputFilePath.c_str(), "rb");
    if ( !input )
      return false;

    //Uppercase function identifier
    std::string functionIdentifier = ra::strings::Lowercase(mContext.functionIdentifier);

    //Build header and cpp file path
    std::string headerPath = getHeaderFilePath(file_path);
    std::string sourcePath = file_path;

    //create c source file
    FILE* fout = fopen(sourcePath.c_str(), "w");
    if ( !fout )
    {
      fclose(input);
      return false;
    }

    //determine file properties
    uint32_t fileSize = ra::filesystem::GetFileSize(input);
    std::string filename = ra::filesystem::GetFilename(mContext.inputFilePath.c_str());
    //long lastSegmentSize = fileSize%chunk_size;
    //size_t numSegments = fileSize/chunk_size + (lastSegmentSize == 0 ? 0 : 1);

    //Build class name
    std::string className = getClassName();

    //Build function 
    std::string getterFunctionName = getGetterFunctionName();

    //Build FileManager class template
    std::string manager = mContext.managerHeaderFilename;

    //write c file heading
    fprintf(fout, "%s", getHeaderTemplate().c_str());
    fprintf(fout, "#if defined(_WIN32) && !defined(_CRT_SECURE_NO_WARNINGS)\n");
    fprintf(fout, "#define _CRT_SECURE_NO_WARNINGS\n");
    fprintf(fout, "#endif\n");
    fprintf(fout, "#include \"%s\"\n", mContext.headerFilename.c_str());
    fprintf(fout, "#include <stdlib.h> // for malloc\n");
    fprintf(fout, "#include <string.h> // for memset\n");
    fprintf(fout, "#include <stdio.h>  // for fopen\n");

    fprintf(fout, "static %s %s_file = { 0 };\n", mContext.baseClass.c_str(), functionIdentifier.c_str());
    fprintf(fout, "static bool %s_initialized = false;\n", functionIdentifier.c_str());
    fprintf(fout, "\n");

    // File registration predeclaration code
    fprintf(fout, "%s", getCFileManagerRegistrationPredeclarationTemplate().c_str());

    fprintf(fout, "bool %s_load()\n", functionIdentifier.c_str());
    fprintf(fout, "{\n");
    fprintf(fout, "  if ( %s_file.buffer )\n", functionIdentifier.c_str());
    fprintf(fout, "    return true;\n");
    fprintf(fout, "\n");
    fprintf(fout, "  static const char * static_buffer = ""\n");

    //create buffer for each chunks from input buffer
    int numLinePrinted = 0;
    unsigned char* buffer = new unsigned char[getContext().chunkSize];
    while ( !feof(input) )
    {
      //read a chunk of the file
      size_t readSize = fread(buffer, 1, getContext().chunkSize, input);

      bool isLastChunk = !(readSize == getContext().chunkSize);

      if ( readSize > 0 )
      {
        if ( numLinePrinted > 0 )
        {
          //end previous line
          fprintf(fout, "\n");
        }

        //convert to cpp string
        std::string cppEncoder;
        switch ( getContext().cppEncoder )
        {
        case CPP_ENCODER_HEX:
          cppEncoder = ra::code::cpp::ToHexString(buffer, readSize);
          break;
        case CPP_ENCODER_OCT:
        default:
          cppEncoder = ra::code::cpp::ToOctString(buffer, readSize, false);
          break;
        };

        //output
        fprintf(fout, "    \"%s\"", cppEncoder.c_str());
        numLinePrinted++;
      }

      //end the string if last chunk printed
      if ( isLastChunk )
      {
        fprintf(fout, ";\n");
      }
    }
    delete[] buffer;
    buffer = NULL;

    //write c source file footer
    fprintf(fout, "\n");
    fprintf(fout, "  %s_file.buffer = static_buffer;\n", functionIdentifier.c_str());
    fprintf(fout, "  return true;\n");
    fprintf(fout, "}\n");

    fprintf(fout, "\n");

    fprintf(fout, "void %s_free()\n", functionIdentifier.c_str());
    fprintf(fout, "{\n");
    fprintf(fout, "  %s_file.buffer = NULL;\n", functionIdentifier.c_str());
    fprintf(fout, "}\n");
    fprintf(fout, "\n");
    fprintf(fout, "bool %s_save(const char* path)\n", functionIdentifier.c_str());
    fprintf(fout, "{\n");
    fprintf(fout, "  if ( !%s_file.buffer )\n", functionIdentifier.c_str());
    fprintf(fout, "    return false;\n");
    fprintf(fout, "  FILE* f = fopen(path, \"wb\");\n");
    fprintf(fout, "  if ( !f )\n");
    fprintf(fout, "    return false;\n");
    fprintf(fout, "  size_t write_size = fwrite(%s_file.buffer, 1, %s_file.size, f);\n", functionIdentifier.c_str(), functionIdentifier.c_str());
    fprintf(fout, "  fclose(f);\n");
    fprintf(fout, "  if ( write_size != %s_file.size )\n", functionIdentifier.c_str());
    fprintf(fout, "    return false;\n");
    fprintf(fout, "  return true;\n");
    fprintf(fout, "}\n");
    fprintf(fout, "\n");
    fprintf(fout, "static inline void %s_init()\n", functionIdentifier.c_str());
    fprintf(fout, "{\n");
    fprintf(fout, "  // remember we already initialized\n");
    fprintf(fout, "  if ( %s_initialized )\n", functionIdentifier.c_str());
    fprintf(fout, "    return;\n");
    fprintf(fout, "  %s_initialized = true;\n", functionIdentifier.c_str());
    fprintf(fout, "\n");
    fprintf(fout, "  // initialize\n");
    fprintf(fout, "  %s* file = &%s_file;\n", mContext.baseClass.c_str(), functionIdentifier.c_str());
    fprintf(fout, "  file->size = %uULL;\n", fileSize);
    fprintf(fout, "  file->file_name = \"%s\";\n", getFileClassFileName().c_str());
    fprintf(fout, "  file->file_path = \"%s\";\n", getFileClassFilePath().c_str());
    fprintf(fout, "  file->buffer = NULL;\n");
    fprintf(fout, "  file->load = %s_load;\n", functionIdentifier.c_str());
    fprintf(fout, "  file->unload = %s_free;\n", functionIdentifier.c_str());
    fprintf(fout, "  file->save = %s_save;\n", functionIdentifier.c_str());
    fprintf(fout, "\n");
    fprintf(fout, "  // load file by default on init as in c++ implementation");
    fprintf(fout, "  file->load();\n");

    if ( mContext.registerFiles )
    {
      fprintf(fout, "  \n");
      fprintf(fout, "  // register\n");
      fprintf(fout, "  %s_filemanager_register_file(file);\n", mContext.codeNamespace.c_str());
    }

    fprintf(fout, "}\n");
    fprintf(fout, "\n");
    fprintf(fout, "%s* %s(void)\n", mContext.baseClass.c_str(), getGetterFunctionName().c_str());
    fprintf(fout, "{\n");
    fprintf(fout, "  %s_init();\n", functionIdentifier.c_str());
    fprintf(fout, "  return &%s_file;\n", functionIdentifier.c_str());
    fprintf(fout, "}\n");

    // File registration implementation code
    fprintf(fout, "%s", getCFileManagerRegistrationImplementationTemplate().c_str());

    fclose(input);
    fclose(fout);

    return true;
  }

}; //bin2cpp