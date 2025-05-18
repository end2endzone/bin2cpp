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
  }

  Win32ResourceGenerator::~Win32ResourceGenerator()
  {
  }

  const char * Win32ResourceGenerator::getName() const
  {
    return "win32";
  }

  bool Win32ResourceGenerator::createCppSourceFile(const char * cpp_file_path)
  {
    bool resourceFileSuccess = createResourceFile(cpp_file_path);
    if (!resourceFileSuccess)
      return false;

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
    //uint32_t fileSize = ra::filesystem::GetFileSize(input);
    std::string filename = ra::filesystem::GetFilename(mContext.inputFilePath.c_str());

    //Build class name
    std::string className = getClassName();

    //Build function 
    std::string getterFunctionName = getGetterFunctionName();

    //write cpp file heading
    fprintf(cpp, "%s", getHeaderTemplate().c_str());
    fprintf(cpp, "#include \"%s\"\n", mContext.headerFilename.c_str() );
    fprintf(cpp, "#if defined(_WIN32) && !defined(_CRT_SECURE_NO_WARNINGS)\n");
    fprintf(cpp, "#define _CRT_SECURE_NO_WARNINGS\n");
    fprintf(cpp, "#endif\n");
    fprintf(cpp, "\n");
    fprintf(cpp, "#include <stdint.h>\n");
    fprintf(cpp, "#include <iostream>\n");
    fprintf(cpp, "#include <fstream>  //for ofstream\n");
    fprintf(cpp, "\n");
    fprintf(cpp, "#ifndef WIN32_LEAN_AND_MEAN\n");
    fprintf(cpp, "#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers\n");
    fprintf(cpp, "#endif\n");
    fprintf(cpp, "#include <windows.h>\n");
    fprintf(cpp, "\n");
    fprintf(cpp, "#include <psapi.h> //for EnumProcessModules()\n");
    fprintf(cpp, "#pragma comment( lib, \"psapi.lib\" )\n");
    fprintf(cpp, "\n");

    fprintf(cpp, "namespace %s\n", getContext().codeNamespace.c_str());
    fprintf(cpp, "{\n");
    fprintf(cpp, "  class %s : public virtual %s::%s\n", className.c_str(), getContext().codeNamespace.c_str(), getContext().baseClass.c_str());
    fprintf(cpp, "  {\n");
    fprintf(cpp, "  public:\n");
    fprintf(cpp, "    %s() :\n", className.c_str());
    fprintf(cpp, "      hProcess(NULL),\n");
    fprintf(cpp, "      hModule(NULL),\n");
    fprintf(cpp, "      hResourceInfoBlock(NULL),\n");
    fprintf(cpp, "      hResHandle(NULL),\n");
    fprintf(cpp, "      mBufferSize(0),\n");
    fprintf(cpp, "      mBuffer(NULL)\n");
    fprintf(cpp, "    {\n");
    fprintf(cpp, "      loadResource();\n");
    fprintf(cpp, "    }\n");
    fprintf(cpp, "    virtual ~%s() { unloadResource(); }\n", className.c_str());
    fprintf(cpp, "    virtual size_t getSize() const { return mBufferSize; }\n");
    fprintf(cpp, "    virtual const char * getFileName() const { %s }\n", getImplOfGetFileName().c_str());
    fprintf(cpp, "    virtual const char * getFilePath() const { %s }\n", getImplOfGetFilePath().c_str());
    fprintf(cpp, "    virtual const char * getBuffer() const { return mBuffer; }\n");
    fprintf(cpp, "    void loadResource()\n");
    fprintf(cpp, "    {\n");
    fprintf(cpp, "      //Get a handle to this process\n");
    fprintf(cpp, "      hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId() );\n");
    fprintf(cpp, "      if (hProcess)\n");
    fprintf(cpp, "      {\n");
    fprintf(cpp, "        //Find the main HMODULE of the process\n");
    fprintf(cpp, "        DWORD cbNeeded;\n");
    fprintf(cpp, "        if ( EnumProcessModules( hProcess, &hModule, sizeof(hModule), &cbNeeded) )\n");
    fprintf(cpp, "        {\n");
    fprintf(cpp, "          //Retrieve the resource\n");
    fprintf(cpp, "          hResourceInfoBlock = FindResourceA(hModule, \"%s\", \"CUSTOM\");\n", getRandomIdentifier(mContext.inputFilePath.c_str()).c_str());
    fprintf(cpp, "          if (hResourceInfoBlock)\n");
    fprintf(cpp, "          {\n");
    fprintf(cpp, "            hResHandle = LoadResource(hModule, hResourceInfoBlock);\n");
    fprintf(cpp, "            if (hResHandle)\n");
    fprintf(cpp, "            {\n");
    fprintf(cpp, "              mBuffer = (const char *)LockResource(hResHandle);\n");
    fprintf(cpp, "              mBufferSize = SizeofResource(hModule, hResourceInfoBlock);\n");
    fprintf(cpp, "            }\n");
    fprintf(cpp, "          }\n");
    fprintf(cpp, "        }\n");
    fprintf(cpp, "      }\n");
    fprintf(cpp, "    }\n");
    fprintf(cpp, "    virtual void unloadResource()\n");
    fprintf(cpp, "    {\n");
    fprintf(cpp, "      if (hResHandle)\n");
    fprintf(cpp, "      {\n");
    fprintf(cpp, "        FreeResource(hResHandle);\n");
    fprintf(cpp, "        hResHandle = NULL;\n");
    fprintf(cpp, "        mBuffer = NULL;\n");
    fprintf(cpp, "        mBufferSize = 0;\n");
    fprintf(cpp, "      }\n");
    fprintf(cpp, "      hResourceInfoBlock = NULL;\n");
    fprintf(cpp, "      hModule = NULL;\n");
    fprintf(cpp, "      if (hProcess)\n");
    fprintf(cpp, "      {\n");
    fprintf(cpp, "        CloseHandle(hProcess);\n");
    fprintf(cpp, "        hProcess = NULL;\n");
    fprintf(cpp, "      }\n");
    fprintf(cpp, "    }\n");
    fprintf(cpp, "%s", getSaveMethodTemplate().c_str());
    fprintf(cpp, "  private:\n");
    fprintf(cpp, "    HANDLE hProcess;\n");
    fprintf(cpp, "    HMODULE hModule;\n");
    fprintf(cpp, "    HRSRC hResourceInfoBlock;\n");
    fprintf(cpp, "    HGLOBAL hResHandle;\n");
    fprintf(cpp, "    DWORD mBufferSize;\n");
    fprintf(cpp, "    const char * mBuffer;\n");
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

  bool Win32ResourceGenerator::createCSourceFile(const char* file_path)
  {
    bool resourceFileSuccess = createResourceFile(file_path);
    if ( !resourceFileSuccess )
      return false;

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
    fprintf(fout, "\n");
    fprintf(fout, "#include \"%s\"\n", mContext.headerFilename.c_str());
    fprintf(fout, "#include <stdlib.h> // for malloc\n");
    fprintf(fout, "#include <string.h> // for memset\n");
    fprintf(fout, "#include <stdio.h>  // for fopen\n");
    fprintf(fout, "\n");
    fprintf(fout, "#ifndef WIN32_LEAN_AND_MEAN\n");
    fprintf(fout, "#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers\n");
    fprintf(fout, "#endif\n");
    fprintf(fout, "#include <windows.h>\n");
    fprintf(fout, "\n");
    fprintf(fout, "#include <psapi.h> //for EnumProcessModules()\n");
    fprintf(fout, "#pragma comment( lib, \"psapi.lib\" )\n");
    fprintf(fout, "\n");
    fprintf(fout, "static %s %s_file = { 0 };\n", mContext.baseClass.c_str(), functionIdentifier.c_str());
    fprintf(fout, "static bool %s_initialized = false;\n", functionIdentifier.c_str());
    fprintf(fout, "typedef struct %s\n", getLocalInfoStructName().c_str());
    fprintf(fout, "{\n");
    fprintf(fout, "  HANDLE hProcess;\n");
    fprintf(fout, "  HMODULE hModule;\n");
    fprintf(fout, "  HRSRC hResourceInfoBlock;\n");
    fprintf(fout, "  HGLOBAL hResHandle;\n");
    fprintf(fout, "  DWORD dwBufferSize;\n");
    fprintf(fout, "} %s;\n", getLocalInfoStructName().c_str());
    fprintf(fout, "static %s %s_info = { 0 };\n", getLocalInfoStructName().c_str(), functionIdentifier.c_str());
    fprintf(fout, "\n");

    // File registration predeclaration code
    fprintf(fout, "%s", getCFileManagerRegistrationPredeclarationTemplate().c_str());

    fprintf(fout, "bool %s_load()\n", functionIdentifier.c_str());
    fprintf(fout, "{\n");
    fprintf(fout, "  if ( %s_file.buffer )\n", functionIdentifier.c_str());
    fprintf(fout, "    return true;\n");
    fprintf(fout, "\n");
    fprintf(fout, "  %s* info = &%s_info;\n", getLocalInfoStructName().c_str(), functionIdentifier.c_str());

    fprintf(fout, "  //Get a handle to this process\n");
    fprintf(fout, "  info->hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId() );\n");
    fprintf(fout, "  if (info->hProcess)\n");
    fprintf(fout, "  {\n");
    fprintf(fout, "    //Find the main HMODULE of the process\n");
    fprintf(fout, "    DWORD cbNeeded;\n");
    fprintf(fout, "    if ( EnumProcessModules( info->hProcess, &info->hModule, sizeof(info->hModule), &cbNeeded) )\n");
    fprintf(fout, "    {\n");
    fprintf(fout, "      //Retrieve the resource\n");
    fprintf(fout, "      info->hResourceInfoBlock = FindResourceA(info->hModule, \"%s\", \"CUSTOM\");\n", getRandomIdentifier(mContext.inputFilePath.c_str()).c_str());
    fprintf(fout, "      if (info->hResourceInfoBlock)\n");
    fprintf(fout, "      {\n");
    fprintf(fout, "        info->hResHandle = LoadResource(info->hModule, info->hResourceInfoBlock);\n");
    fprintf(fout, "        if (info->hResHandle)\n");
    fprintf(fout, "        {\n");
    fprintf(fout, "          %s_file.buffer = (const unsigned char *)LockResource(info->hResHandle);\n", functionIdentifier.c_str());
    fprintf(fout, "          info->dwBufferSize = SizeofResource(info->hModule, info->hResourceInfoBlock);\n");
    fprintf(fout, "          return true;\n");
    fprintf(fout, "        }\n");
    fprintf(fout, "      }\n");
    fprintf(fout, "    }\n");
    fprintf(fout, "  }\n");
    fprintf(fout, "  \n");
    fprintf(fout, "  return false;\n");
    fprintf(fout, "}\n");
    fprintf(fout, "\n");

    fprintf(fout, "void %s_free()\n", functionIdentifier.c_str());
    fprintf(fout, "{\n");
    fprintf(fout, "  if ( %s_file.buffer == NULL)\n", functionIdentifier.c_str());
    fprintf(fout, "    return;\n");
    fprintf(fout, "  %s* info = &%s_info;\n", getLocalInfoStructName().c_str(), functionIdentifier.c_str());
    fprintf(fout, "  if (info->hResHandle)\n");
    fprintf(fout, "  {\n");
    fprintf(fout, "    FreeResource(info->hResHandle);\n");
    fprintf(fout, "    info->hResHandle = NULL;\n");
    fprintf(fout, "    %s_file.buffer = NULL;\n", functionIdentifier.c_str());
    fprintf(fout, "    info->dwBufferSize = 0;\n");
    fprintf(fout, "  }\n");
    fprintf(fout, "  info->hResourceInfoBlock = NULL;\n");
    fprintf(fout, "  info->hModule = NULL;\n");
    fprintf(fout, "  if (info->hProcess)\n");
    fprintf(fout, "  {\n");
    fprintf(fout, "    CloseHandle(info->hProcess);\n");
    fprintf(fout, "    info->hProcess = NULL;\n");
    fprintf(fout, "  }\n");
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

    fprintf(fout, "inline void %s_init()\n", functionIdentifier.c_str());
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

  std::string Win32ResourceGenerator::getResourceFilePath(const char * cpp_file_path)
  {
    //Build header file path
    std::string resourcePath = cpp_file_path;
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

  bool Win32ResourceGenerator::createResourceFile(const char * cpp_file_path)
  {
    //Build resource file path
    std::string resourceFilePath = getResourceFilePath(cpp_file_path);

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

  std::string Win32ResourceGenerator::getRandomIdentifier(const char * cpp_file_path)
  {
    std::string include_guard = getCppIncludeGuardMacroName(cpp_file_path);

    //append a CRC32 checksum of the file path to allow storing multiple files with the same name in resources
    uint32_t checksum = 0;
    crc32Init(&checksum);
    crc32Update(&checksum, (unsigned char *)cpp_file_path, (uint32_t)strlen(cpp_file_path));
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

  std::string Win32ResourceGenerator::getLocalInfoStructName()
  {
    std::string name = ra::strings::Lowercase(mContext.functionIdentifier);
    name = ra::strings::Uppercase(name) + "_INFO";
    return name;
  }

}; //bin2cpp