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

#ifndef IGENERATOR_H
#define IGENERATOR_H

#include <string>
#include "Context.h"
 
namespace bin2cpp
{

  class IGenerator
  {
  public:

    ///<summary>
    ///Sets the current context for the generator.
    ///</summary>
    ///<param name="c">The new context.</param>
    virtual void setContext(const Context& c) = 0;

    ///<summary>
    ///Get the current context set for the generator.
    ///</summary>
    ///<returns>Returns the current context set for the generator.</returns>
    virtual const Context& getContext() const = 0;

    ///<summary>
    ///Creates a header file for embedding a given file into C++ source code.
    ///</summary>
    ///<param name="header_file_path">The path of the header file (*.h) that is used for getting access to the functions.</param>
    ///<returns>Returns true when the file was created. Returns false otherwise.</returns>
    virtual bool createCppHeaderFile(const char * header_file_path) = 0;

    ///<summary>
    ///Creates a cpp source file for embedding a given file into C++ source code.
    ///</summary>
    ///<param name="cpp_file_path">The path of the source file (*.cpp) that is used for getting access to the functions.</param>
    ///<returns>Returns true when the file was created. Returns false otherwise.</returns>
    virtual bool createCppSourceFile(const char * cpp_file_path) = 0;

    ///<summary>
    ///Creates a FileManager header file.
    ///</summary>
    ///<param name="header_file_path">The path of the header file (*.h).</param>
    ///<returns>Returns true when the file was created. Returns false otherwise.</returns>
    virtual bool createManagerHeaderFile(const char * header_file_path) = 0;

    ///<summary>
    ///Creates a FileManager cpp file.
    ///</summary>
    ///<param name="header_file_path">The path of the cpp file (*.h).</param>
    ///<returns>Returns true when the file was created. Returns false otherwise.</returns>
    virtual bool createManagerSourceFile(const char * cpp_file_path) = 0;

    ///<summary>
    ///Print the encoded file content to stdout
    ///</summary>
    ///<returns>Returns true when the operation is succesful. Returns false otherwise.</returns>
    virtual bool printFileContent() = 0;

  };
  
}; //bin2cpp

#endif //IGENERATOR_H
