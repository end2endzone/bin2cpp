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
#include "rapidassist/user.h"

const std::string slash = ra::filesystem::GetPathSeparatorStr();
const std::string home_mp3_directory = ra::user::GetHomeDirectory() + slash + "mp3";
const std::string temp_directory = ra::filesystem::GetTemporaryDirectory();
#ifdef _WIN32
const std::string www_directory = "C:\\inetpub\\wwwroot";
#else
const std::string www_directory = "/var/www";
#endif

void TestCommon::SetUp()
{
}

void TestCommon::TearDown()
{
}

TEST_F(TestCommon, testGetFunctionIdentifierFromPath)
{
  std::string path;

  path = www_directory + "\\static\\css\\theme.dark.css";
  ra::filesystem::NormalizePath(path);
  ASSERT_EQ("ThemedarkCss", bin2cpp::getFunctionIdentifierFromPath(path));

  //test running twice returns the same value
  path = www_directory + "\\static\\css\\theme.dark.css";
  ra::filesystem::NormalizePath(path);
  ASSERT_EQ("ThemedarkCss", bin2cpp::getFunctionIdentifierFromPath(path));

  //test with only a filename
  ASSERT_EQ("ThemedarkCss", bin2cpp::getFunctionIdentifierFromPath("theme.dark.css"));

  //test valid special characters
  ASSERT_EQ("Hello_world_1234567890", bin2cpp::getFunctionIdentifierFromPath("hello_world_1234567890"));

  //test more invalid characters
  ASSERT_EQ("Index1234bangDollars", bin2cpp::getFunctionIdentifierFromPath("index.1234.bang!.dollars$"));
}

TEST_F(TestCommon, testGetUniqueFunctionIdentifierFromPath)
{
  bin2cpp::Dictionary tmp;
  std::string path;

  //test normal scenario
  {
    path = www_directory + "\\static\\css\\theme.dark.css";
    ra::filesystem::NormalizePath(path);

    ASSERT_EQ("ThemedarkCss", bin2cpp::getUniqueFunctionIdentifierFromPath(path, tmp));

    //test duplicates
    ASSERT_EQ("ThemedarkCss_1", bin2cpp::getUniqueFunctionIdentifierFromPath(path, tmp));
    ASSERT_EQ("ThemedarkCss_2", bin2cpp::getUniqueFunctionIdentifierFromPath(path, tmp));
    ASSERT_EQ("ThemedarkCss_3", bin2cpp::getUniqueFunctionIdentifierFromPath(path, tmp));
    ASSERT_EQ("ThemedarkCss_4", bin2cpp::getUniqueFunctionIdentifierFromPath(path, tmp));
    ASSERT_EQ("ThemedarkCss_5", bin2cpp::getUniqueFunctionIdentifierFromPath(path, tmp));
  }

  //test behavior with files that ends with numbers
  {
    path = temp_directory + "\\index.001";
    ra::filesystem::NormalizePath(path);

    ASSERT_EQ("Index001", bin2cpp::getUniqueFunctionIdentifierFromPath(path, tmp));

    //test duplicates
    ASSERT_EQ("Index001_1", bin2cpp::getUniqueFunctionIdentifierFromPath(path, tmp));
    ASSERT_EQ("Index001_2", bin2cpp::getUniqueFunctionIdentifierFromPath(path, tmp));
    ASSERT_EQ("Index001_3", bin2cpp::getUniqueFunctionIdentifierFromPath(path, tmp));
    ASSERT_EQ("Index001_4", bin2cpp::getUniqueFunctionIdentifierFromPath(path, tmp));
    ASSERT_EQ("Index001_5", bin2cpp::getUniqueFunctionIdentifierFromPath(path, tmp));
  }

  //try to bug the system: get an identifier from a file which is already a duplicate identifer.
  ASSERT_EQ("ThemedarkCss_3_1", bin2cpp::getUniqueFunctionIdentifierFromPath("ThemedarkCss_3", tmp));

  //test getting duplicate identifiers because of invalid characters
  {
    path = www_directory + "\\static\\js";
    ra::filesystem::NormalizePath(path);
    path += slash;

    ASSERT_EQ("JqueryJs",   bin2cpp::getUniqueFunctionIdentifierFromPath(path + "jquery.js", tmp));

    //test duplicates
    ASSERT_EQ("JqueryJs_1", bin2cpp::getUniqueFunctionIdentifierFromPath(path + "j.query.js"   , tmp));
    ASSERT_EQ("JqueryJs_2", bin2cpp::getUniqueFunctionIdentifierFromPath(path + "j!qu!ery.js"  , tmp));
    ASSERT_EQ("JqueryJs_3", bin2cpp::getUniqueFunctionIdentifierFromPath(path + "j-query.js"   , tmp));
    ASSERT_EQ("JqueryJs_4", bin2cpp::getUniqueFunctionIdentifierFromPath(path + "jqu.er.y.js"  , tmp));
  }

  //test getting duplicates from different directories
  {
    path = www_directory;
    ra::filesystem::NormalizePath(path);
    path += slash;

    ASSERT_EQ("IndexHtml",   bin2cpp::getUniqueFunctionIdentifierFromPath(path + "index.html", tmp));

    //test duplicates
    ASSERT_EQ("IndexHtml_1", bin2cpp::getUniqueFunctionIdentifierFromPath(path + "home"     + slash + "index.html"                                        , tmp));
    ASSERT_EQ("IndexHtml_2", bin2cpp::getUniqueFunctionIdentifierFromPath(path + "blog"     + slash + "index.html"                                        , tmp));
    ASSERT_EQ("IndexHtml_3", bin2cpp::getUniqueFunctionIdentifierFromPath(path + "blog"     + slash + "how-to-create-a-web-site" + slash + "index.html"   , tmp));
    ASSERT_EQ("IndexHtml_4", bin2cpp::getUniqueFunctionIdentifierFromPath(path + "contact"  + slash + "index.html"                                        , tmp));
    ASSERT_EQ("IndexHtml_5", bin2cpp::getUniqueFunctionIdentifierFromPath(path + "blog"     + slash + "using-bin2cpp" + slash + "index.html"              , tmp));
  }
}

TEST_F(TestCommon, testGetUniqueFilePath)
{
  bin2cpp::Dictionary dict;
  std::string path;
  std::string base_dir;

  //test basic directory and filename path
  {
    path = www_directory + slash + "index.h";
    ra::filesystem::NormalizePath(path);
    base_dir = ra::filesystem::GetParentPath(path);
    base_dir += slash;

    ASSERT_EQ(base_dir + "index.h", bin2cpp::getUniqueFilePath(path, dict));

    ASSERT_EQ(base_dir + "index_1.h", bin2cpp::getUniqueFilePath(path, dict));
    ASSERT_EQ(base_dir + "index_2.h", bin2cpp::getUniqueFilePath(path, dict));
    ASSERT_EQ(base_dir + "index_3.h", bin2cpp::getUniqueFilePath(path, dict));
    ASSERT_EQ(base_dir + "index_4.h", bin2cpp::getUniqueFilePath(path, dict));
    ASSERT_EQ(base_dir + "index_5.h", bin2cpp::getUniqueFilePath(path, dict));
  }

  //test filename only
  {
    path = "icon.h";
    ASSERT_EQ("icon.h", bin2cpp::getUniqueFilePath(path, dict));

    ASSERT_EQ("icon_1.h", bin2cpp::getUniqueFilePath(path, dict));
    ASSERT_EQ("icon_2.h", bin2cpp::getUniqueFilePath(path, dict));
    ASSERT_EQ("icon_3.h", bin2cpp::getUniqueFilePath(path, dict));
    ASSERT_EQ("icon_4.h", bin2cpp::getUniqueFilePath(path, dict));
    ASSERT_EQ("icon_5.h", bin2cpp::getUniqueFilePath(path, dict));
  }

  //test relative paths
  {
    path = "www\\blog\\index.h";
    ra::filesystem::NormalizePath(path);
    base_dir = ra::filesystem::GetParentPath(path);
    base_dir += slash;

    ASSERT_EQ(base_dir + "index.h", bin2cpp::getUniqueFilePath(path, dict));

    ASSERT_EQ(base_dir + "index_1.h", bin2cpp::getUniqueFilePath(path, dict));
    ASSERT_EQ(base_dir + "index_2.h", bin2cpp::getUniqueFilePath(path, dict));
    ASSERT_EQ(base_dir + "index_3.h", bin2cpp::getUniqueFilePath(path, dict));
    ASSERT_EQ(base_dir + "index_4.h", bin2cpp::getUniqueFilePath(path, dict));
    ASSERT_EQ(base_dir + "index_5.h", bin2cpp::getUniqueFilePath(path, dict));
  }
}

TEST_F(TestCommon, testPathSplitJoin)
{
  std::string path;
  std::string dir;
  std::string file_name;
  std::string file_ext;

  // test split
  path = home_mp3_directory + slash + "music.mp3";
  bin2cpp::pathSplit(path, dir, file_name, file_ext);
  ASSERT_EQ(home_mp3_directory, dir);
  ASSERT_EQ("music"           , file_name);
  ASSERT_EQ("mp3"             , file_ext);

  // test join
  dir = home_mp3_directory;
  file_name = "music";
  file_ext = "mp3";
  path = bin2cpp::pathJoin(dir, file_name, file_ext);
  ASSERT_EQ(home_mp3_directory + slash + "music.mp3", path);

  static const char * paths[] = {
    "notepad.exe",                    // filename only
    "dumpfile",                       // filename without an extension
    "dump.file.",                     // file name with a dot in name but without an extension
#ifdef _WIN32
    "C:\\Windows\\notepad.exe",       // absolute file path
    "C:\\temp\\dumpfile",             // directory and filename without an extension
    "C:\\temp\\dump.file.",           // directory and file name with a dot in name
    "C:\\Windows",                    // only a directory
    "C:\\temp\\.tmp",                 // a file without a file name
    "C:\\pagefile.sys",               // file in root directory
    "C:\\bootmgr",                    // file in root directory without an extension
    "C:\\",                           // the root directory
    "www\\blog\\index.html",          // test relative path
#else
    "/home/mp3/music.mp3",            // absolute file path
    "/tmp/dumpfile",                  // directory and filename without an extension
    "/tmp/dump.file.",                // directory and file name with a dot in name
    "/home/dev",                      // only a directory
    "/tmp/.tmp",                      // a file without a file name
    "/bootfile.sys",                  // file in root directory
    "/bootmgr",                       // file in root directory without an extension
    "/",                              // the root directory
    "/var/www/blog/index.html",       // test relative path
#endif
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
