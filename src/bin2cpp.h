#pragma once

#include <string>

namespace bin2cpp
{
  class ErrorCodes
  {
  public:
    enum Enum { Success, MissingArguments, InputFileNotFound, UnableToCreateOutputFiles, OutputFilesAlreadyExist, OutputFilesSkipped };

    ErrorCodes(const ErrorCodes & iEnum);
    ErrorCodes(int iValue);
    inline operator const Enum & () const                          
    {                                                       
      return mValue;                                        
    }                                                       
    inline operator Enum & ()                                      
    {                                                       
      return mValue;                                        
    }
    ErrorCodes & operator = (const int iValue);               
    ErrorCodes & operator = (const Enum & iValue);             
    ErrorCodes & operator = (const ErrorCodes & iValue);       
    bool operator == (const Enum & iValue);                  
    bool operator != (const Enum & iValue);                   
    int getValue() const; 

  private:                                                
    Enum mValue;
  };

  /*
  Description:
    Creates a header and cpp file for embedding a given file into C++ source code.
  Parameters:
    iInputFilename:       The path of the file to embeded into C++ source code.
    iOutputFolder:        The output folder where to save the generated files.
    iHeaderFilename:      The filename of the header file (*.h) that is used for getting access to the functions.
    iFunctionIdentifier:  The unique identifier name for each data accessor functions.
    iChunkSize:           The size in bytes of each chunk of data
  */
  bin2cpp::ErrorCodes createCppEmbeddedFile(const char * iInputFilename, const char * iOutputFolder, const char * iHeaderFilename, const char * iFunctionIdentifier, size_t iChunkSize, bool iOverrideExisting);

  /*
  Description:
    Returns a textual description of ErrorCodes
  */
  const char * getErrorCodeDescription(bin2cpp::ErrorCodes iErrorCode);

  /*
  Description:
    Returns the application version number.
  */
  double getVersionNumber();
  const char * getVersionString();

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
