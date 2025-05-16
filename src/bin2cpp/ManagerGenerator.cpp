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

#include "ManagerGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>

#include "common.h"

namespace bin2cpp
{
  ManagerGenerator::ManagerGenerator()
  {
  }

  ManagerGenerator::~ManagerGenerator()
  {
  }

  const char * ManagerGenerator::getName() const
  {
    return "manager";
  }

  bool ManagerGenerator::createCppHeaderFile(const char* header_file_path)
  {
    FILE* header = fopen(header_file_path, "w");
    if ( !header )
      return false;

    //define macro guard a macro matching the filename
    std::string macroGuard;
    macroGuard += getCppIncludeGuardMacroName(mContext.codeNamespace.c_str()); //prefix the custom namespace for the file manager
    if ( !macroGuard.empty() )
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
    fprintf(header, "namespace %s\n", mContext.codeNamespace.c_str());
    fprintf(header, "{\n");
    fprintf(header, "  #ifndef %s_EMBEDDEDFILE_CLASS\n", classMacroGuardPrefix.c_str());
    fprintf(header, "  #define %s_EMBEDDEDFILE_CLASS\n", classMacroGuardPrefix.c_str());
    fprintf(header, "  class %s\n", mContext.baseClass.c_str());
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
    fprintf(header, "    typedef const %s & (*t_func)();\n", mContext.baseClass.c_str());
    fprintf(header, "    static FileManager & getInstance();\n");
    fprintf(header, "    void registerFile(t_func func);\n");
    fprintf(header, "    size_t getFileCount() const;\n");
    fprintf(header, "    const %s * getFile(const size_t & index) const;\n", mContext.baseClass.c_str());
    fprintf(header, "    bool saveFiles(const char * directory) const;\n");
    fprintf(header, "    bool createParentDirectories(const char * file_path) const;\n");
    fprintf(header, "    bool createDirectories(const char * path) const;\n");
    fprintf(header, "  private:\n");
    fprintf(header, "    std::vector<t_func> functions_;\n");
    fprintf(header, "  };\n");
    fprintf(header, "  #endif //%s_FILEMANAGER_CLASS\n", classMacroGuardPrefix.c_str());
    fprintf(header, "}; //%s\n", mContext.codeNamespace.c_str());
    fprintf(header, "\n");
    fprintf(header, "#endif //%s\n", macroGuard.c_str());

    fclose(header);

    return true;
  }

  bool ManagerGenerator::createCppSourceFile(const char* cpp_file_path)
  {
    FILE* cpp = fopen(cpp_file_path, "w");
    if ( !cpp )
      return false;

    //Build header and cpp file path
    std::string headerPath = getHeaderFilePath(cpp_file_path);
    std::string cppPath = cpp_file_path;

    std::string fileHeader = getHeaderTemplate(false);

    fprintf(cpp, "%s", fileHeader.c_str());
    fprintf(cpp, "#include \"%s\"\n", mContext.managerHeaderFilename.c_str());
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
    fprintf(cpp, "namespace %s\n", mContext.codeNamespace.c_str());
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
    fprintf(cpp, "    const %s::File & resource = ressource_getter_function();\n", mContext.codeNamespace.c_str());
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
    fprintf(cpp, "}; //%s\n", mContext.codeNamespace.c_str());

    fclose(cpp);

    return true;
  }

  bool ManagerGenerator::createCHeaderFile(const char* file_path)
  {
    FILE* fout = fopen(file_path, "w");
    if ( !fout )
      return false;

    //define macro guard a macro matching the filename
    std::string macroGuard;
    macroGuard += getCppIncludeGuardMacroName(mContext.codeNamespace.c_str()); //prefix the custom namespace for the file manager
    if ( !macroGuard.empty() )
      macroGuard += "_";
    macroGuard += getCppIncludeGuardMacroName(file_path);

    std::string classMacroGuardPrefix = getClassMacroGuardPrefix();
    std::string fileHeader = getHeaderTemplate(false);

    fprintf(fout, "%s", fileHeader.c_str());
    fprintf(fout, "#ifndef %s\n", macroGuard.c_str());
    fprintf(fout, "#define %s\n", macroGuard.c_str());
    fprintf(fout, "\n");
    fprintf(fout, "#include <stddef.h>\n");
    fprintf(fout, "#include <stdbool.h>\n");
    fprintf(fout, "\n");
    fprintf(fout, "#ifndef %s_EMBEDDEDFILE_STRUCT\n", classMacroGuardPrefix.c_str());
    fprintf(fout, "#define %s_EMBEDDEDFILE_STRUCT\n", classMacroGuardPrefix.c_str());
    fprintf(fout, "typedef struct %s %s;\n", mContext.baseClass.c_str(), mContext.baseClass.c_str());
    fprintf(fout, "typedef bool(*bin2c_load_func)();\n");
    fprintf(fout, "typedef void(*bin2c_free_func)();\n");
    fprintf(fout, "typedef bool(*bin2c_save_func)(const char*);\n");
    fprintf(fout, "typedef struct %s\n", mContext.baseClass.c_str());
    fprintf(fout, "{\n");
    fprintf(fout, "  size_t size;\n");
    fprintf(fout, "  const char* file_name;\n");
    fprintf(fout, "  const char* file_path;\n");
    fprintf(fout, "  const unsigned char* buffer;\n");
    fprintf(fout, "  bin2c_load_func load;\n");
    fprintf(fout, "  bin2c_free_func unload;\n");
    fprintf(fout, "  bin2c_save_func save;\n");
    fprintf(fout, "} %s;\n", mContext.baseClass.c_str());
    fprintf(fout, "typedef %s* %sPtr;\n", mContext.baseClass.c_str(), mContext.baseClass.c_str());
    fprintf(fout, "#endif //%s_EMBEDDEDFILE_STRUCT\n", classMacroGuardPrefix.c_str());
    fprintf(fout, "\n");
    fprintf(fout, "size_t bin2c_filemanager_get_file_count();\n");
    fprintf(fout, "bool bin2c_filemanager_register_file(%s* file);\n", mContext.baseClass.c_str());
    fprintf(fout, "const %s* bin2c_filemanager_get_file(size_t index);\n", mContext.baseClass.c_str());
    fprintf(fout, "bool bin2c_filemanager_save_files(const char* directory);\n");
    fprintf(fout, "\n");
    fprintf(fout, "#endif //%s\n", macroGuard.c_str());

    fclose(fout);

    return true;
  }

  bool ManagerGenerator::createCSourceFile(const char* file_path)
  {
    FILE* fout = fopen(file_path, "w");
    if ( !fout )
      return false;

    //Build header and cpp file path
    std::string headerPath = getHeaderFilePath(file_path);
    std::string sourcePath = file_path;

    std::string fileHeader = getHeaderTemplate(false);

    fprintf(fout, "%s", fileHeader.c_str());
    fprintf(fout, "#if defined(_WIN32) && !defined(_CRT_SECURE_NO_WARNINGS)\n");
    fprintf(fout, "#define _CRT_SECURE_NO_WARNINGS\n");
    fprintf(fout, "#endif\n");
    fprintf(fout, "\n");
    fprintf(fout, "#include \"%s\"\n", mContext.managerHeaderFilename.c_str());
    fprintf(fout, "#include <stdlib.h> // for malloc\n");
    fprintf(fout, "#include <stdio.h>  // for snprintf()\n");
    fprintf(fout, "#include <string.h> // strlen\n");
    fprintf(fout, "#include <sys/stat.h> // stat\n");
    fprintf(fout, "#include <errno.h>    // errno, EEXIST\n");
    fprintf(fout, "#if defined(_WIN32)\n");
    fprintf(fout, "#include <direct.h>   // _mkdir\n");
    fprintf(fout, "#endif\n");
    fprintf(fout, "#if defined(_WIN32)\n");
    fprintf(fout, "#define portable_stat _stat\n");
    fprintf(fout, "#define portable_mkdir(path) _mkdir(path)\n");
    fprintf(fout, "#define PATH_SEPARATOR_CHAR '\\\\'\n");
    fprintf(fout, "#define PATH_SEPARATOR_STR \"\\\\\"\n");
    fprintf(fout, "#else\n");
    fprintf(fout, "#define portable_stat stat\n");
    fprintf(fout, "#define portable_mkdir(path) mkdir(path, 0755)\n");
    fprintf(fout, "#define PATH_SEPARATOR_CHAR '/'\n");
    fprintf(fout, "#define PATH_SEPARATOR_STR \"/\"\n");
    fprintf(fout, "#endif\n");
    fprintf(fout, "\n");
    fprintf(fout, "#define BIN2C_MAX_PATH 32767\n");
    fprintf(fout, "\n");
    fprintf(fout, "static %s** registered_files = NULL;\n", mContext.baseClass.c_str());
    fprintf(fout, "static size_t registered_files_count = 0;\n");
    fprintf(fout, "\n");
    fprintf(fout, "size_t bin2c_filemanager_get_file_count()\n");
    fprintf(fout, "{\n");
    fprintf(fout, "  return registered_files_count;\n");
    fprintf(fout, "}\n");
    fprintf(fout, "\n");
    fprintf(fout, "bool bin2c_filemanager_register_file(%s* file)\n", mContext.baseClass.c_str());
    fprintf(fout, "{\n");
    fprintf(fout, "  // allocate ram\n");
    fprintf(fout, "  size_t new_ram_size = sizeof(%s**) * (registered_files_count + 1);\n", mContext.baseClass.c_str());
    fprintf(fout, "  %s** tmp = NULL;\n", mContext.baseClass.c_str());
    fprintf(fout, "  if ( registered_files == NULL )\n");
    fprintf(fout, "    tmp = (%s**)malloc(new_ram_size);\n", mContext.baseClass.c_str());
    fprintf(fout, "  else\n");
    fprintf(fout, "    tmp = (%s**)realloc(registered_files, new_ram_size);\n", mContext.baseClass.c_str());
    fprintf(fout, "  if ( tmp == NULL )\n");
    fprintf(fout, "    return false;\n");
    fprintf(fout, "  \n");
    fprintf(fout, "  registered_files = tmp;\n");
    fprintf(fout, "  registered_files_count++;\n");
    fprintf(fout, "  \n");
    fprintf(fout, "  // insert\n");
    fprintf(fout, "  registered_files[registered_files_count - 1] = file;\n");
    fprintf(fout, "  \n");
    fprintf(fout, "  return true;\n");
    fprintf(fout, "}\n");
    fprintf(fout, "\n");
    fprintf(fout, "const %s* bin2c_filemanager_get_file(size_t index)\n", mContext.baseClass.c_str());
    fprintf(fout, "{\n");
    fprintf(fout, "  if ( index >= registered_files_count )\n");
    fprintf(fout, "    return NULL;\n");
    fprintf(fout, "  return registered_files[index];\n");
    fprintf(fout, "}\n");
    fprintf(fout, "\n");
    fprintf(fout, "static inline bool bin2c_filemanager_is_root_directory(const char* path)\n");
    fprintf(fout, "{\n");
    fprintf(fout, "  if ( path == NULL && path[0] == '\0' )\n");
    fprintf(fout, "    return false;\n");
    fprintf(fout, "#if defined(_WIN32)\n");
    fprintf(fout, "  bool is_drive_letter = ((path[0] >= 'a' && path[0] <= 'z') || (path[0] >= 'A' && path[0] <= 'Z'));\n");
    fprintf(fout, "  if ( (is_drive_letter && path[1] == ':' && path[2] == '\0') || // test for C:\n");
    fprintf(fout, "      (is_drive_letter && path[1] == ':' && path[2] == PATH_SEPARATOR_CHAR && path[3] == '\0') ) // test for C:\\ \n");
    fprintf(fout, "    return true;\n");
    fprintf(fout, "#else\n");
    fprintf(fout, "  if ( path[0] == PATH_SEPARATOR_CHAR )\n");
    fprintf(fout, "    return true;\n");
    fprintf(fout, "#endif\n");
    fprintf(fout, "  return false;\n");
    fprintf(fout, "}\n");
    fprintf(fout, "\n");
    fprintf(fout, "bool bin2c_filemanager_create_parent_directories(const char* file_path)\n");
    fprintf(fout, "{\n");
    fprintf(fout, "  if ( file_path == NULL )\n");
    fprintf(fout, "    return false;\n");
    fprintf(fout, "  char* accumulator = (char*)malloc(BIN2C_MAX_PATH);\n");
    fprintf(fout, "  if ( accumulator == NULL )\n");
    fprintf(fout, "    return false;\n");
    fprintf(fout, "  accumulator[0] = '\0';\n");
    fprintf(fout, "  size_t length = strlen(file_path);\n");
    fprintf(fout, "  for ( size_t i = 0; i < length; i++ )\n");
    fprintf(fout, "  {\n");
    fprintf(fout, "    if ( file_path[i] == PATH_SEPARATOR_CHAR && !(accumulator[0] == '\0') && !bin2c_filemanager_is_root_directory(accumulator) )\n");
    fprintf(fout, "    {\n");
    fprintf(fout, "      int ret = portable_mkdir(accumulator);\n");
    fprintf(fout, "      if ( ret != 0 && errno != EEXIST )\n");
    fprintf(fout, "      {\n");
    fprintf(fout, "        free(accumulator);\n");
    fprintf(fout, "        return false;\n");
    fprintf(fout, "      }\n");
    fprintf(fout, "    }\n");
    fprintf(fout, "    \n");
    fprintf(fout, "    // append\n");
    fprintf(fout, "    char tmp[] = { file_path[i], '\0' };\n");
    fprintf(fout, "    strcat(accumulator, tmp);\n");
    fprintf(fout, "  }\n");
    fprintf(fout, "  free(accumulator);\n");
    fprintf(fout, "  return true;\n");
    fprintf(fout, "}\n");
    fprintf(fout, "\n");
    fprintf(fout, "bool bin2c_filemanager_save_files(const char * directory)\n");
    fprintf(fout, "{\n");
    fprintf(fout, "  if (directory == NULL)\n");
    fprintf(fout, "    return false;\n");
    fprintf(fout, "  char* path = (char*)malloc(BIN2C_MAX_PATH);\n");
    fprintf(fout, "  if ( path == NULL )\n");
    fprintf(fout, "    return false;\n");
    fprintf(fout, "  path[0] = '\0';\n");
    fprintf(fout, "  for(size_t i=0; i< registered_files_count; i++)\n");
    fprintf(fout, "  {\n");
    fprintf(fout, "    const Bin2cFile* f = bin2c_filemanager_get_file(i);\n");
    fprintf(fout, "    if ( !f )\n");
    fprintf(fout, "    {\n");
    fprintf(fout, "      free(path);\n");
    fprintf(fout, "      return false;\n");
    fprintf(fout, "    }\n");
    fprintf(fout, "    \n");
    fprintf(fout, "    char path[32767] = { 0 };\n");
    fprintf(fout, "    snprintf(path, sizeof(path), \"%%s%%c%%s\", directory, PATH_SEPARATOR_CHAR, f->file_path);\n");
    fprintf(fout, "    \n");
    fprintf(fout, "    if (!bin2c_filemanager_create_parent_directories(path))\n");
    fprintf(fout, "    {\n");
    fprintf(fout, "      free(path);\n");
    fprintf(fout, "      return false;\n");
    fprintf(fout, "    }\n");
    fprintf(fout, "    bool saved = f->save(path);\n");
    fprintf(fout, "    if (!saved)\n");
    fprintf(fout, "    {\n");
    fprintf(fout, "      free(path);\n");
    fprintf(fout, "      return false;\n");
    fprintf(fout, "    }\n");
    fprintf(fout, "  }\n");
    fprintf(fout, "  free(path);\n");
    fprintf(fout, "  return true;\n");
    fprintf(fout, "}\n");
    fprintf(fout, "\n");

    fclose(fout);

    return true;
  }

  bool ManagerGenerator::printFileContent()
  {
    return false;
  }

}; //bin2cpp