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

#ifndef TEMPLATEPROCESSOR_H
#define TEMPLATEPROCESSOR_H

#include <string>
#include <set>
#include "ITemplateVariableLookup.h"

namespace bin2cpp
{

  ///<summary>
  ///A class for processing a template containing template markers variables
  ///and replace those to their actual values.
  ///</summary>
  class TemplateProcessor
  {
  public:
    TemplateProcessor();
    virtual ~TemplateProcessor();
    virtual void reset();

    void setTemplateText(const std::string* value);
    const std::string* getTemplateText() const;

    void setTemplateVariableLookup(ITemplateVariableLookup* lookup);
    ITemplateVariableLookup* getTemplateVariableLookup() const;

    virtual void TemplateProcessor::writeStream(std::ostream& stream);
    virtual void writeString(std::string& output);
    virtual bool writeFile(const std::string& file_path);

  protected:
    virtual void processTemplate(std::ostream& output_stream, const std::string& value, std::set<std::string>& recursion_history);

    //attributes
    const std::string* mTemplateText;
    ITemplateVariableLookup* mVariableLookup;
  };

}; //bin2cpp

#endif //TEMPLATEPROCESSOR_H
