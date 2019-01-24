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

// main.cpp : Defines the entry point for the console application.
//

#include "SegmentGenerator.h"
#include "StringGenerator.h"
#include "ArrayGenerator.h"
#include "Win32ResourceGenerator.h"

#include <stdlib.h>
#include <stdio.h>
#include <sstream>

#include "rapidassist/cli.h"
#include "rapidassist/logger.h"
#include "rapidassist/strings.h"
#include "rapidassist/filesystem.h"

#include "common.h"

using namespace bin2cpp;

enum APP_ERROR_CODES
{
  APP_ERROR_SUCCESS,
  APP_ERROR_MISSINGARGUMENTS,
  APP_ERROR_INPUTFILENOTFOUND,
  APP_ERROR_UNABLETOCREATEOUTPUTFILES,
  APP_ERROR_TOOMANYARGUMENTS,
  APP_ERROR_INPUTDIRNOTFOUND
};

enum FILE_UPDATE_MODE
{
  WRITING,
  UPDATING,
  OVERWRITING,
  SKIPPING,
};

//default values
static const size_t DEFAULT_CHUNK_SIZE = 200;
static const char * DEFAULT_NAMESPACE = "bin2cpp";
static const char * DEFAULT_BASECLASSNAME = "File";
static const IGenerator::CppEncoderEnum DEFAULT_ENCODING = IGenerator::CPP_ENCODER_OCT;

const char * getErrorCodeDescription(const APP_ERROR_CODES & iErrorCode)
{
  switch(iErrorCode)
  {
  case APP_ERROR_SUCCESS:
    return "Success";
    break;
  case APP_ERROR_MISSINGARGUMENTS:
    return "Missing arguments";
    break;
  case APP_ERROR_INPUTFILENOTFOUND:
    return "Unable to open input file";
    break;
  case APP_ERROR_UNABLETOCREATEOUTPUTFILES:
    return "Unable to create output files";
    break;
  case APP_ERROR_TOOMANYARGUMENTS:
    return "Too many arguments";
    break;
  case APP_ERROR_INPUTDIRNOTFOUND:
    return "Input directory not found";
    break;
  default:
    return "Unknown error";
  };
}

const char * getUpdateModeText(const FILE_UPDATE_MODE & iMode)
{
  switch(iMode)
  {
  case WRITING:
    return "Writing";
  case UPDATING:
    return "Updating";
  case OVERWRITING:
    return "Overwriting";
  case SKIPPING:
    return "Skipping";
  default:
    return "Unknown";
  };
}

std::string getIdentifier(const std::string & value)
{
  std::string id = value;

  int numReplaced = -1;

  //no space character
  numReplaced = ra::strings::replace(id, " ", "");

  //no space character
  numReplaced = ra::strings::replace(id, " ", "");

  return id;
}

std::string getFilenameWithoutExtension(const char * iPath)
{
  if (iPath == NULL)
    return "";

  std::string filename = ra::filesystem::getFilename(iPath);
  std::string extension = ra::filesystem::getFileExtention(iPath);
  
  //extract filename without extension
  std::string filenameWE = filename.substr(0, filename.size() - extension.size());
  
  //remove last dot of the filename if required
  filenameWE = ra::strings::trimRight(filenameWE, '.');

  return filenameWE;
}

struct ARGUMENTS
{
  bool help;
  bool noheader;
  bool quiet;
  bool version;
  bool hasFile;
  bool hasDir;
  std::string inputFile;
  std::string inputDir;
  std::string outputFolder;
  std::string headerFilename;
  std::string functionIdentifier;
  size_t chunkSize;
  bool overrideExisting;
  std::string codeNamespace;
  std::string baseClass;
  IGenerator::CppEncoderEnum encoding;
  std::string generatorName;
};

//pre-declarations
bool generateFile(const std::string & inputFile, const std::string & iOutputFilePath, bin2cpp::IGenerator * generator, bool overrideExisting);
APP_ERROR_CODES processSingleFile(const ARGUMENTS & args, bin2cpp::IGenerator * generator);

void printHeader()
{
  printf("bin2cpp v%s - Convert binary files into C++ source code.\n", getVersionString() );
  printf("Copyright (C) 2013-%d end2endzone.com. All rights reserved.\n", bin2cpp::getCopyrightYear());
  printf("bin2cpp is open source software, see http://github.com/end2endzone/bin2cpp \n");
}

void printUsage()
{
  //usage string in docopt format. See http://docopt.org/
  static const char usage[] = 
    "Usage:\n"
    "  bin2cpp --file=<path> --output=<path> --headerfile=<name> --identifier=<name> [--generator=<name>] [--encoding=<name>] [--chunksize=<value>] [--namespace=<value>] [--baseclass=<value>] [--override] [--noheader] [--quiet]\n"
    "  bin2cpp --help\n"
    "  bin2cpp --version\n"
    "\n"
    "Options:\n"
    "  --help               Display this help message.\n"
    "  --version            Display this application version.\n"
    "  --file=<path>        Path of the input file used for embedding as a C++ source code.\n"
    "  --dir=<path>         Path of the input directory used for embedding all files of the rirectory as a C++ source code.\n"
    #ifdef _WIN32
    "  --output=<path>      Output folder where to create generated code. ie: .\\generated_files\n"
    #else
    "  --output=<path>      Output folder where to create generated code. ie: ./generated_files\n"
    #endif
    "  --headerfile=<name>  File name of the generated C++ Header file. ie: SplashScreen.h\n"
    "  --generator=<name>   Name of the generator to use. Possible values are 'segment', 'string', 'array' and 'win32'. [default: segment].\n"
    "  --encoding=<name>    Name of the binary to string literal encoding to use. Possible values are 'oct' and 'hex'. [default: oct].\n"
    "  --identifier=<name>  Identifier of the function name that is used to get an instance of the file. ie: SplashScreen\n"
    "  --chunksize=<value>  Size in bytes of each string segments (bytes per row). [default: 200].\n"
    "  --baseclass=<value>  The name of the interface for embedded files. [default: File].\n"
    "  --namespace=<value>  The namespace of the generated source code [default: bin2cpp].\n"
    "  --override           Tells bin2cpp to overwrite the destination files.\n"
    "  --noheader           Do not print program header to standard output.\n"
    "  --quiet              Do not log any message to standard output.\n"
    "\n";
  printf("%s", usage);
}

int main(int argc, char* argv[])
{
  ARGUMENTS args;
  args.help = false;
  args.noheader = false;
  args.quiet = false;
  args.version = false;
  args.hasFile = false;
  args.hasDir = false;
  args.chunkSize = 0;
  args.overrideExisting;

  std::string dummy;

  //help
  args.help = ra::cli::parseArgument("help", dummy, argc, argv);
  if (args.help)
  {
    printHeader();
    printUsage();
    return APP_ERROR_SUCCESS;
  }

  //noheader
  args.noheader = ra::cli::parseArgument("noheader", dummy, argc, argv);

  //quiet
  args.quiet = ra::cli::parseArgument("quiet", dummy, argc, argv);

  //force noheader if quiet
  if (args.quiet)
    args.noheader = true;

  ra::logger::setQuietMode(args.quiet);

  //version
  args.version = ra::cli::parseArgument("version", dummy, argc, argv);
  if (args.version)
  {
    if (!args.noheader)
      printHeader();
    return APP_ERROR_SUCCESS;
  }

  if (!args.noheader && !args.quiet)
    printHeader();

  //mandatory arguments
  args.hasFile = ra::cli::parseArgument("file", args.inputFile, argc, argv);
  args.hasDir  = ra::cli::parseArgument("dir",  args.inputDir,  argc, argv);
  if (!args.hasFile && !args.hasDir)
  {
    //file or dir must be specified
    APP_ERROR_CODES error = APP_ERROR_MISSINGARGUMENTS;
    ra::logger::log(ra::logger::LOG_ERROR, "%s (file, dir)", getErrorCodeDescription(error));
    printUsage();
    return error;
  }
  else if (args.hasFile && args.hasDir)
  {
    //file OR dir must be specified, not both
    APP_ERROR_CODES error = APP_ERROR_TOOMANYARGUMENTS;
    ra::logger::log(ra::logger::LOG_ERROR, "%s (file, dir)", getErrorCodeDescription(error));
    printUsage();
    return error;
  }

  if (!ra::cli::parseArgument("output", args.outputFolder, argc, argv))
  {
    APP_ERROR_CODES error = APP_ERROR_MISSINGARGUMENTS;
    ra::logger::log(ra::logger::LOG_ERROR, "%s (output)", getErrorCodeDescription(error));
    printUsage();
    return error;
  }

  if (args.hasDir)
  {
    if (ra::cli::parseArgument("headerfile", args.headerFilename, argc, argv))
    {
      //headerfile not supported with dir argument
      APP_ERROR_CODES error = APP_ERROR_TOOMANYARGUMENTS;
      ra::logger::log(ra::logger::LOG_ERROR, "%s (headerfile)", getErrorCodeDescription(error));
      printUsage();
      return error;
    }
  }
  else if (args.hasFile)
  {
    if (!ra::cli::parseArgument("headerfile", args.headerFilename, argc, argv))
    {
      APP_ERROR_CODES error = APP_ERROR_MISSINGARGUMENTS;
      ra::logger::log(ra::logger::LOG_ERROR, "%s (headerfile)", getErrorCodeDescription(error));
      printUsage();
      return error;
    }
  }

  if (args.hasDir)
  {
    if (ra::cli::parseArgument("identifier", args.functionIdentifier, argc, argv))
    {
      //identifier not supported with dir argument
      APP_ERROR_CODES error = APP_ERROR_TOOMANYARGUMENTS;
      ra::logger::log(ra::logger::LOG_ERROR, "%s (identifier)", getErrorCodeDescription(error));
      printUsage();
      return error;
    }
  }
  else if (args.hasFile)
  {
    if (!ra::cli::parseArgument("identifier", args.functionIdentifier, argc, argv))
    {
      APP_ERROR_CODES error = APP_ERROR_MISSINGARGUMENTS;
      ra::logger::log(ra::logger::LOG_ERROR, "%s (identifier)", getErrorCodeDescription(error));
      printUsage();
      return error;
    }
  }

  //optional arguments

  size_t tmpChunkSize = 0;
  args.chunkSize = DEFAULT_CHUNK_SIZE;
  if (ra::cli::parseArgument("chunksize", tmpChunkSize, argc, argv))
  {
    args.chunkSize = tmpChunkSize;
  }

  args.overrideExisting = ra::cli::parseArgument("override", dummy, argc, argv);

  if (!ra::cli::parseArgument("namespace", args.codeNamespace, argc, argv))
  {
    args.codeNamespace = DEFAULT_NAMESPACE;
  }

  if (!ra::cli::parseArgument("baseclass", args.baseClass, argc, argv))
  {
    args.baseClass = DEFAULT_BASECLASSNAME;
  }

  std::string encodingStr;
  if (ra::cli::parseArgument("encoding", encodingStr, argc, argv))
  {
    if (ra::strings::uppercase(encodingStr) == "OCT")
      args.encoding = IGenerator::CPP_ENCODER_OCT;
    else if (ra::strings::uppercase(encodingStr) == "HEX")
      args.encoding = IGenerator::CPP_ENCODER_HEX;
    else
    {
      APP_ERROR_CODES error = APP_ERROR_MISSINGARGUMENTS;
      ra::logger::log(ra::logger::LOG_ERROR, "%s (encoding)", getErrorCodeDescription(error));
      printUsage();
      return error;
    }
  }
  else
  {
    args.encoding = DEFAULT_ENCODING;
  }

  //select generator
  bin2cpp::SegmentGenerator segmentGenerator;
  bin2cpp::StringGenerator stringGenerator;
  bin2cpp::ArrayGenerator arrayGenerator;
  bin2cpp::Win32ResourceGenerator win32Generator;
  bin2cpp::IGenerator * generator = NULL;

  if (ra::cli::parseArgument("generator", args.generatorName, argc, argv))
  {
    if (args.generatorName == "segment")
    {
      generator = &segmentGenerator;
    }
    else if (args.generatorName == "string")
    {
      generator = &stringGenerator;
    }
    else if (args.generatorName == "array")
    {
      generator = &arrayGenerator;
    }
    else if (args.generatorName == "win32")
    {
      generator = &win32Generator;
    }

    //validate generator selection
    if (generator == NULL)
    {
      APP_ERROR_CODES error = APP_ERROR_MISSINGARGUMENTS;
      ra::logger::log(ra::logger::LOG_ERROR, "%s, unknown values for 'generator' argument!", getErrorCodeDescription(error));
      printUsage();
      return error;
    }
  }

  //apply default generator
  if (generator == NULL)
  {
    generator = &segmentGenerator;
  }

  //process file or directory
  if (args.hasFile)
  {
    APP_ERROR_CODES error = processSingleFile(args, generator);
    if (error != APP_ERROR_SUCCESS)
    {
      ra::logger::log(ra::logger::LOG_ERROR, "%s", getErrorCodeDescription(error));
    }
    return error;
  }
  else if (args.hasDir)
  {
    //check if input dir exists
    if (!ra::filesystem::folderExists(args.inputDir.c_str()))
    {
      APP_ERROR_CODES error = APP_ERROR_INPUTDIRNOTFOUND;
      ra::logger::log(ra::logger::LOG_ERROR, "%s (%s)", getErrorCodeDescription(error), args.inputDir.c_str());
      return error;
    }

    //search all files in the directory
    ra::strings::StringVector files;
    bool found = ra::filesystem::findFiles(files, args.inputDir.c_str());
    if (!found)
    {
      APP_ERROR_CODES error = APP_ERROR_INPUTDIRNOTFOUND;
      ra::logger::log(ra::logger::LOG_ERROR, "%s (%s)", getErrorCodeDescription(error), args.inputDir.c_str());
      return error;
    }

    //remove directories from list
    ra::strings::StringVector tmp;
    for(size_t i=0; i<files.size(); i++)
    {
      const std::string & file = files[i];
      if (ra::filesystem::fileExists(file.c_str()))
        tmp.push_back(file);
    }
    files = tmp;

    //need to process files ?
    if (files.empty())
      return APP_ERROR_SUCCESS;

    //for each files
    for(size_t i=0; i<files.size(); i++)
    {
      const std::string & file = files[i];

      //build a 'headerfile' and 'identifier' argument for this file...
      ARGUMENTS argsCopy = args;

      //replace 'dir' input by current file input
      argsCopy.hasDir = false;
      argsCopy.inputDir = "";
      argsCopy.hasFile = true;
      argsCopy.inputFile = file;

      //use the file name without extension as 'headerfile'.
      argsCopy.headerFilename = getIdentifier(getFilenameWithoutExtension(file.c_str()));
      argsCopy.headerFilename.append(".h");

      //use the file name without extension as 'identifier'.
      argsCopy.functionIdentifier = getIdentifier(getFilenameWithoutExtension(file.c_str()));
      argsCopy.functionIdentifier = ra::strings::capitalizeFirstCharacter(argsCopy.functionIdentifier);

      //process this file...
      APP_ERROR_CODES error = processSingleFile(argsCopy, generator);
      if (error != APP_ERROR_SUCCESS)
      {
        ra::logger::log(ra::logger::LOG_ERROR, "%s", getErrorCodeDescription(error));
        return error;
      }

      //next file
    }

    //all files processed
    return APP_ERROR_SUCCESS;
  }

  return APP_ERROR_UNABLETOCREATEOUTPUTFILES;
}

APP_ERROR_CODES processSingleFile(const ARGUMENTS & args, bin2cpp::IGenerator * generator)
{
  // printing info
  std::string info;
  info << "Embedding \"" << args.inputFile << "\"";
  if (args.chunkSize != DEFAULT_CHUNK_SIZE)
  {
    info << " using chunks of ";
    info << args.chunkSize;
    info << " bytes";
  }
  if (args.overrideExisting)
    info << " overriding existing files";
  info << "...";
  ra::logger::log(ra::logger::LOG_INFO, info.c_str());

  //prepare output files path
  std::string outputHeaderPath = args.outputFolder + ra::filesystem::getPathSeparatorStr() + args.headerFilename;
  std::string outputCppPath = args.outputFolder + ra::filesystem::getPathSeparatorStr() + args.headerFilename;
  std::string cppFilename = args.headerFilename;
  ra::strings::replace(outputCppPath, ".h", ".cpp");
  ra::strings::replace(cppFilename, ".h", ".cpp");  

  //check if input file exists
  if (!ra::filesystem::fileExists(args.inputFile.c_str()))
    return APP_ERROR_INPUTFILENOTFOUND;

  //configure the generator
  generator->setInputFile(args.inputFile.c_str());
  generator->setFunctionIdentifier(args.functionIdentifier.c_str());
  generator->setChunkSize(args.chunkSize);
  generator->setNamespace(args.codeNamespace.c_str());
  generator->setBaseClass(args.baseClass.c_str());
  generator->setCppEncoder(args.encoding);

  //process files
  bool headerResult = generateFile(args.inputFile, outputHeaderPath, generator, args.overrideExisting);
  if (!headerResult)
    return APP_ERROR_UNABLETOCREATEOUTPUTFILES;
  
  bool cppResult =    generateFile(args.inputFile, outputCppPath,    generator, args.overrideExisting);
  if (!cppResult)
    return APP_ERROR_UNABLETOCREATEOUTPUTFILES;

  //success
  return APP_ERROR_SUCCESS;
}

FILE_UPDATE_MODE getFileUpdateMode(const std::string & inputFile, const std::string & iOutputFilePath, bool overrideExisting)
{
  if (!ra::filesystem::fileExists(iOutputFilePath.c_str()))
    return WRITING;
  //at this point, we know that the file exists

  if (overrideExisting)
    return OVERWRITING;

  //do not modify the output file if it is not out of date
  uint64_t lastModifiedDate = ra::filesystem::getFileModifiedDate(inputFile);
  uint64_t outputModifiedDate = bin2cpp::getOutputFileModifiedDate(iOutputFilePath);
  if (outputModifiedDate == 0)
    ra::logger::log(ra::logger::LOG_WARNING, "Unable to get last modified date of file \'%s\'", iOutputFilePath.c_str());
  if (lastModifiedDate == outputModifiedDate)
    return SKIPPING;

  //file is out of date, update it
  return UPDATING;
}

bool generateFile(const std::string & inputFile, const std::string & iOutputFilePath, bin2cpp::IGenerator * generator, bool overrideExisting)
{
  FILE_UPDATE_MODE mode = getFileUpdateMode(inputFile, iOutputFilePath, overrideExisting);

  //writing message
  ra::logger::log(ra::logger::LOG_INFO, "%s file \"%s\"...", getUpdateModeText(mode), iOutputFilePath.c_str());
  
  if (mode == SKIPPING)
    return true; //skipping is success

  //generate file
  bool result = false;
  if (isCppHeaderFile(iOutputFilePath))
  {
    //generate header
    result = generator->createCppHeaderFile(iOutputFilePath.c_str());
  }
  else
  {
    //generate cpp
    result = generator->createCppSourceFile(iOutputFilePath.c_str());
  }
  if (!result)
  {
    //there was an error generating file
    ra::logger::log(ra::logger::LOG_ERROR, "%s", getErrorCodeDescription(APP_ERROR_UNABLETOCREATEOUTPUTFILES));
    ra::logger::log(ra::logger::LOG_ERROR, "Embedding failed!");
  }
  return result;
}
