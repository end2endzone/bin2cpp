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

#ifndef BIN2CPP_COMMON_H
#define BIN2CPP_COMMON_H

#include <stdint.h>
#include <string>
#include <stdio.h>
#include <set>

namespace bin2cpp
{
  typedef std::set<std::string> Dictionary;

  ///<summary>
  ///Returns the application version number.
  ///</summary>
  const char * getVersionString();

  ///<summary>
  ///Returns the modified date from an embedded file's c++ header/source file.
  ///Note that the function returns the number of seconds elapsed since epoch since Jan 1st 1970.
  ///</summary>
  ///<param name="path">The valid path to a c++ header/source file.</param>
  ///<return>Returns the modified date from an embedded file's c++ header/source file.<return>
  uint64_t getOutputFileModifiedDate(const std::string & path);

  ///<summary>
  ///Determine if a given path is a c++ header file.
  ///</summary>
  ///<param name="path">An valid file path.</param>
  ///<return>Returns true if path is a c++ header file. Returns false otherwise.<return>
  bool isCppHeaderFile(const std::string & path);

  ///<summary>
  ///Determine if a given path is a c++ source file.
  ///</summary>
  ///<param name="path">An valid file path.</param>
  ///<return>Returns true if path is a c++ source file. Returns false otherwise.<return>
  bool isCppSourceFile(const std::string & path);

  ///<summary>
  ///Determine the appropriate macro name for the include guard of the given c++ header file.
  ///See also https://en.wikipedia.org/wiki/Include_guard
  ///</summary>
  ///<param name="path">An valid file path.</param>
  ///<return>Returns the macro name for the given c++ header file.<return>
  std::string getCppIncludeGuardMacroName(const std::string & path);

  ///<summary>
  ///Filter a string to only contains the given allowed characters.
  ///</summary>
  ///<param name="str">The input string to filter.</param>
  ///<param name="valid_characters">The list of allowed characters.</param>
  ///<return>Returns a string matching the content of `str` where the characters that are not in `valid_characters` are removed.<return>
  std::string filter(std::string str, const std::string & valid_characters);

  ///<summary>
  ///Build a valid function identifier based on the path of a given file.
  ///For example, given the file `c:\temp\www\static\css\theme.dark.css`, the returned string can be something like `themedarkcss`.
  ///</summary>
  ///<param name="path">The path of a file.</param>
  ///<return>Returns a valid function identifier from a file path.<return>
  std::string getFunctionIdentifierFromPath(const std::string & path);

  ///<summary>
  ///Build a unique function identifier based on the path of a given file.
  ///The function is based on `getFunctionIdentifierFromPath()`.
  ///A dictionary is used to keep track of existing function identifier.
  ///If an identifier is already existing in the dictionary, "_[counter]" is added at the end of the identifier.
  ///</summary>
  ///<param name="path">The path of a file.</param>
  ///<param name="dict">A dictionary that contains the existing function identifier.</param>
  ///<return>Returns a unique function identifier from a file path.<return>
  std::string getUniqueFunctionIdentifierFromPath(const std::string & path, Dictionary & dict);

  ///<summary>
  ///Build a unique file path based on the path of a given file.
  ///A dictionary is used to keep track of existing file paths.
  ///If a file is already existing in the dictionary, "_[counter]" is added at the end of the filename.
  ///</summary>
  ///<param name="path">The path of a file.</param>
  ///<param name="dict">A dictionary that contains the existing file paths.</param>
  ///<return>Returns a unique file path from the path of a given file.<return>
  std::string getUniqueFilePath(const std::string & path, Dictionary & dict);

  ///<summary>
  ///Split a path into individual components: directory, file name, file extension.
  ///</summary>
  ///<param name="path">The path to split.</param>
  ///<param name="directory">The dictionary of the given path.</param>
  ///<param name="file_name">The file name (without the extension) of the given path.</param>
  ///<param name="file_extension">The file extension of the given path.</param>
  void pathSplit(const std::string & path, std::string & directory, std::string & file_name, std::string & file_extension);

  ///<summary>
  ///Join individual components of a path into a full path.
  ///</summary>
  ///<param name="directory">The dictionary of the path to join.</param>
  ///<param name="file_name">The file name (without the extension) of the path to join.</param>
  ///<param name="file_extension">The file extension of the path to join.</param>
  ///<return>Returns the path matching all given components.<return>
  std::string pathJoin(const std::string & directory, const std::string & file_name, const std::string & file_extension);

}; //bin2cpp

#endif //BIN2CPP_COMMON_H
