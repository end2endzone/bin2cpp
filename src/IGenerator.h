#pragma once

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
    ///<param name="iPath">The path of the input file (resource) to embeded as a C++ source code.</param>
    virtual void setInputFile(const char * iPath) = 0;

    ///<summary>
    ///Provides the path of the binary input file.
    ///</summary>
    ///<returns>Returns the path of the binary input file. Returns an empty string if not defined.</returns>
    virtual const char * getInputFile() const = 0;

    ///<summary>
    ///Defines the unique identifier name for the File class getter function.
    ///</summary>
    ///<param name="iFunctionIdentifier">The unique identifier name for the File class getter function.</param>
    virtual void setFunctionIdentifier(const char * iFunctionIdentifier) = 0;

    ///<summary>
    ///Provides the unique identifier name for the File class getter function.
    ///</summary>
    ///<returns>Returns the unique identifier name for the File class getter function. Returns an empty string if not defined.</returns>
    virtual const char * getFunctionIdentifier() const = 0;

    ///<summary>
    ///Defines the size in bytes of each chunk of data.
    ///</summary>
    ///<param name="iChunkSize">The size in bytes of each chunk of data.</param>
    virtual void setChunkSize(size_t iChunkSize) = 0;

    ///<summary>
    ///Provides the size in bytes of each chunk of data.
    ///</summary>
    ///<returns>Returns the unique identifier name for the File class getter function. Returns an empty string if not defined.</returns>
    virtual size_t getChunkSize() const = 0;

    ///<summary>
    ///Defines the namespace of the generated code.
    ///</summary>
    ///<param name="iName">The name of the namespace of the generated code.</param>
    virtual void setNamespace(const char * iName) = 0;

    ///<summary>
    ///Provides the namespace of the generated code.
    ///</summary>
    ///<returns>Returns the name of the namespace of the generated code. Returns an empty string if not defined.</returns>
    virtual const char * getNamespace() const = 0;

    ///<summary>
    ///Defines the base class of the generated code.
    ///</summary>
    ///<param name="iName">The name of the base class of the generated code.</param>
    virtual void setBaseClass(const char * iName) = 0;

    ///<summary>
    ///Provides the base class of the generated code.
    ///</summary>
    ///<returns>Returns the name of the base class of the generated code. Returns an empty string if not defined.</returns>
    virtual const char * getBaseClass() const = 0;

    enum CppEncoderEnum
    {
       CPP_ENCODER_OCT,
       CPP_ENCODER_HEX,
    };

    ///<summary>
    ///Defines the type of cpp encoder to use. See CppEncoderEnum for details.
    ///</summary>
    ///<param name="iCppEncoder">The type of cpp encoder to use</param>
    virtual void setCppEncoder(const CppEncoderEnum & iCppEncoder) = 0;

    ///<summary>
    ///Provides the type of cpp encoder to use. See CppEncoderEnum for details.
    ///</summary>
    ///<returns>Returns the type of cpp encoder to use.</returns>
    virtual CppEncoderEnum getCppEncoder() const = 0;

    ///<summary>
    ///Creates a header file for embedding a given file into C++ source code.
    ///</summary>
    ///<param name="iHeaderFilePath">The path of the header file (*.h) that is used for getting access to the functions.</param>
    ///<returns>Returns true when the file was created. Returns false otherwise.</returns>
    virtual bool createCppHeaderFile(const char * iHeaderFilePath) = 0;

    ///<summary>
    ///Creates a cpp source file for embedding a given file into C++ source code.
    ///</summary>
    ///<param name="iCppFilePath">The path of the source file (*.cpp) that is used for getting access to the functions.</param>
    ///<returns>Returns true when the file was created. Returns false otherwise.</returns>
    virtual bool createCppSourceFile(const char * iCppFilePath) = 0;

  };
  
}; //bin2cpp
