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

#ifndef ITEMPLATE_VARIABLE_LOOKUP_H
#define ITEMPLATE_VARIABLE_LOOKUP_H

#include "enums.h"
#include <string>
#include <ostream>

namespace bin2cpp
{

  class ITemplateVariableLookup
  {
  public:
    virtual ~ITemplateVariableLookup()
    {}

    ///<summary>
    ///Get the associated flags for the given template variable name.
    ///</summary>
    ///<param name="name">The name of the template variable.</param>
    ///<return>Returns the flags for template variable of the given name.<return>
    virtual TemplateVariableFlags getTemplateVariableFlags(const std::string& name) = 0;

    ///<summary>
    ///Write the value of the given template variable name to the output stream.
    ///</summary>
    ///<param name="name">The name of the template variable.</param>
    ///<param name="output">The variable where to output the template variable's value.</param>
    virtual void writeTemplateVariable(const std::string& name, std::ostream& output) = 0;

    ///<summary>
    ///Write the value of the given template variable name to the output string.
    ///</summary>
    ///<param name="name">The name of the template variable.</param>
    ///<param name="output">The variable where to output the template variable's value.</param>
    virtual void writeTemplateVariable(const std::string& name, std::string& output) = 0;

  };
  
}; //bin2cpp

#endif //ITEMPLATE_VARIABLE_LOOKUP_H
