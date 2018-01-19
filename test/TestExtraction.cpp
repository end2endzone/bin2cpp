#include "TestExtraction.h"
#include "gtesthelper.h"

#include "generated_files\testHtml100000\_testHtml100000.h"
#include "generated_files\testRandom1\_testRandom1.h"
#include "generated_files\testRandom2\_testRandom2.h"
#include "generated_files\testRandom3\_testRandom3.h"
#include "generated_files\testRandom4\_testRandom4.h"
#include "generated_files\testRandom5\_testRandom5.h"
#include "generated_files\testRandom6\_testRandom6.h"
#include "generated_files\testRandom7\_testRandom7.h"
#include "generated_files\testRandom8\_testRandom8.h"
#include "generated_files\testSequential1000\_testSequential1000.h"
#include "generated_files\testSequential10MB\_testSequential10MB.h"
#include "generated_files\testGeneratorArray10000\_testGeneratorArray10000.h"
#include "generated_files\testGeneratorSegment10000\_testGeneratorSegment10000.h"
#include "generated_files\testGeneratorString10000\_testGeneratorString10000.h"
#include "generated_files\testText10\_testText10.h"
#include "generated_files\testText1000\_testText1000.h"
#include "generated_files\testText100000\_testText100000.h"
#include "generated_files\testIssue12\_testIssue12.h"
#include "generated_files\testIssue12Mini\_testIssue12Mini.h"
#include "generated_files\testIssue13\_testIssue13.h"

#undef BIN2CPP_EMBEDDEDFILE_CLASS
#include "generated_files\testNamespace\_testNamespace.h"

#undef BIN2CPP_EMBEDDEDFILE_CLASS
#include "generated_files\testBaseClass\_testBaseClass.h"

namespace TestExtractionUtils
{
  gTestHelper & hlp = gTestHelper::getInstance();

  std::string getExpectedFilePath()
  {
    std::string file;
    file.append("generated_files\\");
    file.append(hlp.getTestCaseName());
    file.append("\\");
    file.append(hlp.getTestCaseName());
    file.append(".bin");
    return file;
  }

  std::string getActualFilePath()
  {
    std::string file;
    file.append("generated_files\\");
    file.append(hlp.getTestCaseName());
    file.append("\\");
    file.append(hlp.getTestCaseName());
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
  bool equal = hlp.isFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
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
  bool equal = hlp.isFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
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
  bool equal = hlp.isFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
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
  bool equal = hlp.isFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
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
  bool equal = hlp.isFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
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
  bool equal = hlp.isFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
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
  bool equal = hlp.isFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
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
  bool equal = hlp.isFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
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
  bool equal = hlp.isFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
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
  bool equal = hlp.isFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
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
  bool equal = hlp.isFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
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
  bool equal = hlp.isFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
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
  bool equal = hlp.isFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
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
  bool equal = hlp.isFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
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
  bool equal = hlp.isFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
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
  bool equal = hlp.isFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
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
  bool equal = hlp.isFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
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
  bool equal = hlp.isFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
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
  bool equal = hlp.isFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
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
  bool equal = hlp.isFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
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
  bool equal = hlp.isFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
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
  bool equal = hlp.isFileEquals(expectedFilePath.c_str(), outputFilePath.c_str(), reason);
  ASSERT_TRUE(equal) << reason.c_str();
}
