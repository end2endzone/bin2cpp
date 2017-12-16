// main.cpp : Defines the entry point for the console application.
//

#include "targetver.h"

#include "bin2cpp.h"

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
  printf("Copyright (C) 2013-%d end2endzone.com. All rights reserved.\n", bin2cpp::getCopyrightYear());
}

void printUsage()
{
  printf("Usage:\n");
  printf("  bin2cpp --file=/path/to/file --output=/path/to/output/folder --headerfile=name.h --identifier=value --chunksize=value --override. \n");
  printf("    file:                 Path to the target file to embed in a C++ source code.\n");
  printf("    output:               Output path to create generated source code. ie: .\\generated_files\n");
  printf("    headerfile:           File name of the C++ Header file. ie: SplashScreen.h\n");
  printf("    identifier:           Identifier of the function name that is used to get an instance of the file. ie: SplashScreen\n");
  printf("    chunksize (optional): Size in bytes of each string segments (bytes per row). Defaults to 200.\n");
  printf("    override (optional):  Tells bin2cpp to over write the destination files.\n");
  printf("\n");
}

int main(int argc, char* argv[])
{
  #ifdef ENABLE_BREAKPOINT_DEBUGGING
  MessageBox(NULL, "", "", MB_OK);
  #endif

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

  std::string tmpOverride;
  if (bin2cpp::parseArgument("override", tmpOverride, argc, argv))
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
  printf("Embedding \"%s\" into \"%s\"%s%s...\n", inputFilename.c_str(), headerFilename.c_str(), chunkInfo.c_str(), overrideInfo.c_str());

  //execute
  bin2cpp::ErrorCodes result = bin2cpp::createCppEmbeddedFile(inputFilename.c_str(), outputFolder.c_str(), headerFilename.c_str(), functionIdentifier.c_str(), chunkSize, overrideExisting);
  if (result == bin2cpp::ErrorCodes::Success)
  {
    printf("Done.\n");
	  return 0;
  }
  else if (result == bin2cpp::ErrorCodes::OutputFilesSkipped)
  {
    printf("%s.\n", getErrorCodeDescription(result));
	  return 0;
  }
  
  //error found
  printf("Embedding failed! %s.\n", getErrorCodeDescription(result));
  return 2;
}
