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
#include "ManagerGenerator.h"
#include "Context.h"
#include "INameProvider.h"
#include "LegacyNameProvider.h"

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
#include "wildcard.h"

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
  APP_ERROR_OPERATIONHASFAILED,
  APP_ERROR_INVALIDVALUE,
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
static const char * DEFAULT_NAMESPACE_CPP = "bin2cpp";
static const char * DEFAULT_NAMESPACE_C = "bin2c";
static const char * DEFAULT_BASECLASS_NAME_CPP = "File";
static const char * DEFAULT_BASECLASS_NAME_C = "Bin2cFile";
static const CppEncoderEnum DEFAULT_ENCODING = CPP_ENCODER_OCT;
static const CodeGenerationEnum DEFAULT_CODE_GENERATION = CODE_GENERATION_CPP;
static Dictionary identifiers_dictionary;   // unique values for identifiers
static Dictionary output_files_dictionary;  // unique values for output file names
#define DIRECTORY_FILTER_SEPARATOR_STR ":"
static const char DIRECTORY_FILTER_SEPARATOR = DIRECTORY_FILTER_SEPARATOR_STR[0];

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
  case APP_ERROR_INVALIDVALUE:
    return "Invalid value";
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
  std::string generatorName;
};

//pre-declarations
bool generateOutputFile(const Context & c, const std::string & output_file_path, bin2cpp::IGenerator * generator);
APP_ERROR_CODES processInputFile(const Context & c, bin2cpp::IGenerator * generator);
APP_ERROR_CODES processInputDirectory(const Context & c, bin2cpp::INameProvider* nameProvider, bin2cpp::IGenerator * generator);
APP_ERROR_CODES processManagerFiles(const Context & c);
APP_ERROR_CODES processPlainOutput(const Context & c, bin2cpp::IGenerator * generator);

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
    "  bin2cpp --dir=<path>  --output=<path> [--keepdirs] [--generator=<name>] [--encoding=<name>] [--chunksize=<value>] [--namespace=<value>] [--baseclass=<name>] [--managerfile=<name>] [--registerfile] [--dirincludefilter=<value>] [--direxcludefilter=<value>] [--override] [--noheader] [--quiet]\n"
    "  bin2cpp --help\n"
    "  bin2cpp --version\n"
    "\n"
    "Options:\n"
    "  --help                         Display this help message.\n"
    "  --version                      Display this application version.\n"
    "  --file=<path>                  Path of the input file used for embedding as C++ source code.\n"
    "  --dir=<path>                   Path of the input directory used for embedding all files of the directory as C++ source code.\n"
    "  --output=<path>                Path of the output directory where to create generated code. ie: ." SEPARATOR "generated_files\n"
    "  --headerfile=<path>            File name or relative path of the generated C++ header file. If a relative path from the output directory is specified,\n"
    "                                 the #include statement in the generated cpp file will match the relative path. ie: SplashScreen.h\n"
    "                                 Default value: input file name (without extension)\n"
    "  --identifier=<name>            Identifier of the function name that is used to get an instance of the file. ie: SplashScreen\n"
    "                                 Default value is based on input file with format 'NameExt'.\n"
    "  --generator=<name>             Name of the generator to use. Possible values are 'segment', 'string', 'array' and 'win32'. [default: segment]\n"
    "  --encoding=<name>              Name of the binary to string literal encoding to use. Possible values are 'oct' and 'hex'. [default: oct]\n"
    "  --chunksize=<value>            Size in bytes of each string segments (bytes per LoC). [default: 200]\n"
    "  --baseclass=<name>             The name of the interface for embedded files. [default: File]\n"
    "                                 For C generated code, this parameter is for naming the File structure. [default: Bin2cFile]\n"
    "  --namespace=<name>             The namespace of the generated source code. [default: bin2cpp]\n"
    "                                 For C generated code, this parameter is for setting the prefix of all function names. [default: bin2c]\n"
    "  --reportedfilepath=<path>      The relative reported path of the File. Path returned when calling method getFilePath() of the File class. ie: images" SEPARATOR "DCIM" SEPARATOR "IMG_0001.jpg.\n"
    "                                 Automatically calculated when --dir mode is used.\n"
    "  --managerfile=<path>           File name or relative path of the generated C++ header file for the FileManager class. ie: FileManager.h\n"
    "  --registerfile                 Register the generated file to the FileManager class.\n"
    "                                 This flags is automatically set when parameter 'managerfile' is specified.\n"
    "  --dirincludefilter=<value>     Set a positive filter on the input directory to only select files matching the filter. Wildcard characters are accepted.\n"
    "                                 Separate each filter with the character '" DIRECTORY_FILTER_SEPARATOR_STR "'. Valid only when --dir is used. See wildcard characters definition below.\n"
    "  --direxcludefilter=<value>     Set a negative filter on the input directory to skip files matching the filter. Wildcard characters are accepted.\n"
    "                                 Separate each filter with the character '" DIRECTORY_FILTER_SEPARATOR_STR "'. Valid only when --dir is used. See wildcard characters definition below.\n"
    "                                 The exclude filter has precedence over the include filter.\n"
    "  --keepdirs                     Keep the directory structure. Forces the output files to have the same\n"
    "                                 directory structure as the input files. Valid only when --dir is used.\n"
    "  --plainoutput                  Print the encoded string in plain format to stdout. Useful for scripts and integration with third party application.\n"
    "  --code                         Define the programming language output for code generation. Supported values are 'c', 'cpp', 'c++'.\n"
    "  --override                     Tells bin2cpp to overwrite the destination files.\n"
    "  --noheader                     Do not print program header to standard output.\n"
    "  --quiet                        Do not log any message to standard output.\n"
    "\n"
    "  Wildcard characters:\n"
    "    '?'            Matches any single character.\n"
    "    '*'            Matches zero or more characters.\n"
    "    '#'            Matches exactly one numeric digit (0-9).\n"
    "    [charlist]     Matches any single character inside the brackets.\n"
    "    [a-z]          Matches any single lowercase letter between 'a' and 'z'.\n"
    "    [A-Z]          Matches any single uppercase letter between 'A' and 'A'.\n"
    "    [0-9]          Matches any single digit between '0' and '9'.\n"
    "    [a-zA-Z0-9]    Matches any single letter (uppercase or lowercase) or digit.\n"
    "\n"
    "    For example:\n"
    "       'ker*##.???' would match files that starts with 'ker', and ends with 2 digits, a dot and then 3 characters."
    "       --dir-include-filter=\"*.jpg:*.png\" would include all files whose file extension is 'jpg' or 'png'.\n"
    "       --dir-exclude-filter=\"*.bak\" would exclude all backup files.\n"
    "       --dir-include-filter=\"*.log\" --dir-exclude-filter=\"debug.log\" would include all log files but not the one named 'debug.log'."
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

  Context c;
  INameProvider& nameProvider = LegacyNameProvider();
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
  c.plainOutput = ra::cli::ParseArgument("plainoutput", dummy, argc, argv);
  if ( c.plainOutput )
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
  c.hasInputFile = ra::cli::ParseArgument("file", c.inputFilePath, argc, argv);
  c.hasInputDir  = ra::cli::ParseArgument("dir",  c.inputDirPath,  argc, argv);
  c.hasManagerFile = ra::cli::ParseArgument("managerfile", c.managerHeaderFilename, argc, argv);

  //if no mandatory args is specified
  if (!c.hasInputFile && !c.hasInputDir && !c.hasManagerFile && !c.plainOutput)
  {
    //file, dir, managerfile or plainoutput must be specified
    APP_ERROR_CODES error = APP_ERROR_MISSINGARGUMENTS;
    ra::logging::Log(ra::logging::LOG_ERROR, "%s (file, dir, managerfile, plainoutput)", getErrorCodeDescription(error));
    printUsage();
    return error;
  }
  else if (c.hasInputFile && c.hasInputDir)
  {
    //file OR dir must be specified, not both
    APP_ERROR_CODES error = APP_ERROR_TOOMANYARGUMENTS;
    ra::logging::Log(ra::logging::LOG_ERROR, "%s (file, dir)", getErrorCodeDescription(error));
    printUsage();
    return error;
  }

  //if output args is mandatory
  if (c.hasInputDir || (c.hasInputFile && !c.plainOutput) || c.hasManagerFile)
  {
    c.hasOutputDir = ra::cli::ParseArgument("output", c.outputDirPath, argc, argv);
    if (!c.hasOutputDir )
    {
      APP_ERROR_CODES error = APP_ERROR_MISSINGARGUMENTS;
      ra::logging::Log(ra::logging::LOG_ERROR, "%s (output)", getErrorCodeDescription(error));
      printUsage();
      return error;
    }
  }

  // if headerfile should not be specified 
  if (c.hasInputDir)
  {
    if (ra::cli::ParseArgument("headerfile", c.headerFilename, argc, argv))
    {
      //headerfile not supported with dir argument
      APP_ERROR_CODES error = APP_ERROR_TOOMANYARGUMENTS;
      ra::logging::Log(ra::logging::LOG_ERROR, "%s (headerfile)", getErrorCodeDescription(error));
      printUsage();
      return error;
    }
  }

  // if identifier should not be specified 
  if (c.hasInputDir)
  {
    if (ra::cli::ParseArgument("identifier", c.functionIdentifier, argc, argv))
    {
      //identifier not supported with dir argument
      APP_ERROR_CODES error = APP_ERROR_TOOMANYARGUMENTS;
      ra::logging::Log(ra::logging::LOG_ERROR, "%s (identifier)", getErrorCodeDescription(error));
      printUsage();
      return error;
    }
  }

  //optional arguments

  std::string codeStr;
  if ( ra::cli::ParseArgument("code", codeStr, argc, argv) )
  {
    CodeGenerationEnum codeTmp = parseCode(codeStr);
    if ( codeTmp == CodeGenerationEnum::CODE_GENERATION_UNKNOW )
    {
      APP_ERROR_CODES error = APP_ERROR_INVALIDVALUE;
      ra::logging::Log(ra::logging::LOG_ERROR, "%s (code)", getErrorCodeDescription(error));
      printUsage();
      return error;
    }

    c.code = codeTmp;
  }
  else
  {
    c.code = DEFAULT_CODE_GENERATION;
  }

  if (c.hasInputFile)
  {
    //identifier
    if (!ra::cli::ParseArgument("identifier", c.functionIdentifier, argc, argv))
    {
      //identifier is not manually specified.
      c.functionIdentifier = nameProvider.getDefaultFunctionIdentifier(c.inputFilePath, identifiers_dictionary);
    }

    //headerfile
    if (!ra::cli::ParseArgument("headerfile", c.headerFilename, argc, argv))
    {
      //use the file name without extension as 'headerfile'.
      c.headerFilename = nameProvider.getDefaultHeaderFile(c.inputFilePath);
    }
  }

  size_t tmpChunkSize = 0;
  c.chunkSize = DEFAULT_CHUNK_SIZE;
  if (ra::cli::ParseArgument("chunksize", tmpChunkSize, argc, argv))
  {
    c.chunkSize = tmpChunkSize;
  }

  c.overrideExistingFiles = ra::cli::ParseArgument("override", dummy, argc, argv);

  if (!ra::cli::ParseArgument("namespace", c.codeNamespace, argc, argv))
  {
    switch ( c.code )
    {
    default:
    case CODE_GENERATION_CPP:
      c.codeNamespace = DEFAULT_NAMESPACE_CPP;
      break;
    case CODE_GENERATION_C:
      c.codeNamespace = DEFAULT_NAMESPACE_C;
      break;
    };

  }

  if (!ra::cli::ParseArgument("baseclass", c.baseClass, argc, argv))
  {
    switch ( c.code )
    {
      default:
      case CODE_GENERATION_CPP:
        c.baseClass = DEFAULT_BASECLASS_NAME_CPP;
        break;
      case CODE_GENERATION_C:
        c.baseClass = DEFAULT_BASECLASS_NAME_C;
        break;
    };
  }

  c.registerFiles = ra::cli::ParseArgument("registerfile", dummy, argc, argv);
  
  // directory include filters
  std::string filter;
  c.hasDirectoryIncludeFilters = ra::cli::ParseArgument("dirincludefilter", filter, argc, argv);
  if ( c.hasDirectoryIncludeFilters )
  {
    strSplit(filter, DIRECTORY_FILTER_SEPARATOR, c.directoryIncludeFilters);
  }

  // directory exclude filters
  c.hasDirectoryExcludeFilters = ra::cli::ParseArgument("direxcludefilter", filter, argc, argv);
  if ( c.hasDirectoryExcludeFilters )
  {
    strSplit(filter, DIRECTORY_FILTER_SEPARATOR, c.directoryExcludeFilters);
  }

  //force registerfile if managerfile is specified
  if (c.hasManagerFile)
  {
    c.registerFiles = true;
  }

  c.hasReportedFilePath  = ra::cli::ParseArgument("reportedfilepath",  c.reportedFilePath,  argc, argv);
  if (c.hasReportedFilePath && c.hasInputDir)
  {
    APP_ERROR_CODES error = APP_ERROR_TOOMANYARGUMENTS;
    ra::logging::Log(ra::logging::LOG_ERROR, "%s (reportedfilepath)", getErrorCodeDescription(error));
    printUsage();
    return error;
  }

  c.keepDirectoryStructure = ra::cli::ParseArgument("keepdirs", dummy, argc, argv);

  std::string encodingStr;
  if (ra::cli::ParseArgument("encoding", encodingStr, argc, argv))
  {
    if (ra::strings::Uppercase(encodingStr) == "OCT")
      c.cppEncoder = CPP_ENCODER_OCT;
    else if (ra::strings::Uppercase(encodingStr) == "HEX")
      c.cppEncoder = CPP_ENCODER_HEX;
    else
    {
      APP_ERROR_CODES error = APP_ERROR_INVALIDVALUE;
      ra::logging::Log(ra::logging::LOG_ERROR, "%s (encoding)", getErrorCodeDescription(error));
      printUsage();
      return error;
    }
  }
  else
  {
    c.cppEncoder = DEFAULT_ENCODING;
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
  if (args.generatorName == "win32" && c.plainOutput)
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
  if (c.plainOutput)
  {
    APP_ERROR_CODES error = processPlainOutput(c, generator);
    if (error != APP_ERROR_SUCCESS)
    {
      ra::logging::Log(ra::logging::LOG_ERROR, "%s.", getErrorCodeDescription(error));
      return error;
    }
  }
  else if (c.hasInputFile)
  {
    APP_ERROR_CODES error = processInputFile(c, generator);
    if (error != APP_ERROR_SUCCESS)
    {
      ra::logging::Log(ra::logging::LOG_ERROR, "%s.", getErrorCodeDescription(error));
      return error;
    }
  }
  else if (c.hasInputDir)
  {
    APP_ERROR_CODES error = processInputDirectory(c, &nameProvider, generator);
    if (error != APP_ERROR_SUCCESS)
    {
      ra::logging::Log(ra::logging::LOG_ERROR, "%s.", getErrorCodeDescription(error));
      return error;
    }
  }

  //should we also generate the FileManager class?
  if (c.hasManagerFile)
  {
    APP_ERROR_CODES error = processManagerFiles(c);
    if (error != APP_ERROR_SUCCESS)
    {
      ra::logging::Log(ra::logging::LOG_ERROR, "%s.", getErrorCodeDescription(error));
      return error;
    }
  }

  return APP_ERROR_SUCCESS;
}

APP_ERROR_CODES processInputFile(const Context & c, bin2cpp::IGenerator * generator)
{
  // printing info
  std::string info;
  info << "Embedding \"" << c.inputFilePath << "\"";
  if (c.chunkSize != DEFAULT_CHUNK_SIZE)
  {
    info << " using chunks of ";
    info << ra::strings::ToString(c.chunkSize);
    info << " bytes";
  }
  if (c.overrideExistingFiles)
    info << " overriding existing files";
  info << "...";
  ra::logging::Log(ra::logging::LOG_INFO, info.c_str());

  //check if input file exists
  if (!ra::filesystem::FileExists(c.inputFilePath.c_str()))
    return APP_ERROR_INPUTFILENOTFOUND;

  //prepare output files path
  const std::string   headerExtention = ra::filesystem::GetFileExtention(c.headerFilename);
  const std::string & sourceExtension = getDefaultCodeSourceFileExtension(c.code);
  std::string sourceFilename = c.headerFilename.substr(0, c.headerFilename.size() - headerExtention.size()) + sourceExtension; // strip out header file's extension and add 'cpp'.

  //create a copy of the context.
  //we may have already generated files from a previous call to processInputFile().
  //make sure the file paths are unique.
  Context cCopy = c;

  //build unique output relative file paths
  cCopy.headerFilename = bin2cpp::getUniqueFilePath(cCopy.headerFilename, output_files_dictionary);
  sourceFilename = bin2cpp::getUniqueFilePath(sourceFilename, output_files_dictionary);

  //build full absolute paths
  std::string outputHeaderPath = cCopy.outputDirPath + ra::filesystem::GetPathSeparatorStr() + cCopy.headerFilename;
  std::string outputSourcePath = cCopy.outputDirPath + ra::filesystem::GetPathSeparatorStr() + sourceFilename;

  //configure the generator
  generator->setContext(cCopy);

  //build the output directory structure if required
  if (cCopy.keepDirectoryStructure)
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
  bool headerResult = generateOutputFile(c, outputHeaderPath, generator);
  if (!headerResult)
    return APP_ERROR_UNABLETOCREATEOUTPUTFILES;
  
  bool cppResult =    generateOutputFile(c, outputSourcePath, generator);
  if (!cppResult)
    return APP_ERROR_UNABLETOCREATEOUTPUTFILES;

  //success
  return APP_ERROR_SUCCESS;
}

APP_ERROR_CODES processInputDirectory(const Context& c, bin2cpp::INameProvider * nameProvider, bin2cpp::IGenerator * generator)
{
  //check if input dir exists
  if (!ra::filesystem::DirectoryExists(c.inputDirPath.c_str()))
  {
    APP_ERROR_CODES error = APP_ERROR_INPUTDIRNOTFOUND;
    ra::logging::Log(ra::logging::LOG_ERROR, "%s (%s)", getErrorCodeDescription(error), c.inputDirPath.c_str());
    return error;
  }

  //search all files in the directory
  ra::strings::StringVector files;
  bool found = ra::filesystem::FindFiles(files, c.inputDirPath.c_str());
  if (!found)
  {
    APP_ERROR_CODES error = APP_ERROR_INPUTDIRNOTFOUND;
    ra::logging::Log(ra::logging::LOG_ERROR, "%s (%s)", getErrorCodeDescription(error), c.inputDirPath.c_str());
    return error;
  }

  //remove directory entries from list
  ra::strings::StringVector tmp;
  for(size_t i=0; i<files.size(); i++)
  {
    const std::string & file = files[i];

    // entry is not a file
    if ( !ra::filesystem::FileExists(file.c_str()) )
      continue;

    // should we exclude the file?
    if ( c.hasDirectoryExcludeFilters )
    {
      bool matches_any = bin2cpp::wildcard_match_any(file, c.directoryExcludeFilters);
      if ( matches_any )
        continue; // force exclude this file.
    }

    // should we include the file?
    if ( c.hasDirectoryIncludeFilters )
    {
      bool matches_any = bin2cpp::wildcard_match_any(file, c.directoryIncludeFilters);
      if ( !matches_any )
        continue; // force not include this file.
    }

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
    Context cCopy = c;

    //replace 'dir' input by current file input
    cCopy.hasInputDir = false;
    cCopy.hasInputFile = true;
    cCopy.inputFilePath = file;

    //use the file name without extension as 'headerfile'.
    cCopy.headerFilename = nameProvider->getDefaultHeaderFile(cCopy.inputFilePath);

    //use the file name without extension as 'identifier'.
    cCopy.functionIdentifier = nameProvider->getDefaultFunctionIdentifier(cCopy.inputFilePath, identifiers_dictionary);

    //build a relative file path
    std::string relative_file_path = file;
    relative_file_path.erase(0, cCopy.inputDirPath.size() + 1 ); // convert absolute path to relative path. +1 to remove first \ character
    ra::filesystem::NormalizePath(relative_file_path);

    //automatically build a reported path with --dir mode.
    cCopy.hasReportedFilePath = true;
    cCopy.reportedFilePath = relative_file_path;

    if (c.keepDirectoryStructure)
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

      cCopy.headerFilename = relative_header_file_path;
    }

    //process this file...
    APP_ERROR_CODES error = processInputFile(cCopy, generator);
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

bool generateOutputFile(const Context & c, const std::string & output_file_path, bin2cpp::IGenerator * generator)
{
  FILE_UPDATE_MODE mode = getFileUpdateMode(c.inputFilePath, output_file_path, c.overrideExistingFiles);

  //writing message
  ra::logging::Log(ra::logging::LOG_INFO, "%s file \"%s\"...", getUpdateModeText(mode), output_file_path.c_str());
  
  if (mode == SKIPPING)
    return true; //skipping is success

  //generate file
  bool result = false;
  if (c.code == CODE_GENERATION_CPP && isCppHeaderFile(output_file_path))
  {
    //generate C++ header
    result = generator->createCppHeaderFile(output_file_path.c_str());
  }
  else if ( c.code == CODE_GENERATION_CPP)
  {
    //generate C++ source
    result = generator->createCppSourceFile(output_file_path.c_str());
  }
  else if ( c.code == CODE_GENERATION_C && isCHeaderFile(output_file_path) )
  {
    //generate C header
    result = generator->createCHeaderFile(output_file_path.c_str());
  }
  else if ( c.code == CODE_GENERATION_C )
  {
    //generate C source
    result = generator->createCSourceFile(output_file_path.c_str());
  }
  if (!result)
  {
    //there was an error generating file
    ra::logging::Log(ra::logging::LOG_ERROR, "%s", getErrorCodeDescription(APP_ERROR_UNABLETOCREATEOUTPUTFILES));
    ra::logging::Log(ra::logging::LOG_ERROR, "Embedding failed!");
  }
  return result;
}

APP_ERROR_CODES processManagerFiles(const Context & c)
{
  // printing info
  std::string info;
  info << "Generating \"" << c.managerHeaderFilename << "\"";
  if (c.overrideExistingFiles)
    info << " overriding existing files";
  info << "...";
  ra::logging::Log(ra::logging::LOG_INFO, info.c_str());

  const std::string& sourceFileExtension = "." + getDefaultCodeSourceFileExtension(c.code);

  //prepare output files path
  std::string cppFilename = c.managerHeaderFilename;
  ra::strings::Replace(cppFilename, ".hpp", sourceFileExtension);
  ra::strings::Replace(cppFilename, ".h", sourceFileExtension);
  std::string outputHeaderPath = c.outputDirPath + ra::filesystem::GetPathSeparatorStr() + c.managerHeaderFilename;
  std::string outputSourcePath = c.outputDirPath + ra::filesystem::GetPathSeparatorStr() + cppFilename;

  ManagerGenerator generator;

  //configure the generator
  generator.setContext(c);

  //process files
  bool headerResult = generateOutputFile(c, outputHeaderPath, &generator);
  if (!headerResult)
    return APP_ERROR_UNABLETOCREATEOUTPUTFILES;
  
  bool cppResult =    generateOutputFile(c, outputSourcePath, &generator);
  if (!cppResult)
    return APP_ERROR_UNABLETOCREATEOUTPUTFILES;

  //success
  return APP_ERROR_SUCCESS;
}

APP_ERROR_CODES processPlainOutput(const Context & c, bin2cpp::IGenerator * generator)
{
  //check if input file exists
  if (!ra::filesystem::FileExists(c.inputFilePath.c_str()))
    return APP_ERROR_INPUTFILENOTFOUND;

  //configure the generator
  generator->setContext(c);

  //process file
  bool result = generator->printFileContent();
  if (!result)
    return APP_ERROR_OPERATIONHASFAILED;

  //success
  return APP_ERROR_SUCCESS;
}
