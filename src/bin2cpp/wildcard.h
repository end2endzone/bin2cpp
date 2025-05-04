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

#ifndef BIN2CPP_WILDCARD_H
#define BIN2CPP_WILDCARD_H

#include <string>
#include <vector>

namespace bin2cpp
{

  ///<summary>
  ///Checks if a given value matches a pattern containing wildcard characters definition.
  ///</summary>
  ///<remarks>
  ///  Supported Wildcards :
  ///  -'?' matches any single character.
  ///  -'*' matches zero or more characters.
  ///  -'#' matches any single digit(0 - 9).
  ///  -'[charlist]' matches any character in the provided set.
  ///  -'[a-z]', '[A-Z]', '[0-9]' match characters in respective ranges.
  ///  -'[a-zA-Z0-9]' matches any alphanumeric character.
  ///</remarks>
  ///<param name="value">The file path, value or string to match.</param>
  ///<param name="pattern">The pattern containing wildcards.</param>
  ///<param name="captures">The captured value of wildcard within the input value. The number of captures matches the number of wildcard in the pattern.</param>
  ///<returns>Returns true if the value matches the pattern, otherwise false.</returns>
  bool wildcard_match(const std::string& value, const std::string& pattern, std::vector<std::string>& captures);

  ///<summary>
  ///Checks if a given value matches a pattern containing wildcard characters definition.
  ///</summary>
  ///<remarks>
  ///  Supported Wildcards :
  ///  -'?' matches any single character.
  ///  -'*' matches zero or more characters.
  ///  -'#' matches any single digit(0 - 9).
  ///  -'[charlist]' matches any character in the provided set.
  ///  -'[a-z]', '[A-Z]', '[0-9]' match characters in respective ranges.
  ///  -'[a-zA-Z0-9]' matches any alphanumeric character.
  ///</remarks>
  ///<param name="value">The file path, value or string to match.</param>
  ///<param name="pattern">The pattern containing wildcards.</param>
  ///<returns>Returns true if the value matches the pattern, otherwise false.</returns>
  inline bool wildcard_match(const std::string& value, const std::string& pattern)
  {
    std::vector<std::string> tmp_captures;
    return wildcard_match(value, pattern, tmp_captures);
  }

  ///<summary>
  ///Checks if a given value matches at least one of the given patterns.
  ///</summary>
  ///<param name="value">The file path, value or string to match.</param>
  ///<param name="patterns">The list of patterns containing wildcards.</param>
  ///<returns>Returns true if the value matches the any pattern, otherwise false.</returns>
  bool wildcard_match_any(const std::string& value, const std::vector<std::string>& patterns);

  ///<summary>
  ///Checks if a given value matches at of the given patterns.
  ///</summary>
  ///<param name="value">The file path, value or string to match.</param>
  ///<param name="patterns">The list of patterns containing wildcards.</param>
  ///<returns>Returns true if the value matches the any pattern, otherwise false.</returns>
  bool wildcard_match_all(const std::string& value, const std::vector<std::string>& patterns);

}; //bin2cpp

#endif //BIN2CPP_COMMON_H
