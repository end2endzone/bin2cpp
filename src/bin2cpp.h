#pragma once

#include <string>
#include "errorcodes.h"

namespace bin2cpp
{

  /*
  Description:
    Creates a header and cpp file for embedding a given file into C++ source code.
  Parameters:
    iInputFilename:       The path of the file to embeded into C++ source code.
    iOutputFolder:        The output folder where to save the generated files.
    iHeaderFilename:      The filename of the header file (*.h) that is used for getting access to the functions.
    iFunctionIdentifier:  The unique identifier name for each data accessor functions.
    iChunkSize:           The size in bytes of each chunk of data
  */
  bin2cpp::ErrorCodes createCppEmbeddedFile(const char * iInputFilename, const char * iOutputFolder, const char * iHeaderFilename, const char * iFunctionIdentifier, size_t iChunkSize, bool iOverrideExisting);

}; //bin2cpp
