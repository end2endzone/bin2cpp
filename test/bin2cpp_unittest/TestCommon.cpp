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

#include "TestCommon.h"

#include "common.h"

#include "rapidassist/testing.h"
#include "rapidassist/filesystem.h"
#include "rapidassist/testing.h"


void TestCommon::SetUp()
{
}

void TestCommon::TearDown()
{
}

TEST_F(TestCommon, testGetFunctionIdentifierFromPath)
{
  ASSERT_EQ("themedarkcss", bin2cpp::getFunctionIdentifierFromPath("c:\\temp\\www\\static\\css\\theme.dark.css"));

  //test running twice returns the same value
  ASSERT_EQ("themedarkcss", bin2cpp::getFunctionIdentifierFromPath("c:\\temp\\www\\static\\css\\theme.dark.css"));

  //test with only a filename
  ASSERT_EQ("themedarkcss", bin2cpp::getFunctionIdentifierFromPath("theme.dark.css"));

  //test valid special characters
  ASSERT_EQ("hello_world_1234567890", bin2cpp::getFunctionIdentifierFromPath("hello_world_1234567890"));

  //test more invalid characters
  ASSERT_EQ("index1234bangdollars", bin2cpp::getFunctionIdentifierFromPath("index.1234.bang!.dollars$"));
}

TEST_F(TestCommon, testGetUniqueFunctionIdentifierFromPath)
{
  bin2cpp::Dictionary tmp;

  //test normal scenario
  {
    const std::string input_path = "c:\\temp\\www\\static\\css\\theme.dark.css";

    ASSERT_EQ("themedarkcss", bin2cpp::getUniqueFunctionIdentifierFromPath(input_path, tmp));

    //test duplicates
    ASSERT_EQ("themedarkcss_1", bin2cpp::getUniqueFunctionIdentifierFromPath(input_path, tmp));
    ASSERT_EQ("themedarkcss_2", bin2cpp::getUniqueFunctionIdentifierFromPath(input_path, tmp));
    ASSERT_EQ("themedarkcss_3", bin2cpp::getUniqueFunctionIdentifierFromPath(input_path, tmp));
    ASSERT_EQ("themedarkcss_4", bin2cpp::getUniqueFunctionIdentifierFromPath(input_path, tmp));
    ASSERT_EQ("themedarkcss_5", bin2cpp::getUniqueFunctionIdentifierFromPath(input_path, tmp));
  }

  //test behavior with files that ends with numbers
  {
    const std::string input_path = "c:\\test\\index.001";

    ASSERT_EQ("index001", bin2cpp::getUniqueFunctionIdentifierFromPath(input_path, tmp));

    //test duplicates
    ASSERT_EQ("index001_1", bin2cpp::getUniqueFunctionIdentifierFromPath(input_path, tmp));
    ASSERT_EQ("index001_2", bin2cpp::getUniqueFunctionIdentifierFromPath(input_path, tmp));
    ASSERT_EQ("index001_3", bin2cpp::getUniqueFunctionIdentifierFromPath(input_path, tmp));
    ASSERT_EQ("index001_4", bin2cpp::getUniqueFunctionIdentifierFromPath(input_path, tmp));
    ASSERT_EQ("index001_5", bin2cpp::getUniqueFunctionIdentifierFromPath(input_path, tmp));
  }

  //try to bug the system: get an identifier from a file which is already a duplicate identifer.
  ASSERT_EQ("themedarkcss_3_1", bin2cpp::getUniqueFunctionIdentifierFromPath("themedarkcss_3", tmp));

  //test getting duplicate identifiers because of invalid characters
  {
    ASSERT_EQ("jqueryjs",   bin2cpp::getUniqueFunctionIdentifierFromPath("c:\\foo\\jquery.js", tmp));

    //test duplicates
    ASSERT_EQ("jqueryjs_1", bin2cpp::getUniqueFunctionIdentifierFromPath("c:\\foo\\j.query.js"   , tmp));
    ASSERT_EQ("jqueryjs_2", bin2cpp::getUniqueFunctionIdentifierFromPath("c:\\foo\\j!qu!ery.js"  , tmp));
    ASSERT_EQ("jqueryjs_3", bin2cpp::getUniqueFunctionIdentifierFromPath("c:\\foo\\j-query.js"   , tmp));
    ASSERT_EQ("jqueryjs_4", bin2cpp::getUniqueFunctionIdentifierFromPath("c:\\foo\\jqu.er.y.js"  , tmp));
    ASSERT_EQ("jqueryjs_5", bin2cpp::getUniqueFunctionIdentifierFromPath("c:\\foo\\jqueryjs"     , tmp));
  }

  //test getting duplicates from different directories
  {
    ASSERT_EQ("indexhtml",   bin2cpp::getUniqueFunctionIdentifierFromPath("c:\\www\\index.html", tmp));

    //test duplicates
    ASSERT_EQ("indexhtml_1", bin2cpp::getUniqueFunctionIdentifierFromPath("c:\\www\\home\\index.html"                            , tmp));
    ASSERT_EQ("indexhtml_2", bin2cpp::getUniqueFunctionIdentifierFromPath("c:\\www\\blog\\index.html"                            , tmp));
    ASSERT_EQ("indexhtml_3", bin2cpp::getUniqueFunctionIdentifierFromPath("c:\\www\\blog\\how-to-create-a-web-site\\index.html"  , tmp));
    ASSERT_EQ("indexhtml_4", bin2cpp::getUniqueFunctionIdentifierFromPath("c:\\www\\contact\\index.html"                         , tmp));
    ASSERT_EQ("indexhtml_5", bin2cpp::getUniqueFunctionIdentifierFromPath("c:\\www\\blog\\using-bin2cpp\\index.html"             , tmp));
  }
}

TEST_F(TestCommon, testPathSplitJoin)
{
  std::string path;
  std::string dir;
  std::string file_name;
  std::string file_ext;

  // test split
  path = "C:\\Windows\\notepad.exe";
  bin2cpp::pathSplit(path, dir, file_name, file_ext);
  ASSERT_EQ("C:\\Windows"     , dir);
  ASSERT_EQ("notepad"         , file_name);
  ASSERT_EQ("exe"             , file_ext);

  // test join
  dir = "C:\\Windows";
  file_name = "notepad";
  file_ext = "exe";
  path = bin2cpp::pathJoin(dir, file_name, file_ext);
  ASSERT_EQ("C:\\Windows\\notepad.exe", path);

  static const char * paths[] = {
    "C:\\Windows\\notepad.exe",
    "notepad.exe",
    "dumpfile",                       // filename without an extension
    "dump.file.",                     // file name with a dot in name but without an extension
    "C:\\temp\\dumpfile",             // directory and filename without an extension
    "C:\\temp\\dump.file.",           // directory and file name with a dot in name
    "C:\\Windows",                    // only a directory
    "C:\\temp\\.tmp",                 // an file without a file name
    "C:\\pagefile.sys",               // file in root directory
    "C:\\bootmgr",                    // file in root directory without an extension
    "C:\\",                           // the root directory
  };
  static const size_t num_paths = sizeof(paths)/sizeof(paths[0]);
  for(size_t i=0; i<num_paths; i++)
  {
    path = paths[i];
    bin2cpp::pathSplit(path, dir, file_name, file_ext);
    std::string joined = bin2cpp::pathJoin(dir, file_name, file_ext);

    ASSERT_EQ(path, joined) << "Failed calling pathSplit() and pathJoin() :\n" <<
      "path=\"" << path << "\"\n" <<
      "dir=\"" << dir << "\"\n" <<
      "file_name=\"" << file_name << "\"\n" <<
      "file_ext=\"" << file_ext << "\"\n";
  }
}
