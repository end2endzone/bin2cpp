/**********************************************************************************
 * MIT License
 * 
 * Copyright (c) 2018 Antoine Beauchamp
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *********************************************************************************/

// test.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <iostream>
#include <stdio.h>

#include <gtest/gtest.h>

#include "rapidassist/environment.h"
#include "application.h"

int main(int argc, char **argv)
{
  //register command line arguments with the application
  Application & app = Application::getInstance();
  app.init(argc, argv);

  //define default values for xml output report
  if (ra::environment::IsProcess32Bit())
  {
    if (ra::environment::IsConfigurationDebug())
      ::testing::GTEST_FLAG(output) = "xml:bin2cpp_unittest.x86.debug.xml";
    else
      ::testing::GTEST_FLAG(output) = "xml:bin2cpp_unittest.x86.release.xml";
  }
  else if (ra::environment::IsProcess64Bit())
  {
    if (ra::environment::IsConfigurationDebug())
      ::testing::GTEST_FLAG(output) = "xml:bin2cpp_unittest.x64.debug.xml";
    else
      ::testing::GTEST_FLAG(output) = "xml:bin2cpp_unittest.x64.release.xml";
  }

  ::testing::GTEST_FLAG(filter) = "*";
  ::testing::InitGoogleTest(&argc, argv);

  int wResult = RUN_ALL_TESTS(); //Find and run all tests
  return wResult; // returns 0 if all the tests are successful, or 1 otherwise
}
