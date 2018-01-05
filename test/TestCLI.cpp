#include "TestCLI.h"
#include "gtesthelper.h"
#include "application.h"
#include "errorcodes.h"
#include "common.h"

#define ASSERT_TEXT_IN_FILE(expected, filepath, text) {\
  int line = -1;  int col = -1;  bool textFound = gTestHelper::getInstance().findInFile(filepath, text, line, col);\
  if ((expected)) {\
    ASSERT_TRUE(textFound) << "The token '" << text << "' was NOT found in file '" << filepath << "'.";\
  } else {\
    ASSERT_FALSE(textFound) << "The token '" << text << "' was found in file '" << filepath << "' at line " << line << " column " << col << ".";\
  }\
}

namespace TestCLIUtils
{
  gTestHelper & hlp = gTestHelper::getInstance();

  std::string getExpectedFilePath()
  {
    std::string file;
    file.append(hlp.getTestQualifiedName());
    file.append(".expected.txt");
    return file;
  }

  std::string getActualFilePath()
  {
    std::string file;
    file.append(hlp.getTestQualifiedName());
    file.append(".actual.txt");
    return file;
  }

  bool createDummyFile(const char * iPath)
  {
    FILE * f = fopen(iPath, "w");
    if (!f)
      return false;
    fputs("foobar", f);
    fclose(f);
    return true;
  }
}
using namespace TestCLIUtils;

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
  if(from.empty())
    return;
  size_t start_pos = 0;
  while((start_pos = str.find(from, start_pos)) != std::string::npos)
  {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
  }
}

bool deleteFile(const char * iPath)
{
  if (hlp.fileExists(iPath))
  {
    bool deleted = remove(iPath) == 0;
    return deleted;
  }
  return true; //file does not exists. Success.
}

std::string getBin2cppPath()
{
  Application & app = Application::getInstance();
  const Application::StringVector & arguments = app.getArguments();
  const std::string & appPath = arguments[0];

  //replace bin2cpp_unittest by bin2cpp
  std::string bin2cppPath = appPath;
  replaceAll(bin2cppPath, "_unittest", "");
  return bin2cppPath;
}

void TestCLI::SetUp()
{
}

void TestCLI::TearDown()
{
}

TEST_F(TestCLI, testVersion)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" --version");
  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());

  //run the command
  int returnCode = system(cmdline.c_str());
  ASSERT_EQ(0, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;

  //load output file
  gTestHelper::StringVector lines;
  bool loaded = hlp.getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);

  //assert standard output log
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Copyright (C)");
  ASSERT_TEXT_IN_FILE(false, outputFilePath.c_str(), "Usage:");
}

TEST_F(TestCLI, testHelp)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" --help");
  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());

  //run the command
  int returnCode = system(cmdline.c_str());
  ASSERT_EQ(0, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;

  //load output file
  gTestHelper::StringVector lines;
  bool loaded = hlp.getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);

  //assert standard output log
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Copyright (C)");
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Usage:");
}

TEST_F(TestCLI, testNoArguments)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());

  //run the command
  int returnCode = system(cmdline.c_str());
  ASSERT_NE(0, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;

  //load output file
  gTestHelper::StringVector lines;
  bool loaded = hlp.getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);

  //assert standard output log
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Copyright (C)");
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Usage:");
}

TEST_F(TestCLI, testMinimum)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  std::string headerFileName = std::string("_") + hlp.getTestCaseName().c_str() + ".h";
  std::string headerFilePath = std::string("generated_files\\") + headerFileName;
  std::string cppFilePath = headerFilePath; replaceAll(cppFilePath, ".h", ".cpp");

  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" --file=");
  cmdline.append(getBin2cppPath()); //itself
  cmdline.append(" --output=generated_files");
  cmdline.append(" --headerfile=");
  cmdline.append(headerFileName);
  cmdline.append(" --identifier=");
  cmdline.append(hlp.getTestCaseName().c_str());

  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());

  //delete generated files
  ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
  ASSERT_TRUE(deleteFile(cppFilePath.c_str()));

  //run the command
  int returnCode = system(cmdline.c_str());
  ASSERT_EQ(0, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;

  //load output file
  gTestHelper::StringVector lines;
  bool loaded = hlp.getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);

  //assert standard output log
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Copyright (C)");
  ASSERT_TEXT_IN_FILE(false, outputFilePath.c_str(), "Usage:");

  //assert generated code
  ASSERT_TRUE(hlp.fileExists(headerFilePath.c_str()));
}

TEST_F(TestCLI, testQuiet)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  std::string headerFileName = std::string("_") + hlp.getTestCaseName().c_str() + ".h";
  std::string headerFilePath = std::string("generated_files\\") + headerFileName;
  std::string cppFilePath = headerFilePath; replaceAll(cppFilePath, ".h", ".cpp");

  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" --file=");
  cmdline.append(getBin2cppPath()); //itself
  cmdline.append(" --output=generated_files");
  cmdline.append(" --headerfile=");
  cmdline.append(headerFileName);
  cmdline.append(" --identifier=");
  cmdline.append(hlp.getTestCaseName().c_str());
  cmdline.append(" --quiet");

  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());

  //delete generated files
  ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
  ASSERT_TRUE(deleteFile(cppFilePath.c_str()));

  //run the command
  int returnCode = system(cmdline.c_str());
  ASSERT_EQ(0, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;

  //load output file
  gTestHelper::StringVector lines;
  bool loaded = hlp.getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);

  //assert standard output log
  ASSERT_TEXT_IN_FILE(false, outputFilePath.c_str(), "Copyright (C)");
  ASSERT_TEXT_IN_FILE(false, outputFilePath.c_str(), "Usage:");
  ASSERT_EQ(0, hlp.getFileSize(outputFilePath.c_str()));

  //assert generated code
  ASSERT_TRUE(hlp.fileExists(headerFilePath.c_str()));
}

TEST_F(TestCLI, testNoHeader)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  std::string headerFileName = std::string("_") + hlp.getTestCaseName().c_str() + ".h";
  std::string headerFilePath = std::string("generated_files\\") + headerFileName;
  std::string cppFilePath = headerFilePath; replaceAll(cppFilePath, ".h", ".cpp");

  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" --file=");
  cmdline.append(getBin2cppPath()); //itself
  cmdline.append(" --output=generated_files");
  cmdline.append(" --headerfile=");
  cmdline.append(headerFileName);
  cmdline.append(" --identifier=");
  cmdline.append(hlp.getTestCaseName().c_str());
  cmdline.append(" --noheader");

  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());

  //delete generated files
  ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
  ASSERT_TRUE(deleteFile(cppFilePath.c_str()));

  //run the command
  int returnCode = system(cmdline.c_str());
  ASSERT_EQ(0, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;

  //load output file
  gTestHelper::StringVector lines;
  bool loaded = hlp.getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);

  //assert standard output log
  ASSERT_TEXT_IN_FILE(false, outputFilePath.c_str(), "Copyright (C)");
  ASSERT_TEXT_IN_FILE(false, outputFilePath.c_str(), "Usage:");

  //assert generated code
  ASSERT_TRUE(hlp.fileExists(headerFilePath.c_str()));
}

TEST_F(TestCLI, testGenerators)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  //define all command line generators
  std::vector<std::string> generators;
  generators.push_back("segment");
  generators.push_back("string");
  generators.push_back("array");

  //generate cpp output for each generators
  std::vector<std::string> files;
  for(size_t genIndex = 0; genIndex<generators.size(); genIndex++)
  {
    const char * generatorName = generators[genIndex].c_str();

    std::string headerFileName = std::string("_") + hlp.getTestCaseName().c_str() + "." + generatorName + ".h";
    std::string headerFilePath = std::string("generated_files\\") + headerFileName;
    std::string cppFilePath = headerFilePath; replaceAll(cppFilePath, ".h", ".cpp");

    //build command line
    std::string cmdline;
    cmdline.append(getBin2cppPath());
    cmdline.append(" --file=");
    cmdline.append(getBin2cppPath()); //itself
    cmdline.append(" --output=generated_files");
    cmdline.append(" --headerfile=");
    cmdline.append(headerFileName);
    cmdline.append(" --identifier=");
    cmdline.append(hlp.getTestCaseName().c_str());
    cmdline.append(" --generator=");
    cmdline.append(generatorName);

    cmdline.append(" >");
    cmdline.append(outputFilePath.c_str());

    //delete generated files
    ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
    ASSERT_TRUE(deleteFile(cppFilePath.c_str()));

    //run the command
    int returnCode = system(cmdline.c_str());
    ASSERT_EQ(0, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;

    //remember this source file
    files.push_back(cppFilePath);
  }

  //assert all files different
  if (files.size() >= 2)
  {
    for(size_t i=0; i<files.size()-1; i++)
    {
      for(size_t j=1; j<files.size(); j++)
      {
        const std::string & fileA = files[i];
        const std::string & fileB = files[j];

        if (fileA != fileB)
        {
          std::string reason;
          bool isSourceCodeIdentical = hlp.isFileEquals(fileA.c_str(), fileB.c_str(), reason);
          ASSERT_FALSE( isSourceCodeIdentical ) << reason.c_str();
        }
      }
    }
  }
}

TEST_F(TestCLI, testOverride)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  std::string headerFileName = std::string("_") + hlp.getTestCaseName().c_str() + ".h";
  std::string headerFilePath = std::string("generated_files\\") + headerFileName;
  std::string cppFilePath = headerFilePath; replaceAll(cppFilePath, ".h", ".cpp");

  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" --file=");
  cmdline.append(getBin2cppPath()); //itself
  cmdline.append(" --output=generated_files");
  cmdline.append(" --headerfile=");
  cmdline.append(headerFileName);
  cmdline.append(" --identifier=");
  cmdline.append(hlp.getTestCaseName().c_str());

  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());

  //delete generated files
  ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
  ASSERT_TRUE(deleteFile(cppFilePath.c_str()));

  //run the command
  int returnCode = system(cmdline.c_str());
  ASSERT_EQ(0, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;

  //delete cpp file (header file still exits)
  ASSERT_TRUE(deleteFile(cppFilePath.c_str()));

  //run the command (again, expecting header file already exists)
  returnCode = system(cmdline.c_str());
  ASSERT_EQ(bin2cpp::ErrorCodes::OutputFileAlreadyExist, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;

  //create dummy haeder & cpp file
  ASSERT_TRUE(createDummyFile(headerFilePath.c_str()));
  ASSERT_TRUE(createDummyFile(cppFilePath.c_str()));

  //run the command (again, expecting both files already exists)
  returnCode = system(cmdline.c_str());
  ASSERT_EQ(bin2cpp::ErrorCodes::OutputFileAlreadyExist, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;

  //add the override flag
  bin2cpp::strReplace(cmdline, "--file", "--override --file");

  //run the command (again, expecting overriding both files)
  returnCode = system(cmdline.c_str());
  ASSERT_EQ(0, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;
  ASSERT_TEXT_IN_FILE(false, outputFilePath.c_str(), bin2cpp::getErrorCodeDescription(bin2cpp::ErrorCodes::OutputFileAlreadyExist))
  ASSERT_TEXT_IN_FILE(false, outputFilePath.c_str(), bin2cpp::getErrorCodeDescription(bin2cpp::ErrorCodes::OutputFilesSkipped))

  //run the command (again, expecting skipping files)
  returnCode = system(cmdline.c_str());
  ASSERT_EQ(0, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), bin2cpp::getErrorCodeDescription(bin2cpp::ErrorCodes::OutputFilesSkipped))
}
