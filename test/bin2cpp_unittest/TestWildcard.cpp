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

#include "TestWildcard.h"

#include "wildcard.h"

void TestWildcard::SetUp()
{
}

void TestWildcard::TearDown()
{
}

std::string to_boolean_str(bool value)
{
  return (value ? "true" : "false");
}

TEST_F(TestWildcard, testBasicExamples)
{
  struct TEST_HELPER
  {
    const char* value;
    const char* pattern;
    bool expected_result;
    std::vector<std::string> expected_captures;
  };
  static const TEST_HELPER test_values[] = {
      // ============================== matches ==============================
      // ?
      {"a", "?",                                                      true, {"a"}},
      {"abc", "???",                                                  true, {"a", "b", "c"}},
      {"kernel32.dll", "kernel??.dll",                                true, {"3", "2"}},
      {"kernel32.dll", "kernel32.???",                                true, {"d", "l", "l"}},
      {"kernel32.dll", "???nel32.dll",                                true, {"k", "e", "r"}},

      // *
      {"kernel32.dll", "*",                                           true, {"kernel32.dll"}},
      {"kernel32.dll", "ker*.dll",                                    true, {"nel32"}},
      {"kernel32.dll", "kernel32.*",                                  true, {"dll"}},
      {"kernel32.dll", "*.dll",                                       true, {"kernel32"}},

      // empty '*' wildcard
      {"kernel32.dll", "*kernel32.dll",                               true, {""}},
      {"kernel32.dll", "kernel32*.dll",                               true, {""}},
      {"kernel32.dll", "kernel32.dll*",                               true, {""}},

      // #
      {"kernel32.dll", "kernel##.dll",                                true, {"3", "2"}},

      // [abc]
      {"kernel32.dll", "[Kk]ernel32.dll",                             true, {"k"}},
      {"kernel32.dll", "kernel32.[dD][lL][lL]",                       true, {"d", "l", "l"}},
      {"kernel32.dll", "ke[r]nel32.dll",                              true, {"r"}},

      // [ranges]
      {"kernel32.dll", "kernel[0-9][0-9].dll",                        true, {"3", "2"}},
      {"kernel32.dll", "kernel32.[a-zA-Z0-9][a-zA-Z0-9][a-zA-Z0-9]",  true, {"d", "l", "l"}},
      
      // complex, multiple wildcard
      {"kernel32.dll", "ker*.*",                                      true, {"nel32", "dll"}},
      {"kernel32.dll", "*##.???",                                     true, {"kernel", "3", "2", "d", "l", "l"}},
      {"aabbccdd", "*??*dd",                                          true, {"", "a", "a", "bbcc"}},

      // ============================== failing matches ==============================
      {"kernel32.dll",  "ker*.txt",                                   false, {}},
      
      // too many '?' character
      {"kernel32.dll", "kernel32?.dll",                               false, {}},
      {"kernel32.dll", "kernel32.dll?",                               false, {}},
      {"kernel32.dll", "?kernel32.dll",                               false, {}},

      {"kernel32.dll",  "k##nel32.dll",                               false, {}},
      {"kernel32.dll",  "[aA]ernel32.dll",                            false, {}},
      {"kernel32.dll",  "k[0-9]ernel32.dll",                          false, {}},
      {"!",  "[a-zA-Z0-9]",                                           false, {}},
  };
  static const size_t num_test_values = sizeof(test_values) / sizeof(test_values[0]);

  for ( size_t i = 0; i < num_test_values; i++ )
  {
    const TEST_HELPER& t = test_values[i];

    std::vector<std::string> actual_captures;
    bool actual_result = bin2cpp::wildcard_match(t.value, t.pattern, actual_captures);

    ASSERT_EQ(actual_result, t.expected_result)     << "Test fail with test_values[" << i << "]. The match between value '" << t.value << "' and pattern '" << t.pattern << "' is supposed to return '" << to_boolean_str(t.expected_result) << "' but it actually retuned '" << to_boolean_str(actual_result) << "'.";
    ASSERT_EQ(actual_captures, t.expected_captures) << "Test fail with test_values[" << i << "]. The match between value '" << t.value << "' and pattern '" << t.pattern << "' is has returned '" << to_boolean_str(t.expected_result) << "' but the expected captures does not match.";
  }
}

TEST_F(TestWildcard, testWildcardMatchAny)
{
  const std::string path = "path/to/a/file.jpg";
  std::vector<std::string> patterns;

  {
    // test for no matches
    patterns.clear();
    patterns.push_back("idonotmatch");
    patterns.push_back("idonotmatcheither");

    ASSERT_FALSE(bin2cpp::wildcard_match_any(path, patterns));
  }

  {
    // test matches from first element
    patterns.clear();
    patterns.push_back("*.jpg");
    patterns.push_back("idonotmatch");

    ASSERT_TRUE(bin2cpp::wildcard_match_any(path, patterns));
  }

  {
    // test matches from second element
    patterns.clear();
    patterns.push_back("idonotmatch");
    patterns.push_back("*.jpg");

    ASSERT_TRUE(bin2cpp::wildcard_match_any(path, patterns));
  }
}

TEST_F(TestWildcard, testWildcardMatchAll)
{
  const std::string path = "path/to/a/file.jpg";
  std::vector<std::string> patterns;

  {
    // test for no matches
    patterns.clear();
    patterns.push_back("idonotmatch");
    patterns.push_back("idonotmatcheither");

    ASSERT_FALSE(bin2cpp::wildcard_match_all(path, patterns));
  }

  {
    // test matches from first element
    patterns.clear();
    patterns.push_back("*.jpg");
    patterns.push_back("idonotmatch");

    ASSERT_FALSE(bin2cpp::wildcard_match_all(path, patterns));
  }

  {
    // test matches from second element
    patterns.clear();
    patterns.push_back("idonotmatch");
    patterns.push_back("*.jpg");

    ASSERT_FALSE(bin2cpp::wildcard_match_all(path, patterns));
  }

  {
    // test matches from all elements
    patterns.clear();
    patterns.push_back("*/to/*");
    patterns.push_back("*.jpg");

    ASSERT_TRUE(bin2cpp::wildcard_match_all(path, patterns));
  }
}