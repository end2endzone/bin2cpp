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

#include "TestCLI.h"
#include "application.h"

#include "rapidassist/gtesthelp.h"
#include "rapidassist/filesystem.h"
#include "rapidassist/strings.h"
#include "rapidassist/time_.h"

extern const std::string & gGeneratedFilesDir;
#ifdef _WIN32
  const std::string & gGeneratedFilesDir = "generated_files\\";
#else
  const std::string & gGeneratedFilesDir = "generated_files/";
#endif
  
#define ASSERT_TEXT_IN_FILE(expected, filepath, text) {\
  int line = -1;  int col = -1;  bool textFound = ra::gtesthelp::findInFile(filepath, text, line, col);\
  if ((expected)) {\
    ASSERT_TRUE(textFound) << "The token '" << text << "' was NOT found in file '" << filepath << "'.";\
  } else {\
    ASSERT_FALSE(textFound) << "The token '" << text << "' was found in file '" << filepath << "' at line " << line << " column " << col << ".";\
  }\
}

enum APP_ERROR_CODES
{
  APP_ERROR_SUCCESS,
  APP_ERROR_MISSINGARGUMENTS,
  APP_ERROR_INPUTFILENOTFOUND,
  APP_ERROR_UNABLETOCREATEOUTPUTFILES,
  APP_ERROR_TOOMANYARGUMENTS,
  APP_ERROR_INPUTDIRNOTFOUND
};

namespace TestCLIUtils
{
  std::string getExpectedFilePath()
  {
    std::string file;
    file.append(ra::gtesthelp::getTestQualifiedName());
    file.append(".expected.txt");
    return file;
  }

  std::string getActualFilePath()
  {
    std::string file;
    file.append(ra::gtesthelp::getTestQualifiedName());
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

  bool createDummyFile(const std::string & iPath, int size)
  {
    FILE * f = fopen(iPath.c_str(), "w");
    if (!f)
      return false;
    static const int BUFFER_SIZE = 255;
    char buffer[BUFFER_SIZE];
    for(int i=0; i<BUFFER_SIZE; i++)
    {
      buffer[i] = (i*23+67)%(BUFFER_SIZE+1);
    }
    int numBlocks = size/BUFFER_SIZE;
    for(int i=0; i<numBlocks; i++)
    {
      fwrite(buffer, 1, BUFFER_SIZE, f);
    }
    fclose(f);
    return true;
  }

  bool deleteFile(const char * iPath)
  {
    if (ra::filesystem::fileExists(iPath))
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
    ra::strings::replace(bin2cppPath, "_unittest", "");
    return bin2cppPath;
  }

}
using namespace TestCLIUtils;

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
  ra::strings::StringVector lines;
  bool loaded = ra::gtesthelp::getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);

  //assert standard output log
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Copyright (C)");
  ASSERT_TEXT_IN_FILE(false, outputFilePath.c_str(), "Usage:");

  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
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
  ra::strings::StringVector lines;
  bool loaded = ra::gtesthelp::getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);

  //assert standard output log
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Copyright (C)");
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Usage:");

  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
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
  ra::strings::StringVector lines;
  bool loaded = ra::gtesthelp::getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);

  //assert standard output log
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Copyright (C)");
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Usage:");

  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
}

TEST_F(TestCLI, testMinimum)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  std::string headerFileName = std::string("_") + ra::gtesthelp::getTestCaseName().c_str() + ".h";
  std::string headerFilePath = gGeneratedFilesDir + headerFileName;
  std::string cppFilePath = headerFilePath; ra::strings::replace(cppFilePath, ".h", ".cpp");

  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" --file=");
  cmdline.append(getBin2cppPath()); //itself
  cmdline.append(" --output=generated_files");
  cmdline.append(" --headerfile=");
  cmdline.append(headerFileName);
  cmdline.append(" --identifier=");
  cmdline.append(ra::gtesthelp::getTestCaseName().c_str());

  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());

  //delete generated files
  ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
  ASSERT_TRUE(deleteFile(cppFilePath.c_str()));

  //run the command
  int returnCode = system(cmdline.c_str());
  ASSERT_EQ(0, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;

  //load output file
  ra::strings::StringVector lines;
  bool loaded = ra::gtesthelp::getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);

  //assert standard output log
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Copyright (C)");
  ASSERT_TEXT_IN_FILE(false, outputFilePath.c_str(), "Usage:");

  //assert generated code
  ASSERT_TRUE(ra::filesystem::fileExists(headerFilePath.c_str()));

  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
}

TEST_F(TestCLI, testQuiet)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  std::string headerFileName = std::string("_") + ra::gtesthelp::getTestCaseName().c_str() + ".h";
  std::string headerFilePath = gGeneratedFilesDir + headerFileName;
  std::string cppFilePath = headerFilePath; ra::strings::replace(cppFilePath, ".h", ".cpp");

  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" --file=");
  cmdline.append(getBin2cppPath()); //itself
  cmdline.append(" --output=generated_files");
  cmdline.append(" --headerfile=");
  cmdline.append(headerFileName);
  cmdline.append(" --identifier=");
  cmdline.append(ra::gtesthelp::getTestCaseName().c_str());
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
  ra::strings::StringVector lines;
  bool loaded = ra::gtesthelp::getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);

  //assert standard output log
  ASSERT_TEXT_IN_FILE(false, outputFilePath.c_str(), "Copyright (C)");
  ASSERT_TEXT_IN_FILE(false, outputFilePath.c_str(), "Usage:");
  ASSERT_EQ(0, ra::filesystem::getFileSize(outputFilePath.c_str()));

  //assert generated code
  ASSERT_TRUE(ra::filesystem::fileExists(headerFilePath.c_str()));

  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
}

TEST_F(TestCLI, testNoHeader)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  std::string headerFileName = std::string("_") + ra::gtesthelp::getTestCaseName().c_str() + ".h";
  std::string headerFilePath = gGeneratedFilesDir + headerFileName;
  std::string cppFilePath = headerFilePath; ra::strings::replace(cppFilePath, ".h", ".cpp");

  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" --file=");
  cmdline.append(getBin2cppPath()); //itself
  cmdline.append(" --output=generated_files");
  cmdline.append(" --headerfile=");
  cmdline.append(headerFileName);
  cmdline.append(" --identifier=");
  cmdline.append(ra::gtesthelp::getTestCaseName().c_str());
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
  ra::strings::StringVector lines;
  bool loaded = ra::gtesthelp::getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);

  //assert standard output log
  ASSERT_TEXT_IN_FILE(false, outputFilePath.c_str(), "Copyright (C)");
  ASSERT_TEXT_IN_FILE(false, outputFilePath.c_str(), "Usage:");

  //assert generated code
  ASSERT_TRUE(ra::filesystem::fileExists(headerFilePath.c_str()));

  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
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
  generators.push_back("win32");

  //generate cpp output for each generators
  std::vector<std::string> files;
  for(size_t genIndex = 0; genIndex<generators.size(); genIndex++)
  {
    const char * generatorName = generators[genIndex].c_str();

    printf("Testing '%s' generator...\n", generatorName);

    std::string headerFileName = std::string("_") + ra::gtesthelp::getTestCaseName().c_str() + "." + generatorName + ".h";
    std::string headerFilePath = gGeneratedFilesDir + headerFileName;
    std::string cppFilePath = headerFilePath; ra::strings::replace(cppFilePath, ".h", ".cpp");

    //build command line
    std::string cmdline;
    cmdline.append(getBin2cppPath());
    cmdline.append(" --file=");
    cmdline.append(getBin2cppPath()); //itself
    cmdline.append(" --output=generated_files");
    cmdline.append(" --headerfile=");
    cmdline.append(headerFileName);
    cmdline.append(" --identifier=");
    cmdline.append(ra::gtesthelp::getTestCaseName().c_str());
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
          bool isSourceCodeIdentical = ra::gtesthelp::isFileEquals(fileA.c_str(), fileB.c_str(), reason);
          ASSERT_FALSE( isSourceCodeIdentical ) << reason.c_str();
        }
      }
    }
  }

  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
}

TEST_F(TestCLI, testNewFiles)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  std::string headerFileName = std::string("_") + ra::gtesthelp::getTestCaseName().c_str() + ".h";
  std::string headerFilePath = gGeneratedFilesDir + headerFileName;
  std::string cppFilePath = headerFilePath; ra::strings::replace(cppFilePath, ".h", ".cpp");
  std::string inputFilePath = gGeneratedFilesDir + ra::gtesthelp::getTestCaseName() + ".bin";

  createDummyFile(inputFilePath, 12345);

  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" --file=");
  cmdline.append(inputFilePath);
  cmdline.append(" --output=generated_files");
  cmdline.append(" --headerfile=");
  cmdline.append(headerFileName);
  cmdline.append(" --identifier=");
  cmdline.append(ra::gtesthelp::getTestCaseName().c_str());

  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());

  //delete generated files
  ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
  ASSERT_TRUE(deleteFile(cppFilePath.c_str()));

  //run the command
  int returnCode = system(cmdline.c_str());
  ASSERT_EQ(0, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Writing file");

  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
}

TEST_F(TestCLI, testUpdating)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  std::string headerFileName = std::string("_") + ra::gtesthelp::getTestCaseName().c_str() + ".h";
  std::string headerFilePath = gGeneratedFilesDir + headerFileName;
  std::string cppFilePath = headerFilePath; ra::strings::replace(cppFilePath, ".h", ".cpp");
  std::string inputFilePath = gGeneratedFilesDir + ra::gtesthelp::getTestCaseName() + ".bin";

  createDummyFile(inputFilePath, 12345);

  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" --file=");
  cmdline.append(inputFilePath);
  cmdline.append(" --output=generated_files");
  cmdline.append(" --headerfile=");
  cmdline.append(headerFileName);
  cmdline.append(" --identifier=");
  cmdline.append(ra::gtesthelp::getTestCaseName().c_str());

  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());

  //delete generated files
  ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
  ASSERT_TRUE(deleteFile(cppFilePath.c_str()));

  //run the command
  int returnCode = system(cmdline.c_str());
  ASSERT_EQ(0, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;

  //force a change in the input file
  //which makes the generated files outdated
  ra::time::millisleep(1200);
  createDummyFile(inputFilePath, 1111);

  //run the command (again)
  returnCode = system(cmdline.c_str());
  ASSERT_EQ(0, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;
  
  //look for the expected message
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Updating file");

  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
}

TEST_F(TestCLI, testOverride)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  std::string headerFileName = std::string("_") + ra::gtesthelp::getTestCaseName().c_str() + ".h";
  std::string headerFilePath = gGeneratedFilesDir + headerFileName;
  std::string cppFilePath = headerFilePath; ra::strings::replace(cppFilePath, ".h", ".cpp");
  std::string inputFilePath = gGeneratedFilesDir + ra::gtesthelp::getTestCaseName() + ".bin";

  createDummyFile(inputFilePath, 12345);

  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" --file=");
  cmdline.append(inputFilePath);
  cmdline.append(" --output=generated_files");
  cmdline.append(" --headerfile=");
  cmdline.append(headerFileName);
  cmdline.append(" --identifier=");
  cmdline.append(ra::gtesthelp::getTestCaseName().c_str());

  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());

  //delete generated files
  ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
  ASSERT_TRUE(deleteFile(cppFilePath.c_str()));

  //run the command
  int returnCode = system(cmdline.c_str());
  ASSERT_EQ(0, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;

  //add the override flag
  ra::strings::replace(cmdline, "--file", "--override --file");

  //run the command (again, expecting overriding both files)
  returnCode = system(cmdline.c_str());
  ASSERT_EQ(0, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Overwriting file");

  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
}

TEST_F(TestCLI, testSkipping)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  std::string headerFileName = std::string("_") + ra::gtesthelp::getTestCaseName().c_str() + ".h";
  std::string headerFilePath = gGeneratedFilesDir + headerFileName;
  std::string cppFilePath = headerFilePath; ra::strings::replace(cppFilePath, ".h", ".cpp");
  std::string inputFilePath = gGeneratedFilesDir + ra::gtesthelp::getTestCaseName() + ".bin";

  createDummyFile(inputFilePath, 12345);

  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" --file=");
  cmdline.append(inputFilePath);
  cmdline.append(" --output=generated_files");
  cmdline.append(" --headerfile=");
  cmdline.append(headerFileName);
  cmdline.append(" --identifier=");
  cmdline.append(ra::gtesthelp::getTestCaseName().c_str());

  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());

  //delete generated files
  ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
  ASSERT_TRUE(deleteFile(cppFilePath.c_str()));

  //run the command
  int returnCode = system(cmdline.c_str());
  ASSERT_EQ(0, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;

  //run the command (again)
  returnCode = system(cmdline.c_str());
  ASSERT_EQ(0, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;
  
  //look for the expected message
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Skipping file");

  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
}

TEST_F(TestCLI, testEncoding)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  std::string headerFileName = std::string("_") + ra::gtesthelp::getTestCaseName().c_str() + ".h";
  std::string headerFilePath = gGeneratedFilesDir + headerFileName;
  std::string cppFilePath = headerFilePath; ra::strings::replace(cppFilePath, ".h", ".cpp");
  std::string inputFilePath = gGeneratedFilesDir + ra::gtesthelp::getTestCaseName() + ".bin";

  createDummyFile(inputFilePath, 12345);

  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" --file=");
  cmdline.append(inputFilePath);
  cmdline.append(" --output=generated_files");
  cmdline.append(" --headerfile=");
  cmdline.append(headerFileName);
  cmdline.append(" --identifier=");
  cmdline.append(ra::gtesthelp::getTestCaseName().c_str());
  cmdline.append(" --chuksize=256");
  cmdline.append(" --override");

  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());

  std::string baseCmdLine = cmdline;

  //assert default encoding (OCT)
  {
    //delete generated files
    ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
    ASSERT_TRUE(deleteFile(cppFilePath.c_str()));

    //run the command
    int returnCode = system(cmdline.c_str());
    ASSERT_EQ(0, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;

    //assert generated code
    ASSERT_TRUE(ra::filesystem::fileExists(cppFilePath.c_str()));
    ASSERT_TEXT_IN_FILE(true, cppFilePath.c_str(), "mBuffer.append(\"CZq\\210\\237\\266\\315\\344\\373\\022)");
  }

  //assert encoding (hex)
  {
    std::string cmdline = baseCmdLine;
    ra::strings::replace(cmdline, " --override", " --override --encoding=hEx");

    //delete generated files
    ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
    ASSERT_TRUE(deleteFile(cppFilePath.c_str()));

    //run the command
    int returnCode = system(cmdline.c_str());
    ASSERT_EQ(0, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;

    //assert generated code
    ASSERT_TRUE(ra::filesystem::fileExists(cppFilePath.c_str()));
    ASSERT_TEXT_IN_FILE(true, cppFilePath.c_str(), "mBuffer.append(\"CZq\\x88\\x9f\\xb6\\xcd\\xe4\\xfb\\x12)");
  }

  //assert invalid encoding (abc)
  {
    std::string cmdline = baseCmdLine;
    ra::strings::replace(cmdline, " --override", " --override --encoding=abc");

    //delete generated files
    ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
    ASSERT_TRUE(deleteFile(cppFilePath.c_str()));

    //run the command
    int returnCode = system(cmdline.c_str());
    ASSERT_NE(0, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;
  }

  //assert encoding (oct)
  {
    std::string cmdline = baseCmdLine;
    ra::strings::replace(cmdline, " --override", " --override --encoding=oCt");

    //delete generated files
    ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
    ASSERT_TRUE(deleteFile(cppFilePath.c_str()));

    //run the command
    int returnCode = system(cmdline.c_str());
    ASSERT_EQ(0, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;

    //assert generated code
    ASSERT_TRUE(ra::filesystem::fileExists(cppFilePath.c_str()));
    ASSERT_TEXT_IN_FILE(true, cppFilePath.c_str(), "mBuffer.append(\"CZq\\210\\237\\266\\315\\344\\373\\022)");
  }

  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
}

TEST_F(TestCLI, testDir)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();
 
  std::string headerFileName = std::string("_") + ra::gtesthelp::getTestCaseName().c_str() + ".h";
  std::string headerFilePath = gGeneratedFilesDir + headerFileName;
  std::string cppFilePath = headerFilePath; ra::strings::replace(cppFilePath, ".h", ".cpp");
 
  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" --dir=generated_files\\testDir01\\images");      //use windows path separator
  cmdline.append(" --output=generated_files\\testDir01\\sources");
  cmdline.append(" --noheader");
 
  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());
 
#ifdef __linux__
  //fix path separator
  ra::strings::replace(cmdline, "\\", "/");
#endif

  //build the list of generated files
  const char * generated_files_tmp[] = {
    "generated_files\\testDir01\\sources\\IMG_0001.h"  ,
    "generated_files\\testDir01\\sources\\IMG_0002.h"  ,
    "generated_files\\testDir01\\sources\\IMG_0003.h"  ,
    "generated_files\\testDir01\\sources\\IMG_0004.h"  ,
    "generated_files\\testDir01\\sources\\IMG_0005.h"  ,
    "generated_files\\testDir01\\sources\\IMG_0001.cpp",
    "generated_files\\testDir01\\sources\\IMG_0002.cpp",
    "generated_files\\testDir01\\sources\\IMG_0003.cpp",
    "generated_files\\testDir01\\sources\\IMG_0004.cpp",
    "generated_files\\testDir01\\sources\\IMG_0005.cpp",
  };
  const size_t num_generated_files = sizeof(generated_files_tmp)/sizeof(generated_files_tmp[0]);
  ra::strings::StringVector generated_files;
  for(size_t i=0; i<num_generated_files; i++)
  {
    generated_files.push_back(generated_files_tmp[i]);
#ifdef __linux__
    ra::strings::replace(generated_files[i], "\\", "/"); //fix path separator
#endif
  }

  //delete generated files
  for(size_t i=0; i<generated_files.size(); i++)
  {
    const std::string & generated_file = generated_files[i];
    ASSERT_TRUE(deleteFile(generated_file.c_str()));
  }
 
  //run the command
  int returnCode = system(cmdline.c_str());
  ASSERT_EQ(0, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;
 
  //assert that expected files were generated
  for(size_t i=0; i<generated_files.size(); i++)
  {
    const std::string & generates_file = generated_files[i];
    ASSERT_TRUE(ra::filesystem::fileExists(generates_file.c_str())) << "File not found: '" << generates_file.c_str() << "'.";
  }
 
  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
}

TEST_F(TestCLI, testErrorMissingArgumentFileOrDir)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();
 
  std::string headerFileName = std::string("_") + ra::gtesthelp::getTestCaseName().c_str() + ".h";
  std::string headerFilePath = gGeneratedFilesDir + headerFileName;
  std::string cppFilePath = headerFilePath; ra::strings::replace(cppFilePath, ".h", ".cpp");
 
  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  //cmdline.append(" --file=");
  //cmdline.append(getBin2cppPath()); //itself
  cmdline.append(" --output=generated_files");
  cmdline.append(" --headerfile=");
  cmdline.append(headerFileName);
  cmdline.append(" --identifier=");
  cmdline.append(ra::gtesthelp::getTestCaseName().c_str());
  cmdline.append(" --noheader");
 
  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());
 
  //delete generated files
  ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
  ASSERT_TRUE(deleteFile(cppFilePath.c_str()));
 
  //run the command
  int returnCode = system(cmdline.c_str());
#ifdef __linux__
  returnCode = WEXITSTATUS(returnCode);
#endif
  ASSERT_EQ(APP_ERROR_MISSINGARGUMENTS, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;
 
  //load output file
  ra::strings::StringVector lines;
  bool loaded = ra::gtesthelp::getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);
 
  //assert standard output log
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Error: Missing arguments (file");
 
  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
}
 
TEST_F(TestCLI, testErrorMissingArgumentOutput)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();
 
  std::string headerFileName = std::string("_") + ra::gtesthelp::getTestCaseName().c_str() + ".h";
  std::string headerFilePath = gGeneratedFilesDir + headerFileName;
  std::string cppFilePath = headerFilePath; ra::strings::replace(cppFilePath, ".h", ".cpp");
 
  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" --file=");
  cmdline.append(getBin2cppPath()); //itself
  //cmdline.append(" --output=generated_files");
  cmdline.append(" --headerfile=");
  cmdline.append(headerFileName);
  cmdline.append(" --identifier=");
  cmdline.append(ra::gtesthelp::getTestCaseName().c_str());
  cmdline.append(" --noheader");
 
  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());
 
  //delete generated files
  ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
  ASSERT_TRUE(deleteFile(cppFilePath.c_str()));
 
  //run the command
  int returnCode = system(cmdline.c_str());
#ifdef __linux__
  returnCode = WEXITSTATUS(returnCode);
#endif
  ASSERT_EQ(APP_ERROR_MISSINGARGUMENTS, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;
 
  //load output file
  ra::strings::StringVector lines;
  bool loaded = ra::gtesthelp::getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);
 
  //assert standard output log
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Error: Missing arguments (output)");
 
  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
}
 
TEST_F(TestCLI, testErrorMissingArgumentHeaderfile)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();
 
  std::string headerFileName = std::string("_") + ra::gtesthelp::getTestCaseName().c_str() + ".h";
  std::string headerFilePath = gGeneratedFilesDir + headerFileName;
  std::string cppFilePath = headerFilePath; ra::strings::replace(cppFilePath, ".h", ".cpp");
 
  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" --file=");
  cmdline.append(getBin2cppPath()); //itself
  cmdline.append(" --output=generated_files");
  //cmdline.append(" --headerfile=");
  //cmdline.append(headerFileName);
  cmdline.append(" --identifier=");
  cmdline.append(ra::gtesthelp::getTestCaseName().c_str());
  cmdline.append(" --noheader");
 
  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());
 
  //delete generated files
  ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
  ASSERT_TRUE(deleteFile(cppFilePath.c_str()));
 
  //run the command
  int returnCode = system(cmdline.c_str());
#ifdef __linux__
  returnCode = WEXITSTATUS(returnCode);
#endif
  ASSERT_EQ(APP_ERROR_MISSINGARGUMENTS, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;
 
  //load output file
  ra::strings::StringVector lines;
  bool loaded = ra::gtesthelp::getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);
 
  //assert standard output log
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Error: Missing arguments (headerfile)");
 
  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
}
 
TEST_F(TestCLI, testErrorMissingArgumentIdentifier)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();
 
  std::string headerFileName = std::string("_") + ra::gtesthelp::getTestCaseName().c_str() + ".h";
  std::string headerFilePath = gGeneratedFilesDir + headerFileName;
  std::string cppFilePath = headerFilePath; ra::strings::replace(cppFilePath, ".h", ".cpp");
 
  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" --file=");
  cmdline.append(getBin2cppPath()); //itself
  cmdline.append(" --output=generated_files");
  cmdline.append(" --headerfile=");
  cmdline.append(headerFileName);
  //cmdline.append(" --identifier=");
  //cmdline.append(ra::gtesthelp::getTestCaseName().c_str());
  cmdline.append(" --noheader");
 
  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());
 
  //delete generated files
  ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
  ASSERT_TRUE(deleteFile(cppFilePath.c_str()));
 
  //run the command
  int returnCode = system(cmdline.c_str());
#ifdef __linux__
  returnCode = WEXITSTATUS(returnCode);
#endif
  ASSERT_EQ(APP_ERROR_MISSINGARGUMENTS, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;
 
  //load output file
  ra::strings::StringVector lines;
  bool loaded = ra::gtesthelp::getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);
 
  //assert standard output log
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Error: Missing arguments (identifier)");
 
  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
}
 
TEST_F(TestCLI, testErrorMissingArgumentEncoding)
{
  static const std::string expectedFilePath = getExpectedFilePath();
 static const std::string outputFilePath   = getActualFilePath();
 
  std::string headerFileName = std::string("_") + ra::gtesthelp::getTestCaseName().c_str() + ".h";
  std::string headerFilePath = gGeneratedFilesDir + headerFileName;
  std::string cppFilePath = headerFilePath; ra::strings::replace(cppFilePath, ".h", ".cpp");
 
  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" --file=");
  cmdline.append(getBin2cppPath()); //itself
  cmdline.append(" --output=generated_files");
  cmdline.append(" --headerfile=");
  cmdline.append(headerFileName);
  cmdline.append(" --identifier=");
  cmdline.append(ra::gtesthelp::getTestCaseName().c_str());
  cmdline.append(" --encoding=123456789");
  cmdline.append(" --noheader");
 
  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());
 
  //delete generated files
  ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
  ASSERT_TRUE(deleteFile(cppFilePath.c_str()));
 
  //run the command
  int returnCode = system(cmdline.c_str());
#ifdef __linux__
  returnCode = WEXITSTATUS(returnCode);
#endif
  ASSERT_EQ(APP_ERROR_MISSINGARGUMENTS, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;
 
  //load output file
  ra::strings::StringVector lines;
  bool loaded = ra::gtesthelp::getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);
 
  //assert standard output log
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Error: Missing arguments (encoding)");
 
  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
}
 
TEST_F(TestCLI, testErrorMissingArgumentGenerator)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();
 
  std::string headerFileName = std::string("_") + ra::gtesthelp::getTestCaseName().c_str() + ".h";
  std::string headerFilePath = gGeneratedFilesDir + headerFileName;
  std::string cppFilePath = headerFilePath; ra::strings::replace(cppFilePath, ".h", ".cpp");
 
  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" --file=");
  cmdline.append(getBin2cppPath()); //itself
  cmdline.append(" --output=generated_files");
  cmdline.append(" --headerfile=");
  cmdline.append(headerFileName);
  cmdline.append(" --identifier=");
  cmdline.append(ra::gtesthelp::getTestCaseName().c_str());
  cmdline.append(" --generator=123456789");
  cmdline.append(" --noheader");
 
  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());
 
  //delete generated files
  ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
  ASSERT_TRUE(deleteFile(cppFilePath.c_str()));
 
  //run the command
  int returnCode = system(cmdline.c_str());
#ifdef __linux__
  returnCode = WEXITSTATUS(returnCode);
#endif
  ASSERT_EQ(APP_ERROR_MISSINGARGUMENTS, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;
 
  //load output file
  ra::strings::StringVector lines;
  bool loaded = ra::gtesthelp::getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);
 
  //assert standard output log
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Error: Missing arguments");
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "'generator' argument");
 
  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
}
 
TEST_F(TestCLI, testErrorInputFileNotFound)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();
 
  std::string headerFileName = std::string("_") + ra::gtesthelp::getTestCaseName().c_str() + ".h";
  std::string headerFilePath = gGeneratedFilesDir + headerFileName;
  std::string cppFilePath = headerFilePath; ra::strings::replace(cppFilePath, ".h", ".cpp");
 
  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" --file=123456789");
  //cmdline.append(getBin2cppPath()); //itself
  cmdline.append(" --output=generated_files");
  cmdline.append(" --headerfile=");
  cmdline.append(headerFileName);
  cmdline.append(" --identifier=");
  cmdline.append(ra::gtesthelp::getTestCaseName().c_str());
  cmdline.append(" --noheader");
 
  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());
 
  //delete generated files
  ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
  ASSERT_TRUE(deleteFile(cppFilePath.c_str()));
 
  //run the command
  int returnCode = system(cmdline.c_str());
#ifdef __linux__
  returnCode = WEXITSTATUS(returnCode);
#endif
  ASSERT_EQ(APP_ERROR_INPUTFILENOTFOUND, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;
 
  //load output file
  ra::strings::StringVector lines;
  bool loaded = ra::gtesthelp::getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);
 
  //assert standard output log
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Error: Unable to open input file");
 
  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
}
 
TEST_F(TestCLI, testErrorOutputFile)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();
 
  std::string headerFileName = std::string("_") + ra::gtesthelp::getTestCaseName().c_str() + ".h";
  std::string headerFilePath = gGeneratedFilesDir + headerFileName;
  std::string cppFilePath = headerFilePath; ra::strings::replace(cppFilePath, ".h", ".cpp");
 
  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" --file=");
  cmdline.append(getBin2cppPath()); //itself
  cmdline.append(" --output=generated_files");
#ifdef _WIN32
  cmdline.append(" --headerfile=:?.h");
#else
  cmdline.append(" --headerfile=.");
#endif
  //cmdline.append(headerFileName);
  cmdline.append(" --identifier=");
  cmdline.append(ra::gtesthelp::getTestCaseName().c_str());
  cmdline.append(" --noheader");
 
  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());
 
  //delete generated files
  ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
  ASSERT_TRUE(deleteFile(cppFilePath.c_str()));
 
  //run the command
  int returnCode = system(cmdline.c_str());
#ifdef __linux__
  returnCode = WEXITSTATUS(returnCode);
#endif
  ASSERT_EQ(APP_ERROR_UNABLETOCREATEOUTPUTFILES, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;
 
  //load output file
  ra::strings::StringVector lines;
  bool loaded = ra::gtesthelp::getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);
 
  //assert standard output log
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Error: Unable to create output files");
 
  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
}
 
TEST_F(TestCLI, testErrorTooManyArgumentsFileAndDir)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();
 
  std::string headerFileName = std::string("_") + ra::gtesthelp::getTestCaseName().c_str() + ".h";
  std::string headerFilePath = gGeneratedFilesDir + headerFileName;
  std::string cppFilePath = headerFilePath; ra::strings::replace(cppFilePath, ".h", ".cpp");
 
  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  cmdline.append(" --file=");
  cmdline.append(getBin2cppPath()); //itself
  cmdline.append(" --dir=.");
  cmdline.append(" --output=generated_files");
  cmdline.append(" --headerfile=");
  cmdline.append(headerFileName);
  cmdline.append(" --identifier=");
  cmdline.append(ra::gtesthelp::getTestCaseName().c_str());
  cmdline.append(" --noheader");
 
  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());
 
  //delete generated files
  ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
  ASSERT_TRUE(deleteFile(cppFilePath.c_str()));
 
  //run the command
  int returnCode = system(cmdline.c_str());
#ifdef __linux__
  returnCode = WEXITSTATUS(returnCode);
#endif
  ASSERT_EQ(APP_ERROR_TOOMANYARGUMENTS, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;
 
  //load output file
  ra::strings::StringVector lines;
  bool loaded = ra::gtesthelp::getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);
 
  //assert standard output log
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Error: Too many arguments (file");
 
  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
}
 
TEST_F(TestCLI, testErrorTooManyArgumentsHeaderfile)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();
 
  std::string headerFileName = std::string("_") + ra::gtesthelp::getTestCaseName().c_str() + ".h";
  std::string headerFilePath = gGeneratedFilesDir + headerFileName;
  std::string cppFilePath = headerFilePath; ra::strings::replace(cppFilePath, ".h", ".cpp");
 
  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  //cmdline.append(" --file=");
  //cmdline.append(getBin2cppPath()); //itself
  cmdline.append(" --dir=.");
  cmdline.append(" --output=generated_files");
  cmdline.append(" --headerfile=");
  cmdline.append(headerFileName);
  //cmdline.append(" --identifier=");
  //cmdline.append(ra::gtesthelp::getTestCaseName().c_str());
  cmdline.append(" --noheader");
 
  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());
 
  //delete generated files
  ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
  ASSERT_TRUE(deleteFile(cppFilePath.c_str()));
 
  //run the command
  int returnCode = system(cmdline.c_str());
#ifdef __linux__
  returnCode = WEXITSTATUS(returnCode);
#endif
  ASSERT_EQ(APP_ERROR_TOOMANYARGUMENTS, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;
 
  //load output file
  ra::strings::StringVector lines;
  bool loaded = ra::gtesthelp::getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);
 
  //assert standard output log
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Error: Too many arguments (headerfile)");
 
  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
}
 
TEST_F(TestCLI, testErrorTooManyArgumentsIdentifier)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();
 
  std::string headerFileName = std::string("_") + ra::gtesthelp::getTestCaseName().c_str() + ".h";
  std::string headerFilePath = gGeneratedFilesDir + headerFileName;
  std::string cppFilePath = headerFilePath; ra::strings::replace(cppFilePath, ".h", ".cpp");
 
  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  //cmdline.append(" --file=");
  //cmdline.append(getBin2cppPath()); //itself
  cmdline.append(" --dir=.");
  cmdline.append(" --output=generated_files");
  //cmdline.append(" --headerfile=");
  //cmdline.append(headerFileName);
  cmdline.append(" --identifier=");
  cmdline.append(ra::gtesthelp::getTestCaseName().c_str());
  cmdline.append(" --noheader");
 
  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());
 
  //delete generated files
  ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
  ASSERT_TRUE(deleteFile(cppFilePath.c_str()));
 
  //run the command
  int returnCode = system(cmdline.c_str());
#ifdef __linux__
  returnCode = WEXITSTATUS(returnCode);
#endif
  ASSERT_EQ(APP_ERROR_TOOMANYARGUMENTS, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;
 
  //load output file
  ra::strings::StringVector lines;
  bool loaded = ra::gtesthelp::getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);
 
  //assert standard output log
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Error: Too many arguments (identifier)");
 
  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
}
 
TEST_F(TestCLI, testErrorInputDirNotFound)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();
 
  std::string headerFileName = std::string("_") + ra::gtesthelp::getTestCaseName().c_str() + ".h";
  std::string headerFilePath = gGeneratedFilesDir + headerFileName;
  std::string cppFilePath = headerFilePath; ra::strings::replace(cppFilePath, ".h", ".cpp");
 
  //build command line
  std::string cmdline;
  cmdline.append(getBin2cppPath());
  //cmdline.append(" --file=");
  //cmdline.append(getBin2cppPath()); //itself
  cmdline.append(" --dir=123456789");
  cmdline.append(" --output=generated_files");
  //cmdline.append(" --headerfile=");
  //cmdline.append(headerFileName);
  //cmdline.append(" --identifier=");
  //cmdline.append(ra::gtesthelp::getTestCaseName().c_str());
  cmdline.append(" --noheader");
 
  cmdline.append(" >");
  cmdline.append(outputFilePath.c_str());
 
  //delete generated files
  ASSERT_TRUE(deleteFile(headerFilePath.c_str()));
  ASSERT_TRUE(deleteFile(cppFilePath.c_str()));
 
  //run the command
  int returnCode = system(cmdline.c_str());
#ifdef __linux__
  returnCode = WEXITSTATUS(returnCode);
#endif
  ASSERT_EQ(APP_ERROR_INPUTDIRNOTFOUND, returnCode) << "The command line '" << cmdline.c_str() << "' returned " << returnCode;
 
  //load output file
  ra::strings::StringVector lines;
  bool loaded = ra::gtesthelp::getTextFileContent(outputFilePath.c_str(), lines);
  ASSERT_TRUE(loaded);
 
  //assert standard output log
  ASSERT_TEXT_IN_FILE(true, outputFilePath.c_str(), "Error: Input directory not found");
 
  //cleanup
  ASSERT_TRUE(deleteFile(outputFilePath.c_str()));
}
