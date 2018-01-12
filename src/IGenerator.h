#pragma once

#include <string>
#include "errorcodes.h"
 
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
    ///Creates a header file for embedding a given file into C++ source code.
    ///</summary>
    ///<param name="iInputFilename">The path of the input file (resource) to embeded as a C++ source code.</param>
    ///<param name="iHeaderFilePath">The path of the header file (*.h) that is used for getting access to the functions.</param>
    ///<param name="iFunctionIdentifier">The unique identifier name for each data accessor functions.</param>
    ///<returns>Returns an enum of type ErrorCodes which defines the results.</returns>
    virtual bin2cpp::ErrorCodes createHeaderEmbededFile(const char * iInputFilename, const char * iHeaderFilePath, const char * iFunctionIdentifier) = 0;

    ///<summary>
    ///Creates a cpp source file for embedding a given file into C++ source code.
    ///</summary>
    ///<param name="iInputFilename">The path of the input file (resource) to embeded as a C++ source code.</param>
    ///<param name="iCppFilePath">The path of the source file (*.cpp) that is used for getting access to the functions.</param>
    ///<param name="iFunctionIdentifier">The unique identifier name for each data accessor functions.</param>
    ///<param name="iChunkSize">The size in bytes of each chunk of data.</param>
    ///<returns>Returns an enum of type ErrorCodes which defines the results.</returns>
    virtual bin2cpp::ErrorCodes createCppEmbeddedFile(const char * iInputFilename, const char * iCppFilePath, const char * iFunctionIdentifier, size_t iChunkSize) = 0;
  };
  
}; //bin2cpp
