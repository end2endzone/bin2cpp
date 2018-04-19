#include "TestFilesystemFunc.h"
#include "filesystemfunc.h"
#include "gtesthelper.h"

using namespace filesystem;

namespace filesystem { namespace test
{
  bool createDummyFile(const char * iPath)
  {
    FILE * f = fopen(iPath, "w");
    if (f == NULL)
      return false;
    fputs("FOO!\n", f);
    fputs("&\n", f);
    fputs("BAR\n", f);
    fclose(f);

    return true;
  }

  int countValues(const std::vector<std::string> & iList, const std::string & iValue)
  {
    int count = 0;
    for(size_t i=0; i<iList.size(); i++)
    {
      const std::string & value = iList[i];
      if (value == iValue)
        count++;
    }
    return count;
  }

  //--------------------------------------------------------------------------------------------------
  void TestFilesystemFunc::SetUp()
  {
  }
  //--------------------------------------------------------------------------------------------------
  void TestFilesystemFunc::TearDown()
  {
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestFilesystemFunc, testGetFileSize)
  {
    //test NULL
    {
      const char * path = NULL;
      uint32_t size = filesystem::getFileSize(path);
      ASSERT_EQ(0, size);
    }

    //test actual value
    {
      //create dummy file
      std::string filename = gTestHelper::getInstance().getTestQualifiedName();
      ASSERT_TRUE( createDummyFile(filename.c_str()) );

#ifdef WIN32
      static const uint32_t EXPECTED = 14;
#elif UNIX
      static const uint32_t EXPECTED = 11;
#endif

      //test `const char *` api
      uint32_t size = filesystem::getFileSize(filename.c_str());
      ASSERT_EQ(EXPECTED, size);
      size = 0;

      //test `FILE*` api
      FILE * ptr = fopen(filename.c_str(), "r");
      ASSERT_TRUE(ptr != NULL);
      size = filesystem::getFileSize(ptr);
      fclose(ptr);
      ASSERT_EQ(EXPECTED, size);
    }

  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestFilesystemFunc, testGetFilename)
  {
    //test NULL
    {
      static const std::string EXPECTED = "";
      std::string filename = filesystem::getFilename(NULL);
      ASSERT_EQ(EXPECTED, filename);
    }

    //test filename only
    {
      static const std::string EXPECTED = "foo.bar";
      std::string filename = filesystem::getFilename("foo.bar");
      ASSERT_EQ(EXPECTED, filename);
    }

    //test full path (unix style)
    {
      static const std::string EXPECTED = "foo.bar";
      std::string filename = filesystem::getFilename("/home/myFolder/foo.bar");
      ASSERT_EQ(EXPECTED, filename);
    }

    //test full path (windows style)
    {
      static const std::string EXPECTED = "foo.bar";
      std::string filename = filesystem::getFilename("C:\\Users\\Antoine\\Desktop\\myFolder\\foo.bar");
      ASSERT_EQ(EXPECTED, filename);
    }
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestFilesystemFunc, testFileExists)
  {
    //test NULL
    {
      bool exists = filesystem::fileExists(NULL);
      ASSERT_FALSE(exists);
    }

    //test not found
    {
      bool exists = filesystem::fileExists("foo.bar.notfound.bang");
      ASSERT_FALSE(exists);
    }

    //test found
    {
      //create dummy file
      std::string filename = gTestHelper::getInstance().getTestQualifiedName();
      ASSERT_TRUE( createDummyFile(filename.c_str()) );

      bool exists = filesystem::fileExists(filename.c_str());
      ASSERT_TRUE(exists);
    }
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestFilesystemFunc, testFolderExists)
  {
    //test NULL
    {
      bool exists = filesystem::folderExists(NULL);
      ASSERT_FALSE(exists);
    }

    //test not found
    {
      bool exists = filesystem::folderExists("/home/fooBAR");
      ASSERT_FALSE(exists);
    }

    //test found
    {
      //create dummy file
      std::string currentFolder = filesystem::getCurrentFolder();
      ASSERT_TRUE( !currentFolder.empty() );

      bool exists = filesystem::folderExists(currentFolder.c_str());
      ASSERT_TRUE(exists);
    }
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestFilesystemFunc, testGetTemporaryFileName)
  {
    //test not empty
    {
      std::string filename = filesystem::getTemporaryFileName();
      ASSERT_TRUE( !filename.empty() );
    }

    //test repetitive
    {
      std::vector<std::string> filenames;
      static const size_t numTest = 20;
      for(size_t i=0; i<numTest; i++)
      {
        std::string filename = filesystem::getTemporaryFileName();
        filenames.push_back(filename);
      }

      //assert that all values are unique
      for(size_t i=0; i<numTest; i++)
      {
        const std::string & filename = filenames[i];
        int count = countValues(filenames, filename);
        ASSERT_EQ(1, count) << "Found value '" << filename << "' " << count << " times in the list.";
      }
    }
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestFilesystemFunc, testGetTemporaryFilePath)
  {
    //test not empty
    {
      std::string path = filesystem::getTemporaryFilePath();
      ASSERT_TRUE( !path.empty() );
    }

    //test repetitive
    {
      std::vector<std::string> paths;
      static const size_t numTest = 20;
      for(size_t i=0; i<numTest; i++)
      {
        std::string path = filesystem::getTemporaryFilePath();
        paths.push_back(path);
      }

      //assert that all values are unique
      for(size_t i=0; i<numTest; i++)
      {
        const std::string & path = paths[i];
        int count = countValues(paths, path);
        ASSERT_EQ(1, count) << "Found value '" << path << "' " << count << " times in the list.";
      }
    }
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestFilesystemFunc, testGetParentPath)
  {
    //test no folder
    {
      static const std::string EXPECTED = "";
      std::string parent = filesystem::getParentPath("filename.bar");
      ASSERT_EQ(EXPECTED, parent);
    }

    //test unix style
    {
      static const std::string EXPECTED = "/home/myFolder";
      std::string parent = filesystem::getParentPath("/home/myFolder/foo.bar");
      ASSERT_EQ(EXPECTED, parent);
    }

    //test windows style
    {
      static const std::string EXPECTED = "C:\\Users\\Antoine\\Desktop\\myFolder";
      std::string parent = filesystem::getParentPath("C:\\Users\\Antoine\\Desktop\\myFolder\\foo.bar");
      ASSERT_EQ(EXPECTED, parent);
    }
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestFilesystemFunc, testGetShortPathForm)
  {
    //test spaces in filename
    {
      static const std::string EXPECTED = "ABC~1.TXT";
      std::string shortPath = filesystem::getShortPathForm("a b c.txt");
      ASSERT_EQ(EXPECTED, shortPath);
    }

    //test too long file extension
    {
      static const std::string EXPECTED = "ABCDEF~1.TEX";
      std::string shortPath = filesystem::getShortPathForm("abcdefgh.text");
      ASSERT_EQ(EXPECTED, shortPath);
    }

    //test too long filename
    {
      static const std::string EXPECTED = "ABCDEF~1.TXT";
      std::string shortPath = filesystem::getShortPathForm("abcdefghijklmnopqrstuvwxyz.txt");
      ASSERT_EQ(EXPECTED, shortPath);
    }

    //test spaces in file extension
    {
      static const std::string EXPECTED = "ABCDE~1.TXT";
      std::string shortPath = filesystem::getShortPathForm("abcde.t x t");
      ASSERT_EQ(EXPECTED, shortPath);
    }

    //test program files (windows style)
    {
      static const std::string EXPECTED = "PROGRA~1";
      std::string shortPath = filesystem::getShortPathForm("Program Files (x86)");
      ASSERT_EQ(EXPECTED, shortPath);
    }
  }
  //--------------------------------------------------------------------------------------------------
} // End namespace test
} // End namespace filesystem
