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
#include "TemplateProcessor.h"
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

  bool StringGenerator::createCppSourceFile(const char * file_path)
  {
    //check if input file exists
    if ( !ra::filesystem::FileExists(mContext.inputFilePath.c_str()) )
      return false;

    const std::string text = ""
      "${bin2cpp_output_file_header_template}"
      "#include \"${bin2cpp_header_file_include_path}\"\n"
      "#include <iostream>\n"
      "#include <fstream>  //for ofstream\n"
      "namespace ${bin2cpp_namespace}\n"
      "{\n"
      "  class ${bin2cpp_classname} : public virtual ${bin2cpp_namespace}::${bin2cpp_baseclass}\n"
      "  {\n"
      "  public:\n"
      "    ${bin2cpp_classname}() {}\n"
      "    virtual ~${bin2cpp_classname}() {}\n"
      "    virtual size_t getSize() const { return ${bin2cpp_input_file_size}; }\n"
      "    virtual const char * getFileName() const { return \"${bin2cpp_file_object_file_name}\"; }\n"
      "    virtual const char * getFilePath() const { return \"${bin2cpp_file_object_file_path}\"; }\n"
      "    virtual const char * getBuffer() const\n"
      "    {\n"
      "      const char * buffer = \n${bin2cpp_insert_input_file_as_code}" // INPUT FILE AS CODE HERE
      "      return buffer;\n"
      "    }\n"
      "${bin2cpp_cpp_save_method_template}"
      "  };\n"
      "  const ${bin2cpp_baseclass} & ${bin2cpp_file_object_getter_function_name}() { static ${bin2cpp_classname} _instance; return _instance; }\n"
      "${bin2cpp_file_manager_cpp_registration_implementation}"
      "}; //${bin2cpp_namespace}\n";

    TemplateProcessor processor(&text);
    processor.setTemplateVariableLookup(this);
    bool write_success = processor.writeFile(file_path);

    return write_success;
  }

  bool StringGenerator::createCSourceFile(const char* file_path)
  {
    //check if input file exists
    if ( !ra::filesystem::FileExists(mContext.inputFilePath.c_str()) )
      return false;

    const std::string text = ""
      "${bin2cpp_output_file_header_template}"
      "#if defined(_WIN32) && !defined(_CRT_SECURE_NO_WARNINGS)\n"
      "#define _CRT_SECURE_NO_WARNINGS\n"
      "#endif\n"
      "#include \"${bin2cpp_header_file_include_path}\"\n"
      "#include <stdlib.h> // for malloc\n"
      "#include <string.h> // for memset\n"
      "#include <stdio.h>  // for fopen\n"
      "static ${bin2cpp_baseclass} ${bin2cpp_function_identifier_lowercase}_file = { 0 };\n"
      "static bool ${bin2cpp_function_identifier_lowercase}_initialized = false;\n"
      "\n"
      "${bin2cpp_file_manager_c_registration_predeclaration}"
      "bool ${bin2cpp_function_identifier_lowercase}_load()\n"
      "{\n"
      "  if ( ${bin2cpp_function_identifier_lowercase}_file.buffer )\n"
      "    return true;\n"
      "\n"
      "  static const char * static_buffer = \n${bin2cpp_insert_input_file_as_code}" // INPUT FILE AS CODE HERE
      "\n"
      "  ${bin2cpp_function_identifier_lowercase}_file.buffer = static_buffer;\n"
      "  return true;\n"
      "}\n"
      "\n"
      "void ${bin2cpp_function_identifier_lowercase}_free()\n"
      "{\n"
      "  ${bin2cpp_function_identifier_lowercase}_file.buffer = NULL;\n"
      "}\n"
      "\n"
      "bool ${bin2cpp_function_identifier_lowercase}_save(const char* path)\n"
      "{\n"
      "  if ( !${bin2cpp_function_identifier_lowercase}_file.buffer )\n"
      "    return false;\n"
      "  FILE* f = fopen(path, \"wb\");\n"
      "  if ( !f )\n"
      "    return false;\n"
      "  size_t write_size = fwrite(${bin2cpp_function_identifier_lowercase}_file.buffer, 1, ${bin2cpp_function_identifier_lowercase}_file.size, f);\n"
      "  fclose(f);\n"
      "  if ( write_size != ${bin2cpp_function_identifier_lowercase}_file.size )\n"
      "    return false;\n"
      "  return true;\n"
      "}\n"
      "\n"
      "static inline void ${bin2cpp_function_identifier_lowercase}_init()\n"
      "{\n"
      "  // remember we already initialized\n"
      "  if ( ${bin2cpp_function_identifier_lowercase}_initialized )\n"
      "    return;\n"
      "  ${bin2cpp_function_identifier_lowercase}_initialized = true;\n"
      "\n"
      "  // initialize\n"
      "  ${bin2cpp_baseclass}* file = &${bin2cpp_function_identifier_lowercase}_file;\n"
      "  file->size = ${bin2cpp_input_file_size}ULL;\n"
      "  file->file_name = \"${bin2cpp_file_object_file_name}\";\n"
      "  file->file_path = \"${bin2cpp_file_object_file_path}\";\n"
      "  file->buffer = NULL;\n"
      "  file->load = ${bin2cpp_function_identifier_lowercase}_load;\n"
      "  file->unload = ${bin2cpp_function_identifier_lowercase}_free;\n"
      "  file->save = ${bin2cpp_function_identifier_lowercase}_save;\n"
      "\n"
      "  // load file by default on init as in c++ implementation\n"
      "  file->load();\n"
      "${bin2cpp_file_manager_c_registration_post_init_implementation}"
      "}\n"
      "\n"
      "${bin2cpp_baseclass}* ${bin2cpp_file_object_getter_function_name}(void)\n"
      "{\n"
      "  ${bin2cpp_function_identifier_lowercase}_init();\n"
      "  return &${bin2cpp_function_identifier_lowercase}_file;\n"
      "}\n"
      "${bin2cpp_file_manager_c_registration_implementation}";

    TemplateProcessor processor(&text);
    processor.setTemplateVariableLookup(this);
    bool write_success = processor.writeFile(file_path);

    return write_success;
  }

  void StringGenerator::writeInputFileChunkAsCode(const unsigned char* buffer, size_t buffer_size, size_t index, size_t count, bool is_last_chunk, std::ostream& output)
  {
    size_t indentation = 0;

    if ( mContext.plainOutput )
      indentation = 0;
    else if ( mContext.code == CodeGenerationEnum::CODE_GENERATION_CPP )
      indentation = 8;
    else if ( mContext.code == CodeGenerationEnum::CODE_GENERATION_C )
      indentation = 4;

    std::string str;
    if ( indentation )
      str += std::string(indentation, ' ');

    str += "\"";

    //convert to cpp string
    switch ( mContext.cppEncoder )
    {
    case CPP_ENCODER_HEX:
      str += ra::code::cpp::ToHexString(buffer, buffer_size);
      break;
    case CPP_ENCODER_OCT:
    default:
      str += ra::code::cpp::ToOctString(buffer, buffer_size, false);
      break;
    };

    str += "\"";
    if ( is_last_chunk )
      str += ";";
    str += "\n";

    output << str;
  }

}; //bin2cpp