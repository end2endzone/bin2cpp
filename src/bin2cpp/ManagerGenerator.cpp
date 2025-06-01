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
#include "TemplateProcessor.h"
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

  bool ManagerGenerator::createCppHeaderFile(const char* file_path)
  {
    const std::string text = ""
      "${bin2cpp_file_manager_file_header}"
      "#ifndef ${bin2cpp_file_manager_macro_guard_prefix}\n"
      "#define ${bin2cpp_file_manager_macro_guard_prefix}\n"
      "\n"
      "#include <stddef.h>\n"
      "#include <vector>\n"
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
      "\n"
      "  #ifndef ${bin2cpp_file_object_macro_guard_prefix}_FILEMANAGER_CLASS\n"
      "  #define ${bin2cpp_file_object_macro_guard_prefix}_FILEMANAGER_CLASS\n"
      "  class FileManager\n"
      "  {\n"
      "  private:\n"
      "    FileManager();\n"
      "    ~FileManager();\n"
      "  public:\n"
      "    typedef const ${bin2cpp_baseclass} & (*t_func)();\n"
      "    static FileManager & getInstance();\n"
      "    void registerFile(t_func func);\n"
      "    size_t getFileCount() const;\n"
      "    const ${bin2cpp_baseclass} * getFile(const size_t & index) const;\n"
      "    bool saveFiles(const char * directory) const;\n"
      "    bool createParentDirectories(const char * file_path) const;\n"
      "    bool createDirectories(const char * path) const;\n"
      "  private:\n"
      "    std::vector<t_func> functions_;\n"
      "  };\n"
      "  #endif //${bin2cpp_file_object_macro_guard_prefix}_FILEMANAGER_CLASS\n"
      "}; //${bin2cpp_namespace}\n"
      "\n"
      "#endif //${bin2cpp_file_manager_macro_guard_prefix}\n"
    ;

    TemplateProcessor processor(&text);
    processor.setTemplateVariableHandler(this);
    bool write_success = processor.writeFile(file_path);

    return write_success;
  }

  bool ManagerGenerator::createCppSourceFile(const char* file_path)
  {
    const std::string text = ""
      "${bin2cpp_file_manager_file_header}"
      "#include \"${bin2cpp_file_manager_header_file_name}\"\n"
      "#include <string>\n"
      "#include <string.h> // strlen\n"
      "#include <sys/stat.h> // stat\n"
      "#include <errno.h>    // errno, EEXIST\n"
      "#if defined(_WIN32)\n"
      "#include <direct.h>   // _mkdir\n"
      "#endif\n"
      "\n"
      "#if defined(_WIN32)\n"
      "#define portable_stat _stat\n"
      "#define portable_mkdir(path) _mkdir(path)\n"
      "#define PATH_SEPARATOR_CHAR '\\\\'\n"
      "#define PATH_SEPARATOR_STR \"\\\\\"\n"
      "#else\n"
      "#define portable_stat stat\n"
      "#define portable_mkdir(path) mkdir(path, 0755)\n"
      "#define PATH_SEPARATOR_CHAR '/'\n"
      "#define PATH_SEPARATOR_STR \"/\"\n"
      "#endif\n"
      "\n"
      "namespace ${bin2cpp_namespace}\n"
      "{\n"
      "  bool RegisterFile(FileManager::t_func functionPointer)\n"
      "  {\n"
      "    if (functionPointer == NULL)\n"
      "      return false;\n"
      "    FileManager::getInstance().registerFile(functionPointer);\n"
      "    return true;\n"
      "  }\n"
      "  FileManager::FileManager() {}\n"
      "  FileManager::~FileManager() {}\n"
      "  FileManager & FileManager::getInstance() { static FileManager _mgr; return _mgr; }\n"
      "  void FileManager::registerFile(t_func func) { functions_.push_back(func); }\n"
      "  size_t FileManager::getFileCount() const { return functions_.size(); }\n"
      "  const File * FileManager::getFile(const size_t & index) const\n"
      "  {\n"
      "    if (index >= functions_.size())\n"
      "      return NULL;\n"
      "    t_func ressource_getter_function = functions_[index];\n"
      "    const ${bin2cpp_namespace}::File & resource = ressource_getter_function();\n"
      "    return &resource;\n"
      "  }\n"
      "  bool FileManager::saveFiles(const char * directory) const\n"
      "  {\n"
      "    if (directory == NULL)\n"
      "      return false;\n"
      "    size_t count = getFileCount();\n"
      "    for(size_t i=0; i<count; i++)\n"
      "    {\n"
      "      const File * f = getFile(i);\n"
      "      if (!f)\n"
      "        return false;\n"
      "      std::string path;\n"
      "      path.append(directory);\n"
      "      path.append(1,PATH_SEPARATOR_CHAR);\n"
      "      path.append(f->getFilePath());\n"
      "      if (!createParentDirectories(path.c_str()))\n"
      "        return false;\n"
      "      bool saved = f->save(path.c_str());\n"
      "      if (!saved)\n"
      "        return false;\n"
      "    }\n"
      "    return true;\n"
      "  }\n"
      "  static inline bool isRootDirectory(const char * path)\n"
      "  {\n"
      "    if (path == NULL && path[0] == '\\0')\n"
      "      return false;\n"
      "  #if defined(_WIN32)\n"
      "    bool isDriveLetter = ((path[0] >= 'a' && path[0] <= 'z') || (path[0] >= 'A' && path[0] <= 'Z'));\n"
      "    if ((isDriveLetter && path[1] == ':' && path[2] == '\\0') || // test for C:\n"
      "        (isDriveLetter && path[1] == ':' && path[2] == PATH_SEPARATOR_CHAR && path[3] == '\\0')) // test for C:\\ \n"
      "      return true;\n"
      "  #else\n"
      "    if (path[0] == PATH_SEPARATOR_CHAR)\n"
      "      return true;\n"
      "  #endif\n"
      "    return false;\n"
      "  }\n"
      "  bool FileManager::createParentDirectories(const char * file_path) const\n"
      "  {\n"
      "    if (file_path == NULL)\n"
      "      return false;\n"
      "    std::string accumulator;\n"
      "    size_t length = strlen(file_path);\n"
      "    for(size_t i=0; i<length; i++)\n"
      "    {\n"
      "      if (file_path[i] == PATH_SEPARATOR_CHAR && !accumulator.empty() && !isRootDirectory(accumulator.c_str()))\n"
      "      {\n"
      "        int ret = portable_mkdir(accumulator.c_str());\n"
      "        if (ret != 0 && errno != EEXIST)\n"
      "          return false;\n"
      "      }\n"
      "      accumulator += file_path[i];\n"
      "    }\n"
      "    return true;\n"
      "  }\n"
      "  bool FileManager::createDirectories(const char * path) const\n"
      "  {\n"
      "    if (path == NULL)\n"
      "      return false;\n"
      "    std::string path_copy = path;\n"
      "    path_copy.append(1,PATH_SEPARATOR_CHAR);\n"
      "    return createParentDirectories(path_copy.c_str());\n"
      "  }\n"
      "}; //${bin2cpp_namespace}\n"
    ;

    TemplateProcessor processor(&text);
    processor.setTemplateVariableHandler(this);
    bool write_success = processor.writeFile(file_path);

    return write_success;
  }

  bool ManagerGenerator::createCHeaderFile(const char* file_path)
  {
    const std::string text = ""
      "${bin2cpp_file_manager_file_header}"
      "#ifndef ${bin2cpp_file_manager_macro_guard_prefix}\n"
      "#define ${bin2cpp_file_manager_macro_guard_prefix}\n"
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
      "\n"
      "size_t ${bin2cpp_namespace}_filemanager_get_file_count();\n"
      "bool ${bin2cpp_namespace}_filemanager_register_file(${bin2cpp_baseclass}* file);\n"
      "const ${bin2cpp_baseclass}* ${bin2cpp_namespace}_filemanager_get_file(size_t index);\n"
      "bool ${bin2cpp_namespace}_filemanager_save_files(const char* directory);\n"
      "\n"
      "#endif //${bin2cpp_file_manager_macro_guard_prefix}\n"
    ;

    TemplateProcessor processor(&text);
    processor.setTemplateVariableHandler(this);
    bool write_success = processor.writeFile(file_path);

    return write_success;
  }

  bool ManagerGenerator::createCSourceFile(const char* file_path)
  {
    const std::string text = ""
      "${bin2cpp_file_manager_file_header}"
      "#if defined(_WIN32) && !defined(_CRT_SECURE_NO_WARNINGS)\n"
      "#define _CRT_SECURE_NO_WARNINGS\n"
      "#endif\n"
      "\n"
      "#include \"${bin2cpp_file_manager_header_file_name}\"\n"
      "#include <stdlib.h> // for malloc\n"
      "#include <stdio.h>  // for snprintf()\n"
      "#include <string.h> // strlen\n"
      "#include <sys/stat.h> // stat\n"
      "#include <errno.h>    // errno, EEXIST\n"
      "#if defined(_WIN32)\n"
      "#include <direct.h>   // _mkdir\n"
      "#endif\n"
      "#if defined(_WIN32)\n"
      "#define portable_stat _stat\n"
      "#define portable_mkdir(path) _mkdir(path)\n"
      "#define PATH_SEPARATOR_CHAR '\\\\'\n"
      "#define PATH_SEPARATOR_STR \"\\\\\"\n"
      "#else\n"
      "#define portable_stat stat\n"
      "#define portable_mkdir(path) mkdir(path, 0755)\n"
      "#define PATH_SEPARATOR_CHAR '/'\n"
      "#define PATH_SEPARATOR_STR \"/\"\n"
      "#endif\n"
      "\n"
      "#define BIN2C_MAX_PATH 32767\n"
      "\n"
      "static ${bin2cpp_baseclass}** registered_files = NULL;\n"
      "static size_t registered_files_count = 0;\n"
      "\n"
      "size_t ${bin2cpp_namespace}_filemanager_get_file_count()\n"
      "{\n"
      "  return registered_files_count;\n"
      "}\n"
      "\n"
      "bool ${bin2cpp_namespace}_filemanager_register_file(${bin2cpp_baseclass}* file)\n"
      "{\n"
      "  // check if already registered\n"
      "  if ( registered_files_count && registered_files )\n"
      "  {\n"
      "    for ( size_t i = 0; i < registered_files_count; i++ )\n"
      "    {\n"
      "      const ${bin2cpp_baseclass}* existing_file = registered_files[i];\n"
      "      if ( existing_file == file )\n"
      "        return true; // nothing to do\n"
      "    }\n"
      "  }\n"
      "  \n"
      "  // allocate ram\n"
      "  size_t new_ram_size = sizeof(${bin2cpp_baseclass}**) * (registered_files_count + 1);\n"
      "  ${bin2cpp_baseclass}** tmp = NULL;\n"
      "  if ( registered_files == NULL )\n"
      "    tmp = (${bin2cpp_baseclass}**)malloc(new_ram_size);\n"
      "  else\n"
      "    tmp = (${bin2cpp_baseclass}**)realloc(registered_files, new_ram_size);\n"
      "  if ( tmp == NULL )\n"
      "    return false;\n"
      "  \n"
      "  registered_files = tmp;\n"
      "  registered_files_count++;\n"
      "  \n"
      "  // insert\n"
      "  registered_files[registered_files_count - 1] = file;\n"
      "  \n"
      "  return true;\n"
      "}\n"
      "\n"
      "const ${bin2cpp_baseclass}* ${bin2cpp_namespace}_filemanager_get_file(size_t index)\n"
      "{\n"
      "  if ( index >= registered_files_count )\n"
      "    return NULL;\n"
      "  return registered_files[index];\n"
      "}\n"
      "\n"
      "static inline bool ${bin2cpp_namespace}_filemanager_is_root_directory(const char* path)\n"
      "{\n"
      "  if ( path == NULL && path[0] == '\\0' )\n"
      "    return false;\n"
      "#if defined(_WIN32)\n"
      "  bool is_drive_letter = ((path[0] >= 'a' && path[0] <= 'z') || (path[0] >= 'A' && path[0] <= 'Z'));\n"
      "  if ( (is_drive_letter && path[1] == ':' && path[2] == '\\0') || // test for C:\n"
      "      (is_drive_letter && path[1] == ':' && path[2] == PATH_SEPARATOR_CHAR && path[3] == '\\0') ) // test for C:\\ \n"
      "    return true;\n"
      "#else\n"
      "  if ( path[0] == PATH_SEPARATOR_CHAR )\n"
      "    return true;\n"
      "#endif\n"
      "  return false;\n"
      "}\n"
      "\n"
      "bool ${bin2cpp_namespace}_filemanager_create_parent_directories(const char* file_path)\n"
      "{\n"
      "  if ( file_path == NULL )\n"
      "    return false;\n"
      "  char* accumulator = (char*)malloc(BIN2C_MAX_PATH);\n"
      "  if ( accumulator == NULL )\n"
      "    return false;\n"
      "  accumulator[0] = '\\0';\n"
      "  size_t length = strlen(file_path);\n"
      "  for ( size_t i = 0; i < length; i++ )\n"
      "  {\n"
      "    if ( file_path[i] == PATH_SEPARATOR_CHAR && !(accumulator[0] == '\\0') && !${bin2cpp_namespace}_filemanager_is_root_directory(accumulator) )\n"
      "    {\n"
      "      int ret = portable_mkdir(accumulator);\n"
      "      if ( ret != 0 && errno != EEXIST )\n"
      "      {\n"
      "        free(accumulator);\n"
      "        return false;\n"
      "      }\n"
      "    }\n"
      "    \n"
      "    // append\n"
      "    char tmp[] = { file_path[i], '\\0' };\n"
      "    strcat(accumulator, tmp);\n"
      "  }\n"
      "  free(accumulator);\n"
      "  return true;\n"
      "}\n"
      "\n"
      "bool ${bin2cpp_namespace}_filemanager_save_files(const char * directory)\n"
      "{\n"
      "  if (directory == NULL)\n"
      "    return false;\n"
      "  char* path = (char*)malloc(BIN2C_MAX_PATH);\n"
      "  if ( path == NULL )\n"
      "    return false;\n"
      "  path[0] = '\\0';\n"
      "  for(size_t i=0; i< registered_files_count; i++)\n"
      "  {\n"
      "    const ${bin2cpp_baseclass}* f = ${bin2cpp_namespace}_filemanager_get_file(i);\n"
      "    if ( !f )\n"
      "    {\n"
      "      free(path);\n"
      "      return false;\n"
      "    }\n"
      "    \n"
      "    snprintf(path, sizeof(path), \"%s%c%s\", directory, PATH_SEPARATOR_CHAR, f->file_path);\n"
      "    \n"
      "    if (!${bin2cpp_namespace}_filemanager_create_parent_directories(path))\n"
      "    {\n"
      "      free(path);\n"
      "      return false;\n"
      "    }\n"
      "    bool saved = f->save(path);\n"
      "    if (!saved)\n"
      "    {\n"
      "      free(path);\n"
      "      return false;\n"
      "    }\n"
      "  }\n"
      "  free(path);\n"
      "  return true;\n"
      "}\n"
      "\n"
    ;

    TemplateProcessor processor(&text);
    processor.setTemplateVariableHandler(this);
    bool write_success = processor.writeFile(file_path);

    return write_success;
  }

  bool ManagerGenerator::printFileContent()
  {
    return false;
  }

}; //bin2cpp