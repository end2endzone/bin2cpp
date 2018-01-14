#pragma once

#include "BaseGenerator.h"

namespace bin2cpp
{

  ///<summary>
  ///This generator stores data in a huge array.
  ///The syntax is the following:     const char foo[] = { 97, 98, 99, 100, ..., 0 };
  ///</summary>
  class ArrayGenerator : public BaseGenerator
  {
  public:
    ArrayGenerator();
    virtual ~ArrayGenerator();
    virtual const char * getName() const;
    virtual bool createCppSourceFile(const char * iInputFilename, const char * iCppFilePath, const char * iFunctionIdentifier, size_t iChunkSize);
  };

}; //bin2cpp
