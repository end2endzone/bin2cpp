#include "TestExtraction.h"
#include "gtesthelper.h"

#include "generated_files\testHtml100000\_testHtml100000.h"
#include "generated_files\testRandom1\_testRandom1.h"
#include "generated_files\testRandom2\_testRandom2.h"
#include "generated_files\testRandom3\_testRandom3.h"
#include "generated_files\testSequential10MB\_testSequential10MB.h"
#include "generated_files\testText10\_testText10.h"
#include "generated_files\testText1000\_testText1000.h"
#include "generated_files\testText100000\_testText100000.h"

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
