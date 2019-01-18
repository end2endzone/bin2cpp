/**********************************************************************************
 * MIT License
 * 
 * Copyright (c) 2018 Antoine Beauchamp
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *********************************************************************************/

#ifndef BASEGENERATOR_H
#define BASEGENERATOR_H

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

#endif //BASEGENERATOR_H
