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
  ASSERT_EQ(0, returnCode);

  //load output file
  gTestHelper::StringVector lines;
  bool loaded = hlp.getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);

  //assert Copyright header is found
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Copyright (C)");
  
  //assert Usage: is NOT found
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
  ASSERT_EQ(0, returnCode);

  //load output file
  gTestHelper::StringVector lines;
  bool loaded = hlp.getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);

  //assert Copyright header is found
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Copyright (C)");

  //assert Usage: is found
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
  ASSERT_NE(0, returnCode);

  //load output file
  gTestHelper::StringVector lines;
  bool loaded = hlp.getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);

  //assert Copyright header is found
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Copyright (C)");

  //assert Usage: is found
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Usage:");
}

TEST_F(TestCLI, testNoHeader)
{
}
