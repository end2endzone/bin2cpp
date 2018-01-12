// main.cpp : Defines the entry point for the console application.
//

#include "targetver.h"

#include "errorcodes.h"
#include "SegmentGenerator.h"
#include "StringGenerator.h"
#include "ArrayGenerator.h"

#include <stdlib.h>
#include <stdio.h>
#include <sstream>

#include "common.h"
#include "argumentparser.h"
#include "logger.h"
#include "md5support.h"

//#define ENABLE_BREAKPOINT_DEBUGGING

#ifdef ENABLE_BREAKPOINT_DEBUGGING
#include <windows.h>
#endif

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
    "  bin2cpp --file=<path> --output=<path> --headerfile=<name> --identifier=<name> [--generator=<name>] [--chunksize=<value>] [--override] [--noheader] [--quiet]\n"
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
    "  --identifier=<name>  Identifier of the function name that is used to get an instance of the file. ie: SplashScreen\n"
    "  --chunksize=<value>  Size in bytes of each string segments (bytes per row). [default: 200].\n"
    "  --override           Tells bin2cpp to overwrite the destination files.\n"
    "  --noheader           Do not print program header to standard output.\n"
    "  --quiet              Do not log any message to standard output.\n"
    "\n";
  printf("%s", usage);
}

int main(int argc, char* argv[])
{
  #ifdef ENABLE_BREAKPOINT_DEBUGGING
  MessageBox(NULL, "", "", MB_OK);
  #endif

  //help
  std::string dummy;
  if (bin2cpp::parseArgument("help", dummy, argc, argv))
  {
    printHeader();
    printUsage();
    return bin2cpp::ErrorCodes::Success;
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
    return bin2cpp::ErrorCodes::Success;
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
    bin2cpp::ErrorCodes error = bin2cpp::ErrorCodes::MissingArguments;
    bin2cpp::log(bin2cpp::LOG_ERROR, "%s (file)", getErrorCodeDescription(error));
    printUsage();
    return error;
  }

  if (!bin2cpp::parseArgument("output", outputFolder, argc, argv))
  {
    bin2cpp::ErrorCodes error = bin2cpp::ErrorCodes::MissingArguments;
    bin2cpp::log(bin2cpp::LOG_ERROR, "%s (output)", getErrorCodeDescription(error));
    printUsage();
    return error;
  }

  if (!bin2cpp::parseArgument("headerfile", headerFilename, argc, argv))
  {
    bin2cpp::ErrorCodes error = bin2cpp::ErrorCodes::MissingArguments;
    bin2cpp::log(bin2cpp::LOG_ERROR, "%s (headerfile)", getErrorCodeDescription(error));
    printUsage();
    return error;
  }

  if (!bin2cpp::parseArgument("identifier", functionIdentifier, argc, argv))
  {
    bin2cpp::ErrorCodes error = bin2cpp::ErrorCodes::MissingArguments;
    bin2cpp::log(bin2cpp::LOG_ERROR, "%s (identifier)", getErrorCodeDescription(error));
    printUsage();
    return error;
  }

  //optional arguments
  static const size_t DEFAULT_CHUNK_SIZE = 200;
  size_t chunkSize = DEFAULT_CHUNK_SIZE;
  bool overrideExisting = false;

  size_t tmpChunkSize = 0;
  if (bin2cpp::parseArgument("chunksize", tmpChunkSize, argc, argv))
  {
    chunkSize = tmpChunkSize;
  }

  if (bin2cpp::parseArgument("override", dummy, argc, argv))
  {
    overrideExisting = true;
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
      bin2cpp::ErrorCodes error = bin2cpp::ErrorCodes::MissingArguments;
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
  std::string chunkInfo = "";
  if (chunkSize != DEFAULT_CHUNK_SIZE)
  {
    std::stringstream ss;
    ss << " using chunks of ";
    ss << chunkSize;
    ss << " bytes";
    chunkInfo.append(ss.str());
  }
  std::string overrideInfo = "";
  if (overrideExisting)
    overrideInfo = " overriding existing files";
  bin2cpp::log(bin2cpp::LOG_INFO, "Embedding \"%s\" into \"%s\"%s%s...", inputFile.c_str(), headerFilename.c_str(), chunkInfo.c_str(), overrideInfo.c_str());
  
  //generate header
  std::string outputHeaderPath = outputFolder + "\\" + headerFilename;
  bin2cpp::log(bin2cpp::LOG_INFO, "Writing file \"%s\"...", outputHeaderPath.c_str());
  bin2cpp::ErrorCodes headerResult = bin2cpp::ErrorCodes::Success;

  //check if header file already exists
  if (bin2cpp::fileExists(outputHeaderPath.c_str()))
  {
    if (!overrideExisting)
    {
      bin2cpp::ErrorCodes error = bin2cpp::ErrorCodes::OutputFileAlreadyExist;
      bin2cpp::log(bin2cpp::LOG_ERROR, "%s (%s)", getErrorCodeDescription(error), outputHeaderPath.c_str());
      return error;
    }
  }

  if (headerResult == bin2cpp::ErrorCodes::Success)
  {
    //generate file or override existing
    headerResult = generator->createHeaderEmbededFile(inputFile.c_str(), outputHeaderPath.c_str(), functionIdentifier.c_str());
  }
  if (headerResult == bin2cpp::ErrorCodes::Success)
  {
    //OK
  }
  else if (headerResult == bin2cpp::ErrorCodes::OutputFilesSkipped)
  {
    bin2cpp::log(bin2cpp::LOG_WARNING, "%s", getErrorCodeDescription(headerResult));
  }
  else
  {
    bin2cpp::log(bin2cpp::LOG_ERROR, "%s", getErrorCodeDescription(headerResult));
    bin2cpp::log(bin2cpp::LOG_ERROR, "Embedding failed!");
    return headerResult;
  }

  //generate cpp
  std::string outputCppPath = outputFolder + "\\" + headerFilename;
  bin2cpp::strReplace(outputCppPath, ".h", ".cpp");
  bin2cpp::log(bin2cpp::LOG_INFO, "Writing file \"%s\"...", outputCppPath.c_str());
  bin2cpp::ErrorCodes cppResult = bin2cpp::ErrorCodes::Success;
 
  //check if cpp file already exists
  std::string cppFilename = headerFilename;
  bin2cpp::strReplace(cppFilename, ".h", ".cpp");
  if (bin2cpp::fileExists(outputCppPath.c_str()))
  {
    if (!overrideExisting)
    {
      bin2cpp::ErrorCodes error = bin2cpp::ErrorCodes::OutputFileAlreadyExist;
      bin2cpp::log(bin2cpp::LOG_ERROR, "%s (%s)", getErrorCodeDescription(error), outputCppPath.c_str());
      return error;
    }
  }

  if (cppResult == bin2cpp::ErrorCodes::Success)
  {
    //generate file or override existing
    cppResult = generator->createCppEmbeddedFile(inputFile.c_str(), outputCppPath.c_str(), functionIdentifier.c_str(), chunkSize);
  }
  if (cppResult == bin2cpp::ErrorCodes::Success)
  {
    //OK
  }
  else if (cppResult == bin2cpp::ErrorCodes::OutputFilesSkipped)
  {
    bin2cpp::log(bin2cpp::LOG_WARNING, "%s", getErrorCodeDescription(cppResult));
  }
  else
  {
    bin2cpp::log(bin2cpp::LOG_ERROR, "%s", getErrorCodeDescription(cppResult));
    bin2cpp::log(bin2cpp::LOG_ERROR, "Embedding failed!");
    return cppResult;
  }
 
  //success
  return 0;
}
