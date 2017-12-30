#pragma once

#include "IGenerator.h"

namespace bin2cpp
{

  ///<summary>
  ///This generator stores data in a huge array.
  ///The syntax is the following:     const char foo[] = { 97, 98, 99, 100, ..., 0 };
  ///</summary>
  class ArrayGenerator : public virtual IGenerator
  {
  public:
    ArrayGenerator();
    virtual ~ArrayGenerator();
    bin2cpp::ErrorCodes createCppEmbeddedFile(const char * iInputFilename, const char * iOutputFolder, const char * iHeaderFilename, const char * iFunctionIdentifier, size_t iChunkSize, bool iOverrideExisting);
  };

}; //bin2cpp
