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
  BaseGenerator::BaseGenerator() :
    mCppEncoder(IGenerator::CPP_ENCODER_OCT),
    mManagerEnabled(false)
  {
  }

  BaseGenerator::~BaseGenerator()
  {
  }

  void BaseGenerator::setInputFilePath(const char * path)
  {
    if (path)
      mInputFilePath = path;
  }

  const char * BaseGenerator::getInputFilePath() const
  {
    return mInputFilePath.c_str();
  }

  void BaseGenerator::setHeaderFilename(const char * path)
  {
    if (path)
      mHeaderFilename = path;
  }

  const char * BaseGenerator::getHeaderFilename() const
  {
    return mHeaderFilename.c_str();
  }

  void BaseGenerator::setFunctionIdentifier(const char * function_identifier)
  {
    if (function_identifier)
      mFunctionIdentifier = function_identifier;
  }

  const char * BaseGenerator::getFunctionIdentifier() const
  {
    return mFunctionIdentifier.c_str();
  }

  void BaseGenerator::setReportedFilePath(const char * path)
  {
    if (path)
      mReportedFilePath = path;
  }

  const char * BaseGenerator::getReportedFilePath() const
  {
    return mReportedFilePath.c_str();
  }

  void BaseGenerator::setChunkSize(size_t chunk_size)
  {
    mChunkSize = chunk_size;
  }

  size_t BaseGenerator::getChunkSize() const
  {
    return mChunkSize;
  }

  void BaseGenerator::setNamespace(const char * name)
  {
    if (name)
      mNamespace = name;
  }

  const char * BaseGenerator::getNamespace() const
  {
    return mNamespace.c_str();
  }

  void BaseGenerator::setBaseClass(const char * name)
  {
    if (name)
      mBaseClass = name;
  }

  const char * BaseGenerator::getBaseClass() const
  {
    return mBaseClass.c_str();
  }

  void BaseGenerator::setCppEncoder(const IGenerator::CppEncoderEnum & cpp_encoder)
  {
    mCppEncoder = cpp_encoder;
  }

  IGenerator::CppEncoderEnum BaseGenerator::getCppEncoder() const
  {
    return mCppEncoder;
  }

  void BaseGenerator::setManagerHeaderFilename(const char * manager_file)
  {
    if (manager_file)
      mManagerHeaderFilename = manager_file;
  }

  const char * BaseGenerator::getManagerHeaderFilename() const
  {
    return mManagerHeaderFilename.c_str();
  }

  void BaseGenerator::setRegisterFileEnabled(bool register_file_enabled)
  {
    mManagerEnabled = register_file_enabled;
  }

  bool BaseGenerator::isRegisterFileEnabled() const
  {
    return mManagerEnabled;
  }

  //-------------------------------
  //protected methods
  //-------------------------------

  std::string BaseGenerator::getGetterFunctionName()
  {
    //Uppercase function identifier
    std::string functionIdentifier = ra::strings::CapitalizeFirstCharacter(mFunctionIdentifier);

    std::string getter;
    getter.append("get");
    getter.append(functionIdentifier);
    getter.append("File");
    return getter;
  }

  std::string BaseGenerator::getHeaderFilePath(const char * cpp_file_path)
  {
    //Build header file path
    std::string headerPath = cpp_file_path;
    ra::strings::Replace(headerPath, ".cpp", ".h");
    return headerPath;
  }

  std::string BaseGenerator::getCppFilePath(const char * header_file_path)
  {
    //Build header file path
    std::string cppPath = header_file_path;
    ra::strings::Replace(cppPath, ".cpp", ".h");
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
      std::string filename = ra::filesystem::GetFilename(mInputFilePath.c_str());
      uint64_t lastModifiedDate = ra::filesystem::GetFileModifiedDate(mInputFilePath);
      header << " * Source code for file '" << filename << "', last modified " << lastModifiedDate << ".\n";
    }
    header << " * Do not modify this file.\n";
    header << " */\n";
    return header;
  }

  std::string BaseGenerator::getSaveMethodTemplate()
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

  std::string BaseGenerator::getFileManagerRegistrationTemplate()
  {
    if (!this->isRegisterFileEnabled())
      return std::string();

    //Build class name
    std::string className = getClassName();

    std::string output;
    output << "  typedef const " << mBaseClass << " & (*t_func)();\n";
    output << "  extern bool RegisterFile(t_func iFunctionPointer);\n";
    output << "  static bool k" << className << "Registered = " << mNamespace << "::RegisterFile(&" << getGetterFunctionName() << ");\n";
    return output;
  }

  std::string BaseGenerator::getClassName()
  {
    std::string functionIdentifier = ra::strings::CapitalizeFirstCharacter(mFunctionIdentifier);
    std::string className;
    className.append(functionIdentifier.c_str());
    className.append("File");
    return className;
  }

  std::string BaseGenerator::getClassMacroGuardPrefix()
  {
    std::string macroGuardPrefix = ra::strings::Uppercase(mNamespace);

    //remove namespace separators
    ra::strings::Replace(macroGuardPrefix, "::", "_");

    return macroGuardPrefix;
  }

  std::string BaseGenerator::getImplOfGetFileName()
  {
    std::string output;

    std::string inputFileName = ra::filesystem::GetFilename(getInputFilePath());

    //could we report getFileName() as a substring of getFilePath() ?
    const char * reported_path = getReportedFilePath();
    if (reported_path != NULL && reported_path[0] != '\0')
    {
      size_t offset = mReportedFilePath.find(inputFileName);
      if (offset != std::string::npos)
      {
        output = "return &getFilePath()[";
        output += ra::strings::ToString(offset);
        output += "];";
        return output;
      }
    }

    //return default implementation
    output = "return \"";
    output += inputFileName;
    output += "\";";
    return output;
  }

  std::string BaseGenerator::getImplOfGetFilePath()
  {
    std::string output;

    //convert mReportedFilePath string to c++
    std::string path = mReportedFilePath;
#ifdef _WIN32
    //escape backslash characters for c++
    static const std::string BACKSLASH = "\\";
    static const std::string BACKSLASH_ESCAPED = "\\\\";
    ra::strings::Replace(path, BACKSLASH, BACKSLASH_ESCAPED);
#endif

    //is there a reported path specified ?
    const char * reported_path = getReportedFilePath();
    if (reported_path != NULL && reported_path[0] != '\0')
    {
      output = "return \"";
      output += path;
      output += "\";";
      return output;
    }
    else
    {
      //if reported path is not specified ?
      //report the same as getFileName()
      output = "return getFileName();";
      return output;
    }

    //return default implementation
    output = "return \"";
    output += path;
    output += "\";";
    return output;
  }

  bool BaseGenerator::createCppHeaderFile(const char * header_file_path)
  {
    FILE * header = fopen(header_file_path, "w");
    if (!header)
      return false;

    //define macro guard matching the filename
    std::string macroGuard = getCppIncludeGuardMacroName(header_file_path);

    std::string classMacroGuardPrefix = getClassMacroGuardPrefix();
    std::string fileHeader = getHeaderTemplate();

    fprintf(header, "%s", fileHeader.c_str());
    fprintf(header, "#ifndef %s\n", macroGuard.c_str());
    fprintf(header, "#define %s\n", macroGuard.c_str());
    fprintf(header, "\n");
    fprintf(header, "#include <stddef.h>\n");
    fprintf(header, "\n");
    fprintf(header, "namespace %s\n", mNamespace.c_str());
    fprintf(header, "{\n");
    fprintf(header, "  #ifndef %s_EMBEDDEDFILE_CLASS\n", classMacroGuardPrefix.c_str());
    fprintf(header, "  #define %s_EMBEDDEDFILE_CLASS\n", classMacroGuardPrefix.c_str());
    fprintf(header, "  class %s\n", mBaseClass.c_str());
    fprintf(header, "  {\n");
    fprintf(header, "  public:\n");
    fprintf(header, "    virtual size_t getSize() const = 0;\n");
    fprintf(header, "    /* DEPRECATED */ virtual inline const char * getFilename() const { return getFileName(); }\n");
    fprintf(header, "    virtual const char * getFileName() const = 0;\n");
    fprintf(header, "    virtual const char * getFilePath() const = 0;\n");
    fprintf(header, "    virtual const char * getBuffer() const = 0;\n");
    fprintf(header, "    virtual bool save(const char * filename) const = 0;\n");
    fprintf(header, "  };\n");
    fprintf(header, "  #endif //%s_EMBEDDEDFILE_CLASS\n", classMacroGuardPrefix.c_str());
    fprintf(header, "  const %s & %s();\n", mBaseClass.c_str(), getGetterFunctionName().c_str());
    fprintf(header, "}; //%s\n", mNamespace.c_str());
    fprintf(header, "\n");
    fprintf(header, "#endif //%s\n", macroGuard.c_str());

    fclose(header);

    return true;
  }

  bool BaseGenerator::createManagerHeaderFile(const char * header_file_path)
  {
    FILE * header = fopen(header_file_path, "w");
    if (!header)
      return false;

    //define macro guard a macro matching the filename
    std::string macroGuard;
    macroGuard += getCppIncludeGuardMacroName(mNamespace.c_str()); //prefix the custom namespace for the file manager
    if (!macroGuard.empty())
      macroGuard += "_";
    macroGuard += getCppIncludeGuardMacroName(header_file_path);

    std::string classMacroGuardPrefix = getClassMacroGuardPrefix();
    std::string fileHeader = getHeaderTemplate(false);

    fprintf(header, "%s", fileHeader.c_str());
    fprintf(header, "#ifndef %s\n", macroGuard.c_str());
    fprintf(header, "#define %s\n", macroGuard.c_str());
    fprintf(header, "\n");
    fprintf(header, "#include <stddef.h>\n");
    fprintf(header, "#include <vector>\n");
    fprintf(header, "\n");
    fprintf(header, "namespace %s\n", mNamespace.c_str());
    fprintf(header, "{\n");
    fprintf(header, "  #ifndef %s_EMBEDDEDFILE_CLASS\n", classMacroGuardPrefix.c_str());
    fprintf(header, "  #define %s_EMBEDDEDFILE_CLASS\n", classMacroGuardPrefix.c_str());
    fprintf(header, "  class %s\n", mBaseClass.c_str());
    fprintf(header, "  {\n");
    fprintf(header, "  public:\n");
    fprintf(header, "    virtual size_t getSize() const = 0;\n");
    fprintf(header, "    /* DEPRECATED */ virtual inline const char * getFilename() const { return getFileName(); }\n");
    fprintf(header, "    virtual const char * getFileName() const = 0;\n");
    fprintf(header, "    virtual const char * getFilePath() const = 0;\n");
    fprintf(header, "    virtual const char * getBuffer() const = 0;\n");
    fprintf(header, "    virtual bool save(const char * filename) const = 0;\n");
    fprintf(header, "  };\n");
    fprintf(header, "  #endif //%s_EMBEDDEDFILE_CLASS\n", classMacroGuardPrefix.c_str());
    fprintf(header, "\n");
    fprintf(header, "  #ifndef %s_FILEMANAGER_CLASS\n", classMacroGuardPrefix.c_str());
    fprintf(header, "  #define %s_FILEMANAGER_CLASS\n", classMacroGuardPrefix.c_str());        
    fprintf(header, "  class FileManager\n");
    fprintf(header, "  {\n");
    fprintf(header, "  private:\n");
    fprintf(header, "    FileManager();\n");
    fprintf(header, "    ~FileManager();\n");
    fprintf(header, "  public:\n");
    fprintf(header, "    typedef const %s & (*t_func)();\n", mBaseClass.c_str());
    fprintf(header, "    static FileManager & getInstance();\n");
    fprintf(header, "    void registerFile(t_func func);\n");
    fprintf(header, "    size_t getFileCount() const;\n");
    fprintf(header, "    const %s * getFile(const size_t & index) const;\n", mBaseClass.c_str());
    fprintf(header, "    bool saveFiles(const char * directory) const;\n");
    fprintf(header, "    bool createParentDirectories(const char * file_path) const;\n");
    fprintf(header, "    bool createDirectories(const char * path) const;\n");
    fprintf(header, "  private:\n");
    fprintf(header, "    std::vector<t_func> functions_;\n");
    fprintf(header, "  };\n");
    fprintf(header, "  #endif //%s_FILEMANAGER_CLASS\n", classMacroGuardPrefix.c_str());
    fprintf(header, "}; //%s\n", mNamespace.c_str());
    fprintf(header, "\n");
    fprintf(header, "#endif //%s\n", macroGuard.c_str());

    fclose(header);

    return true;
  }

  bool BaseGenerator::createManagerSourceFile(const char * cpp_file_path)
  {
    FILE * cpp = fopen(cpp_file_path, "w");
    if (!cpp)
      return false;

    //Build header and cpp file path
    std::string headerPath = getHeaderFilePath(cpp_file_path);
    std::string cppPath = cpp_file_path;

    std::string fileHeader = getHeaderTemplate(false);

    fprintf(cpp, "%s", fileHeader.c_str());
    fprintf(cpp, "#include \"%s\"\n", getManagerHeaderFilename());
    fprintf(cpp, "#include <string>\n");
    fprintf(cpp, "#include <string.h> // strlen\n");
    fprintf(cpp, "#include <sys/stat.h> // stat\n");
    fprintf(cpp, "#include <errno.h>    // errno, EEXIST\n");
    fprintf(cpp, "#if defined(_WIN32)\n");
    fprintf(cpp, "#include <direct.h>   // _mkdir\n");
    fprintf(cpp, "#endif\n");
    fprintf(cpp, "\n");
    fprintf(cpp, "#if defined(_WIN32)\n");
    fprintf(cpp, "#define portable_stat _stat\n");
    fprintf(cpp, "#define portable_mkdir(path) _mkdir(path)\n");
    fprintf(cpp, "#define PATH_SEPARATOR_CHAR '\\\\'\n");
    fprintf(cpp, "#define PATH_SEPARATOR_STR \"\\\\\"\n");
    fprintf(cpp, "#else\n");
    fprintf(cpp, "#define portable_stat stat\n");
    fprintf(cpp, "#define portable_mkdir(path) mkdir(path, 0755)\n");
    fprintf(cpp, "#define PATH_SEPARATOR_CHAR '/'\n");
    fprintf(cpp, "#define PATH_SEPARATOR_STR \"/\"\n");
    fprintf(cpp, "#endif\n");
    fprintf(cpp, "\n");
    fprintf(cpp, "namespace %s\n", mNamespace.c_str());
    fprintf(cpp, "{\n");
    fprintf(cpp, "  bool RegisterFile(FileManager::t_func functionPointer)\n");
    fprintf(cpp, "  {\n");
    fprintf(cpp, "    if (functionPointer == NULL)\n");
    fprintf(cpp, "      return false;\n");
    fprintf(cpp, "    FileManager::getInstance().registerFile(functionPointer);\n");
    fprintf(cpp, "    return true;\n");
    fprintf(cpp, "  }\n");
    fprintf(cpp, "  FileManager::FileManager() {}\n");
    fprintf(cpp, "  FileManager::~FileManager() {}\n");
    fprintf(cpp, "  FileManager & FileManager::getInstance() { static FileManager _mgr; return _mgr; }\n");
    fprintf(cpp, "  void FileManager::registerFile(t_func func) { functions_.push_back(func); }\n");
    fprintf(cpp, "  size_t FileManager::getFileCount() const { return functions_.size(); }\n");
    fprintf(cpp, "  const File * FileManager::getFile(const size_t & index) const\n");
    fprintf(cpp, "  {\n");
    fprintf(cpp, "    if (index >= functions_.size())\n");
    fprintf(cpp, "      return NULL;\n");
    fprintf(cpp, "    t_func ressource_getter_function = functions_[index];\n");
    fprintf(cpp, "    const %s::File & resource = ressource_getter_function();\n", mNamespace.c_str());
    fprintf(cpp, "    return &resource;\n");
    fprintf(cpp, "  }\n");
    fprintf(cpp, "  bool FileManager::saveFiles(const char * directory) const\n");
    fprintf(cpp, "  {\n");
    fprintf(cpp, "    if (directory == NULL)\n");
    fprintf(cpp, "      return false;\n");
    fprintf(cpp, "    size_t count = getFileCount();\n");
    fprintf(cpp, "    for(size_t i=0; i<count; i++)\n");
    fprintf(cpp, "    {\n");
    fprintf(cpp, "      const File * f = getFile(i);\n");
    fprintf(cpp, "      if (!f)\n");
    fprintf(cpp, "        return false;\n");
    fprintf(cpp, "      std::string path;\n");
    fprintf(cpp, "      path.append(directory);\n");
    fprintf(cpp, "      path.append(1,PATH_SEPARATOR_CHAR);\n");
    fprintf(cpp, "      path.append(f->getFilePath());\n");
    fprintf(cpp, "      if (!createParentDirectories(path.c_str()))\n");
    fprintf(cpp, "        return false;\n");
    fprintf(cpp, "      bool saved = f->save(path.c_str());\n");
    fprintf(cpp, "      if (!saved)\n");
    fprintf(cpp, "        return false;\n");
    fprintf(cpp, "    }\n");
    fprintf(cpp, "    return true;\n");
    fprintf(cpp, "  }\n");
    fprintf(cpp, "  static inline bool isRootDirectory(const char * path)\n");
    fprintf(cpp, "  {\n");
    fprintf(cpp, "    if (path == NULL && path[0] == '\\0')\n");
    fprintf(cpp, "      return false;\n");
    fprintf(cpp, "  #if defined(_WIN32)\n");
    fprintf(cpp, "    bool isDriveLetter = ((path[0] >= 'a' && path[0] <= 'z') || (path[0] >= 'A' && path[0] <= 'Z'));\n");
    fprintf(cpp, "    if ((isDriveLetter && path[1] == ':' && path[2] == '\\0') || // test for C:\n");
    fprintf(cpp, "        (isDriveLetter && path[1] == ':' && path[2] == PATH_SEPARATOR_CHAR && path[3] == '\\0')) // test for C:\\ \n");
    fprintf(cpp, "      return true;\n");
    fprintf(cpp, "  #else\n");
    fprintf(cpp, "    if (path[0] == PATH_SEPARATOR_CHAR)\n");
    fprintf(cpp, "      return true;\n");
    fprintf(cpp, "  #endif\n");
    fprintf(cpp, "    return false;\n");
    fprintf(cpp, "  }\n");
    fprintf(cpp, "  bool FileManager::createParentDirectories(const char * file_path) const\n");
    fprintf(cpp, "  {\n");
    fprintf(cpp, "    if (file_path == NULL)\n");
    fprintf(cpp, "      return false;\n");
    fprintf(cpp, "    std::string accumulator;\n");
    fprintf(cpp, "    size_t length = strlen(file_path);\n");
    fprintf(cpp, "    for(size_t i=0; i<length; i++)\n");
    fprintf(cpp, "    {\n");
    fprintf(cpp, "      if (file_path[i] == PATH_SEPARATOR_CHAR && !accumulator.empty() && !isRootDirectory(accumulator.c_str()))\n");
    fprintf(cpp, "      {\n");
    fprintf(cpp, "        int ret = portable_mkdir(accumulator.c_str());\n");
    fprintf(cpp, "        if (ret != 0 && errno != EEXIST)\n");
    fprintf(cpp, "          return false;\n");
    fprintf(cpp, "      }\n");
    fprintf(cpp, "      accumulator += file_path[i];\n");
    fprintf(cpp, "    }\n");
    fprintf(cpp, "    return true;\n");
    fprintf(cpp, "  }\n");
    fprintf(cpp, "  bool FileManager::createDirectories(const char * path) const\n");
    fprintf(cpp, "  {\n");
    fprintf(cpp, "    if (path == NULL)\n");
    fprintf(cpp, "      return false;\n");
    fprintf(cpp, "    std::string path_copy = path;\n");
    fprintf(cpp, "    path_copy.append(1,PATH_SEPARATOR_CHAR);\n");
    fprintf(cpp, "    return createParentDirectories(path_copy.c_str());\n");
    fprintf(cpp, "  }\n");
    fprintf(cpp, "}; //%s\n", mNamespace.c_str());

    fclose(cpp);

    return true;
  }

  bool BaseGenerator::printFileContent()
  {
    //check if input file exists
    FILE * input = fopen(getInputFilePath(), "rb");
    if (!input)
      return false;

    //determine file properties
    uint32_t fileSize = ra::filesystem::GetFileSize(input);

    //create buffer for each chunks from input buffer
    int numLinePrinted = 0;
    unsigned char * buffer = new unsigned char[mChunkSize];
    while(!feof(input))
    {
      //read a chunk of the file
      size_t readSize = fread(buffer, 1, mChunkSize, input);

      bool isLastChunk = !(readSize == mChunkSize);

      if (readSize > 0)
      {
        if (numLinePrinted > 0)
        {
          //end previous line
          printf("\n");
        }

        //output
        std::string text;
        switch(mCppEncoder)
        {
        case IGenerator::CPP_ENCODER_HEX:
          text = ra::code::cpp::ToHexString(buffer, readSize);
          break;
        case IGenerator::CPP_ENCODER_OCT:
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

}; //bin2cpp