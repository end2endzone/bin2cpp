#ifndef TESTSTRINGFUNC_H
#define TESTSTRINGFUNC_H

#include <gtest/gtest.h>

namespace stringfunc { namespace test
{
  class TestStringFunc : public ::testing::Test
  {
  public:
    virtual void SetUp();
    virtual void TearDown();
  };

} // End namespace test
} // End namespace stringfunc

#endif //TESTSTRINGFUNC_H
