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

#include "Win32ResourceGenerator.h"
#include "TemplateProcessor.h"
#include "common.h"
#include "crc32.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>
#include <string.h> //for strlen()

#include "rapidassist/code_cpp.h"
#include "rapidassist/strings.h"
#include "rapidassist/filesystem.h"

namespace bin2cpp
{
  Win32ResourceGenerator::Win32ResourceGenerator()
  {
    registerTemplateVariable("bin2cpp_win32_resource_random_identifier");
    registerTemplateVariable("bin2cpp_win32_local_info_struct_name");
  }

  Win32ResourceGenerator::~Win32ResourceGenerator()
  {
  }

  const char * Win32ResourceGenerator::getName() const
  {
    return "win32";
  }

  bool Win32ResourceGenerator::createCppSourceFile(const char * file_path)
  {
    bool resourceFileSuccess = createResourceFile(file_path);
    if (!resourceFileSuccess)
      return false;

    //check if input file exists
    if ( !ra::filesystem::FileExists(mContext.inputFilePath.c_str()) )
      return false;

    const std::string text = ""
      "${bin2cpp_output_file_header}"
      "#include \"${bin2cpp_header_source_file_include_path}\"\n"
      "#if defined(_WIN32) && !defined(_CRT_SECURE_NO_WARNINGS)\n"
      "#define _CRT_SECURE_NO_WARNINGS\n"
      "#endif\n"
      "\n"
      "#include <stdint.h>\n"
      "#include <iostream>\n"
      "#include <fstream>  //for ofstream\n"
      "\n"
      "#ifndef WIN32_LEAN_AND_MEAN\n"
      "#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers\n"
      "#endif\n"
      "#include <windows.h>\n"
      "\n"
      "#include <psapi.h> //for EnumProcessModules()\n"
      "#pragma comment( lib, \"psapi.lib\" )\n"
      "\n"
      "namespace ${bin2cpp_namespace}\n"
      "{\n"
      "  class ${bin2cpp_classname} : public virtual ${bin2cpp_namespace}::${bin2cpp_baseclass}\n"
      "  {\n"
      "  public:\n"
      "    ${bin2cpp_classname}() :\n"
      "      hProcess(NULL),\n"
      "      hModule(NULL),\n"
      "      hResourceInfoBlock(NULL),\n"
      "      hResHandle(NULL),\n"
      "      mBufferSize(0),\n"
      "      mBuffer(NULL)\n"
      "    {\n"
      "      loadResource();\n"
      "    }\n"
      "    virtual ~${bin2cpp_classname}() { unloadResource(); }\n"
      "    virtual size_t getSize() const { return mBufferSize; }\n"
      "    virtual const char * getFileName() const { ${bin2cpp_file_object_file_name_impl} }\n"
      "    virtual const char * getFilePath() const { ${bin2cpp_file_object_file_path_impl} }\n"
      "    virtual const char * getBuffer() const { return mBuffer; }\n"
      "    void loadResource()\n"
      "    {\n"
      "      //Get a handle to this process\n"
      "      hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId() );\n"
      "      if (hProcess)\n"
      "      {\n"
      "        //Find the main HMODULE of the process\n"
      "        DWORD cbNeeded;\n"
      "        if ( EnumProcessModules( hProcess, &hModule, sizeof(hModule), &cbNeeded) )\n"
      "        {\n"
      "          //Retrieve the resource\n"
      "          hResourceInfoBlock = FindResourceA(hModule, \"${bin2cpp_win32_resource_random_identifier}\", \"CUSTOM\");\n"
      "          if (hResourceInfoBlock)\n"
      "          {\n"
      "            hResHandle = LoadResource(hModule, hResourceInfoBlock);\n"
      "            if (hResHandle)\n"
      "            {\n"
      "              mBuffer = (const char *)LockResource(hResHandle);\n"
      "              mBufferSize = SizeofResource(hModule, hResourceInfoBlock);\n"
      "            }\n"
      "          }\n"
      "        }\n"
      "      }\n"
      "    }\n"
      "    virtual void unloadResource()\n"
      "    {\n"
      "      if (hResHandle)\n"
      "      {\n"
      "        FreeResource(hResHandle);\n"
      "        hResHandle = NULL;\n"
      "        mBuffer = NULL;\n"
      "        mBufferSize = 0;\n"
      "      }\n"
      "      hResourceInfoBlock = NULL;\n"
      "      hModule = NULL;\n"
      "      if (hProcess)\n"
      "      {\n"
      "        CloseHandle(hProcess);\n"
      "        hProcess = NULL;\n"
      "      }\n"
      "    }\n"
      "${bin2cpp_cpp_get_save_method_impl}"
      "  private:\n"
      "    HANDLE hProcess;\n"
      "    HMODULE hModule;\n"
      "    HRSRC hResourceInfoBlock;\n"
      "    HGLOBAL hResHandle;\n"
      "    DWORD mBufferSize;\n"
      "    const char * mBuffer;\n"
      "  };\n"
      "  const ${bin2cpp_baseclass} & ${bin2cpp_getter_function_name}() { static ${bin2cpp_classname} _instance; return _instance; }\n"
      "${bin2cpp_cpp_get_file_manager_registration_impl}"
      "}; //${bin2cpp_namespace}\n";

    TemplateProcessor processor(&text);
    processor.setTemplateVariableHandler(this);
    bool write_success = processor.writeFile(file_path);

    return write_success;
  }

  bool Win32ResourceGenerator::createCSourceFile(const char* file_path)
  {
    bool resourceFileSuccess = createResourceFile(file_path);
    if ( !resourceFileSuccess )
      return false;

    //check if input file exists
    if ( !ra::filesystem::FileExists(mContext.inputFilePath.c_str()) )
      return false;

    const std::string text = ""
      "${bin2cpp_output_file_header}"
      "#if defined(_WIN32) && !defined(_CRT_SECURE_NO_WARNINGS)\n"
      "#define _CRT_SECURE_NO_WARNINGS\n"
      "#endif\n"
      "\n"
      "#include \"${bin2cpp_header_source_file_include_path}\"\n"
      "#include <stdlib.h> // for malloc\n"
      "#include <string.h> // for memset\n"
      "#include <stdio.h>  // for fopen\n"
      "\n"
      "#ifndef WIN32_LEAN_AND_MEAN\n"
      "#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers\n"
      "#endif\n"
      "#include <windows.h>\n"
      "\n"
      "#include <psapi.h> //for EnumProcessModules()\n"
      "#pragma comment( lib, \"psapi.lib\" )\n"
      "\n"
      "static ${bin2cpp_baseclass} ${bin2cpp_function_identifier_lowercase}_file = { 0 };\n"
      "static bool ${bin2cpp_function_identifier_lowercase}_initialized = false;\n"
      "typedef struct ${bin2cpp_win32_local_info_struct_name}\n"
      "{\n"
      "  HANDLE hProcess;\n"
      "  HMODULE hModule;\n"
      "  HRSRC hResourceInfoBlock;\n"
      "  HGLOBAL hResHandle;\n"
      "  DWORD dwBufferSize;\n"
      "} ${bin2cpp_win32_local_info_struct_name};\n"
      "static ${bin2cpp_win32_local_info_struct_name} ${bin2cpp_function_identifier_lowercase}_info = { 0 };\n"
      "\n"
      "${bin2cpp_c_file_manager_registration_predeclaration}"
      "bool ${bin2cpp_function_identifier_lowercase}_load()\n"
      "{\n"
      "  if ( ${bin2cpp_function_identifier_lowercase}_file.buffer )\n"
      "    return true;\n"
      "\n"
      "  ${bin2cpp_win32_local_info_struct_name}* info = &${bin2cpp_function_identifier_lowercase}_info;\n"
      "  //Get a handle to this process\n"
      "  info->hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId() );\n"
      "  if (info->hProcess)\n"
      "  {\n"
      "    //Find the main HMODULE of the process\n"
      "    DWORD cbNeeded;\n"
      "    if ( EnumProcessModules( info->hProcess, &info->hModule, sizeof(info->hModule), &cbNeeded) )\n"
      "    {\n"
      "      //Retrieve the resource\n"
      "      info->hResourceInfoBlock = FindResourceA(info->hModule, \"${bin2cpp_win32_resource_random_identifier}\", \"CUSTOM\");\n"
      "      if (info->hResourceInfoBlock)\n"
      "      {\n"
      "        info->hResHandle = LoadResource(info->hModule, info->hResourceInfoBlock);\n"
      "        if (info->hResHandle)\n"
      "        {\n"
      "          ${bin2cpp_function_identifier_lowercase}_file.buffer = (const unsigned char *)LockResource(info->hResHandle);\n"
      "          info->dwBufferSize = SizeofResource(info->hModule, info->hResourceInfoBlock);\n"
      "          return true;\n"
      "        }\n"
      "      }\n"
      "    }\n"
      "  }\n"
      "  \n"
      "  return false;\n"
      "}\n"
      "\n"
      "void ${bin2cpp_function_identifier_lowercase}_free()\n"
      "{\n"
      "  if ( ${bin2cpp_function_identifier_lowercase}_file.buffer == NULL)\n"
      "    return;\n"
      "  ${bin2cpp_win32_local_info_struct_name}* info = &${bin2cpp_function_identifier_lowercase}_info;\n"
      "  if (info->hResHandle)\n"
      "  {\n"
      "    FreeResource(info->hResHandle);\n"
      "    info->hResHandle = NULL;\n"
      "    ${bin2cpp_function_identifier_lowercase}_file.buffer = NULL;\n"
      "    info->dwBufferSize = 0;\n"
      "  }\n"
      "  info->hResourceInfoBlock = NULL;\n"
      "  info->hModule = NULL;\n"
      "  if (info->hProcess)\n"
      "  {\n"
      "    CloseHandle(info->hProcess);\n"
      "    info->hProcess = NULL;\n"
      "  }\n"
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
      "${bin2cpp_c_file_manager_registration_post_init_impl}"
      "}\n"
      "\n"
      "${bin2cpp_baseclass}* ${bin2cpp_getter_function_name}(void)\n"
      "{\n"
      "  ${bin2cpp_function_identifier_lowercase}_init();\n"
      "  return &${bin2cpp_function_identifier_lowercase}_file;\n"
      "}\n"
      "${bin2cpp_c_file_manager_registration_implementation}";

    TemplateProcessor processor(&text);
    processor.setTemplateVariableHandler(this);
    bool write_success = processor.writeFile(file_path);

    return write_success;
  }

  std::string Win32ResourceGenerator::getResourceFilePath(const char * file_path)
  {
    //Build header file path
    std::string resourcePath = file_path;
    switch ( mContext.code )
    {
    default:
    case CODE_GENERATION_CPP:
      ra::strings::Replace(resourcePath, ".cpp", ".rc");
      break;
    case CODE_GENERATION_C:
      ra::strings::Replace(resourcePath, ".c", ".rc");
      break;
    };
    return resourcePath;
  }

  bool Win32ResourceGenerator::createResourceFile(const char * file_path)
  {
    //Build resource file path
    std::string resourceFilePath = getResourceFilePath(file_path);

    //create resource file
    FILE * res = fopen(resourceFilePath.c_str(), "w");
    if (!res)
    {
      return false;
    }

    std::string filename = ra::filesystem::GetFilename(mContext.inputFilePath.c_str());
 
    //write res file heading
    fprintf(res, "%s", getHeaderTemplate().c_str());
    fprintf(res, "#include <windows.h>\n");
    fprintf(res, "%s CUSTOM \"%s\"\n", getRandomIdentifier(mContext.inputFilePath.c_str()).c_str(), filename.c_str());

    fclose(res);

    return true;
  }

  std::string Win32ResourceGenerator::getRandomIdentifier(const char * file_path)
  {
    std::string include_guard = getCppIncludeGuardMacroName(file_path);

    //append a CRC32 checksum of the file path to allow storing multiple files with the same name in resources
    uint32_t checksum = 0;
    crc32Init(&checksum);
    crc32Update(&checksum, (unsigned char *)file_path, (uint32_t)strlen(file_path));
    crc32Finish(&checksum);

    std::string checksumString;
    crc32String(&checksum, &checksumString);
    checksumString = ra::strings::Uppercase(checksumString);

    //build the final identifier
    std::string id;
    id.append(include_guard);
    id.append("_");
    id.append(checksumString);

    return id;
  }

  bool Win32ResourceGenerator::printFileContent()
  {
    return false; // not supported
  }

  TemplateVariableFlags Win32ResourceGenerator::getTemplateVariableFlags(const std::string& name)
  {
    if ( name == "bin2cpp_win32_resource_random_identifier" ) return TEMPLATE_VARIABLE_FLAG_STRINGNABLE;
    if ( name == "bin2cpp_win32_local_info_struct_name" ) return TEMPLATE_VARIABLE_FLAG_STRINGNABLE;
    return BaseGenerator::getTemplateVariableFlags(name);
  }

  void Win32ResourceGenerator::writeTemplateVariable(const std::string& name, std::string& output)
  {
    if ( name == "bin2cpp_win32_resource_random_identifier" )
    {
      output = getRandomIdentifier(mContext.inputFilePath.c_str());
      return;
    }
    if ( name == "bin2cpp_win32_local_info_struct_name" )
    {
      output = getLocalInfoStructName();
      return;
    }

    // Unknown name
    return this->BaseGenerator::writeTemplateVariable(name, output);
  }

  std::string Win32ResourceGenerator::getLocalInfoStructName()
  {
    std::string name = ra::strings::Lowercase(mContext.functionIdentifier);
    name = ra::strings::Uppercase(name) + "_INFO";
    return name;
  }

}; //bin2cpp