#ifndef TESTENVIRONMENTFUNC_H
#define TESTENVIRONMENTFUNC_H

#include <gtest/gtest.h>

namespace environment { namespace test
{
  class TestEnvironmentFunc : public ::testing::Test
  {
  public:
    virtual void SetUp();
    virtual void TearDown();
  };

} // End namespace test
} // End namespace environment

#endif //TESTENVIRONMENTFUNC_H
