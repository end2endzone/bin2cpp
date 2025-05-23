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
#include <map>

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
    virtual void setContext(const Context& c);
    virtual const Context & getContext() const;

    //ITemplateVariableLookup methods
    virtual std::string lookupTemplateVariable(const std::string& name);

    //same header file for all generators
    virtual bool createCppHeaderFile(const char * header_file_path);
    virtual bool printFileContent();
    virtual bool createCHeaderFile(const char* file_path);
    virtual bool createCSourceFile(const char* file_path);

  protected:

    virtual std::string getGetterFunctionName();
    virtual std::string getHeaderFilePath(const char * cpp_file_path);
    virtual std::string getCppFilePath(const char * header_file_path);
    virtual std::string getHeaderTemplate();
    virtual std::string getHeaderTemplate(bool include_source_file);
    virtual std::string getSaveMethodTemplate();
    virtual std::string getCppFileManagerRegistrationImplementationTemplate();
    virtual std::string getCFileManagerRegistrationPredeclarationImplementation();
    virtual std::string getCFileManagerStaticFileRegistrationImplementation();
    virtual std::string getClassName();
    virtual std::string getClassMacroGuardPrefix();
    virtual std::string getImplOfGetFileName();
    virtual std::string getImplOfGetFilePath();
    virtual std::string getFileClassFileName();
    virtual std::string getFileClassFilePath();
    virtual std::string getCppHeaderIncludePath();
    virtual std::string getInputFileDataAsCode();
    virtual std::string getInputFileChunkAsCode(const unsigned char * buffer, size_t buffer_size, size_t index, size_t count, bool is_last_chunk);

    //attributes
    Context mContext;
  };

}; //bin2cpp

#endif //BASEGENERATOR_H
