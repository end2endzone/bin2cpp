#pragma once

#include <string>

namespace bin2cpp
{

  /*
  Description:
    Returns the md5 hexadecial digest of a file.
    Returns an empty string if not found.
  */
  std::string getFileHexDigest(const char * iFilePath);

  /*
  Description:
    Returns the md5 hexadecial digest of an embeded source file.
    Returns an empty string if not found.
  */
  std::string getGeneratedFileHexDigest(const char * iFilePath);

}; //bin2cpp
