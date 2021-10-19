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
#include "rapidassist/logging.h"
#include "rapidassist/strings.h"
#include "rapidassist/filesystem.h"
#include "rapidassist/process.h"
#include "rapidassist/timing.h"

#include "common.h"

using namespace bin2cpp;

enum APP_ERROR_CODES
{
  APP_ERROR_SUCCESS = 0,
  APP_ERROR_MISSINGARGUMENTS,
  APP_ERROR_INPUTFILENOTFOUND,
  APP_ERROR_UNABLETOCREATEOUTPUTFILES,
  APP_ERROR_TOOMANYARGUMENTS,
  APP_ERROR_INPUTDIRNOTFOUND,
  AAP_ERROR_NOTSUPPORTED,
  APP_ERROR_OPERATIONHASFAILED
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
static Dictionary identifiers_dictionary;   // unique values for identifiers
static Dictionary output_files_dictionary;  // unique values for output file names

const char * getErrorCodeDescription(const APP_ERROR_CODES & error_code)
{
  switch(error_code)
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
  case AAP_ERROR_NOTSUPPORTED:
    return "Operation not supported";
    break;
  case APP_ERROR_OPERATIONHASFAILED:
    return "Operation has failed";
    break;
  default:
    return "Unknown error";
  };
}

const char * getUpdateModeText(const FILE_UPDATE_MODE & mode)
{
  switch(mode)
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

struct ARGUMENTS
{
  bool help;
  bool noheader;
  bool quiet;
  bool version;
  bool hasFile;                 // true if 'inputFilePath' is set.
  bool hasDir;                  // true if 'inputDirPath' is set.
  bool hasReportedFilePath;     // true if 'reportedFilePath' is set.
  bool hasManagerFile;          // true if 'managerHeaderFilename' is set.
  bool keepDirectoryStructure;  // true if the output files must have the same directory structure as the input files. Valid only when --dir is used.
  std::string inputFilePath;    // path of the input binary file
  std::string inputDirPath;
  std::string outputDirPath;
  std::string headerFilename;
  std::string functionIdentifier;
  std::string reportedFilePath; // path reported in the public api when calling getFilePath();
  size_t chunkSize;
  bool plainOutput;
  bool overrideExisting;
  std::string codeNamespace;
  std::string baseClass;
  std::string managerHeaderFilename;
  bool registerfile;
  IGenerator::CppEncoderEnum encoding;
  std::string generatorName;
};

//pre-declarations
bool generateFile(const ARGUMENTS & args, const std::string & output_file_path, bin2cpp::IGenerator * generator);
bool generateManagerFile(const ARGUMENTS & args, const std::string & output_file_path, bin2cpp::IGenerator * generator);
APP_ERROR_CODES processInputFile(const ARGUMENTS & args, bin2cpp::IGenerator * generator);
APP_ERROR_CODES processInputDirectory(const ARGUMENTS & args, bin2cpp::IGenerator * generator);
APP_ERROR_CODES processManagerFiles(const ARGUMENTS & args, bin2cpp::IGenerator * generator);
APP_ERROR_CODES processPlainOutput(const ARGUMENTS & args, bin2cpp::IGenerator * generator);
std::string getDefaultFunctionIdentifier(const ARGUMENTS & args, Dictionary & identifiers_dictionary);
std::string getDefaultHeaderFile(const ARGUMENTS & args);

void printHeader()
{
  printf("bin2cpp v%s - Convert binary files into C++ source code.\n", getVersionString() );
  printf("Copyright (C) 2013-%d end2endzone.com. All rights reserved.\n", ra::timing::GetCopyrightYear());
  printf("bin2cpp is open source software, see http://github.com/end2endzone/bin2cpp \n");
}

void printUsage()
{
#ifdef _WIN32
#define SEPARATOR "\\"
#else
#define SEPARATOR "/"
#endif

  //usage string in docopt format. See http://docopt.org/
  static const char usage[] = 
    "Usage:\n"
    "  bin2cpp --file=<path> --output=<path> [--headerfile=<name>] [--identifier=<name>] [--generator=<name>] [--encoding=<name>] [--chunksize=<value>] [--namespace=<value>] [--baseclass=<name>] [--managerfile=<name>] [--registerfile] [--reportedfilepath=<value>] [--override] [--noheader] [--quiet]\n"
    "  bin2cpp --dir=<path>  --output=<path> [--keepdirs] [--generator=<name>] [--encoding=<name>] [--chunksize=<value>] [--namespace=<value>] [--baseclass=<name>] [--managerfile=<name>] [--registerfile] [--override] [--noheader] [--quiet]\n"
    "  bin2cpp --help\n"
    "  bin2cpp --version\n"
    "\n"
    "Options:\n"
    "  --help                     Display this help message.\n"
    "  --version                  Display this application version.\n"
    "  --file=<path>              Path of the input file used for embedding as C++ source code.\n"
    "  --dir=<path>               Path of the input directory used for embedding all files of the directory as C++ source code.\n"
    "  --output=<path>            Path of the output directory where to create generated code. ie: ." SEPARATOR "generated_files\n"
    "  --headerfile=<path>        File name or relative path of the generated C++ header file. If a relative path from the output directory is specified,\n"
    "                             the #include statement in the generated cpp file will match the relative path. ie: SplashScreen.h\n"
    "                             Default value: input file name (without extension)\n"
    "  --identifier=<name>        Identifier of the function name that is used to get an instance of the file. ie: SplashScreen\n"
    "                             Default value is based on input file with format 'NameExt'.\n"
    "  --generator=<name>         Name of the generator to use. Possible values are 'segment', 'string', 'array' and 'win32'. [default: segment]\n"
    "  --encoding=<name>          Name of the binary to string literal encoding to use. Possible values are 'oct' and 'hex'. [default: oct]\n"
    "  --chunksize=<value>        Size in bytes of each string segments (bytes per LoC). [default: 200]\n"
    "  --baseclass=<name>         The name of the interface for embedded files. [default: File]\n"
    "  --namespace=<name>         The namespace of the generated source code. [default: bin2cpp]\n"
    "  --reportedfilepath=<path>  The relative reported path of the File. Path returned when calling method getFilePath() of the File class. ie: images" SEPARATOR "DCIM" SEPARATOR "IMG_0001.jpg.\n"
    "                             Automatically calculated when --dir mode is used.\n"
    "  --managerfile=<path>       File name or relative path of the generated C++ header file for the FileManager class. ie: FileManager.h\n"
    "  --registerfile             Register the generated file to the FileManager class.\n"
    "                             This flags is automatically set when parameter 'managerfile' is specified.\n"
    "  --keepdirs                 Keep the directory structure. Forces the output files to have the same\n"
    "                             directory structure as the input files. Valid only when --dir is used.\n"
    "  --plainoutput              Print the encoded string in plain format to stdout. Useful for scripts and integration with third party application.\n"
    "  --override                 Tells bin2cpp to overwrite the destination files.\n"
    "  --noheader                 Do not print program header to standard output.\n"
    "  --quiet                    Do not log any message to standard output.\n"
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
  args.hasReportedFilePath = false;
  args.hasManagerFile = false;
  args.keepDirectoryStructure = false;
  args.chunkSize = 0;
  args.plainOutput = false;
  args.overrideExisting = false;
  args.registerfile = false;

  std::string dummy;

  //help
  args.help = ra::cli::ParseArgument("help", dummy, argc, argv);
  if (args.help)
  {
    printHeader();
    printUsage();
    return APP_ERROR_SUCCESS;
  }

  //noheader
  args.noheader = ra::cli::ParseArgument("noheader", dummy, argc, argv);

  //quiet
  args.quiet = ra::cli::ParseArgument("quiet", dummy, argc, argv);

  //force quiet and noheader if plain output
  args.plainOutput = ra::cli::ParseArgument("plainoutput", dummy, argc, argv);
  if (args.plainOutput)
  {
    args.quiet = true;
    args.noheader = true;
  }

  //force noheader if quiet
  if (args.quiet)
    args.noheader = true;

  ra::logging::SetQuietMode(args.quiet);

  //version
  args.version = ra::cli::ParseArgument("version", dummy, argc, argv);
  if (args.version)
  {
    if (!args.noheader)
      printHeader();
    return APP_ERROR_SUCCESS;
  }

  if (!args.noheader && !args.quiet)
    printHeader();

  //mandatory arguments
  args.hasFile = ra::cli::ParseArgument("file", args.inputFilePath, argc, argv);
  args.hasDir  = ra::cli::ParseArgument("dir",  args.inputDirPath,  argc, argv);
  args.hasManagerFile = ra::cli::ParseArgument("managerfile", args.managerHeaderFilename, argc, argv);

  //if no mandatory args is specified
  if (!args.hasFile && !args.hasDir && !args.hasManagerFile && !args.plainOutput)
  {
    //file, dir, managerfile or plainoutput must be specified
    APP_ERROR_CODES error = APP_ERROR_MISSINGARGUMENTS;
    ra::logging::Log(ra::logging::LOG_ERROR, "%s (file, dir, managerfile, plainoutput)", getErrorCodeDescription(error));
    printUsage();
    return error;
  }
  else if (args.hasFile && args.hasDir)
  {
    //file OR dir must be specified, not both
    APP_ERROR_CODES error = APP_ERROR_TOOMANYARGUMENTS;
    ra::logging::Log(ra::logging::LOG_ERROR, "%s (file, dir)", getErrorCodeDescription(error));
    printUsage();
    return error;
  }

  //if output args is mandatory
  if (args.hasDir || (args.hasFile && !args.plainOutput) || args.hasManagerFile)
  {
    if (!ra::cli::ParseArgument("output", args.outputDirPath, argc, argv))
    {
      APP_ERROR_CODES error = APP_ERROR_MISSINGARGUMENTS;
      ra::logging::Log(ra::logging::LOG_ERROR, "%s (output)", getErrorCodeDescription(error));
      printUsage();
      return error;
    }
  }

  // if headerfile should not be specified 
  if (args.hasDir)
  {
    if (ra::cli::ParseArgument("headerfile", args.headerFilename, argc, argv))
    {
      //headerfile not supported with dir argument
      APP_ERROR_CODES error = APP_ERROR_TOOMANYARGUMENTS;
      ra::logging::Log(ra::logging::LOG_ERROR, "%s (headerfile)", getErrorCodeDescription(error));
      printUsage();
      return error;
    }
  }

  // if identifier should not be specified 
  if (args.hasDir)
  {
    if (ra::cli::ParseArgument("identifier", args.functionIdentifier, argc, argv))
    {
      //identifier not supported with dir argument
      APP_ERROR_CODES error = APP_ERROR_TOOMANYARGUMENTS;
      ra::logging::Log(ra::logging::LOG_ERROR, "%s (identifier)", getErrorCodeDescription(error));
      printUsage();
      return error;
    }
  }

  //optional arguments

  if (args.hasFile)
  {
    //identifier
    if (!ra::cli::ParseArgument("identifier", args.functionIdentifier, argc, argv))
    {
      //identifier is not manually specified.
      args.functionIdentifier = getDefaultFunctionIdentifier(args, identifiers_dictionary);
    }

    //headerfile
    if (!ra::cli::ParseArgument("headerfile", args.headerFilename, argc, argv))
    {
      //use the file name without extension as 'headerfile'.
      args.headerFilename = getDefaultHeaderFile(args);
    }
  }

  size_t tmpChunkSize = 0;
  args.chunkSize = DEFAULT_CHUNK_SIZE;
  if (ra::cli::ParseArgument("chunksize", tmpChunkSize, argc, argv))
  {
    args.chunkSize = tmpChunkSize;
  }

  args.overrideExisting = ra::cli::ParseArgument("override", dummy, argc, argv);

  if (!ra::cli::ParseArgument("namespace", args.codeNamespace, argc, argv))
  {
    args.codeNamespace = DEFAULT_NAMESPACE;
  }

  if (!ra::cli::ParseArgument("baseclass", args.baseClass, argc, argv))
  {
    args.baseClass = DEFAULT_BASECLASSNAME;
  }

  args.registerfile = ra::cli::ParseArgument("registerfile", dummy, argc, argv);
  
  //force registerfile if managerfile is specified
  if (args.hasManagerFile)
  {
    args.registerfile = true;
  }

  args.hasReportedFilePath  = ra::cli::ParseArgument("reportedfilepath",  args.reportedFilePath,  argc, argv);
  if (args.hasReportedFilePath && args.hasDir)
  {
    APP_ERROR_CODES error = APP_ERROR_TOOMANYARGUMENTS;
    ra::logging::Log(ra::logging::LOG_ERROR, "%s (reportedfilepath)", getErrorCodeDescription(error));
    printUsage();
    return error;
  }

  args.keepDirectoryStructure = ra::cli::ParseArgument("keepdirs", dummy, argc, argv);

  std::string encodingStr;
  if (ra::cli::ParseArgument("encoding", encodingStr, argc, argv))
  {
    if (ra::strings::Uppercase(encodingStr) == "OCT")
      args.encoding = IGenerator::CPP_ENCODER_OCT;
    else if (ra::strings::Uppercase(encodingStr) == "HEX")
      args.encoding = IGenerator::CPP_ENCODER_HEX;
    else
    {
      APP_ERROR_CODES error = APP_ERROR_MISSINGARGUMENTS;
      ra::logging::Log(ra::logging::LOG_ERROR, "%s (encoding)", getErrorCodeDescription(error));
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

  if (ra::cli::ParseArgument("generator", args.generatorName, argc, argv))
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
      ra::logging::Log(ra::logging::LOG_ERROR, "%s, unknown values for 'generator' argument!", getErrorCodeDescription(error));
      printUsage();
      return error;
    }
  }

  //win32 generator does not support plain output
  if (args.generatorName == "win32" && args.plainOutput)
  {
    APP_ERROR_CODES error = AAP_ERROR_NOTSUPPORTED;
    ra::logging::Log(ra::logging::LOG_ERROR, "%s.", getErrorCodeDescription(error));
    return error;
  }

  //apply default generator
  if (generator == NULL)
  {
    generator = &segmentGenerator;
  }

  //process file, directory or plain format
  if (args.plainOutput)
  {
    APP_ERROR_CODES error = processPlainOutput(args, generator);
    if (error != APP_ERROR_SUCCESS)
    {
      ra::logging::Log(ra::logging::LOG_ERROR, "%s.", getErrorCodeDescription(error));
      return error;
    }
  }
  else if (args.hasFile)
  {
    APP_ERROR_CODES error = processInputFile(args, generator);
    if (error != APP_ERROR_SUCCESS)
    {
      ra::logging::Log(ra::logging::LOG_ERROR, "%s.", getErrorCodeDescription(error));
      return error;
    }
  }
  else if (args.hasDir)
  {
    APP_ERROR_CODES error = processInputDirectory(args, generator);
    if (error != APP_ERROR_SUCCESS)
    {
      ra::logging::Log(ra::logging::LOG_ERROR, "%s.", getErrorCodeDescription(error));
      return error;
    }
  }

  //should we also generate the FileManager class?
  if (args.hasManagerFile)
  {
    APP_ERROR_CODES error = processManagerFiles(args, generator);
    if (error != APP_ERROR_SUCCESS)
    {
      ra::logging::Log(ra::logging::LOG_ERROR, "%s.", getErrorCodeDescription(error));
      return error;
    }
  }

  return APP_ERROR_SUCCESS;
}

std::string getDefaultFunctionIdentifier(const ARGUMENTS & args, Dictionary & identifiers_dictionary)
{
  std::string output;

  //use the file name without extension as 'identifier'.
  output = getUniqueFunctionIdentifierFromPath(args.inputFilePath.c_str(), identifiers_dictionary);
  output = ra::strings::CapitalizeFirstCharacter(output);

  return output;
}

std::string getDefaultHeaderFile(const ARGUMENTS & args)
{
  std::string output;

  //use the file name without extension as 'headerfile'.
  output = ra::filesystem::GetFilenameWithoutExtension(args.inputFilePath.c_str());
  output += ".h";

  return output;
}

APP_ERROR_CODES processInputFile(const ARGUMENTS & args, bin2cpp::IGenerator * generator)
{
  // printing info
  std::string info;
  info << "Embedding \"" << args.inputFilePath << "\"";
  if (args.chunkSize != DEFAULT_CHUNK_SIZE)
  {
    info << " using chunks of ";
    info << ra::strings::ToString(args.chunkSize);
    info << " bytes";
  }
  if (args.overrideExisting)
    info << " overriding existing files";
  info << "...";
  ra::logging::Log(ra::logging::LOG_INFO, info.c_str());

  //check if input file exists
  if (!ra::filesystem::FileExists(args.inputFilePath.c_str()))
    return APP_ERROR_INPUTFILENOTFOUND;

  ARGUMENTS argsCopy = args;

  //prepare output files path
  std::string cppFilename = argsCopy.headerFilename;
  ra::strings::Replace(cppFilename, ".hpp", ".cpp");
  ra::strings::Replace(cppFilename, ".h", ".cpp");  

  //build unique output relative file paths
  argsCopy.headerFilename = bin2cpp::getUniqueFilePath(argsCopy.headerFilename, output_files_dictionary);
  cppFilename = bin2cpp::getUniqueFilePath(cppFilename, output_files_dictionary);

  //build full absolute paths
  std::string outputHeaderPath = argsCopy.outputDirPath + ra::filesystem::GetPathSeparatorStr() + argsCopy.headerFilename;
  std::string outputCppPath = argsCopy.outputDirPath + ra::filesystem::GetPathSeparatorStr() + cppFilename;

  //configure the generator
  generator->setInputFilePath(argsCopy.inputFilePath.c_str());
  generator->setHeaderFilename(argsCopy.headerFilename.c_str());
  generator->setFunctionIdentifier(argsCopy.functionIdentifier.c_str());
  generator->setReportedFilePath(argsCopy.reportedFilePath.c_str());
  generator->setChunkSize(argsCopy.chunkSize);
  generator->setNamespace(argsCopy.codeNamespace.c_str());
  generator->setBaseClass(argsCopy.baseClass.c_str());
  generator->setCppEncoder(argsCopy.encoding);
  generator->setManagerHeaderFilename(argsCopy.managerHeaderFilename.c_str());
  generator->setRegisterFileEnabled(argsCopy.registerfile);

  //build the output directory structure if required
  if (argsCopy.keepDirectoryStructure)
  {
    std::string parent_directory = ra::filesystem::GetParentPath(outputHeaderPath);
    if (!parent_directory.empty() && !ra::filesystem::DirectoryExists(parent_directory.c_str()))
    {
      ra::logging::Log(ra::logging::LOG_INFO, "Creating directory \"%s\"...", parent_directory.c_str());
      bool success = ra::filesystem::CreateDirectory(parent_directory.c_str());
      if (!success)
      {
        ra::logging::Log(ra::logging::LOG_ERROR, "Failed to create directory \"%s\".", parent_directory.c_str());
        return APP_ERROR_UNABLETOCREATEOUTPUTFILES;
      }
    }
  }

  //process files
  bool headerResult = generateFile(args, outputHeaderPath, generator);
  if (!headerResult)
    return APP_ERROR_UNABLETOCREATEOUTPUTFILES;
  
  bool cppResult =    generateFile(args, outputCppPath, generator);
  if (!cppResult)
    return APP_ERROR_UNABLETOCREATEOUTPUTFILES;

  //success
  return APP_ERROR_SUCCESS;
}

APP_ERROR_CODES processInputDirectory(const ARGUMENTS & args, bin2cpp::IGenerator * generator)
{
  //check if input dir exists
  if (!ra::filesystem::DirectoryExists(args.inputDirPath.c_str()))
  {
    APP_ERROR_CODES error = APP_ERROR_INPUTDIRNOTFOUND;
    ra::logging::Log(ra::logging::LOG_ERROR, "%s (%s)", getErrorCodeDescription(error), args.inputDirPath.c_str());
    return error;
  }

  //search all files in the directory
  ra::strings::StringVector files;
  bool found = ra::filesystem::FindFiles(files, args.inputDirPath.c_str());
  if (!found)
  {
    APP_ERROR_CODES error = APP_ERROR_INPUTDIRNOTFOUND;
    ra::logging::Log(ra::logging::LOG_ERROR, "%s (%s)", getErrorCodeDescription(error), args.inputDirPath.c_str());
    return error;
  }

  //remove directory entries from list
  ra::strings::StringVector tmp;
  for(size_t i=0; i<files.size(); i++)
  {
    const std::string & file = files[i];
    if (ra::filesystem::FileExists(file.c_str()))
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
    argsCopy.hasFile = true;
    argsCopy.inputFilePath = file;

    //use the file name without extension as 'headerfile'.
    argsCopy.headerFilename = getDefaultHeaderFile(argsCopy);

    //use the file name without extension as 'identifier'.
    argsCopy.functionIdentifier = getDefaultFunctionIdentifier(argsCopy, identifiers_dictionary);

    //build a relative file path
    std::string relative_file_path = file;
    relative_file_path.erase(0, argsCopy.inputDirPath.size() + 1 ); // convert absolute path to relative path. +1 to remove first \ character
    ra::filesystem::NormalizePath(relative_file_path);

    //automatically build a reported path with --dir mode.
    argsCopy.hasReportedFilePath = true;
    argsCopy.reportedFilePath = relative_file_path;

    if (args.keepDirectoryStructure)
    {
      // To keep the directory structure, we need to
      // make headerFilename a relative path
      // inside the output directory
      std::string relative_header_file_path = relative_file_path;

      // change the file extension to *.h
      std::string extension = ra::filesystem::GetFileExtention(relative_header_file_path);
      if (relative_header_file_path.size() >= extension.size())
      {
        relative_header_file_path.erase(relative_header_file_path.size() - extension.size());
        relative_header_file_path += "h";
      }

      argsCopy.headerFilename = relative_header_file_path;
    }

    //process this file...
    APP_ERROR_CODES error = processInputFile(argsCopy, generator);
    if (error != APP_ERROR_SUCCESS)
      return error;

    //next file
  }

  //all files processed
  return APP_ERROR_SUCCESS;
}

FILE_UPDATE_MODE getFileUpdateMode(const std::string & input_file_path, const std::string & output_file_path, bool overrideExisting)
{
  if (!ra::filesystem::FileExists(output_file_path.c_str()))
    return WRITING;
  //at this point, we know that the file exists

  if (overrideExisting)
    return OVERWRITING;

  //do not modify the output file if it is not out of date
  uint64_t lastModifiedDate = ra::filesystem::GetFileModifiedDate(input_file_path);
  uint64_t outputModifiedDate = bin2cpp::getOutputFileModifiedDate(output_file_path);
  if (outputModifiedDate == 0)
    ra::logging::Log(ra::logging::LOG_WARNING, "Unable to get last modified date of file \'%s\'", output_file_path.c_str());
  if (lastModifiedDate == outputModifiedDate)
    return SKIPPING;

  //file is out of date, update it
  return UPDATING;
}

bool generateFile(const ARGUMENTS & args, const std::string & output_file_path, bin2cpp::IGenerator * generator)
{
  FILE_UPDATE_MODE mode = getFileUpdateMode(args.inputFilePath, output_file_path, args.overrideExisting);

  //writing message
  ra::logging::Log(ra::logging::LOG_INFO, "%s file \"%s\"...", getUpdateModeText(mode), output_file_path.c_str());
  
  if (mode == SKIPPING)
    return true; //skipping is success

  //generate file
  bool result = false;
  if (isCppHeaderFile(output_file_path))
  {
    //generate header
    result = generator->createCppHeaderFile(output_file_path.c_str());
  }
  else
  {
    //generate cpp
    result = generator->createCppSourceFile(output_file_path.c_str());
  }
  if (!result)
  {
    //there was an error generating file
    ra::logging::Log(ra::logging::LOG_ERROR, "%s", getErrorCodeDescription(APP_ERROR_UNABLETOCREATEOUTPUTFILES));
    ra::logging::Log(ra::logging::LOG_ERROR, "Embedding failed!");
  }
  return result;
}

bool generateManagerFile(const ARGUMENTS & args, const std::string & output_file_path, bin2cpp::IGenerator * generator)
{
  std::string processPath = ra::process::GetCurrentProcessPath();
  FILE_UPDATE_MODE mode = getFileUpdateMode(processPath, output_file_path, args.overrideExisting);

  //writing message
  ra::logging::Log(ra::logging::LOG_INFO, "%s file \"%s\"...", getUpdateModeText(mode), output_file_path.c_str());
  
  if (mode == SKIPPING)
    return true; //skipping is success

  //generate file
  bool result = false;
  if (isCppHeaderFile(output_file_path))
  {
    //generate header
    result = generator->createManagerHeaderFile(output_file_path.c_str());
  }
  else
  {
    //generate cpp
    result = generator->createManagerSourceFile(output_file_path.c_str());
  }
  if (!result)
  {
    //there was an error generating file
    ra::logging::Log(ra::logging::LOG_ERROR, "%s failed!", getUpdateModeText(mode));
  }
  return result;
}

APP_ERROR_CODES processManagerFiles(const ARGUMENTS & args, bin2cpp::IGenerator * generator)
{
  // printing info
  std::string info;
  info << "Generating \"" << args.managerHeaderFilename << "\"";
  if (args.overrideExisting)
    info << " overriding existing files";
  info << "...";
  ra::logging::Log(ra::logging::LOG_INFO, info.c_str());

  //prepare output files path
  std::string cppFilename = args.managerHeaderFilename;
  ra::strings::Replace(cppFilename, ".hpp", ".cpp");
  ra::strings::Replace(cppFilename, ".h", ".cpp");
  std::string outputHeaderPath = args.outputDirPath + ra::filesystem::GetPathSeparatorStr() + args.managerHeaderFilename;
  std::string outputCppPath = args.outputDirPath + ra::filesystem::GetPathSeparatorStr() + cppFilename;

  //process files
  bool headerResult = generateManagerFile(args, outputHeaderPath, generator);
  if (!headerResult)
    return APP_ERROR_UNABLETOCREATEOUTPUTFILES;
  
  bool cppResult =    generateManagerFile(args, outputCppPath, generator);
  if (!cppResult)
    return APP_ERROR_UNABLETOCREATEOUTPUTFILES;

  //success
  return APP_ERROR_SUCCESS;
}

APP_ERROR_CODES processPlainOutput(const ARGUMENTS & args, bin2cpp::IGenerator * generator)
{
  //check if input file exists
  if (!ra::filesystem::FileExists(args.inputFilePath.c_str()))
    return APP_ERROR_INPUTFILENOTFOUND;

  ARGUMENTS argsCopy = args;

  //configure the generator
  generator->setInputFilePath(argsCopy.inputFilePath.c_str());
  generator->setChunkSize(argsCopy.chunkSize);
  generator->setCppEncoder(argsCopy.encoding);

  //process file
  bool result = generator->printFileContent();
  if (!result)
    return APP_ERROR_OPERATIONHASFAILED;

  //success
  return APP_ERROR_SUCCESS;
}
