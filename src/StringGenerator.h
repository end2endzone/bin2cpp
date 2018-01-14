#pragma once

#include "BaseGenerator.h"

namespace bin2cpp
{

  ///<summary>
  ///This generator stores data in a single string of a maximum of 65535 bytes.
  ///</summary>
  class StringGenerator : public BaseGenerator
  {
  public:
    StringGenerator();
    virtual ~StringGenerator();
    virtual const char * getName() const;
    virtual bool createCppSourceFile(const char * iInputFilename, const char * iCppFilePath, const char * iFunctionIdentifier, size_t iChunkSize);
  };

}; //bin2cpp
