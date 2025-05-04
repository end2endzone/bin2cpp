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
#include <vector>
#include "enums.h"

namespace bin2cpp
{

  ///<summary>
  ///This is the command line context for generating code.
  ///</summary>
  class Context
  {
  public:
    Context();
    Context(const Context & other);
    virtual ~Context();

    Context& operator=(const Context& other);

    // flags
    bool hasInputFile;            // true if 'inputFilePath' is set.
    bool hasInputDir;             // true if 'inputDirPath' is set.
    bool hasOutputDir;            // true if 'outputDirPath' is set.
    bool hasReportedFilePath;     // true if 'reportedFilePath' is set.
    bool hasManagerFile;          // true if 'managerHeaderFilename' is set.
    bool keepDirectoryStructure;  // true if the output files must have the same directory structure as the input files. Valid only when --dir is used.
    bool overrideExistingFiles;
    bool registerFiles;
    bool plainOutput;
    bool hasDirectoryIncludeFilters;
    bool hasDirectoryExcludeFilters;

    // public attributes
    std::string inputFilePath;    // The path of the input file (resource) to embeded as C++ source code.
    std::string inputDirPath;
    std::string outputDirPath;
    std::string headerFilename;   // The path of the input file (resource) to embeded as C++ source code.
    std::string functionIdentifier;
    std::string reportedFilePath; // path reported in the public api when calling getFilePath();
    size_t chunkSize;
    std::string codeNamespace;
    std::string baseClass;
    std::string managerHeaderFilename;
    CppEncoderEnum cppEncoder;
    std::string generatorName;
    std::vector<std::string> directoryIncludeFilters;
    std::vector<std::string> directoryExcludeFilters;

    void reset();

  protected:

    //attributes

  };

}; //bin2cpp

#endif //BASEGENERATOR_H
