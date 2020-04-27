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

  //assert content is the same
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

  //assert content is the same
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

  //assert content is the same
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

  //assert content is the same
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

  //assert content is the same
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

  //assert content is the same
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

  //assert content is the same
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

  //assert content is the same
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

  //assert content is the same
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

  //assert content is the same
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

  //assert content is the same
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

  //assert content is the same
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

  //assert content is the same
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

  //assert content is the same
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

  //assert content is the same
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

  //assert content is the same
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

  //assert content is the same
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

  //assert content is the same
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

  //assert content is the same
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

  //assert content is the same
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

  //assert content is the same
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

  //assert content is the same
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

  //assert content is the same
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

  //assert content is the same
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

  //assert content is the same
  std::string reason;
  bool equal = ra::testing::IsFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}
