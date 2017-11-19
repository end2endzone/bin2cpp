#include "TestExtraction.h"
#include "gtesthelper.h"

#include "generated_files\testBinaryFile\EmbeddedBinaryFile.h"
#include "generated_files\testTextFile\EmbeddedTextFile.h"

gTestHelper & hlp = gTestHelper::getInstance();

void TestExtraction::SetUp()
{
}

void TestExtraction::TearDown()
{
}

TEST_F(TestExtraction, testBinaryFile)
{
  static const char * expectedFilePath = "..\\docs\\icon.xcf";
  static const char * outputFilePath = "generated_files\\testBinaryFile\\icon.output.xcf";

  bin2cpp::File & file = bin2cpp::getIconFile();
  bool extractSuccess = file.save(outputFilePath);
  ASSERT_TRUE(extractSuccess);

  //assert content is the same
  std::string reason;
  bool equal = hlp.isFileEquals(expectedFilePath, outputFilePath, reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testTextFile)
{
  static const char * expectedFilePath = "..\\README.md";
  static const char * outputFilePath = "generated_files\\testTextFile\\readme.output.md";

  bin2cpp::File & file = bin2cpp::getReadmeFile();
  bool extractSuccess = file.save(outputFilePath);
  ASSERT_TRUE(extractSuccess);

  //assert content is the same
  std::string reason;
  bool equal = hlp.isFileEquals(expectedFilePath, outputFilePath, reason);
  ASSERT_TRUE(equal) << reason.c_str();
}
