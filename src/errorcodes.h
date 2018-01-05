#pragma once

#include <string>

namespace bin2cpp
{

  class ErrorCodes
  {
  public:
    enum Enum { Success, MissingArguments, InputFileNotFound, UnableToCreateOutputFiles, OutputFileAlreadyExist, OutputFilesSkipped };

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
    Returns a textual description of ErrorCodes
  */
  const char * getErrorCodeDescription(bin2cpp::ErrorCodes iErrorCode);

}; //bin2cpp
