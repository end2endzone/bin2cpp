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
    virtual bool createCppHeaderFile(const char * iInputFilename, const char * iHeaderFilePath, const char * iFunctionIdentifier);

  protected:
    virtual std::string getGetterFunctionName(const char * iFunctionIdentifier);
    virtual std::string getHeaderFilePath(const char * iCppFilePath);
    virtual std::string getCppFilePath(const char * iHeaderFilePath);
    virtual std::string getFileHeading(const char * iInputFilename);
    virtual std::string getSaveMethodImplementation();
  };

}; //bin2cpp
