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

  Context::Context(const Context& other)
  {
      (*this) = other;
  }

  Context::~Context()
  {
  }

  Context& Context::operator=(const Context& other)
  {
    if ( this != &other )
    {
      this->hasInputFile                    =  other.hasInputFile                       ;
      this->hasInputDir                     =  other.hasInputDir                        ;
      this->hasOutputDir                    =  other.hasOutputDir                       ;
      this->hasReportedFilePath             =  other.hasReportedFilePath                ;
      this->hasManagerFile                  =  other.hasManagerFile                     ;
      this->hasDirectoryIncludeFilters      =  other.hasDirectoryIncludeFilters         ;
      this->hasDirectoryExcludeFilters      =  other.hasDirectoryExcludeFilters         ;
      this->keepDirectoryStructure          =  other.keepDirectoryStructure             ;
      this->overrideExistingFiles           =  other.overrideExistingFiles              ;
      this->registerFiles                   =  other.registerFiles                      ;
      this->plainOutput                     =  other.plainOutput                        ;
      this->inputFilePath                   =  other.inputFilePath                      ;
      this->inputDirPath                    =  other.inputDirPath                       ;
      this->outputDirPath                   =  other.outputDirPath                      ;
      this->headerFilename                  =  other.headerFilename                     ;
      this->functionIdentifier              =  other.functionIdentifier                 ;
      this->reportedFilePath                =  other.reportedFilePath                   ;
      this->chunkSize                       =  other.chunkSize                          ;
      this->codeNamespace                   =  other.codeNamespace                      ;
      this->baseClass                       =  other.baseClass                          ;
      this->managerHeaderFilename           =  other.managerHeaderFilename              ;
      this->cppEncoder                      =  other.cppEncoder                         ;
      this->generatorName                   =  other.generatorName                      ;
      this->directoryIncludeFilters         =  other.directoryIncludeFilters            ;
      this->directoryExcludeFilters         =  other.directoryExcludeFilters            ;
    }
    return *this;
  }

  void Context::reset()
  {
    hasInputFile = false;
    hasInputDir = false;
    hasOutputDir = false;
    hasReportedFilePath = false;
    hasManagerFile = false;
    keepDirectoryStructure = false;
    overrideExistingFiles = false;
    registerFiles = false;
    plainOutput = false;
    hasDirectoryIncludeFilters = false;
    hasDirectoryExcludeFilters = false;

    inputFilePath.clear();
    inputDirPath.clear();
    outputDirPath.clear();
    headerFilename.clear();
    functionIdentifier.clear();
    reportedFilePath.clear();
    chunkSize = 0;
    codeNamespace.clear();
    baseClass.clear();
    managerHeaderFilename.clear();
    cppEncoder = CppEncoderEnum::CPP_ENCODER_HEX;
    generatorName.clear();
    directoryIncludeFilters.clear();
    directoryExcludeFilters.clear();
  }


  //-------------------------------
  //protected methods
  //-------------------------------


}; //bin2cpp