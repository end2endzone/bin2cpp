#pragma once

#include "argumentparser.h"

namespace cmdline
{
  bool parseArgument(const std::string & name, std::string & value, int argc, char **argv)
  {
    value = "";

    //look for --name=value pattern
    std::string pattern;
    pattern.append("--");
    pattern.append(name);
    pattern.append("=");
    for(int i=0; i<argc; i++)
    {
      std::string arg = argv[i];
      if (arg.length() > pattern.length() && arg.substr(0,pattern.length()) == pattern)
      {
        value = arg.substr(pattern.length(), arg.length()-pattern.length());
        return true;
      }
    }

    //look for exactly --name pattern
    pattern = "";
    pattern.append("--");
    pattern.append(name);
    for(int i=0; i<argc; i++)
    {
      std::string arg = argv[i];
      if (arg == pattern)
      {
        value = "";
        return true;
      }
    }

    return false;
  }

  bool parseArgument(const std::string & name, int & value, int argc, char **argv)
  {
    std::string tmp;
    bool found = parseArgument(name, tmp, argc, argv);
    if (found)
    {
      value = atoi(tmp.c_str());
      return true;
    }
    return false;
  }

  bool parseArgument(const std::string & name, size_t & value, int argc, char **argv)
  {
    std::string tmp;
    bool found = parseArgument(name, tmp, argc, argv);
    if (found)
    {
#ifdef _WIN32
      static const char * format = "%Iu";
#else
      static const char * format = "%zu";
#endif
      if (1 == sscanf(tmp.c_str(), format, &value))
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    return false;
  }

}; //cmdline
