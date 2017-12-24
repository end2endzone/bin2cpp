#pragma once

#include "common.h"
#include "..\version_info.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>

namespace bin2cpp
{

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
    itoa(intValue, buffer, 10);
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

  const char * getVersionString()
  {
    return BINCPP_VERSION;
  }

}; //bin2cpp
