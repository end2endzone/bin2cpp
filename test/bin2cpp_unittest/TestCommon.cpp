#include "TestCommon.h"

#include "rapidassist/gtesthelp.h"
#include "rapidassist/filesystem.h"

namespace TestCommonUtils
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

}
using namespace TestCommonUtils;

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
