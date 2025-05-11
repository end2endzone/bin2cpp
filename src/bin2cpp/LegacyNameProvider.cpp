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

#include "LegacyNameProvider.h"

#include "common.h"

#include "rapidassist/strings.h"
#include "rapidassist/filesystem.h"

namespace bin2cpp
{
  LegacyNameProvider::LegacyNameProvider()
  {
  }

  LegacyNameProvider::~LegacyNameProvider()
  {
  }

  std::string LegacyNameProvider::getDefaultFunctionIdentifier(const std::string& path, Dictionary& dict)
  {
    std::string output;

    //use the file name without extension as 'identifier'.
    output = getUniqueFunctionIdentifierFromPath(path.c_str(), dict);
    output = ra::strings::CapitalizeFirstCharacter(output);

    return output;
  }

  std::string LegacyNameProvider::getDefaultHeaderFile(const std::string& path)
  {
    std::string output;

    //use the file name without extension as 'headerfile'.
    output = ra::filesystem::GetFilenameWithoutExtension(path.c_str());
    output += ".h";

    return output;
  }

}; //bin2cpp