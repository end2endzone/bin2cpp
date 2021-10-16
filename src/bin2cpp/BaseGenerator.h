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
    virtual void setInputFilePath(const char * path);
    virtual const char * getInputFilePath() const;
    virtual void setHeaderFilename(const char * path);
    virtual const char * getHeaderFilename() const;
    virtual void setFunctionIdentifier(const char * function_identifier);
    virtual const char * getFunctionIdentifier() const;
    virtual void setReportedFilePath(const char * path);
    virtual const char * getReportedFilePath() const;
    virtual void setChunkSize(size_t chunk_size);
    virtual size_t getChunkSize() const;
    virtual void setNamespace(const char * name);
    virtual const char * getNamespace() const;
    virtual void setBaseClass(const char * name);
    virtual const char * getBaseClass() const;
    virtual void setCppEncoder(const CppEncoderEnum & cpp_encoder);
    virtual CppEncoderEnum getCppEncoder() const;
    virtual void setManagerHeaderFilename(const char * manager_file);
    virtual const char * getManagerHeaderFilename() const;
    virtual void setRegisterFileEnabled(bool register_file_enabled);
    virtual bool isRegisterFileEnabled() const;

    //same header file for all generators
    virtual bool createCppHeaderFile(const char * header_file_path);
    virtual bool createManagerHeaderFile(const char * header_file_path);
    virtual bool createManagerSourceFile(const char * cpp_file_path);
    virtual bool printFileContent();

  protected:

    virtual std::string getGetterFunctionName();
    virtual std::string getHeaderFilePath(const char * cpp_file_path);
    virtual std::string getCppFilePath(const char * header_file_path);
    virtual std::string getHeaderTemplate();
    virtual std::string getHeaderTemplate(bool include_source_file);
    virtual std::string getSaveMethodTemplate();
    virtual std::string getFileManagerRegistrationTemplate();
    virtual std::string getClassName();
    virtual std::string getClassMacroGuardPrefix();
    virtual std::string getImplOfGetFileName();
    virtual std::string getImplOfGetFilePath();

    //attributes
    std::string mInputFilePath;
    std::string mHeaderFilename;
    std::string mReportedFilePath;
    std::string mFunctionIdentifier;
    size_t mChunkSize;
    std::string mNamespace;
    std::string mBaseClass;
    std::string mManagerHeaderFilename;
    IGenerator::CppEncoderEnum mCppEncoder;
    bool mManagerEnabled;
  };

}; //bin2cpp

#endif //BASEGENERATOR_H
