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

#include "BaseGenerator.h"
#include "TemplateProcessor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>

#include "common.h"

#include "rapidassist/strings.h"
#include "rapidassist/filesystem.h"
#include "rapidassist/timing.h"
#include "rapidassist/code_cpp.h"

namespace bin2cpp
{
  BaseGenerator::BaseGenerator()
  {
  }

  BaseGenerator::~BaseGenerator()
  {
  }

  void BaseGenerator::setContext(const Context& c)
  {
    mContext = c;
  }

  const Context& BaseGenerator::getContext() const
  {
    return mContext;
  }

  bool BaseGenerator::lookupStringVariable(const std::string& name, std::string& output)
  {
    if ( name == "bin2cpp_baseclass" ) { output = mContext.baseClass; return true; }
    if ( name == "bin2cpp_classname" ) { output = getClassName(); return true; }
    if ( name == "bin2cpp_cpp_save_method_template" ) { output = getCppSaveMethodTemplate(); return true; }
    if ( name == "bin2cpp_file_manager_c_registration_implementation" && mContext.registerFiles ) { output = getCFileManagerStaticFileRegistrationImplementation(); return true; }
    if ( name == "bin2cpp_file_manager_c_registration_post_init_implementation" && mContext.registerFiles ) { output = getCFileManagerRegistrationPostInitImplementation(); return true; }
    if ( name == "bin2cpp_file_manager_c_registration_predeclaration" && mContext.registerFiles ) { output = getCFileManagerRegistrationPredeclarationImplementation(); return true; }
    if ( name == "bin2cpp_file_manager_cpp_registration_implementation" && mContext.registerFiles ) { output = getCppFileManagerRegistrationImplementationTemplate(); return true; }
    if ( name == "bin2cpp_file_manager_file_header" ) { output = getHeaderTemplate(false); return true; }
    if ( name == "bin2cpp_file_manager_header_file_name" ) { output = mContext.managerHeaderFilename; return true; }
    if ( name == "bin2cpp_file_manager_macro_guard_prefix" ) { output = getFileManagerMacroGuardPrefix(); return true; }
    if ( name == "bin2cpp_file_object_file_name" ) { output = getFileObjectFileName(); return true; }
    if ( name == "bin2cpp_file_object_file_path" ) { output = getFileObjectFilePath(); return true; }
    if ( name == "bin2cpp_file_object_getter_function_name" ) { output = getFileObjectGetterFunctionName(); return true; }
    if ( name == "bin2cpp_file_object_macro_guard_prefix" ) { output = getClassMacroGuardPrefix(); return true; }
    if ( name == "bin2cpp_function_identifier_lowercase" ) { output = ra::strings::Lowercase(mContext.functionIdentifier); return true; }
    if ( name == "bin2cpp_function_identifier" ) { output = mContext.functionIdentifier; return true; }
    if ( name == "bin2cpp_header_file_include_path" ) { output = getHeaderFileIncludePath(); return true; }
    if ( name == "bin2cpp_input_file_size" ) { output = ra::strings::ToString(ra::filesystem::GetFileSize(mContext.inputFilePath.c_str())); return true; }
    if ( name == "bin2cpp_namespace" ) { output = mContext.codeNamespace; return true; }
    if ( name == "bin2cpp_output_file_header_template" ) { output = getHeaderTemplate(); return true; }
    if ( name == "bin2cpp_output_file_macro_guard" ) { output = getIncludeGuardMacroName(mContext.headerFilename); return true; }

    // Unknown name
    return false;
  }

  bool BaseGenerator::lookupStreamVariable(const std::string& name, std::ostream& output)
  {
    if ( name == "bin2cpp_insert_input_file_as_code" )
    {
      writeInputFileDataAsCode(output);
      return true;
    }

    return false;
  }

  //-------------------------------
  //protected methods
  //-------------------------------

  std::string BaseGenerator::getFileObjectGetterFunctionName()
  {

    std::string getter;
    switch ( mContext.code )
    {
    default:
    case CODE_GENERATION_CPP:
      {
        //Uppercase function identifier
        std::string functionIdentifier = ra::strings::CapitalizeFirstCharacter(mContext.functionIdentifier);

        getter.append("get");
        getter.append(functionIdentifier);
        getter.append("File");
      }
      break;
    case CODE_GENERATION_C:
      {
        //Uppercase function identifier
        std::string functionIdentifier = ra::strings::Lowercase(mContext.functionIdentifier);

        getter.append(mContext.codeNamespace);
        getter.append("_get_file_");
        getter.append(functionIdentifier);
    }
      break;
    };

    return getter;
  }

  std::string BaseGenerator::getHeaderFilePath(const char * cpp_file_path)
  {
    //Build header file path
    std::string headerPath = cpp_file_path;
    switch ( mContext.code )
    {
    default:
    case CODE_GENERATION_CPP:
      ra::strings::Replace(headerPath, ".cpp", ".h");
      break;
    case CODE_GENERATION_C:
      ra::strings::Replace(headerPath, ".c", ".h");
      break;
    };

    return headerPath;
  }

  std::string BaseGenerator::getCppFilePath(const char * header_file_path)
  {
    //Build header file path
    std::string cppPath = header_file_path;
    switch ( mContext.code )
    {
    default:
    case CODE_GENERATION_CPP:
      ra::strings::Replace(cppPath, ".cpp", ".h");
      break;
    case CODE_GENERATION_C:
      ra::strings::Replace(cppPath, ".c", ".h");
      break;
    };

    return cppPath;
  }

  inline std::string BaseGenerator::getHeaderTemplate()
  {
    return getHeaderTemplate(true);
  }

  std::string BaseGenerator::getHeaderTemplate(bool include_source_file)
  {
    std::string header;
    header << 
    header << "/**\n";
    header << " * This file was generated by bin2cpp v" << bin2cpp::getVersionString() << "\n";
    header << " * Copyright (C) 2013-" << ra::timing::GetCopyrightYear() << " end2endzone.com. All rights reserved.\n";
    header << " * bin2cpp is open source software, see http://github.com/end2endzone/bin2cpp\n";
    if (include_source_file)
    {
      std::string filename = ra::filesystem::GetFilename(mContext.inputFilePath.c_str());
      uint64_t lastModifiedDate = ra::filesystem::GetFileModifiedDate(mContext.inputFilePath);
      header << " * Source code for file '" << filename << "', last modified " << lastModifiedDate << ".\n";
    }
    header << " * Do not modify this file.\n";
    header << " */\n";
    return header;
  }

  std::string BaseGenerator::getCppSaveMethodTemplate()
  {
    std::string output;
    output << "    virtual bool save(const char * filename) const\n";
    output << "    {\n";
    output << "      std::ofstream f(filename, std::ios::out | std::ios::binary | std::ios::trunc);\n";
    output << "      if (f.fail()) return false;\n";
    output << "      size_t fileSize = getSize();\n";
    output << "      const char * buffer = getBuffer();\n";
    output << "      f.write(buffer, fileSize);\n";
    output << "      f.close();\n";
    output << "      return true;\n";
    output << "    }\n";
    return output;
  }

  std::string BaseGenerator::getCppFileManagerRegistrationImplementationTemplate()
  {
    if (!mContext.registerFiles)
      return std::string();

    //Build class name
    std::string className = getClassName();

    std::string output;
    output << "  typedef const " << mContext.baseClass << " & (*t_func)();\n";
    output << "  extern bool RegisterFile(t_func iFunctionPointer);\n";
    output << "  static bool k" << className << "Registered = " << mContext.codeNamespace << "::RegisterFile(&" << getFileObjectGetterFunctionName() << ");\n";
    output << "  \n";
    return output;
  }

  std::string BaseGenerator::getCFileManagerRegistrationPredeclarationImplementation()
  {
    if ( !mContext.registerFiles )
      return std::string();

    std::string output;
    output << "extern bool " << mContext.codeNamespace << "_filemanager_register_file(" << mContext.baseClass << " * file); \n";
    output << "\n";
    return output;
  }

  std::string BaseGenerator::getCFileManagerStaticFileRegistrationImplementation()
  {
    if ( !mContext.registerFiles )
      return std::string();

    //Lowercase function identifier
    std::string functionIdentifier = ra::strings::Lowercase(mContext.functionIdentifier);

    std::string output;
    output << "\n";
    output << "#if (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__)  // GCC 4.0+ required, Clang supports it by default\n";
    output << "__attribute__((constructor))\n";
    output << "#endif\n";
    output << "void " << mContext.codeNamespace << "_" << functionIdentifier << "_register_file_static_init(void)\n";
    output << "{\n";
    output << "  " << mContext.baseClass << "* this_file = " << mContext.codeNamespace << "_get_file_" << functionIdentifier << "();\n";
    output << "  " << mContext.codeNamespace << "_filemanager_register_file(this_file);\n";
    output << "}\n";
    output << "#if _MSC_VER >= 1920  // Visual Studio 2019 or later\n";
    output << "#pragma section(\".CRT$XCU\", read)\n";
    output << "__declspec(allocate(\".CRT$XCU\")) void (*init_ptr_" << mContext.codeNamespace << "_" << functionIdentifier << ")(void) = " << mContext.codeNamespace << "_" << functionIdentifier << "_register_file_static_init" << ";\n";
    output << "#endif\n";
    return output;
  }

  std::string BaseGenerator::getCFileManagerRegistrationPostInitImplementation()
  {
    std::string output;
    output += "  \n";
    output += "  // register when loaded if static initialisation does not work\n";
    output += "  ${bin2cpp_namespace}_filemanager_register_file(file);\n";
    return output;
  }

  std::string BaseGenerator::getClassName()
  {
    std::string functionIdentifier = ra::strings::CapitalizeFirstCharacter(mContext.functionIdentifier);
    std::string className;
    className.append(functionIdentifier.c_str());
    className.append("File");
    return className;
  }

  std::string BaseGenerator::getClassMacroGuardPrefix()
  {
    std::string macroGuardPrefix = ra::strings::Uppercase(mContext.codeNamespace);

    //remove namespace separators
    ra::strings::Replace(macroGuardPrefix, "::", "_");

    return macroGuardPrefix;
  }

  std::string BaseGenerator::getFileManagerMacroGuardPrefix()
  {
    //define macro guard a macro matching the filename
    std::string output;
    output += getIncludeGuardMacroName(mContext.codeNamespace.c_str()); //prefix the custom namespace for the file manager
    if ( !output.empty() )
      output += "_";
    output += getIncludeGuardMacroName(mContext.managerHeaderFilename);
    return output;
  }

  std::string BaseGenerator::getFileObjectFileName()
  {
    std::string output;

    std::string inputFileName = ra::filesystem::GetFilename(mContext.inputFilePath.c_str());

    //return default implementation
    output += inputFileName;
    return output;
  }

  std::string BaseGenerator::getFileObjectFilePath()
  {
    std::string output;

    //convert mReportedFilePath string to c++
    std::string path = mContext.reportedFilePath;
#ifdef _WIN32
    //escape backslash characters for c++
    static const std::string BACKSLASH = "\\";
    static const std::string BACKSLASH_ESCAPED = "\\\\";
    ra::strings::Replace(path, BACKSLASH, BACKSLASH_ESCAPED);
#endif

    //is there a reported path specified ?
    const char * reported_path = mContext.reportedFilePath.c_str();
    if (reported_path != NULL && reported_path[0] != '\0')
    {
      output += path;
      return output;
    }
    else
    {
      //if reported path is not specified ?
      //report the same as getFileName()
      output = getFileObjectFileName();
      return output;
    }

    //return default implementation
    output += path;
    return output;
  }

  std::string BaseGenerator::getHeaderFileIncludePath()
  {
    return mContext.headerFilename;
  }

  void BaseGenerator::writeInputFileDataAsCode(std::ostream& output)
  {
    //check if input file exists
    FILE* fin = fopen(mContext.inputFilePath.c_str(), "rb");
    if ( !fin )
      return;

    uint64_t fileSize = ra::filesystem::GetFileSize64(mContext.inputFilePath.c_str());
    size_t chunkCount = fileSize / mContext.chunkSize;
    if ( fileSize % mContext.chunkSize > 0 )
      chunkCount++;

    //create buffer for each chunks from input buffer
    int numLinePrinted = 0;
    size_t chunkIndex = 0;
    unsigned char* buffer = new unsigned char[mContext.chunkSize];
    while ( !feof(fin) )
    {
      //read a chunk of the file
      size_t readSize = fread(buffer, 1, mContext.chunkSize, fin);

      bool isLastChunk = (chunkIndex == (chunkCount - 1));

      if ( readSize > 0 )
      {
        //append chunk as code in output stream
        writeInputFileChunkAsCode(buffer, readSize, chunkIndex, chunkCount, isLastChunk, output);

        numLinePrinted++;
        chunkIndex++;
      }

    }
    delete[] buffer;
    buffer = NULL;

    fclose(fin);
  }

  void BaseGenerator::writeInputFileChunkAsCode(const unsigned char* buffer, size_t buffer_size, size_t index, size_t count, bool is_last_chunk, std::ostream& output)
  {
  }

  bool BaseGenerator::createCppHeaderFile(const char * header_file_path)
  {
    const std::string text = ""
      "${bin2cpp_output_file_header_template}"
      "#ifndef ${bin2cpp_output_file_macro_guard}\n"
      "#define ${bin2cpp_output_file_macro_guard}\n"
      "\n"
      "#include <stddef.h>\n"
      "\n"
      "namespace ${bin2cpp_namespace}\n"
      "{\n"
      "  #ifndef ${bin2cpp_file_object_macro_guard_prefix}_FILE_OBJECT_CLASS\n"
      "  #define ${bin2cpp_file_object_macro_guard_prefix}_FILE_OBJECT_CLASS\n"
      "  class ${bin2cpp_baseclass}\n"
      "  {\n"
      "  public:\n"
      "    virtual size_t getSize() const = 0;\n"
      "    /* DEPRECATED */ virtual inline const char * getFilename() const { return getFileName(); }\n"
      "    virtual const char * getFileName() const = 0;\n"
      "    virtual const char * getFilePath() const = 0;\n"
      "    virtual const char * getBuffer() const = 0;\n"
      "    virtual bool save(const char * filename) const = 0;\n"
      "  };\n"
      "  #endif //${bin2cpp_file_object_macro_guard_prefix}_FILE_OBJECT_CLASS\n"
      "  const ${bin2cpp_baseclass} & ${bin2cpp_file_object_getter_function_name}();\n"
      "}; //${bin2cpp_namespace}\n"
      "\n"
      "#endif //${bin2cpp_output_file_macro_guard}\n"
    ;

    TemplateProcessor processor(&text);
    processor.setTemplateVariableLookup(this);
    bool write_success = processor.writeFile(header_file_path);

    return write_success;
  }

  bool BaseGenerator::printFileContent()
  {
    //check if input file exists
    FILE * input = fopen(mContext.inputFilePath.c_str(), "rb");
    if (!input)
      return false;

    //determine file properties
    uint32_t fileSize = ra::filesystem::GetFileSize(input);

    //create buffer for each chunks from input buffer
    int numLinePrinted = 0;
    unsigned char * buffer = new unsigned char[mContext.chunkSize];
    while(!feof(input))
    {
      //read a chunk of the file
      size_t readSize = fread(buffer, 1, mContext.chunkSize, input);

      bool isLastChunk = !(readSize == mContext.chunkSize);

      if (readSize > 0)
      {
        if (numLinePrinted > 0)
        {
          //end previous line
          printf("\n");
        }

        //output
        std::string text;
        switch(mContext.cppEncoder)
        {
        case CPP_ENCODER_HEX:
          text = ra::code::cpp::ToHexString(buffer, readSize);
          break;
        case CPP_ENCODER_OCT:
        default:
          text = ra::code::cpp::ToOctString(buffer, readSize, false);
          break;
        };
        printf("\"%s\"", text.c_str());
        numLinePrinted++;
      }
    }
    delete[] buffer;
    buffer = NULL;

    fclose(input);

    return true;
  }

  bool BaseGenerator::createCHeaderFile(const char* file_path)
  {
    const std::string text = ""
      "${bin2cpp_output_file_header_template}"
      "#ifndef ${bin2cpp_output_file_macro_guard}\n"
      "#define ${bin2cpp_output_file_macro_guard}\n"
      "\n"
      "#include <stddef.h>\n"
      "#include <stdbool.h>\n"
      "\n"
      "#ifndef ${bin2cpp_file_object_macro_guard_prefix}_FILE_OBJECT_STRUCT\n"
      "#define ${bin2cpp_file_object_macro_guard_prefix}_FILE_OBJECT_STRUCT\n"
      "typedef struct ${bin2cpp_baseclass} ${bin2cpp_baseclass};\n"
      "typedef bool(*${bin2cpp_namespace}_load_func)();\n"
      "typedef void(*${bin2cpp_namespace}_free_func)();\n"
      "typedef bool(*${bin2cpp_namespace}_save_func)(const char*);\n"
      "typedef struct ${bin2cpp_baseclass}\n"
      "{\n"
      "  size_t size;\n"
      "  const char* file_name;\n"
      "  const char* file_path;\n"
      "  const unsigned char* buffer;\n"
      "  ${bin2cpp_namespace}_load_func load;\n"
      "  ${bin2cpp_namespace}_free_func unload;\n"
      "  ${bin2cpp_namespace}_save_func save;\n"
      "} ${bin2cpp_baseclass};\n"
      "typedef ${bin2cpp_baseclass}* ${bin2cpp_baseclass}Ptr;\n"
      "#endif //${bin2cpp_file_object_macro_guard_prefix}_FILE_OBJECT_STRUCT\n"
      "${bin2cpp_baseclass}* ${bin2cpp_file_object_getter_function_name}(void);\n"
      "\n"
      "#endif //${bin2cpp_output_file_macro_guard}\n"
    ;

    TemplateProcessor processor(&text);
    processor.setTemplateVariableLookup(this);
    bool write_success = processor.writeFile(file_path);

    return write_success;
  }

  bool BaseGenerator::createCSourceFile(const char* file_path)
  {
    // not supported yet
    return false;
  }

}; //bin2cpp