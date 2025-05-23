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

#include "TemplateProcessor.h"
#include <fstream>
#include <sstream>

namespace bin2cpp
{
  TemplateProcessor::TemplateProcessor()
  {
    reset();
  }

  TemplateProcessor::~TemplateProcessor()
  {
  }

  void TemplateProcessor::reset()
  {
    mTemplateText = NULL;
    mVariableLookup = NULL;
  }

  void TemplateProcessor::setTemplateText(const std::string* value)
  {
    mTemplateText = value;
  }

  const std::string* TemplateProcessor::getTemplateText() const
  {
    return mTemplateText;
  }

  void TemplateProcessor::setTemplateVariableLookup(ITemplateVariableLookup* lookup)
  {
    mVariableLookup = lookup;
  }

  ITemplateVariableLookup* TemplateProcessor::getTemplateVariableLookup() const
  {
    return mVariableLookup;
  }

  void TemplateProcessor::writeString(std::string& output)
  {
    std::ostringstream output_stream;
    std::set<std::string> recursion_history;
    processTemplate(output_stream, *mTemplateText, recursion_history);
    output = output_stream.str();
  }

  bool TemplateProcessor::writeFile(const std::string& file_path)
  {
    std::ofstream output_file(file_path);
    if ( !output_file.is_open() ) return false;

    std::set<std::string> recursion_history;
    processTemplate(output_file, *mTemplateText, recursion_history);
    return true;
  }

  //-------------------------------
  //protected methods
  //-------------------------------

  void TemplateProcessor::processTemplate(std::ostream& output_stream, const std::string& value, std::set<std::string>& recursion_history)
  {
    size_t pos = 0;
    while ( pos < value.size() )
    {
      if ( value[pos] == '$' && pos + 1 < value.size() && value[pos + 1] == '{' )
      {
        size_t end_pos = value.find('}', pos);
        if ( end_pos != std::string::npos )
        {
          std::string variable_name = value.substr(pos + 2, end_pos - pos - 2);

          // Detect actual circular dependency within same recursion path
          if ( recursion_history.find(variable_name) != recursion_history.end() )
          {
            pos = end_pos + 1;
            continue;
          }

          // Do the variable expansion
          std::string expanded_value = mVariableLookup ? mVariableLookup->lookupTemplateVariable(variable_name) : "";

          // Add variable to recursion history before expanding
          recursion_history.insert(variable_name);

          // Recursively process expanded value with updated recursion tracking
          processTemplate(output_stream, expanded_value, recursion_history);

          // Remove variable from recursion history after recursion returns
          recursion_history.erase(variable_name);

          pos = end_pos + 1;
        }
        else
        {
          output_stream.put(value[pos++]);
        }
      }
      else
      {
        output_stream.put(value[pos++]);
      }
    }
  }

}; //bin2cpp