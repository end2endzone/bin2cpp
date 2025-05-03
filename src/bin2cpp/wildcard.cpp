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

#include "wildcard.h"
#include "bin2cpp/version.h"

namespace bin2cpp
{
  static inline bool isdigit(const char& c)
  {
    return '0' <= c && c <= '9';
  }

  ///<param name="value_index">The search index within the value string.</param>
  ///<param name="pattern_index">The search index within the pattern string.</param>
  bool wildcard_match_helper(const std::string& value, const std::string& pattern, size_t value_index, size_t pattern_index, std::vector<std::string>& captures)
  {
    // Base case: value and pattern are both exhausted. All characrters matches.
    if ( value_index == value.size() && pattern_index == pattern.size() )
    {
      return true;
    }

    // If pattern is exhausted but value has more characters, no match
    if ( pattern_index == pattern.size() ) return false;

    // Handle '*': Capture a variable-length substring
    if ( pattern[pattern_index] == '*' )
    {
      for ( size_t i = value_index; i <= value.size(); ++i )
      {
        captures.push_back(value.substr(value_index, i - value_index));

        // Recurse to resolve for the remaining characters.
        bool match = wildcard_match_helper(value, pattern, i, pattern_index + 1, captures);
        if ( match ) return true;
        captures.pop_back(); // Remove last match if unsuccessful
      }
      return false;
    }

    // Handle '?': Capture a single character.
    if ( value_index < value.size() && pattern[pattern_index] == '?' )
    {
      captures.push_back(std::string(1, value[value_index])); // Store single-character match

      // Recurse to resolve for the remaining characters.
      bool match = wildcard_match_helper(value, pattern, value_index + 1, pattern_index + 1, captures);
      return match;
    }

    // Handle '#': Capture any single digit (0-9)
    if ( value_index < value.size() && pattern[pattern_index] == '#' && isdigit(value[value_index]) )
    {
      captures.push_back(std::string(1, value[value_index]));

      // Recurse to resolve for the remaining characters.
      bool match = wildcard_match_helper(value, pattern, value_index + 1, pattern_index + 1, captures);
      return match;
    }

    // Handling character lists like '[xyz]' or ranges like '[a-z]'.
    // This assumes that first range character is smaller than second range character.
    if ( pattern[pattern_index] == '[' )
    {
      size_t closing_bracket_pos = pattern.find(']', pattern_index);
      if ( closing_bracket_pos == std::string::npos ) return false; // Malformed pattern

      char matchChar = value[value_index];
      bool found = false;

      // For each characters in within the brackets
      for ( size_t i = pattern_index + 1; i < closing_bracket_pos; ++i )
      {
        // Is this a range?
        if ( pattern[i] == '-' && i > pattern_index + 1 && i < closing_bracket_pos - 1 )
        {
          // Handle range [x-y]
          if ( matchChar >= pattern[i - 1] && matchChar <= pattern[i + 1] ) found = true;
        }
        else if ( pattern[i] == matchChar )
        {
          found = true;
        }
      }

      // If match is found, capture it and continue recursion
      if ( found )
      {
        captures.push_back(std::string(1, matchChar));

        // Recurse to resolve for the remaining characters.
        bool match = wildcard_match_helper(value, pattern, value_index + 1, closing_bracket_pos + 1, captures);
        return match;
      }
      else
      {
        return false;
      }
    }

    // Exact character match
    if ( value_index < value.size() && pattern[pattern_index] == value[value_index] )
    {
      // Recurse to resolve for the remaining characters.
      bool match = wildcard_match_helper(value, pattern, value_index + 1, pattern_index + 1, captures);
      return match;
    }

    return false;
  }

  bool wildcard_match(const std::string& value, const std::string& pattern, std::vector<std::string>& captures)
  {
    captures.clear();  // Ensure captures vector is empty before starting
    bool match = wildcard_match_helper(value, pattern, 0, 0, captures);
    if ( !match )
      captures.clear();
    return match;
  }

}; //bin2cpp
