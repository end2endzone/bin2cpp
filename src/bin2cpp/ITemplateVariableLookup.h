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
    ///Lookup the string value for a given template variable name.
    ///String template variables supports recursive lookup.
    ///</summary>
    ///<param name="name">The name of the template variable.</param>
    ///<param name="output">The output string for getting the value or the template variable.</param>
    ///<return>Returns true when the template variable is handled by the instance. Returns false otherwise.<return>
    virtual bool lookupStringVariable(const std::string& name, std::string& output) = 0;

    ///<summary>
    ///Lookup the string value for a given template variable name.
    ///Stream template variables do not support template variables recursive lookup.
    ///If a streams loopup outputs text such as `${foo}`, it will not be processed and will be output as literal string `${foo}`.
    ///</summary>
    ///<param name="name">The name of the template variable.</param>
    ///<param name="output">The output string for getting the value or the template variable.</param>
    ///<return>Returns true when the template variable is handled by the instance. Returns false otherwise.<return>
    virtual bool lookupStreamVariable(const std::string& name, std::ostream& output) = 0;

  };
  
}; //bin2cpp

#endif //ITEMPLATE_VARIABLE_LOOKUP_H
