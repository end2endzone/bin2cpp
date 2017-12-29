#pragma once

#include "IGenerator.h"

namespace bin2cpp
{

  ///<summary>
  ///This generator stores data in segment of a maximum of 65535 bytes.
  ///Each 'segment' can be accessed separately.
  ///</summary>
  class SegmentGenerator : public virtual IGenerator
  {
  public:
    SegmentGenerator();
    virtual ~SegmentGenerator();
    bin2cpp::ErrorCodes createCppEmbeddedFile(const char * iInputFilename, const char * iOutputFolder, const char * iHeaderFilename, const char * iFunctionIdentifier, size_t iChunkSize, bool iOverrideExisting);
  };

}; //bin2cpp
