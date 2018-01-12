#pragma once

#include "common.h"
#include "..\version_info.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <direct.h>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
#include <unistd.h>
#endif
#ifdef WIN32
#define stat _stat
#endif

namespace bin2cpp
{
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

  int getCopyrightYear()
  {
    static const int DEFAULT_YEAR = 2016;
    std::string compilationDate = __DATE__;
    size_t space1Pos = compilationDate.find(" ", 0);
    if (space1Pos == std::string::npos)
      return DEFAULT_YEAR;
    size_t space2Pos = compilationDate.find(" ", space1Pos+1);
    if (space2Pos == std::string::npos)
      return DEFAULT_YEAR;
    const char * yearStr = &compilationDate[space2Pos+1];
    int year = atoi(yearStr);
    return year;
  }

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

  long getFileSize(FILE * f)
  {
    if (f == NULL)
      return 0;
    long initPos = ftell(f);
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, initPos, SEEK_SET);
    return size;
  }

  std::string getFilename(const char * iPath)
  {
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    _splitpath(iPath, drive, dir, fname, ext);

    std::string filename;
    filename.append(fname);
    filename.append(ext);

    return filename;
  }

  bool fileExists(const char * iFilePath)
  {
    FILE * f = fopen(iFilePath, "rb");
    if (!f)
      return false;
    fclose(f);
    return true;
  }

  std::string getEnvironmentVariable(const char * iName)
  {
    const char * value = getenv(iName);
    if (value == NULL)
      return std::string();
    else
      return std::string(value);
  }

  std::string getTemporaryFileName()
  {
    char rndvalue[1024];
    _itoa(rand(), rndvalue, 10);

    std::string name = std::string("random") + rndvalue + ".tmp";
    return name;
  }

  std::string getTemporaryFilePath()
  {
    std::string temp = getEnvironmentVariable("TEMP");

    std::string rndpath = temp + "\\" + getTemporaryFileName();
    return rndpath;
  }

  const char * getVersionString()
  {
    return BINCPP_VERSION;
  }

  void toHexString(unsigned char c, unsigned char & c1, unsigned char & c2)
  {
    static const char * hexCharacters = "0123456789abcdef";
    c1 = hexCharacters[c/16];
    c2 = hexCharacters[c%16];
  }

  void toOctString(unsigned char c, unsigned char & c1, unsigned char & c2, unsigned char & c3)
  {
    static const char * octCharacters = "01234567";
    c3 = octCharacters[c%8];
    c /= 8;
    c2 = octCharacters[c%8];
    c /= 8;
    c1 = octCharacters[c];
  }

  bool isPrintableCharacter(const char c)
  {
    if (c == 39) // character ' must be escaped with \' which is not supported right now
      return false;
    if (c == 92) // character \ must be escaped with \\ which is not supported right now
      return false;
    if (c >= 32 && c<= 126)
      return true;
    return false;
  }

  std::string getParentPath(const std::string & iPath)
  {
    std::string parent;

    char separator = getPathSeparator();

    std::size_t offset = iPath.find_last_of("/\\");
    if (offset != std::string::npos)
    {
      //found
      parent = iPath.substr(0,offset);
    }

    return parent;
  }

  void splitPath(const std::string & iPath, std::string & oFolder, std::string & oFilename)
  {
    //char separator = getPathSeparator();

    ////reset everything
    //oFilename = "";
    //oFolder = "";

    ////start from the end and capture each character into oFilename
    //bool wDumpingInFilename = true;
    //for(int i = iPath.size() - 1; i >= 0; i--)
    //{
    //  char c = iPath[i];
    //  if (c == separator && wDumpingInFilename)
    //  {
    //    wDumpingInFilename = false;
    //  }

    //  //decide where the character will be dumped
    //  std::string * wDestination = NULL;
    //  if (wDumpingInFilename)
    //  {
    //    wDestination = &oFilename;
    //  }
    //  else
    //  {
    //    wDestination = &oFolder;
    //  }

    //  //dump c into destination string
    //  wDestination->insert(0, 1, c);
    //}

    //if (oFolder.size() == 0)
    //{
    //  oFolder = std::string(".") + separator;
    //}

    oFolder = "";
    oFilename = "";

    char separator = getPathSeparator();

    std::size_t offset = iPath.find_last_of("/\\");
    if (offset != std::string::npos)
    {
      //found
      oFolder = iPath.substr(0,offset);
      oFilename = iPath.substr(offset+1);
    }
    else
    {
      oFilename = iPath;
    }
  }

  char getPathSeparator()
  {
#ifdef _WIN32
    return '\\';
#endif
  }

  std::string getCurrentFolder()
  {
    return std::string(_getcwd(NULL, 0));
  }

  std::string getFileExtention(const std::string & iPath)
  {
    //extract filename from path to prevent
    //reading a folder's extension
    std::string folder;
    std::string filename;
    splitPath(iPath, folder, filename);

    std::string extension;
    std::size_t offset = filename.find_last_of(".");
    if (offset != std::string::npos)
    {
      //found
      //name = filename.substr(0,offset);
      extension = filename.substr(offset+1);
    }

    return extension;
  }
  
  std::string getUserFriendlySize(uint64_t iBytesSize)
  {
    static const uint64_t kbLimit = 1024;
    static const uint64_t mbLimit = kbLimit*1000;
    static const uint64_t gbLimit = 1024*mbLimit;
    static const uint64_t tbLimit = 1024*gbLimit;

    FileSizeEnum preferedUnit = Bytes;
    
    if (iBytesSize < kbLimit)
    {
      //bytes
    }
    else if (iBytesSize < mbLimit)
    {
      preferedUnit = Kilobytes;
    }
    else if (iBytesSize < gbLimit)
    {
      preferedUnit = Megabytes;
    }
    else if (iBytesSize < tbLimit)
    {
      preferedUnit = Gigabytes;
    }
    else
    {
      preferedUnit = Terabytes;
    }

    return getUserFriendlySize(iBytesSize, preferedUnit);
  }

  std::string getUserFriendlySize(uint64_t iBytesSize, FileSizeEnum iUnit)
  {
    static const uint64_t digitsPrecision = 100;
    static const uint64_t factor = 1024;
    static const uint64_t kbPrecision = 1;
    static const uint64_t mbPrecision = 1024*kbPrecision;
    static const uint64_t gbPrecision = 1024*mbPrecision;
    static const uint64_t tbPrecision = 1024*gbPrecision;

    std::string friendlySize;

    //Convert iSize to a formattedSize
    //double unitPower = double(iUnit.getValue());
    //double multiplicator = pow(1024.0, unitPower);
    //double sizeInSpecifiedUnit = double(iSize)/multiplicator;
    //double formattedSize = double(uint64_t(sizeInSpecifiedUnit * 100.0)) / 100.0;
    double formattedSize = 0.0;
    switch(iUnit)
    {
    case Bytes:
      formattedSize = double( iBytesSize );
      break;
    case Kilobytes:
      formattedSize = double( ((iBytesSize*digitsPrecision)/factor)/kbPrecision )/double(digitsPrecision);
      break;
    case Megabytes:
      formattedSize = double( uint64_t(uint64_t(iBytesSize/factor)*digitsPrecision)/mbPrecision )/double(digitsPrecision);
      break;
    case Gigabytes:
      formattedSize = double( uint64_t(uint64_t(iBytesSize/factor)*digitsPrecision)/gbPrecision )/double(digitsPrecision);
      break;
    case Terabytes:
      formattedSize = double( uint64_t(uint64_t(iBytesSize/factor)*digitsPrecision)/tbPrecision )/double(digitsPrecision);
      break;
    };

    //Add formattedSize to friendlySize
    static const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    sprintf(buffer, "%.2f", formattedSize);
    friendlySize = buffer;

    //Append unit descrition to friendlySize
    switch(iUnit)
    {
    case Bytes:
      {
        friendlySize = toStringT(iBytesSize);
        friendlySize += " bytes";
      };
      break;
    case Kilobytes:
      {
        friendlySize += " KB";
      };
      break;
    case Megabytes:
      {
        friendlySize += " MB";
      };
      break;
    case Gigabytes:
      {
        friendlySize += " GB";
      };
      break;
    case Terabytes:
      {
        friendlySize += " TB";
      };
      break;
    };

    return friendlySize;
  }

  uint64_t getFileModifiedDate(const std::string & iPath)
  {
    struct stat result;
    uint64_t mod_time = 0;
    if(stat(iPath.c_str(), &result)==0)
    {
      mod_time = result.st_mtime;
    }
    return mod_time;
  }

  std::string toString(const uint64_t & value)
  {
    return toStringT(value);
  }

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

  std::string& operator<<(std::string& str, const uint64_t & value)
  {
    std::stringstream out;
    out << value;
    str.append( out.str() );
    return str;
  }

}; //bin2cpp
