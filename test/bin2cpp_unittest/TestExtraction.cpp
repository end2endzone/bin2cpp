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

#include "TestExtraction.h"

#include "rapidassist/testing.h"
#include "rapidassist/filesystem.h"
#include "rapidassist/testing.h"

//generated_files
#include "testHtml100000/_testHtml100000.h"
#include "testRandom1/_testRandom1.h"
#include "testRandom2/_testRandom2.h"
#include "testRandom3/_testRandom3.h"
#include "testRandom4/_testRandom4.h"
#include "testRandom5/_testRandom5.h"
#include "testRandom6/_testRandom6.h"
#include "testRandom7/_testRandom7.h"
#include "testRandom8/_testRandom8.h"
#include "testSequential1000/_testSequential1000.h"
#include "testSequential10MB/_testSequential10MB.h"
#include "testGeneratorArray10000/_testGeneratorArray10000.h"
#include "testGeneratorSegment10000/_testGeneratorSegment10000.h"
#include "testGeneratorString10000/_testGeneratorString10000.h"
#ifdef _WIN32
#include "testGeneratorWin32/_testGeneratorWin32.h"
#endif
#include "testEncodingOct/_testEncodingOct.h"
#include "testEncodingHex/_testEncodingHex.h"
#include "testText10/_testText10.h"
#include "testText1000/_testText1000.h"
#include "testText100000/_testText100000.h"
#include "testIssue12/_testIssue12.h"
#include "testIssue12Mini/_testIssue12Mini.h"
#include "testIssue13/_testIssue13.h"
#include "testIssue47/generated/_testIssue47.h"
#include "testIssue50/_testIssue50.h"
#include "testIssue50/_testIssue50.h"
#include "testIssue56a/compiled_sources/_testIssue56a.index.1234.h"
#include "testIssue56a/compiled_sources/_testIssue56a.index.4321.h"
#include "testReportedPathFile1/_testReportedPathFile1.h"
#include "testReportedPathFile2/_testReportedPathFile2.h"
#include "testReportedPathDir/generated_sources/FileManagerReportedPathDir.h"

#undef BIN2CPP_EMBEDDEDFILE_CLASS
#include "testNamespace/_testNamespace.h"

#undef BIN2CPP_EMBEDDEDFILE_CLASS
#include "testBaseClass/_testBaseClass.h"

extern const std::string & gGeneratedFilesDir;

namespace TestExtractionUtils
{
  std::string getExpectedFilePath()
  {
    std::string file;
    file.append(gGeneratedFilesDir);
    file.append(ra::testing::GetTestCaseName());
    file.append(ra::filesystem::GetPathSeparatorStr());
    file.append(ra::testing::GetTestCaseName());
    file.append(".bin");
    return file;
  }

  std::string getActualFilePath()
  {
    std::string file;
    file.append(gGeneratedFilesDir);
    file.append(ra::testing::GetTestCaseName());
    file.append(ra::filesystem::GetPathSeparatorStr());
    file.append(ra::testing::GetTestCaseName());
    file.append(".output");
    return file;
  }
}
using namespace TestExtractionUtils;

void TestExtraction::SetUp()
{
}

void TestExtraction::TearDown()
{
}

TEST_F(TestExtraction, testText10)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::File & file = bin2cpp::getTestText10File();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testText1000)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::File & file = bin2cpp::getTestText1000File();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testText100000)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::File & file = bin2cpp::getTestText100000File();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testHtml100000)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::File & file = bin2cpp::getTestHtml100000File();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testRandom1)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::File & file = bin2cpp::getTestRandom1File();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testRandom2)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::File & file = bin2cpp::getTestRandom2File();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testRandom3)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::File & file = bin2cpp::getTestRandom3File();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testRandom4)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::File & file = bin2cpp::getTestRandom4File();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testRandom5)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::File & file = bin2cpp::getTestRandom5File();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testRandom6)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::File & file = bin2cpp::getTestRandom6File();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testRandom7)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::File & file = bin2cpp::getTestRandom7File();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testRandom8)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::File & file = bin2cpp::getTestRandom8File();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testSequential1000)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::File & file = bin2cpp::getTestSequential1000File();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testSequential10MB)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::File & file = bin2cpp::getTestSequential10MBFile();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testGeneratorArray10000)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::File & file = bin2cpp::getTestGeneratorArray10000File();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testGeneratorSegment10000)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::File & file = bin2cpp::getTestGeneratorSegment10000File();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testGeneratorString10000)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::File & file = bin2cpp::getTestGeneratorString10000File();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

#ifdef _WIN32
TEST_F(TestExtraction, testGeneratorWin32)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::File & file = bin2cpp::getTestGeneratorWin32File();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}
#endif

TEST_F(TestExtraction, testEncodingOct)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::File & file = bin2cpp::getTestEncodingOctFile();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testEncodingHex)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::File & file = bin2cpp::getTestEncodingHexFile();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testNamespace)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const foobar::File & file = foobar::getTestNamespaceFile();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testBaseClass)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::Resource & file = bin2cpp::getTestBaseClassFile();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testIssue12)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::File & file = bin2cpp::getTestIssue12File();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testIssue12Mini)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::File & file = bin2cpp::getTestIssue12MiniFile();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testIssue13)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp::File & file = bin2cpp::getTestIssue13File();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testIssue47)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp47::File & file = bin2cpp47::getTestIssue47File();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();

  //assert custom subfolder 'generated' is found in include directives.

  int line = 0;
  int index = 0;

  //_testIssue47.h
  {
    std::string path = "generated_files/testIssue47/generated/_testIssue47.cpp";
    ra::filesystem::NormalizePath(path);
    ASSERT_TRUE( ra::filesystem::FileExists(path.c_str()) ) << "File '" << path.c_str() << "' not found!";
    ASSERT_TRUE( ra::testing::FindInFile(path.c_str(), "#include \"generated/_testIssue47.h\"", line, index) );
  }

  //FileManager47.cpp
  {
    std::string path = "generated_files/testIssue47/generated/FileManager47.cpp";
    ra::filesystem::NormalizePath(path);
    ASSERT_TRUE( ra::filesystem::FileExists(path.c_str()) ) << "File '" << path.c_str() << "' not found!";
    ASSERT_TRUE( ra::testing::FindInFile(path.c_str(), "#include \"generated/FileManager47.h\"", line, index) );
  }

}

TEST_F(TestExtraction, testIssue50)
{
  static const std::string expectedFilePath = getExpectedFilePath();
  static const std::string outputFilePath   = getActualFilePath();

  const bin2cpp50::File & file = bin2cpp50::getTestIssue50File();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();

  //assert macro guards are prefixed with "BIN2CPP50"

  int line = 0;
  int index = 0;

  //_testIssue50.h
  {
    std::string path = "generated_files/testIssue50/_testIssue50.h";
    ra::filesystem::NormalizePath(path);
    ASSERT_TRUE( ra::filesystem::FileExists(path.c_str()) ) << "File '" << path.c_str() << "' not found!";
    ASSERT_TRUE( ra::testing::FindInFile(path.c_str(), "BIN2CPP50_EMBEDDEDFILE_CLASS", line, index) );
  }

  //FileManager50.h
  {
    std::string path = "generated_files/testIssue50/FileManager50.h";
    ra::filesystem::NormalizePath(path);
    ASSERT_TRUE( ra::filesystem::FileExists(path.c_str()) ) << "File '" << path.c_str() << "' not found!";
    ASSERT_TRUE( ra::testing::FindInFile(path.c_str(), "BIN2CPP50_EMBEDDEDFILE_CLASS", line, index) );
    ASSERT_TRUE( ra::testing::FindInFile(path.c_str(), "BIN2CPP50_FILEMANAGER_CLASS",  line, index) );
  }
}

TEST_F(TestExtraction, testIssue56a)
{
  //save _testIssue56a.index.1234.css
  std::string output_path1234 = "generated_files/testIssue56a/saved_files/_testIssue56a.index.1234.css";
  {
    ra::filesystem::NormalizePath(output_path1234);
    const issue56a::File & file = issue56a::get_testIssue56aindex1234CssFile();
    bool extractSuccess = file.save(output_path1234.c_str());
    ASSERT_TRUE(extractSuccess);
  }

  //save _testIssue56a.index.4321.css
  std::string output_path4321 = "generated_files/testIssue56a/saved_files/_testIssue56a.index.4321.css";
  {
    ra::filesystem::NormalizePath(output_path4321);
    const issue56a::File & file = issue56a::get_testIssue56aindex4321CssFile();
    bool extractSuccess = file.save(output_path4321.c_str());
    ASSERT_TRUE(extractSuccess);
  }

  //assert binary content is the same
  std::string reason;
  std::string expected_path1234 = "generated_files/testIssue56a/input_files/_testIssue56a.index.1234.css";
  ra::filesystem::NormalizePath(expected_path1234);
  bool equal1234 = ra::testing::IsFileEquals(expected_path1234.c_str(), output_path1234.c_str(), reason);
  ASSERT_TRUE(equal1234) << reason.c_str();

  std::string expected_path4321 = "generated_files/testIssue56a/input_files/_testIssue56a.index.4321.css";
  ra::filesystem::NormalizePath(expected_path4321);
  bool equal4321 = ra::testing::IsFileEquals(expected_path4321.c_str(), output_path4321.c_str(), reason);
  ASSERT_TRUE(equal4321) << reason.c_str();

  //assert correct identifiers

  int line = 0;
  int index = 0;

  //_testIssue56a.index.1234.h
  {
    std::string path = "generated_files/testIssue56a/compiled_sources/_testIssue56a.index.1234.h";
    ra::filesystem::NormalizePath(path);
    ASSERT_TRUE( ra::filesystem::FileExists(path.c_str()) ) << "File '" << path.c_str() << "' not found!";
    ASSERT_TRUE( ra::testing::FindInFile(path.c_str(), "get_testIssue56aindex1234CssFile()", line, index) );
  }

  //_testIssue56a.index.4321.h
  {
    std::string path = "generated_files/testIssue56a/compiled_sources/_testIssue56a.index.4321.h";
    ra::filesystem::NormalizePath(path);
    ASSERT_TRUE( ra::filesystem::FileExists(path.c_str()) ) << "File '" << path.c_str() << "' not found!";
    ASSERT_TRUE( ra::testing::FindInFile(path.c_str(), "get_testIssue56aindex4321CssFile()", line, index) );
  }
}

TEST_F(TestExtraction, testDeprecatedGetFilename)
{
  const testReportedPathFile1::File & file = testReportedPathFile1::getTestReportedPathFile1File();
  ASSERT_TRUE( file.getFileName() != NULL );
  ASSERT_TRUE( file.getFilename() != NULL );
  ASSERT_EQ(0, strcmp(file.getFileName(), file.getFilename()) );
}

TEST_F(TestExtraction, testReportedPathFile1)
{
  const testReportedPathFile1::File & file = testReportedPathFile1::getTestReportedPathFile1File();

  // @BIN2CPP_TARGET_FILE@ --noheader --file=.\generated_files\testReportedPathFile1\testReportedPathFile1.bin
  // --output=.\generated_files\testReportedPathFile1 --chunksize=50
  // --headerfile=_testReportedPathFile1.h --identifier=testReportedPathFile1
  // --namespace=testReportedPathFile1 --override --reportedfilepath=foo\bar\testReportedPathFile1.bin

  std::string actual_file_name = file.getFileName();
  std::string expected_file_name = "testReportedPathFile1.bin";
  ASSERT_EQ(expected_file_name, actual_file_name);

  std::string actual_file_path = file.getFilePath();
  std::string expected_file_path = "foo\\bar\\testReportedPathFile1.bin";
  ra::filesystem::NormalizePath(expected_file_path);
  ASSERT_EQ(expected_file_path, actual_file_path);
}

TEST_F(TestExtraction, testReportedPathFile2)
{
  const testReportedPathFile2::File & file = testReportedPathFile2::getTestReportedPathFile2File();

  // @BIN2CPP_TARGET_FILE@ --noheader --file=.\generated_files\testReportedPathFile2\testReportedPathFile2.bin
  // --output=.\generated_files\testReportedPathFile2 --chunksize=50
  // --headerfile=_testReportedPathFile2.h --identifier=testReportedPathFile2
  // --namespace=testReportedPathFile2 --override --reportedfilepath=ham\eggs\breakfast.bin

  std::string actual_file_name = file.getFileName();
  std::string expected_file_name = "testReportedPathFile2.bin";
  ASSERT_EQ(expected_file_name, actual_file_name);

  std::string actual_file_path = file.getFilePath();
  std::string expected_file_path = "ham\\eggs\\breakfast.bin";
  ra::filesystem::NormalizePath(expected_file_path);
  ASSERT_EQ(expected_file_path, actual_file_path);
}

TEST_F(TestExtraction, testReportedPathDir)
{
  testReportedPathDir::FileManager & mgr = testReportedPathDir::FileManager::getInstance();

  std::string expected_path;
  expected_path += "www";
  expected_path += ra::filesystem::GetPathSeparatorStr();

  size_t count = mgr.getFileCount();
  for(size_t i=0; i<count; i++)
  {
    const testReportedPathDir::File * file = mgr.getFile(i);
    ASSERT_TRUE( file != NULL );
    std::string file_path = file->getFilePath();
    
    //assert in www directory
    ASSERT_EQ( 0, file_path.compare(0, expected_path.size(), expected_path) ) << "file_path=" << file_path;
  }
}

TEST_F(TestExtraction, testFileSave)
{
  std::string expectedFilePath = "generated_files/testEncodingHex/testEncodingHex.bin";
  std::string outputFilePath   = "generated_files/testEncodingHex/testEncodingHex.output";
  ra::filesystem::NormalizePath(expectedFilePath);
  ra::filesystem::NormalizePath(outputFilePath  );

  const bin2cpp::File & file = bin2cpp::getTestEncodingHexFile();
  bool extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //change the content of the file
  ra::testing::ChangeFileContent(outputFilePath.c_str(), 0, 'T');
  ra::testing::ChangeFileContent(outputFilePath.c_str(), 1, 'E');
  ra::testing::ChangeFileContent(outputFilePath.c_str(), 2, 'S');
  ra::testing::ChangeFileContent(outputFilePath.c_str(), 3, 'T');

  //assert calling save() when the file already exists should replace the content
  extractSuccess = file.save(outputFilePath.c_str());
  ASSERT_TRUE(extractSuccess);

  //assert binary content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();

}
