#pragma once

#include <stdint.h>
#include <string>
#include <vector>

namespace filesystem
{

  ///<summary>
  ///Returns the size of the given file path in bytes.
  ///</summary>
  ///<param name="f">An valid file path.</param>
  ///<return>Returns the size of the given file path in bytes.<return>
  uint32_t getFileSize(const char * iPath);

  ///<summary>
  ///Returns the size of the given FILE* in bytes.
  ///</summary>
  ///<param name="f">An valid FILE pointer.</param>
  ///<return>Returns the size of the given FILE* in bytes.<return>
  uint32_t getFileSize(FILE * f);

  ///<summary>
  ///Returns the filename of the given path.
  ///</summary>
  ///<param name="iPath">An valid file path.</param>
  ///<return>Returns the filename of the given path.<return>
  std::string getFilename(const char * iPath);

  ///<summary>
  ///Determine if a file exists.
  ///</summary>
  ///<param name="iPath">An valid file path.</param>
  ///<return>Returns true when the file exists. Returns false otherwise.<return>
  bool fileExists(const char * iPath);

  ///<summary>
  ///Determine if a folder exists.
  ///</summary>
  ///<param name="iPath">An valid folder path.</param>
  ///<return>Returns true when the folder exists. Returns false otherwise.<return>
  bool folderExists(const char * iPath);

  ///<summary>
  ///Returns the file name of a tempporary file.
  ///</summary>
  ///<return>Returns the file name of a tempporary file.<return>
  std::string getTemporaryFileName();

  ///<summary>
  ///Returns the path of a tempporary file.
  ///</summary>
  ///<return>Returns the path of a tempporary file.<return>
  std::string getTemporaryFilePath();

  ///<summary>
  ///Returns the parent element of a path. For files, returns the file's directory. For folders, returns the parent path
  ///</summary>
  ///<param name="iPath">The input path to split.</param>
  ///<return>Returns the parent element of a path.<return>
  std::string getParentPath(const std::string & iPath);

  ///<summary>
  ///Convert a long file path to the short path form (8.3 format).
  ///If the system does not support automatic conversion, an estimated
  ///version is returned.
  ///</summary>
  ///<param name="iPath">The input path to convert.</param>
  ///<return>Returns the short path form of the given path.<return>
  std::string getShortPathForm(const std::string & iPath);
 
  ///<summary>
  ///Splits a path into a folder and a filename.
  ///</summary>
  ///<param name="iPath">The input path to split.</param>
  ///<param name="oFolder">The output folder of the given path.</param>
  ///<param name="oFile">The output file of the given path.</param>
  void splitPath(const std::string & iPath, std::string & oFolder, std::string & oFilename);

  ///<summary>
  ///Splits a path into each element.
  ///</summary>
  ///<param name="iPath">The input path to split.</param>
  ///<param name="oElements">The output list which contains all path elements.</param>
  void splitPath(const std::string & iPath, std::vector<std::string> & oElements);

  ///<summary>
  ///Gets the character that represents the path separator.
  ///</summary>
  ///<return>Returns the character that represents the path separator.<return>
  char getPathSeparator();

  ///<summary>
  ///Returns the current folder
  ///</summary>
  ///<return>Returns the current folder<return>
  std::string getCurrentFolder();

  ///<summary>
  ///Returns the extension of a file.
  ///</summary>
  ///<param name="iPath">The valid path to a file.</param>
  ///<return>Returns the extension of a file.<return>
  std::string getFileExtention(const std::string & iPath);
  
  enum FileSizeEnum {Bytes, Kilobytes, Megabytes, Gigabytes, Terabytes};

  ///<summary>
  ///Returns a given size in a user friendly format and units.
  ///</summary>
  ///<param name="iPath">The size in bytes</param>
  ///<return>Returns a given size in a user friendly format and units.<return>
  std::string getUserFriendlySize(uint64_t iBytesSize);

  ///<summary>
  ///Returns a given size in a user friendly format and units.
  ///</summary>
  ///<param name="iPath">The size in bytes</param>
  ///<return>Returns a given size in a user friendly format and units.<return>
  std::string getUserFriendlySize(uint64_t iBytesSize, FileSizeEnum iUnit);

  ///<summary>
  ///Returns the modified date of the given file.
  ///Note that the function returns the number of seconds elapsed since epoch since Jan 1st 1970.
  ///</summary>
  ///<param name="iPath">The valid path to a file.</param>
  ///<return>Returns the modified date of the given file.<return>
  uint64_t getFileModifiedDate(const std::string & iPath);

}; //filesystem
