#pragma once

#include "BaseGenerator.h"

namespace bin2cpp
{

  ///<summary>
  ///This generator stores data in segment of a maximum of 65535 bytes.
  ///Each 'segment' can be accessed separately.
  ///</summary>
  class SegmentGenerator : public BaseGenerator
  {
  public:
    SegmentGenerator();
    virtual ~SegmentGenerator();
    virtual const char * getName() const;
    virtual bin2cpp::ErrorCodes createCppEmbeddedFile(const char * iInputFilename, const char * iCppFilePath, const char * iFunctionIdentifier, size_t iChunkSize);
  };

}; //bin2cpp
