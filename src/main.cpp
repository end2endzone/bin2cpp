// main.cpp : Defines the entry point for the console application.
//

#include "targetver.h"

#include "SegmentGenerator.h"
#include "StringGenerator.h"
#include "ArrayGenerator.h"

#include <stdlib.h>
#include <stdio.h>
#include <sstream>

#include "common.h"
#include "argumentparser.h"
#include "logger.h"

using namespace bin2cpp;

enum APP_ERROR_CODES
{
  APP_ERROR_SUCCESS,
  APP_ERROR_MISSINGARGUMENTS,
  APP_ERROR_INPUTFILENOTFOUND,
  APP_ERROR_UNABLETOCREATEOUTPUTFILES,
};

enum FILE_UPDATE_MODE
{
  WRITING,
  UPDATING,
  OVERWRITING,
  SKIPPING,
};

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

//pre-declarations
bool processFile(const std::string & inputFile, const std::string & iOutputFilePath, bin2cpp::IGenerator * generator, bool overrideExisting);

void printHeader()
{
  printf("bin2cpp v%s - Convert binary files into C++ source code.\n", bin2cpp::getVersionString() );
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
    "  --output=<path>      Output folder where to create generated code. ie: .\\generated_files\n"
    "  --headerfile=<name>  File name of the generated C++ Header file. ie: SplashScreen.h\n"
    "  --generator=<name>   Name of the generator to use. Possible values are 'segment', 'string' and 'array'. [default: segment].\n"
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
  //help
  std::string dummy;
  if (bin2cpp::parseArgument("help", dummy, argc, argv))
  {
    printHeader();
    printUsage();
    return APP_ERROR_SUCCESS;
  }

  //noheader
  bool noheader = false;
  if (bin2cpp::parseArgument("noheader", dummy, argc, argv))
  {
    noheader = true;
  }

  //quiet
  bool quiet = false;
  if (bin2cpp::parseArgument("quiet", dummy, argc, argv))
  {
    quiet = true;
  }

  //force noheader if quiet
  if (quiet)
    noheader = true;

  bin2cpp::setQuietMode(quiet);

  //version
  if (bin2cpp::parseArgument("version", dummy, argc, argv))
  {
    if (!noheader)
      printHeader();
    return APP_ERROR_SUCCESS;
  }

  if (!noheader && !quiet)
    printHeader();

  //mandatory arguments
  std::string inputFile;
  std::string outputFolder;
  std::string headerFilename;
  std::string functionIdentifier;

  if (!bin2cpp::parseArgument("file", inputFile, argc, argv))
  {
    APP_ERROR_CODES error = APP_ERROR_MISSINGARGUMENTS;
    bin2cpp::log(bin2cpp::LOG_ERROR, "%s (file)", getErrorCodeDescription(error));
    printUsage();
    return error;
  }

  if (!bin2cpp::parseArgument("output", outputFolder, argc, argv))
  {
    APP_ERROR_CODES error = APP_ERROR_MISSINGARGUMENTS;
    bin2cpp::log(bin2cpp::LOG_ERROR, "%s (output)", getErrorCodeDescription(error));
    printUsage();
    return error;
  }

  if (!bin2cpp::parseArgument("headerfile", headerFilename, argc, argv))
  {
    APP_ERROR_CODES error = APP_ERROR_MISSINGARGUMENTS;
    bin2cpp::log(bin2cpp::LOG_ERROR, "%s (headerfile)", getErrorCodeDescription(error));
    printUsage();
    return error;
  }

  if (!bin2cpp::parseArgument("identifier", functionIdentifier, argc, argv))
  {
    APP_ERROR_CODES error = APP_ERROR_MISSINGARGUMENTS;
    bin2cpp::log(bin2cpp::LOG_ERROR, "%s (identifier)", getErrorCodeDescription(error));
    printUsage();
    return error;
  }

  //optional arguments
  static const size_t DEFAULT_CHUNK_SIZE = 200;
  size_t chunkSize = DEFAULT_CHUNK_SIZE;
  bool overrideExisting = false;
  static const char * DEFAULT_NAMESPACE = "bin2cpp";
  static const char * DEFAULT_BASECLASSNAME = "File";
  static const IGenerator::CppEncoderEnum DEFAULT_ENCODING = IGenerator::CPP_ENCODER_OCT;
  std::string codeNamespace;
  std::string baseClass;
  IGenerator::CppEncoderEnum encoding;
  std::string encodingStr;

  size_t tmpChunkSize = 0;
  if (bin2cpp::parseArgument("chunksize", tmpChunkSize, argc, argv))
  {
    chunkSize = tmpChunkSize;
  }

  if (bin2cpp::parseArgument("override", dummy, argc, argv))
  {
    overrideExisting = true;
  }

  if (!bin2cpp::parseArgument("namespace", codeNamespace, argc, argv))
  {
    codeNamespace = DEFAULT_NAMESPACE;
  }

  if (!bin2cpp::parseArgument("baseclass", baseClass, argc, argv))
  {
    baseClass = DEFAULT_BASECLASSNAME;
  }

  if (bin2cpp::parseArgument("encoding", encodingStr, argc, argv))
  {
    if (uppercase(encodingStr) == "OCT")
      encoding = IGenerator::CPP_ENCODER_OCT;
    else if (uppercase(encodingStr) == "HEX")
      encoding = IGenerator::CPP_ENCODER_HEX;
    else
    {
      APP_ERROR_CODES error = APP_ERROR_MISSINGARGUMENTS;
      bin2cpp::log(bin2cpp::LOG_ERROR, "%s (encoding)", getErrorCodeDescription(error));
      printUsage();
      return error;
    }
  }
  else
  {
    encoding = DEFAULT_ENCODING;
  }

  //select generator
  bin2cpp::SegmentGenerator segmentGenerator;
  bin2cpp::StringGenerator stringGenerator;
  bin2cpp::ArrayGenerator arrayGenerator;
  bin2cpp::IGenerator * generator = NULL;

  std::string generatorName;
  if (bin2cpp::parseArgument("generator", generatorName, argc, argv))
  {
    if (generatorName == "segment")
    {
      generator = &segmentGenerator;
    }
    else if (generatorName == "string")
    {
      generator = &stringGenerator;
    }
    else if (generatorName == "array")
    {
      generator = &arrayGenerator;
    }

    //validate generator selection
    if (generator == NULL)
    {
      APP_ERROR_CODES error = APP_ERROR_MISSINGARGUMENTS;
      bin2cpp::log(bin2cpp::LOG_ERROR, "%s, unknown values for 'generator' argument!", getErrorCodeDescription(error));
      printUsage();
      return error;
    }
  }

  //apply default generator
  if (generator == NULL)
  {
    generator = &segmentGenerator;
  }

  // printing info
  std::string info;
  info << "Embedding \"" << inputFile << "\"";
  if (chunkSize != DEFAULT_CHUNK_SIZE)
  {
    info << " using chunks of ";
    info << chunkSize;
    info << " bytes";
  }
  if (overrideExisting)
    info << " overriding existing files";
  info << "...";
  bin2cpp::log(bin2cpp::LOG_INFO, info.c_str());

  //prepare output files path
  std::string outputHeaderPath = outputFolder + "\\" + headerFilename;
  std::string outputCppPath = outputFolder + "\\" + headerFilename;         bin2cpp::strReplace(outputCppPath, ".h", ".cpp");
  std::string cppFilename = headerFilename;                                 bin2cpp::strReplace(cppFilename, ".h", ".cpp");  

  //check if input file exists
  if (!bin2cpp::fileExists(inputFile.c_str()))
    return APP_ERROR_INPUTFILENOTFOUND;

  //configure the generator
  generator->setInputFile(inputFile.c_str());
  generator->setFunctionIdentifier(functionIdentifier.c_str());
  generator->setChunkSize(chunkSize);
  generator->setNamespace(codeNamespace.c_str());
  generator->setBaseClass(baseClass.c_str());
  generator->setCppEncoder(encoding);

  //process files
  bool headerResult = processFile(inputFile, outputHeaderPath, generator, overrideExisting);
  if (!headerResult)
    return APP_ERROR_UNABLETOCREATEOUTPUTFILES;
  
  bool cppResult =    processFile(inputFile, outputCppPath,    generator, overrideExisting);
  if (!cppResult)
    return APP_ERROR_UNABLETOCREATEOUTPUTFILES;

  //success
  return APP_ERROR_SUCCESS;
}

FILE_UPDATE_MODE getFileUpdateMode(const std::string & inputFile, const std::string & iOutputFilePath, bool overrideExisting)
{
  if (!bin2cpp::fileExists(iOutputFilePath.c_str()))
    return WRITING;
  //at this point, we know that the file exists

  if (overrideExisting)
    return OVERWRITING;

  //do not modify the output file if it is not out of date
  uint64_t lastModifiedDate = getFileModifiedDate(inputFile);
  uint64_t outputModifiedDate = getOutputFileModifiedDate(iOutputFilePath);
  if (outputModifiedDate == 0)
    bin2cpp::log(bin2cpp::LOG_WARNING, "Unable to get last modified date of file \'%s\'", iOutputFilePath.c_str());
  if (lastModifiedDate == outputModifiedDate)
    return SKIPPING;

  //file is out of date, update it
  return UPDATING;
}

bool processFile(const std::string & inputFile, const std::string & iOutputFilePath, bin2cpp::IGenerator * generator, bool overrideExisting)
{
  FILE_UPDATE_MODE mode = getFileUpdateMode(inputFile, iOutputFilePath, overrideExisting);

  //writing message
  bin2cpp::log(bin2cpp::LOG_INFO, "%s file \"%s\"...", getUpdateModeText(mode), iOutputFilePath.c_str());
  
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
    bin2cpp::log(bin2cpp::LOG_ERROR, "%s", getErrorCodeDescription(APP_ERROR_UNABLETOCREATEOUTPUTFILES));
    bin2cpp::log(bin2cpp::LOG_ERROR, "Embedding failed!");
  }
  return result;
}
