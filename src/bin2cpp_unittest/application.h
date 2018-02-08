#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <vector>

class Application
{
  //------------------------
  // Singleton
  //------------------------
private:
  Application();
public:
  ~Application();
  static Application & getInstance();

public:
  //------------------------
  // Inner classes & structures
  //------------------------

  typedef std::vector<std::string> StringVector;

public:
  //------------------------
  // Public Methods
  //------------------------

  ///<summary>
  ///Initialize the application with command line arguments
  ///</summary>
  ///<param name="argc">Argument count</param>
  ///<param name="argv">The argument c-string values array.</param>
  void init(int argc, char * const * argv);

  ///<summary>
  ///Provides the list of arguments.
  ///</summary>
  ///<returns>A vector of string which contains the list of all arguments</returns>
  const StringVector & getArguments() const;

private:
  //------------------------
  // Private Methods
  //------------------------

  StringVector mArguments;
};

#endif //APPLICATION_H
