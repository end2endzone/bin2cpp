#pragma once

#include "IGenerator.h"

namespace bin2cpp
{

  ///<summary>
  ///This generator stores data in a single string of a maximum of 65535 bytes.
  ///</summary>
  class StringGenerator : public virtual IGenerator
  {
  public:
    StringGenerator();
    virtual ~StringGenerator();
    bin2cpp::ErrorCodes createCppEmbeddedFile(const char * iInputFilename, const char * iOutputFolder, const char * iHeaderFilename, const char * iFunctionIdentifier, size_t iChunkSize, bool iOverrideExisting);
  };

}; //bin2cpp
