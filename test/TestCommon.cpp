#include "TestCommon.h"
#include "gtesthelper.h"
#include "common.h"

namespace TestCommonUtils
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
using namespace TestCommonUtils;
using namespace bin2cpp;

namespace TestCLIUtils
{
  extern bool createDummyFile(const char * iPath);
};

void TestCommon::SetUp()
{
}

void TestCommon::TearDown()
{
}

TEST_F(TestCommon, testSplitPath)
{
  std::string folder;
  std::string filename;
  
  //from full path
  {
    splitPath("C:\\foo\\bar\\file.txt", folder, filename);
    ASSERT_EQ(folder, "C:\\foo\\bar");
    ASSERT_EQ(filename, "file.txt");
  }
  {
    splitPath("C:\\foo\\bar\\file", folder, filename);
    ASSERT_EQ(folder, "C:\\foo\\bar");
    ASSERT_EQ(filename, "file");
  }
  {
    splitPath("C:\\foo\\bar\\file.", folder, filename);
    ASSERT_EQ(folder, "C:\\foo\\bar");
    ASSERT_EQ(filename, "file.");
  }

  //from filename only
  {
    splitPath("file.txt", folder, filename);
    ASSERT_EQ(folder, "");
    ASSERT_EQ(filename, "file.txt");
  }
  {
    splitPath("file.", folder, filename);
    ASSERT_EQ(folder, "");
    ASSERT_EQ(filename, "file.");
  }
  {
    splitPath("file", folder, filename);
    ASSERT_EQ(folder, "");
    ASSERT_EQ(filename, "file");
  }

  //empty strings
  {
    splitPath("", folder, filename);
    ASSERT_EQ(folder, "");
    ASSERT_EQ(filename, "");
  }
}

TEST_F(TestCommon, testCurrentFolder)
{
  std::string curdir = getCurrentFolder();
  ASSERT_NE(curdir, "");
}

TEST_F(TestCommon, testFileExtention)
{
  std::string ext;

  //from valid filename
  {
    ext = getFileExtention("file.txt");
    ASSERT_EQ(ext, "txt");
  }
  {
    ext = getFileExtention("file.");
    ASSERT_EQ(ext, "");
  }
  {
    ext = getFileExtention("file");
    ASSERT_EQ(ext, "");
  }

  //from full path
  {
    ext = getFileExtention("C:\\foo\\bar\\file.txt");
    ASSERT_EQ(ext, "txt");
  }

  //from filename with multiple dots
  {
    ext = getFileExtention("file.subfile.txt");
    ASSERT_EQ(ext, "txt");
  }

  //empty strings
  {
    ext = getFileExtention("");
    ASSERT_EQ(ext, "");
  }

  //from special case path
  {
    ext = getFileExtention("C:\\foo.bar\\file");
    ASSERT_EQ(ext, "");
  }

}

TEST_F(TestCommon, testFileModifiedDate)
{
  //assert with file not found
  {
    uint64_t date = getFileModifiedDate("foobar.txt");
    ASSERT_EQ(0, date);
  }

  //assert with a new file
  {
    std::string filePath = getActualFilePath();
    TestCLIUtils::createDummyFile(filePath.c_str());
    uint64_t date = getFileModifiedDate(filePath.c_str());
    ASSERT_GT(date, 0);
  }
}
