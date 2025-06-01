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

#include "TestTemplateProcessor.h"

#include "TemplateProcessor.h"

#include "rapidassist/testing.h"
#include "rapidassist/filesystem.h"
#include "rapidassist/testing.h"
#include "rapidassist/user.h"

 // Sample variable lookup implementation
class SampleVariableLookup : public bin2cpp::ITemplateVariableHandler
{
public:
  bin2cpp::TemplateVariableFlags getTemplateVariableFlags(const std::string& name)
  {
    if ( name == "first-name" ) return bin2cpp::TEMPLATE_VARIABLE_FLAG_STRINGNABLE;
    if ( name == "last-name" ) return bin2cpp::TEMPLATE_VARIABLE_FLAG_STRINGNABLE;
    if ( name == "full-name" ) return bin2cpp::TEMPLATE_VARIABLE_FLAG_STRINGNABLE;
    if ( name == "age" ) return bin2cpp::TEMPLATE_VARIABLE_FLAG_STRINGNABLE;
    if ( name == "job" ) return bin2cpp::TEMPLATE_VARIABLE_FLAG_STRINGNABLE;
    if ( name == "children" ) return bin2cpp::TEMPLATE_VARIABLE_FLAG_STRINGNABLE;
    if ( name == "foo" ) return bin2cpp::TEMPLATE_VARIABLE_FLAG_STRINGNABLE;
    if ( name == "bar" ) return bin2cpp::TEMPLATE_VARIABLE_FLAG_STRINGNABLE;
    if ( name == "baz" ) return bin2cpp::TEMPLATE_VARIABLE_FLAG_STRINGNABLE;
    return bin2cpp::TEMPLATE_VARIABLE_FLAG_NONE;
  }

  void writeTemplateVariable(const std::string& name, std::ostream& output)
  {}

  inline std::string getTemplateVariableAsString(const std::string& name)
  {
    if ( name == "first-name" ) return "Luke";
    if ( name == "last-name" ) return "Skywalker";
    if ( name == "full-name" ) return "${first-name} ${last-name}";
    if ( name == "age" ) return "53";
    if ( name == "job" ) return "Jedi Knight";
    if ( name == "children" ) return "2";
    if ( name == "foo" ) return "foo is ${bar}";
    if ( name == "bar" ) return "bar is ${baz}";
    if ( name == "baz" ) return "baz is ${foo}";
    return "";
  }

  void writeTemplateVariable(const std::string& name, std::string& output) override
  {
    output = getTemplateVariableAsString(name);
  }
};

void TestTemplateProcessor::SetUp()
{
}

void TestTemplateProcessor::TearDown()
{
}

TEST_F(TestTemplateProcessor, testBaseSingleVariable)
{
  bin2cpp::TemplateProcessor processor;
  SampleVariableLookup lookup;
  processor.setTemplateVariableHandler(&lookup);

  const std::string actual_input =    "I am ${age} years old.";
  const std::string expected_output = "I am 53 years old.";
  processor.setTemplateText(&actual_input);
  std::string actual_output;
  processor.writeString(actual_output);
  ASSERT_EQ(actual_output, expected_output);
}

TEST_F(TestTemplateProcessor, testUnknownVariable)
{
  bin2cpp::TemplateProcessor processor;
  SampleVariableLookup lookup;
  processor.setTemplateVariableHandler(&lookup);

  const std::string actual_input =    "My ${father} tried to kill me.";
  const std::string expected_output = "My  tried to kill me.";
  processor.setTemplateText(&actual_input);
  std::string actual_output;
  processor.writeString(actual_output);
  ASSERT_EQ(actual_output, expected_output);
}

TEST_F(TestTemplateProcessor, testCaseSensitive)
{
  bin2cpp::TemplateProcessor processor;
  SampleVariableLookup lookup;
  processor.setTemplateVariableHandler(&lookup);

  const std::string actual_input =    "The variable '${children}' should expand to a value but '${Children}' should be empty.";
  const std::string expected_output = "The variable '2' should expand to a value but '' should be empty.";
  processor.setTemplateText(&actual_input);
  std::string actual_output;
  processor.writeString(actual_output);
  ASSERT_EQ(actual_output, expected_output);
}

TEST_F(TestTemplateProcessor, testRecursive)
{
  bin2cpp::TemplateProcessor processor;
  SampleVariableLookup lookup;
  processor.setTemplateVariableHandler(&lookup);

  const std::string actual_input =    "My name is ${full-name}. I work as a ${job} now.";
  const std::string expected_output = "My name is Luke Skywalker. I work as a Jedi Knight now.";
  processor.setTemplateText(&actual_input);
  std::string actual_output;
  processor.writeString(actual_output);
  ASSERT_EQ(actual_output, expected_output);
}

TEST_F(TestTemplateProcessor, testMultipleTwinMarkers)
{
  bin2cpp::TemplateProcessor processor;
  SampleVariableLookup lookup;
  processor.setTemplateVariableHandler(&lookup);

  const std::string actual_input =    "My name is ${first-name} but everyone calls me Lucky-${first-name}.";
  const std::string expected_output = "My name is Luke but everyone calls me Lucky-Luke.";
  processor.setTemplateText(&actual_input);
  std::string actual_output;
  processor.writeString(actual_output);
  ASSERT_EQ(actual_output, expected_output);
}

TEST_F(TestTemplateProcessor, testCircularReference)
{
  bin2cpp::TemplateProcessor processor;
  SampleVariableLookup lookup;
  processor.setTemplateVariableHandler(&lookup);

  const std::string actual_input =    "<foo>${foo}</foo>";
  const std::string expected_output = "<foo>foo is bar is baz is </foo>";
  processor.setTemplateText(&actual_input);
  std::string actual_output;
  processor.writeString(actual_output);
  ASSERT_EQ(actual_output, expected_output);
}
