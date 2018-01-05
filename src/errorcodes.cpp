#pragma once

#include "errorcodes.h"

namespace bin2cpp
{

  ErrorCodes::ErrorCodes(const ErrorCodes & iEnum)                      
  {                                                       
    mValue = iEnum.mValue;                                
  }                                                       
  ErrorCodes::ErrorCodes(int iValue)                                   
  {                                                       
    mValue = Enum(iValue);                                
  }                                                       
  ErrorCodes & ErrorCodes::operator = (const int iValue)               
  {                                                       
    mValue = Enum(iValue);                                
    return ( *this );                                     
  }                                                       
  ErrorCodes & ErrorCodes::operator = (const Enum & iValue)            
  {                                                       
    mValue = iValue;                                      
    return ( *this );                                     
  }                                                       
  ErrorCodes & ErrorCodes::operator = (const ErrorCodes & iValue)       
  {                                                       
    mValue = iValue.mValue;                               
    return ( *this );                                     
  }                                                       
  bool ErrorCodes::operator == (const Enum & iValue)                  
  {                                                       
    return (mValue == iValue);                            
  }                                                       
  bool ErrorCodes::operator != (const Enum & iValue)                  
  {                                                       
    return (mValue != iValue);                            
  }                                                       
  int ErrorCodes::getValue() const                                    
  {                                                       
    return mValue;                                        
  }                                                       

  const char * getErrorCodeDescription(bin2cpp::ErrorCodes iErrorCode)
  {
    //Success, MissingArguments, InputFileNotFound, UnableToCreateOutputFiles
    switch(iErrorCode)
    {
    case bin2cpp::ErrorCodes::Success:
      return "Success";
      break;
    case bin2cpp::ErrorCodes::MissingArguments:
      return "Missing arguments";
      break;
    case bin2cpp::ErrorCodes::InputFileNotFound:
      return "Unable to open input file";
      break;
    case bin2cpp::ErrorCodes::UnableToCreateOutputFiles:
      return "Unable to create output files";
      break;
    case bin2cpp::ErrorCodes::OutputFileAlreadyExist:
      return "Output file already exist";
      break;
    case bin2cpp::ErrorCodes::OutputFilesSkipped:
      return "No change to input files. Skipping file generation";
      break;
    };
    return "Unknown";
  }

}; //bin2cpp
