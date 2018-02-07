#pragma once

#include "BaseGenerator.h"

namespace bin2cpp
{

  ///<summary>
  ///This generator stores data in a single string of a maximum of 65535 bytes.
  ///</summary>
  class Win32ResourceGenerator : public BaseGenerator
  {
  public:
    Win32ResourceGenerator();
    virtual ~Win32ResourceGenerator();
    virtual const char * getName() const;
    virtual bool createCppSourceFile(const char * iCppFilePath);
  protected:
    virtual std::string getResourceFilePath(const char * iCppFilePath);
    virtual bool createResourceFile(const char * iCppFilePath);
    virtual std::string getRandomIdentifier(const char * iCppFilePath);
  };

}; //bin2cpp
