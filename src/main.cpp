// main.cpp : Defines the entry point for the console application.
//

#include "targetver.h"

#include "bin2cpp.h"

#include <stdlib.h>
#include <stdio.h>
#include <sstream>

//#define ENABLE_BREAKPOINT_DEBUGGING

#ifdef ENABLE_BREAKPOINT_DEBUGGING
#include <windows.h>
#endif

void printHeader()
{
  printf("bin2cpp v%s\n", bin2cpp::getVersionString() );
  printf("Copyright (C) 2013-2014 end2endzone.com. All rights reserved.\n");
}

void printUsage()
{
  printf("Usage:\n");
  printf("  bin2cpp [inputfile] [output folder] [header filename] [function identifier] [chunk size] [-override]. \n");
  printf("    inputfile:             Path to the target file to embed in a C++ source code. \n");
  printf("    output folder:         Output path to create generated source code. ie: .\\generated_files\\\n");
  printf("    header filename:       File name of the C++ Header file. ie: SplashScreen.h\n");
  printf("    function identifier:   Identifier of the function name that is used to get an instance of the file. ie: SplashScreen\n");
  printf("    chunk size (optional): Size of each string segments. Defaults to 200]\n");
  printf("    override (optional):   Tells bin2cpp to over write the destination files.\n");
  printf("\n");
}

bool isNumeric(const char * iValue)
{
  int intValue = atoi(iValue);
  
  //try to convert the int value back to string and check for equality.
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  itoa(intValue, buffer, 10);
  if (std::string(buffer) == iValue)
    return true;
  return false;
}

int main(int argc, char* argv[])
{
  #ifdef ENABLE_BREAKPOINT_DEBUGGING
  MessageBox(NULL, "", "", MB_OK);
  #endif

  printHeader();

  const int numMandatory = 4;
  const int numOptional = 2;

  if ( argc < (1+numMandatory) )
  {
    printUsage();
    bin2cpp::ErrorCodes error = bin2cpp::ErrorCodes::MissingArguments;
    printf("Error: %s. Please specify all %d mandatory arguments.\n", getErrorCodeDescription(error), (numMandatory) );
    return error;
  }
  if ( argc > (1+numMandatory+numOptional) )
  {
    printUsage();
    bin2cpp::ErrorCodes error = bin2cpp::ErrorCodes::MissingArguments;
    printf("Error: %s. Too many arguments (max %d).\n", getErrorCodeDescription(error), (numMandatory+numOptional) );
    return error;
  }

  //mandatory arguments
  const char * inputFilename = argv[1];
  const char * outputFolder = argv[2];
  const char * headerFilename = argv[3];
  const char * functionIdentifier = argv[4];

  //optional arguments
  static const size_t DEFAULT_CHUNK_SIZE = 200;
  size_t chunkSize = DEFAULT_CHUNK_SIZE;
  bool overrideExisting = false;

  //parse optional arguments
  for(int i=(1+numMandatory); i<argc; i++)
  {
    const char * value = argv[i];
    if (isNumeric(value))
    {
      //assume it is chunk size
      chunkSize = atoi(argv[i]);
    }
    if (std::string(value) == "-override")
    {
      overrideExisting = true;
    }
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
  printf("Embedding \"%s\" into \"%s\"%s%s...\n", inputFilename, headerFilename, chunkInfo.c_str(), overrideInfo.c_str());

  //execute
  bin2cpp::ErrorCodes result = bin2cpp::createCppEmbeddedFile(inputFilename, outputFolder, headerFilename, functionIdentifier, chunkSize, overrideExisting);
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
