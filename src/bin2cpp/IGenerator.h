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
 
namespace bin2cpp
{

  class IGenerator
  {
  public:

    ///<summary>
    ///Get the name of the generator.
    ///</summary>
    ///<returns>Returns the name of the generator</returns>
    virtual const char * getName() const = 0;

    ///<summary>
    ///Defines the path of the binary input file.
    ///</summary>
    ///<param name="path">The path of the input file (resource) to embeded as C++ source code.</param>
    virtual void setInputFilePath(const char * path) = 0;

    ///<summary>
    ///Provides the path of the binary input file.
    ///</summary>
    ///<returns>Returns the path of the binary input file. Returns an empty string if not defined.</returns>
    virtual const char * getInputFilePath() const = 0;

    ///<summary>
    ///Defines the filename of the generated header.
    ///</summary>
    ///<param name="path">The path of the input file (resource) to embeded as C++ source code.</param>
    virtual void setHeaderFilename(const char * path) = 0;

    ///<summary>
    ///Provides the filename of the generated header.
    ///</summary>
    ///<returns>Returns the path of the binary input file. Returns an empty string if not defined.</returns>
    virtual const char * getHeaderFilename() const = 0;

    ///<summary>
    ///Defines the unique identifier name for the File class getter function.
    ///</summary>
    ///<param name="function_identifier">The unique identifier name for the File class getter function.</param>
    virtual void setFunctionIdentifier(const char * function_identifier) = 0;

    ///<summary>
    ///Provides the unique identifier name for the File class getter function.
    ///</summary>
    ///<returns>Returns the unique identifier name for the File class getter function. Returns an empty string if not defined.</returns>
    virtual const char * getFunctionIdentifier() const = 0;

    ///<summary>
    ///Defines the path reported by the getFilePath() method of the File class.
    ///</summary>
    ///<param name="path">The path of the input file (resource) to embeded as C++ source code.</param>
    virtual void setReportedFilePath(const char * path) = 0;

    ///<summary>
    ///Provides the path reported by the getFilePath() method of the File class.
    ///</summary>
    ///<returns>The path reported by the getFilePath() method of the File class. Returns an empty string if not defined.</returns>
    virtual const char * getReportedFilePath() const = 0;

    ///<summary>
    ///Defines the size in bytes of each chunk of data.
    ///</summary>
    ///<param name="chunk_size">The size in bytes of each chunk of data.</param>
    virtual void setChunkSize(size_t chunk_size) = 0;

    ///<summary>
    ///Provides the size in bytes of each chunk of data.
    ///</summary>
    ///<returns>Returns the unique identifier name for the File class getter function. Returns an empty string if not defined.</returns>
    virtual size_t getChunkSize() const = 0;

    ///<summary>
    ///Defines the namespace of the generated code.
    ///</summary>
    ///<param name="name">The name of the namespace of the generated code.</param>
    virtual void setNamespace(const char * name) = 0;

    ///<summary>
    ///Provides the namespace of the generated code.
    ///</summary>
    ///<returns>Returns the name of the namespace of the generated code. Returns an empty string if not defined.</returns>
    virtual const char * getNamespace() const = 0;

    ///<summary>
    ///Defines the base class of the generated code.
    ///</summary>
    ///<param name="name">The name of the base class of the generated code.</param>
    virtual void setBaseClass(const char * name) = 0;

    ///<summary>
    ///Provides the base class of the generated code.
    ///</summary>
    ///<returns>Returns the name of the base class of the generated code. Returns an empty string if not defined.</returns>
    virtual const char * getBaseClass() const = 0;

    ///<summary>
    ///Defines the filename of the FileManager generated header.
    ///</summary>
    ///<param name="path">The path of the FileManager output file.</param>
    virtual void setManagerHeaderFilename(const char * path) = 0;

    ///<summary>
    ///Provides the filename of the FileManager generated header.
    ///</summary>
    ///<returns>Returns the path of the FileManager output file. Returns an empty string if not defined.</returns>
    virtual const char * getManagerHeaderFilename() const = 0;

    ///<summary>
    ///Enable or disable the registration of the generated file to the FileManager.
    ///</summary>
    ///<param name="register_file_enabled">The new value of the flag.</param>
    virtual void setRegisterFileEnabled(bool register_file_enabled) = 0;

    ///<summary>
    ///Returns true if the generated file should be registated FileManager should be used in generated code.
    ///</summary>
    ///<returns>Returns true if the FileManager should be used in generated code. Returns false otherwise.</returns>
    virtual bool isRegisterFileEnabled() const = 0;

    ///<summary>
    ///Defines the different type of cpp encoding.
    ///See setCppEncoder() and getCppEncoder() functions.
    ///</summary>
    enum CppEncoderEnum
    {
       CPP_ENCODER_OCT,
       CPP_ENCODER_HEX,
    };

    ///<summary>
    ///Defines the type of cpp encoder to use. See CppEncoderEnum for details.
    ///</summary>
    ///<param name="cpp_encoder">The type of cpp encoder to use</param>
    virtual void setCppEncoder(const CppEncoderEnum & cpp_encoder) = 0;

    ///<summary>
    ///Provides the type of cpp encoder to use. See CppEncoderEnum for details.
    ///</summary>
    ///<returns>Returns the type of cpp encoder to use.</returns>
    virtual CppEncoderEnum getCppEncoder() const = 0;

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
