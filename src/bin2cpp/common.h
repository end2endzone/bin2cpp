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

namespace bin2cpp
{
  ///<summary>
  ///Returns the year when the code was build.
  ///Usefull for copyright messages.
  ///</summary>
  ///<return>The year when the function was compiled.<return>
  int getCopyrightYear();

  /*
  Description:
    Returns the application version number.
  */
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

}; //bin2cpp

#endif //BIN2CPP_COMMON_H
