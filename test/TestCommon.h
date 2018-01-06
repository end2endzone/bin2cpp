#pragma once

#include <gtest/gtest.h>

class TestCommon : public ::testing::Test
{
public:
  virtual void SetUp();
  virtual void TearDown();
};
