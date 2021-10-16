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
    //uint32_t fileSize = ra::filesystem::GetFileSize(input);
    std::string filename = ra::filesystem::GetFilename(getInputFilePath());

    //Build class name
    std::string className = getClassName();

    //Build function 
    std::string getterFunctionName = getGetterFunctionName();

    //write cpp file heading
    fprintf(cpp, "%s", getHeaderTemplate().c_str());
    fprintf(cpp, "#include \"%s\"\n", getHeaderFilename() );
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

    fprintf(cpp, "namespace %s\n", mNamespace.c_str());
    fprintf(cpp, "{\n");
    fprintf(cpp, "  class %s : public virtual %s::%s\n", className.c_str(), mNamespace.c_str(), mBaseClass.c_str());
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
    fprintf(cpp, "          hResourceInfoBlock = FindResource(hModule, \"%s\", \"CUSTOM\");\n", getRandomIdentifier(getInputFilePath()).c_str());
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

  std::string Win32ResourceGenerator::getResourceFilePath(const char * cpp_file_path)
  {
    //Build header file path
    std::string resourcePath = cpp_file_path;
    ra::strings::Replace(resourcePath, ".cpp", ".rc");
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

    std::string filename = ra::filesystem::GetFilename(getInputFilePath());
 
    //write res file heading
    fprintf(res, "%s", getHeaderTemplate().c_str());
    fprintf(res, "#include <windows.h>\n");
    fprintf(res, "%s CUSTOM \"%s\"\n", getRandomIdentifier(getInputFilePath()).c_str(), filename.c_str());

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

}; //bin2cpp