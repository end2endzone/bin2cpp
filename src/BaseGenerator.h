#pragma once

#include "IGenerator.h"

namespace bin2cpp
{

  ///<summary>
  ///This generator is the base of all generators.
  ///</summary>
  class BaseGenerator : public virtual IGenerator
  {
  public:
    BaseGenerator();
    virtual ~BaseGenerator();

    //same header file for all generators
    virtual bin2cpp::ErrorCodes createHeaderEmbededFile(const char * iOutputFolder, const char * iHeaderFilename, const char * iFunctionIdentifier, bool iOverrideExisting);

  protected:
    virtual std::string getGetterFunctionName(const char * iFunctionIdentifier);
  };

}; //bin2cpp
