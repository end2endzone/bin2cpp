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

#include "Context.h"

namespace bin2cpp
{
  Context::Context()
  {
    reset();
  }

  Context::~Context()
  {
  }

  void Context::reset()
  {
    mHasInputFile = false;
    mHasInputDir = false;
    mHasOutputDir = false;
    mHasReportedFilePath = false;
    mHasManagerFile = false;
    mChunkSize = 0;
    mCppEncoder = IGenerator::CppEncoderEnum::CPP_ENCODER_HEX;

    memset(mFlags, 0, sizeof(mFlags));
  }

  bool Context::hasInputFileSet() const
  {
    return mHasInputFile;
  }

  bool Context::hasInputDirSet() const
  {
    return mHasInputDir;
  }

  bool Context::hasOutputDirSet() const
  {
    return mHasOutputDir;
  }

  bool Context::hasReportedFilePath() const
  {
    return mHasReportedFilePath;
  }

  void Context::setFlag(const Flags& flag, bool value)
  {
    if ( flag > 0 && flag < FLAG_COUNT )
      mFlags[flag] = value;
  }

  bool Context::isFlagSet(const Flags& flag) const
  {
    if ( flag > 0 && flag < FLAG_COUNT )
      return mFlags[flag];
    return false;
  }

  void Context::setInputFilePath(const char* path)
  {
    if ( path )
    {
      mInputFilePath = path;
      mHasInputFile = true;
    }
  }

  const std::string & Context::getInputFilePath() const
  {
    return mInputFilePath;
  }

  void Context::setInputDirPath(const char* path)
  {
    if ( path )
    {
      mInputDirPath = path;
      mHasInputDir = true;
    }
  }

  const std::string & Context::getInputDirPath() const
  {
    return mInputDirPath;
  }

  void Context::setOutputDirPath(const char* path)
  {
    if ( path )
    {
      mOutputDirPath = path;
      mHasOutputDir = true;
    }
  }

  const std::string & Context::getOutputDirPath() const
  {
    return mOutputDirPath;
  }

  void Context::setHeaderFilename(const char* path)
  {
    if ( path )
      mHeaderFilename = path;
  }

  const std::string & Context::getHeaderFilename() const
  {
    return mHeaderFilename;
  }

  void Context::setFunctionIdentifier(const char* function_identifier)
  {
    if ( function_identifier )
      mFunctionIdentifier = function_identifier;
  }

  const std::string & Context::getFunctionIdentifier() const
  {
    return mFunctionIdentifier;
  }

  void Context::setReportedFilePath(const char* path)
  {
    if ( path )
      mReportedFilePath = path;
  }

  const std::string & Context::getReportedFilePath() const
  {
    return mReportedFilePath;
  }

  void Context::setChunkSize(size_t chunk_size)
  {
    mChunkSize = chunk_size;
  }

  size_t Context::getChunkSize() const
  {
    return mChunkSize;
  }

  void Context::setNamespace(const char* name)
  {
    if ( name )
      mNamespace = name;
  }

  const std::string & Context::getNamespace() const
  {
    return mNamespace;
  }

  void Context::setBaseClass(const char* name)
  {
    if ( name )
      mBaseClass = name;
  }

  const std::string & Context::getBaseClass() const
  {
    return mBaseClass;
  }

  void Context::setManagerHeaderFilename(const char* manager_file)
  {
    if ( manager_file )
      mManagerHeaderFilename = manager_file;
  }

  const std::string & Context::getManagerHeaderFilename() const
  {
    return mManagerHeaderFilename;
  }

  void Context::setCppEncoder(const IGenerator::CppEncoderEnum& cpp_encoder)
  {
    mCppEncoder = cpp_encoder;
  }

  IGenerator::CppEncoderEnum Context::getCppEncoder() const
  {
    return mCppEncoder;
  }


  //-------------------------------
  //protected methods
  //-------------------------------


}; //bin2cpp