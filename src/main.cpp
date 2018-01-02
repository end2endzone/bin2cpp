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

//#define ENABLE_BREAKPOINT_DEBUGGING

#ifdef ENABLE_BREAKPOINT_DEBUGGING
#include <windows.h>
#endif

void printHeader()
{
  printf("bin2cpp v%s\n", bin2cpp::getVersionString() );
  printf("Convert binary files into C++ source code.\n");
  printf("Copyright (C) 2013-%d end2endzone.com. All rights reserved.\n", bin2cpp::getCopyrightYear());
  printf("bin2cpp is open source software, see http://github.com/end2endzone/bin2cpp \n");
}

void printUsage()
{
  //usage string in docopt format. See http://docopt.org/
  static const char usage[] = 
    "Usage:\n"
    "  bin2cpp --file=<path> --output=<path> --headerfile=<name> --identifier=<name> [--chunksize=<value>] [--override] [--noheader] [--quiet]\n"
    "  bin2cpp -h | --help\n"
    "  bin2cpp --version\n"
    "\n"
    "Options:\n"
    "  --help               Display this help message.\n"
    "  --version            Display this application version.\n"
    "  --file=<path>        Path of the input file used for embedding as a C++ source code.\n"
    "  --output=<path>      Output folder where to create generated code. ie: .\\generated_files\n"
    "  --headerfile=<name>  File name of the generated C++ Header file. ie: SplashScreen.h\n"
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

  //look for help
  std::string dummy;
  if (bin2cpp::parseArgument("help", dummy, argc, argv))
  {
    printHeader();
    printUsage();
    return bin2cpp::ErrorCodes::Success;
  }

  //look for noheader
  bool noheader = false;
  if (bin2cpp::parseArgument("noheader", dummy, argc, argv))
  {
    noheader = true;
  }

  //look for quiet
  bool quiet = false;
  if (bin2cpp::parseArgument("quiet", dummy, argc, argv))
  {
    quiet = true;
  }

  //force noheader if quiet
  if (quiet)
    noheader = true;

  //look for version
  if (bin2cpp::parseArgument("version", dummy, argc, argv))
  {
    if (!noheader)
      printHeader();
    return bin2cpp::ErrorCodes::Success;
  }

  if (!noheader && !quiet)
    printHeader();

  //mandatory arguments
  std::string inputFilename;
  std::string outputFolder;
  std::string headerFilename;
  std::string functionIdentifier;

  if (!bin2cpp::parseArgument("file", inputFilename, argc, argv))
  {
    bin2cpp::ErrorCodes error = bin2cpp::ErrorCodes::MissingArguments;
    printf("Error (%s). Missing 'file' argument!\n", getErrorCodeDescription(error));
    printUsage();
    return error;
  }

  if (!bin2cpp::parseArgument("output", outputFolder, argc, argv))
  {
    bin2cpp::ErrorCodes error = bin2cpp::ErrorCodes::MissingArguments;
    printf("Error (%s). Missing 'output' argument!\n", getErrorCodeDescription(error));
    printUsage();
    return error;
  }

  if (!bin2cpp::parseArgument("headerfile", headerFilename, argc, argv))
  {
    bin2cpp::ErrorCodes error = bin2cpp::ErrorCodes::MissingArguments;
    printf("Error (%s). Missing 'headerfile' argument!\n", getErrorCodeDescription(error));
    printUsage();
    return error;
  }

  if (!bin2cpp::parseArgument("identifier", functionIdentifier, argc, argv))
  {
    bin2cpp::ErrorCodes error = bin2cpp::ErrorCodes::MissingArguments;
    printf("Error (%s). Missing 'identifier' argument!\n", getErrorCodeDescription(error));
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
  if (!quiet)
  {
    printf("Embedding \"%s\" into \"%s\"%s%s...\n", inputFilename.c_str(), headerFilename.c_str(), chunkInfo.c_str(), overrideInfo.c_str());
  }

  //select generator
  //bin2cpp::StringGenerator generator;
  //bin2cpp::ArrayGenerator generator;
  bin2cpp::SegmentGenerator generator;

  //generate header
  bool headerSuccess = false;
  if (!quiet)
  {
    printf("Generating header file...\n");
  }
  bin2cpp::ErrorCodes headerResult = generator.createHeaderEmbededFile(outputFolder.c_str(), headerFilename.c_str(), functionIdentifier.c_str(), overrideExisting);
  if (headerResult == bin2cpp::ErrorCodes::Success)
  {
    if (!quiet)
    {
      printf("Done.\n");
    }
    headerSuccess = true;
  }
  else if (headerResult == bin2cpp::ErrorCodes::OutputFilesSkipped)
  {
    if (!quiet)
    {
      printf("Warning: %s.\n", getErrorCodeDescription(headerResult));
    }
    headerSuccess = true;
  }
  else
  {
    if (!quiet)
    {
      printf("Error: %s.\n", getErrorCodeDescription(headerResult));
    }
    headerSuccess = false;
  }

  //generate cpp
  bool cppSuccess = false;
  if (!quiet)
  {
    printf("Generating cpp file...\n");
  }
  bin2cpp::ErrorCodes cppResult = generator.createCppEmbeddedFile(inputFilename.c_str(), outputFolder.c_str(), headerFilename.c_str(), functionIdentifier.c_str(), chunkSize, overrideExisting);
  if (cppResult == bin2cpp::ErrorCodes::Success)
  {
    if (!quiet)
    {
      printf("Done.\n");
    }
    cppSuccess = true;
  }
  else if (cppResult == bin2cpp::ErrorCodes::OutputFilesSkipped)
  {
    if (!quiet)
    {
      printf("Warning: %s.\n", getErrorCodeDescription(cppResult));
    }
    cppSuccess = true;
  }
  else
  {
    if (!quiet)
    {
      printf("Error: %s.\n", getErrorCodeDescription(cppResult));
    }
    cppSuccess = false;
  }
  
  if (cppSuccess && headerSuccess)
    return 0;

  //error found
  printf("Embedding failed!\n");
  return 2;
}
