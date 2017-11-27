#include "TestExtraction.h"
#include "gtesthelper.h"

#include "generated_files\testHtml100000\_testHtml100000.h"
#include "generated_files\testPredictable100000\_testPredictable100000.h"
#include "generated_files\testRandom100000\_testRandom100000.h"
#include "generated_files\testSequential10MB\_testSequential10MB.h"
#include "generated_files\testText100000\_testText100000.h"
#include "generated_files\testSmall10\_testSmall10.h"

gTestHelper & hlp = gTestHelper::getInstance();

void TestExtraction::SetUp()
{
}

void TestExtraction::TearDown()
{
}

TEST_F(TestExtraction, testSmall10)
{
  static const char * expectedFilePath =  "generated_files\\testSmall10\\testSmall10.bin";
  static const char * outputFilePath =    "generated_files\\testSmall10\\testSmall10.output";

  const bin2cpp::File & file = bin2cpp::getTestSmall10File();
  bool extractSuccess = file.save(outputFilePath);
  ASSERT_TRUE(extractSuccess);

  //assert content is the same
  std::string reason;
  bool equal = hlp.isFileEquals(expectedFilePath, outputFilePath, reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testHtml100000)
{
  static const char * expectedFilePath =  "generated_files\\testHtml100000\\testHtml100000.bin";
  static const char * outputFilePath =    "generated_files\\testHtml100000\\testHtml100000.output";

  const bin2cpp::File & file = bin2cpp::getTestHtml100000File();
  bool extractSuccess = file.save(outputFilePath);
  ASSERT_TRUE(extractSuccess);

  //assert content is the same
  std::string reason;
  bool equal = hlp.isFileEquals(expectedFilePath, outputFilePath, reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testPredictable100000)
{
  static const char * expectedFilePath =  "generated_files\\testPredictable100000\\testPredictable100000.bin";
  static const char * outputFilePath =    "generated_files\\testPredictable100000\\testPredictable100000.output";

  const bin2cpp::File & file = bin2cpp::getTestPredictable100000File();
  bool extractSuccess = file.save(outputFilePath);
  ASSERT_TRUE(extractSuccess);

  //assert content is the same
  std::string reason;
  bool equal = hlp.isFileEquals(expectedFilePath, outputFilePath, reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testRandom100000)
{
  static const char * expectedFilePath =  "generated_files\\testRandom100000\\testRandom100000.bin";
  static const char * outputFilePath =    "generated_files\\testRandom100000\\testRandom100000.output";

  const bin2cpp::File & file = bin2cpp::getTestRandom100000File();
  bool extractSuccess = file.save(outputFilePath);
  ASSERT_TRUE(extractSuccess);

  //assert content is the same
  std::string reason;
  bool equal = hlp.isFileEquals(expectedFilePath, outputFilePath, reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testSequential10MB)
{
  static const char * expectedFilePath =  "generated_files\\testSequential10MB\\testSequential10MB.bin";
  static const char * outputFilePath =    "generated_files\\testSequential10MB\\testSequential10MB.output";

  const bin2cpp::File & file = bin2cpp::getTestSequential10MBFile();
  bool extractSuccess = file.save(outputFilePath);
  ASSERT_TRUE(extractSuccess);

  //assert content is the same
  std::string reason;
  bool equal = hlp.isFileEquals(expectedFilePath, outputFilePath, reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testText100000)
{
  static const char * expectedFilePath =  "generated_files\\testText100000\\testText100000.bin";
  static const char * outputFilePath =    "generated_files\\testText100000\\testText100000.output";

  const bin2cpp::File & file = bin2cpp::getTestText100000File();
  bool extractSuccess = file.save(outputFilePath);
  ASSERT_TRUE(extractSuccess);

  //assert content is the same
  std::string reason;
  bool equal = hlp.isFileEquals(expectedFilePath, outputFilePath, reason);
  ASSERT_TRUE(equal) << reason.c_str();
}
