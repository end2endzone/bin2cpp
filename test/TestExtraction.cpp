#include "TestExtraction.h"
#include "gtesthelper.h"

#include "EmbededSplashScreen.h"
#include "EmbededProjectFile.h"

gTestHelper & hlp = gTestHelper::getInstance();

void TestExtraction::SetUp()
{
}

void TestExtraction::TearDown()
{
}

TEST_F(TestExtraction, testBinaryFile)
{
  static const char * fileInput = "splashscreen.png";
  static const char * fileOutput = "splashscreen.output.png";

  bin2cpp::File & splash = bin2cpp::getSplashScreenFile();
  bool extractSuccess = splash.save(fileOutput);
  ASSERT_TRUE(extractSuccess);

  //assert content is the same
  std::string reason;
  bool equal = hlp.isFileEquals(fileInput, fileOutput, reason);
  ASSERT_TRUE(equal) << reason.c_str();
}

TEST_F(TestExtraction, testTextFile)
{
  static const char * fileInput = "bin2cppTest.vcproj";
  static const char * fileOutput = "bin2cppTest.vcproj.output.txt";

  bin2cpp::File & vcprojFile = bin2cpp::getVcprojFile();
  bool extractSuccess = vcprojFile.save(fileOutput);
  ASSERT_TRUE(extractSuccess);

  //assert content is the same
  std::string reason;
  bool equal = hlp.isFileEquals(fileInput, fileOutput, reason);
  ASSERT_TRUE(equal) << reason.c_str();
}
