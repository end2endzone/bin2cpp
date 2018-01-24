#pragma once

#include <string>

namespace cmdline
{
  ///<summary>
  ///Parses an argument from the command line parameters (argc, argv)
  ///The argument must be in the form of --name=value or --name
  ///</summary>
  ///<param name="name">Name of the argument. Case sensitive</param>
  ///<param name="value">The value of the argument in --name=value</param>
  ///<param name="argc">The number of element in argv</param>
  ///<param name="argv">The array of const char * representing each argument within the command line.</param>
  ///<return>True when an argument named [name] is found. False otherwise.<return>
  bool parseArgument(const std::string & name, std::string & value, int argc, char **argv);

  ///<summary>
  ///Parses an argument from the command line parameters (argc, argv)
  ///The argument must be in the form of --name=value or --name
  ///</summary>
  ///<param name="name">Name of the argument. Case sensitive</param>
  ///<param name="value">The value of the argument in --name=value</param>
  ///<param name="argc">The number of element in argv</param>
  ///<param name="argv">The array of const char * representing each argument within the command line.</param>
  ///<return>True when an argument named [name] is found. False otherwise.<return>
  bool parseArgument(const std::string & name, int & value, int argc, char **argv);

  ///<summary>
  ///Parses an argument from the command line parameters (argc, argv)
  ///The argument must be in the form of --name=value or --name
  ///</summary>
  ///<param name="name">Name of the argument. Case sensitive</param>
  ///<param name="value">The value of the argument in --name=value</param>
  ///<param name="argc">The number of element in argv</param>
  ///<param name="argv">The array of const char * representing each argument within the command line.</param>
  ///<return>True when an argument named [name] is found. False otherwise.<return>
  bool parseArgument(const std::string & name, size_t & value, int argc, char **argv);

}; //cmdline
