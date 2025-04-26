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

#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
#include "IGenerator.h"

namespace bin2cpp
{

  ///<summary>
  ///This is the command line context for generating code.
  ///</summary>
  class Context
  {
  public:
    Context();
    virtual ~Context();

    enum Flags
    {
      KEEP_DIRECTORY_STRUCTURE = 0,   // true if the output files must have the same directory structure as the input files. Valid only when --dir is used.
      PLAIN_OUTPUT,
      OVERRIDE_EXISTING,
      REGISTER_FILE,

      FLAG_COUNT
    };

    void reset();

    bool hasInputFileSet() const;
    bool hasInputDirSet() const;
    bool hasOutputDirSet() const;
    bool hasReportedFilePath() const;
    bool hasManagerFile() const;

    void setFlag(const Flags & flag, bool value);
    bool isFlagSet(const Flags& flag) const;

    void setInputFilePath(const char* path);
    const std::string& getInputFilePath() const;
    void setInputDirPath(const char* path);
    const std::string& getInputDirPath() const;
    void setOutputDirPath(const char* path);
    const std::string& getOutputDirPath() const;
    void setHeaderFilename(const char* path);
    const std::string& getHeaderFilename() const;
    void setFunctionIdentifier(const char* function_identifier);
    const std::string& getFunctionIdentifier() const;
    void setReportedFilePath(const char* path);
    const std::string& getReportedFilePath() const;
    void setChunkSize(size_t chunk_size);
    size_t getChunkSize() const;
    void setNamespace(const char* name);
    const std::string& getNamespace() const;
    void setBaseClass(const char* name);
    const std::string& getBaseClass() const;
    void setManagerHeaderFilename(const char* manager_file);
    const std::string& getManagerHeaderFilename() const;
    void setCppEncoder(const IGenerator::CppEncoderEnum& cpp_encoder);
    IGenerator::CppEncoderEnum getCppEncoder() const;


  protected:

    //attributes
    bool mHasInputFile;               // true if 'mInputFilePath' is set.
    bool mHasInputDir;                // true if 'mInputDirPath' is set.
    bool mHasOutputDir;               // true if 'mOutputDirPath' is set.
    bool mHasReportedFilePath;        // true if 'mReportedFilePath' is set.
    bool mHasManagerFile;             // true if 'mManagerHeaderFilename' is set.
    
    bool mFlags[Flags::FLAG_COUNT];

    std::string mInputFilePath;       // path of the input binary file
    std::string mInputDirPath;
    std::string mOutputDirPath;
    std::string mHeaderFilename;
    std::string mFunctionIdentifier;
    std::string mReportedFilePath; // path reported in the public api when calling getFilePath();
    size_t mChunkSize;
    std::string mNamespace;
    std::string mBaseClass;
    std::string mManagerHeaderFilename;
    IGenerator::CppEncoderEnum mCppEncoder;
    std::string mGeneratorName;
  };

}; //bin2cpp

#endif //BASEGENERATOR_H
