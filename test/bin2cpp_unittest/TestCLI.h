#pragma once

#include <gtest/gtest.h>

class TestCLI : public ::testing::Test
{
public:
  virtual void SetUp();
  virtual void TearDown();
};
