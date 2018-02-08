#pragma once

#include "stringfunc.h"

#include <sstream>  //for std::stringstream

namespace stringfunc
{
  template <class T>
  inline bool parseValueT (const std::string& str, T & t)
  {
    static const T ZERO = (T)0;
    static const T MULTIPLIER = (T)10;
    static const T SIGN_MULTIPLIER = (T)-1;
    bool parseOK = false;
    t = ZERO;
    for(size_t i=0; i<str.size(); i++)
    {
      char c = str[i];
      if (c >= '0' && c <= '9')
      {
        t *= MULTIPLIER;
        c -= '0'; //convert character to numeric value
        t += (T)c;
        parseOK = true;
      }
      else if (c == '-')
      {
        t *= SIGN_MULTIPLIER;
        parseOK = true;
      }
    }
    return parseOK;
  }

  //Note: http://www.parashift.com/c++-faq-lite/misc-technical-issues.html#faq-39.2
  template <class T>
  inline std::string toStringT (const T & t)
  {
    std::stringstream out;
    out << t;
    std::string s;
    s = out.str().c_str();
    return s;
  }

  //template <class T>
  //inline void toT (const char * iValue, T & t)
  //{
  //  std::string tmpString = iValue;
  //  std::istringstream inputStream(tmpString);
  //  inputStream >> t;
  //}

  //specializations
  template<>
  inline std::string toStringT<unsigned char>(const unsigned char & t)
  {
    std::stringstream out;
    out << (int)t;
    std::string s;
    s = out.str();
    return s;
  }
  template<>
  inline std::string toStringT<char>(const char & t)
  {
    std::stringstream out;
    out << (int)t;
    std::string s;
    s = out.str();
    return s;
  }

  //template<>
  //inline void toT<unsigned char>(const char * iValue, unsigned char & t)
  //{
  //  std::string tmpString = iValue;
  //  std::istringstream inputStream(tmpString);
  //  uint16 tmp = 0;
  //  inputStream >> tmp;
  //  t = (unsigned char)tmp;
  //}
  //template<>
  //inline void toT<char>(const char * iValue, char & t)
  //{
  //  std::string tmpString = iValue;
  //  std::istringstream inputStream(tmpString);
  //  sint16 tmp = 0;
  //  inputStream >> tmp;
  //  t = (char)tmp;
  //}

  bool isNumeric(const char * iValue)
  {
    int intValue = atoi(iValue);
  
    //try to convert the int value back to string and check for equality.
    static const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    _itoa(intValue, buffer, 10);
    if (std::string(buffer) == iValue)
      return true;
    return false;
  }

  int strReplace(std::string & iString, const char * iOldValue, const char * iNewValue)
  {
    std::string tmpOldValue = iOldValue;
    std::string tmpNewValue = iNewValue;

    int numOccurance = 0;

    if (tmpOldValue.size() > 0)
    {
      size_t startPos = 0;    
      size_t findPos = std::string::npos;
      do
      {
          findPos = iString.find(tmpOldValue, startPos);
          if (findPos != std::string::npos)
          {
            iString.replace(findPos, tmpOldValue.length(), tmpNewValue);
            startPos = findPos + tmpNewValue.length();
            numOccurance++;
          }
      }
      while (findPos != -1);
    }
    return numOccurance;
  }

  std::string toString(const uint64_t & value)
  {
    return toStringT(value);
  }

  bool parseValue(const std::string& str, uint64_t & oValue)
  {
    return parseValueT(str, oValue);
  }

  std::string capitalizeFirstCharacter(const std::string & iValue)
  {
    std::string copy = iValue;
    if (!copy.empty())
    {
      copy[0] = toupper(copy[0]);
    }
    return copy;
  }

  std::string uppercase(const std::string & value)
  {
    std::string copy = value;
    for(size_t i=0; i<copy.size(); i++)
    {
      copy[i] = (char)toupper(copy[i]);
    }
    return copy;
  }

  std::string lowercase(const std::string & value)
  {
    std::string copy = value;
    for(size_t i=0; i<copy.size(); i++)
    {
      copy[i] = (char)tolower(copy[i]);
    }
    return copy;
  }

}; //stringfunc

std::string& operator<<(std::string& str, const std::string & value)
{
  str.append(value);
  return str;
}

std::string& operator<<(std::string& str, const int & value)
{
  char buffer[1024];
  sprintf(buffer, "%d", value);
  str.append(buffer);
  return str;
}

std::string& operator<<(std::string& str, const size_t & value)
{
  std::stringstream out;
  out << value;
  str.append( out.str() );
  return str;
}

std::string& operator<<(std::string& str, const uint64_t & value)
{
  std::stringstream out;
  out << value;
  str.append( out.str() );
  return str;
}
