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

    //IGenerator methods
    virtual void setInputFile(const char * iPath);
    virtual const char * getInputFile() const;
    virtual void setFunctionIdentifier(const char * iFunctionIdentifier);
    virtual const char * getFunctionIdentifier() const;
    virtual void setChunkSize(size_t iChunkSize);
    virtual size_t getChunkSize() const;
    virtual void setNamespace(const char * iName);
    virtual const char * getNamespace() const;
    virtual void setBaseClass(const char * iName);
    virtual const char * getBaseClass() const;
    virtual void setCppEncoder(const CppEncoderEnum & iCppEncoder);
    virtual CppEncoderEnum getCppEncoder() const;

    //same header file for all generators
    virtual bool createCppHeaderFile(const char * iHeaderFilePath);

  protected:
    virtual std::string getGetterFunctionName();
    virtual std::string getHeaderFilePath(const char * iCppFilePath);
    virtual std::string getCppFilePath(const char * iHeaderFilePath);
    virtual std::string getHeaderTemplate();
    virtual std::string getSaveMethodTemplate();

    //attributes
    std::string mInputFile;
    std::string mFunctionIdentifier;
    size_t mChunkSize;
    std::string mNamespace;
    std::string mBaseClass;
    IGenerator::CppEncoderEnum mCppEncoder;
  };

}; //bin2cpp
