// test.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <iostream>
#include <stdio.h>

#include <gtest/gtest.h>

#include "rapidassist/gtesthelp.h"
#include "application.h"

int main(int argc, char **argv)
{
  //register command line arguments with the application
  Application & app = Application::getInstance();
  app.init(argc, argv);

  //define default values for xml output report
  if (ra::gtesthelp::isProcessorX86())
  {
    if (ra::gtesthelp::isDebugCode())
      ::testing::GTEST_FLAG(output) = "xml:bin2cppTest.x86.debug.xml";
    else
      ::testing::GTEST_FLAG(output) = "xml:bin2cppTest.x86.release.xml";
  }
  else if (ra::gtesthelp::isProcessorX64())
  {
    if (ra::gtesthelp::isDebugCode())
      ::testing::GTEST_FLAG(output) = "xml:bin2cppTest.x64.debug.xml";
    else
      ::testing::GTEST_FLAG(output) = "xml:bin2cppTest.x64.release.xml";
  }

  ::testing::GTEST_FLAG(filter) = "*";
  ::testing::InitGoogleTest(&argc, argv);

  int wResult = RUN_ALL_TESTS(); //Find and run all tests
  return wResult; // returns 0 if all the tests are successful, or 1 otherwise
}
