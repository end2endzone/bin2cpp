#include "TestCLI.h"
#include "gtesthelper.h"
#include "application.h"

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
