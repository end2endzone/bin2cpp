#pragma once

#include "stringfunc.h"
#include "environmentfunc.h"
#include "filesystemfunc.h"

#include <direct.h> //for _getcwd()
#include <algorithm> //for std::transform()

#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
#include <unistd.h>
#endif
#ifdef WIN32
#define stat _stat
#include <Windows.h> //for GetShortPathName()
#endif

namespace filesystem
{

  uint32_t getFileSize(const char * iPath)
  {
    if (iPath == NULL || iPath[0] == '\0')
      return 0;

    FILE * f = fopen(iPath, "rb");
    if (!f)
      return 0;
    uint32_t size = getFileSize(f);
    fclose(f);
    return size;
  }

  uint32_t getFileSize(FILE * f)
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
    if (iPath == NULL || iPath[0] == '\0')
      return std::string();

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

  bool fileExists(const char * iPath)
  {
    if (iPath == NULL || iPath[0] == '\0')
      return false;

    FILE * f = fopen(iPath, "rb");
    if (!f)
      return false;
    fclose(f);
    return true;
  }

  bool folderExists(const char * iPath)
  {
    if (iPath == NULL || iPath[0] == '\0')
      return false;

    std::string localFolder = getCurrentFolder();
    bool success = (_chdir(iPath) == 0);
    if (success)
      _chdir(localFolder.c_str());
    return success;
  }

  std::string getTemporaryFileName()
  {
    //get positive random value
    int value = rand();
    if (value < 0)
      value *= -1;

    //RAND_MAX expected to get as high as 32768
    //5 characters is required for printing the value

    char str[1024];
    sprintf(str, "random.%05d.tmp", value);

    return std::string(str);
  }

  std::string getTemporaryFilePath()
  {
#ifdef WIN32
    std::string temp = environment::getEnvironmentVariable("TEMP");
#elif UNIX
    std::string temp = "/tmp";
#endif
    std::string rndpath = temp + getPathSeparator() + getTemporaryFileName();
    return rndpath;
  }

  std::string getParentPath(const std::string & iPath)
  {
    std::string parent;

    char separator = filesystem::getPathSeparator();

    std::size_t offset = iPath.find_last_of("/\\");
    if (offset != std::string::npos)
    {
      //found
      parent = iPath.substr(0,offset);
    }

    return parent;
  }

  std::string getShortPathFormEstimation(const std::string & iPath)
  {
    std::string shortPath;

    std::vector<std::string> pathElements;
    splitPath(iPath, pathElements);
    for(size_t i=0; i<pathElements.size(); i++)
    {
      const std::string & element = pathElements[i];
      if (element.size() > 12 || element.find(' ') != std::string::npos)
      {
        std::string element83 = element;
        std::string ext = getFileExtention(element);
        stringfunc::strReplace(element83, (std::string(".")+ext).c_str(), ""); //remove extension from filename
        stringfunc::strReplace(ext, " ", ""); //remove spaces in extension
        ext = ext.substr(0, 3); //truncate file extension
        stringfunc::strReplace(element83, " ", ""); //remove spaces
        element83 = element83.substr(0, 6); //truncate file name
        element83.append("~1");
        if (!ext.empty())
        {
          element83.append(".");
          element83.append(ext);
        }
 
        //uppercase everything
        std::transform(element83.begin(), element83.end(), element83.begin(), ::toupper);
 
        //add to shortPath
        if (!shortPath.empty())
          shortPath.append("\\");
        shortPath.append(element83);
      }
      else
      {
        if (!shortPath.empty())
          shortPath.append("\\");
        shortPath.append(element);
      }
    }

    return shortPath;
  }

  std::string getShortPathFormWin32(const std::string & iPath)
  {
    std::string shortPath;

  #ifdef _WIN32
    // First obtain the size needed by passing NULL and 0.
    long length = GetShortPathName(iPath.c_str(), NULL, 0);
    if (length == 0)
      return "";
 
    // Dynamically allocate the correct size
    // (terminating null char was included in length)
    char * buffer = new char[length];
 
    // Now simply call again using same long path.
    length = GetShortPathName(iPath.c_str(), buffer, length);
    if (length == 0)
      return "";
 
    shortPath = buffer;
 
    delete [] buffer;
  #endif

    return shortPath;
  }

  std::string getShortPathForm(const std::string & iPath)
  {
#ifdef WIN32
    if (fileExists(iPath.c_str()) || folderExists(iPath.c_str()))
    {
      //file must exist to use WIN32 api
      return getShortPathFormWin32(iPath);
    }
    else
    {
      return getShortPathFormEstimation(iPath);
    }
#elif UNIX
    //no such thing as short path form in unix
    return getShortPathFormEstimation(iPath);
#endif
  }

  void splitPath(const std::string & iPath, std::string & oFolder, std::string & oFilename)
  {
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

  void splitPath(const std::string & iPath, std::vector<std::string> & oElements)
  {
    oElements.clear();
    std::string s = iPath;
    std::string accumulator;
    for(unsigned int i = 0; i<s.size(); i++)
    {
      const char & c = s[i];
      if ((c == '/' || c == '\\'))
      {
        if (accumulator.size() > 0)
        {
          oElements.push_back(accumulator);
          accumulator = "";
        }
      }
      else
        accumulator += c;
    }
    if (accumulator.size() > 0)
    {
      oElements.push_back(accumulator);
      accumulator = "";
    }
  }

  char getPathSeparator()
  {
#ifdef _WIN32
    return '\\';
#elif UNIX
    return '/';
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
        friendlySize = stringfunc::toString(iBytesSize);
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

}; //filesystem
