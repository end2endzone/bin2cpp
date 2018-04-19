#include "TestStringFunc.h"
#include "stringfunc.h"

using namespace stringfunc;

namespace stringfunc { namespace test
{
  
  //--------------------------------------------------------------------------------------------------
  void TestStringFunc::SetUp()
  {
  }
  //--------------------------------------------------------------------------------------------------
  void TestStringFunc::TearDown()
  {
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestStringFunc, testIsNumeric)
  {
    //lazy test
    for(int i = -100; i <= 100; i++)
    {
      for(int j=0; j<=100; j++)
      {
        //build a string from i and j
        static const int BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];

        //integers
        sprintf(buffer, "%d", i);
        ASSERT_TRUE(stringfunc::isNumeric(buffer)) << "isNumeric(\"" << buffer << "\") returned false.";

        //floating point
        sprintf(buffer, "%d.%03d", i, j);
        ASSERT_TRUE(stringfunc::isNumeric(buffer)) << "isNumeric(\"" << buffer << "\") returned false.";
      }
    }

    //test special case
    ASSERT_TRUE(stringfunc::isNumeric("+12"));

    //plus or minus sign error
    ASSERT_FALSE(stringfunc::isNumeric("12+123"));
    ASSERT_FALSE(stringfunc::isNumeric("12-123"));
    ASSERT_FALSE(stringfunc::isNumeric("+12+123"));
    ASSERT_FALSE(stringfunc::isNumeric("+12-123"));

    //multiple dots
    ASSERT_FALSE(stringfunc::isNumeric("12.345.67"));
    ASSERT_FALSE(stringfunc::isNumeric("+12.345.67"));

    //alpha characters
    ASSERT_FALSE(stringfunc::isNumeric("+12.34a"));
    ASSERT_FALSE(stringfunc::isNumeric("+12.34!"));
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestStringFunc, testStrReplace)
  {
    //at the beginning
    {
      const std::string EXPECTED = "DeaDbeef";
      std::string str = "deadbeef";
      int numReplacements = strReplace(str, "d", "D");
      ASSERT_EQ(2, numReplacements);
      ASSERT_EQ(EXPECTED, str);
    }

    //at the end
    {
      const std::string EXPECTED = "beefDeaD";
      std::string str = "beefdead";
      int numReplacements = strReplace(str, "d", "D");
      ASSERT_EQ(2, numReplacements);
      ASSERT_EQ(EXPECTED, str);
    }

    //replace everything
    {
      const std::string EXPECTED = "aabbccddeeff";
      std::string str = "deadbeef";
      int numReplacements = strReplace(str, "deadbeef", "aabbccddeeff");
      ASSERT_EQ(1, numReplacements);
      ASSERT_EQ(EXPECTED, str);
    }

    //replace nothing
    {
      const std::string EXPECTED = "deadbeef";
      std::string str = "deadbeef";
      int numReplacements = strReplace(str, "notfound", "error");
      ASSERT_EQ(0, numReplacements);
      ASSERT_EQ(EXPECTED, str);
    }

    //replace short string by longer string
    {
      const std::string EXPECTED = "super deadbeef";
      std::string str = "deadbeef";
      int numReplacements = strReplace(str, "dead", "super dead");
      ASSERT_EQ(1, numReplacements);
      ASSERT_EQ(EXPECTED, str);
    }

    //replace long string by short string
    {
      const std::string EXPECTED = "dead!";
      std::string str = "deadbeef";
      int numReplacements = strReplace(str, "beef", "!");
      ASSERT_EQ(1, numReplacements);
      ASSERT_EQ(EXPECTED, str);
    }
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestStringFunc, testToStringParseValue)
  {
    //uint64_t
    struct uint64_test
    {
      const char * EXPECTED_STR;
      const uint64_t EXPECTED_VALUE;
    };
    static const uint64_test tests[] = {
      {"0", 0ull},
      {"54321", 54321ull},
      {"1234567890123456789", 1234567890123456789ull},
      {"576460752303423488", 0x800000000000000ull},
      {"1152921504606846975", 0xFFFFFFFFFFFFFFFull},
    };
    size_t numTests = sizeof(tests)/sizeof(tests[0]);
    for(size_t i=0; i<numTests; i++)
    {
      const char * EXPECTED_STR = tests[i].EXPECTED_STR;
      const uint64_t & EXPECTED_VALUE = tests[i].EXPECTED_VALUE;

      //assert toString()
      std::string actualStr = toString( EXPECTED_VALUE );
      ASSERT_EQ(EXPECTED_STR, actualStr);

      //assert parseValue()
      uint64_t actualValue = 0;
      bool parsed = parseValue(EXPECTED_STR, actualValue);
      ASSERT_TRUE(parsed);
      ASSERT_EQ(EXPECTED_VALUE, actualValue);
    }
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestStringFunc, testCapitalizeFirstCharacter)
  {
    {
      //normal
      const std::string EXPECTED = "Deadbeef";
      const std::string value = "deadbeef";
      std::string actual = capitalizeFirstCharacter(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //empty
      const std::string EXPECTED = "";
      const std::string value = "";
      std::string actual = capitalizeFirstCharacter(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //single letter
      const std::string EXPECTED = "A";
      const std::string value = "a";
      std::string actual = capitalizeFirstCharacter(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //special character
      const std::string EXPECTED = "#!/bin/bash";
      const std::string value = "#!/bin/bash";
      std::string actual = capitalizeFirstCharacter(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //spaces
      const std::string EXPECTED = " foo";
      const std::string value = " foo";
      std::string actual = capitalizeFirstCharacter(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //numbers
      const std::string EXPECTED = "5foo";
      const std::string value = "5foo";
      std::string actual = capitalizeFirstCharacter(value);
      ASSERT_EQ(EXPECTED, actual);
    }
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestStringFunc, testUppercase)
  {
    {
      //normal
      const std::string EXPECTED = "DEADBEEF";
      const std::string value = "deadbeef";
      std::string actual = uppercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //empty
      const std::string EXPECTED = "";
      const std::string value = "";
      std::string actual = uppercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //single letter
      const std::string EXPECTED = "A";
      const std::string value = "a";
      std::string actual = uppercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //special character
      const std::string EXPECTED = "#!/BIN/BASH";
      const std::string value = "#!/bin/bash";
      std::string actual = uppercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //spaces
      const std::string EXPECTED = " FOO";
      const std::string value = " foo";
      std::string actual = uppercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //numbers
      const std::string EXPECTED = "5FOO";
      const std::string value = "5foo";
      std::string actual = uppercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestStringFunc, testLowercase)
  {
    {
      //normal
      const std::string EXPECTED = "deadbeef";
      const std::string value = "DeAdBeEf";
      std::string actual = lowercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //empty
      const std::string EXPECTED = "";
      const std::string value = "";
      std::string actual = lowercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //single letter
      const std::string EXPECTED = "a";
      const std::string value = "A";
      std::string actual = lowercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //special character
      const std::string EXPECTED = "#!/bin/bash";
      const std::string value = "#!/bIn/BaSh";
      std::string actual = lowercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //spaces
      const std::string EXPECTED = " foo";
      const std::string value = " fOO";
      std::string actual = lowercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //numbers
      const std::string EXPECTED = "5foo";
      const std::string value = "5fOO";
      std::string actual = lowercase(value);
      ASSERT_EQ(EXPECTED, actual);
    }
  }
  //--------------------------------------------------------------------------------------------------
  TEST_F(TestStringFunc, testStreamOperator)
  {
    {
      //const void * value
      const std::string HEADER = "fooBAR";
      const std::string EXPECTED = "fooBAR0x12345678";
      const void * value = (const void *)0x12345678;
      std::string actual = HEADER;
      actual << value;
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //const std::string & value
      const std::string HEADER = "fooBAR";
      const std::string EXPECTED = "fooBARtest";
      const std::string value = "test";
      std::string actual = HEADER;
      actual << value;
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //const char * value
      const std::string HEADER = "fooBAR";
      const std::string EXPECTED = "fooBARtest";
      const char * value = "test";
      std::string actual = HEADER;
      actual << value;
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //const int8_t & value
      const std::string HEADER = "fooBAR";
      const std::string EXPECTED = "fooBAR-23";
      const int8_t value = -23;
      std::string actual = HEADER;
      actual << value;
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //const uint8_t & value
      const std::string HEADER = "fooBAR";
      const std::string EXPECTED = "fooBAR200";
      const uint8_t value = 200;
      std::string actual = HEADER;
      actual << value;
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //const int16_t & value
      const std::string HEADER = "fooBAR";
      const std::string EXPECTED = "fooBAR-31234";
      const int16_t value = -31234;
      std::string actual = HEADER;
      actual << value;
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //const uint16_t & value
      const std::string HEADER = "fooBAR";
      const std::string EXPECTED = "fooBAR51234";
      const uint16_t value = 51234;
      std::string actual = HEADER;
      actual << value;
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //const int32_t & value
      const std::string HEADER = "fooBAR";
      const std::string EXPECTED = "fooBAR-2147483600";
      const int32_t value = -2147483600l; //min is -2147483648
      std::string actual = HEADER;
      actual << value;
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //const uint32_t & value
      const std::string HEADER = "fooBAR";
      const std::string EXPECTED = "fooBAR4123456789";
      const uint32_t value = 4123456789ul; //max is 4294967295
      std::string actual = HEADER;
      actual << value;
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //const int64_t & value
      const std::string HEADER = "fooBAR";
      const std::string EXPECTED = "fooBAR-9123456789012345678";
      const int64_t value = -9123456789012345678ll; //min is -9223372036854775808
      std::string actual = HEADER;
      actual << value;
      ASSERT_EQ(EXPECTED, actual);
    }
    {
      //const uint64_t & value
      const std::string HEADER = "fooBAR";
      const std::string EXPECTED = "fooBAR18123456789012345678";
      const uint64_t value = 18123456789012345678ull; //max is 18446744073709551615
      std::string actual = HEADER;
      actual << value;
      ASSERT_EQ(EXPECTED, actual);
    }
  }
  //--------------------------------------------------------------------------------------------------
} // End namespace test
} // End namespace stringfunc
