#include "TestNativeFunc.h"
#include "nativefunc.h"
#include <time.h>

using namespace nativefunc;

namespace nativefunc { namespace test
{
  std::tm getLocalSystemTime()
  {
    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    return (*timeinfo);
  }

  //--------------------------------------------------------------------------------------------------
  void TestNativeFunc::SetUp()
  {
  }
  //--------------------------------------------------------------------------------------------------
  void TestNativeFunc::TearDown()
  {
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestNativeFunc, testMillisleep)
  {
    //assert that millisleep() is actually sleeping
    std::tm time1 = getLocalSystemTime();
    ASSERT_EQ(0, nativefunc::millisleep(5000 + 50)); //at least 5 seconds
    std::tm time2 = getLocalSystemTime();

    //convert hour, minute and seconds to absolute seconds
    int seconds1 = time1.tm_hour*3600+time1.tm_min*60+time1.tm_sec;
    int seconds2 = time2.tm_hour*3600+time2.tm_min*60+time2.tm_sec;
    
    static const int EXPECTED = 5;
    int diff = seconds2 - seconds1;

    //assert near
    ASSERT_GT(diff, EXPECTED-1); //allow 1 seconds of difference
    ASSERT_LT(diff, EXPECTED+1); //allow 1 seconds of difference
  }
  //--------------------------------------------------------------------------------------------------
} // End namespace test
} // End namespace nativefunc
