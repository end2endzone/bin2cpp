#include "application.h"

Application::Application()
{
}

Application::~Application()
{
}

Application & Application::getInstance()
{
  static Application instance;
  return instance;
}

//------------------------
// Public Methods
//------------------------
void Application::init(int argc, char * const * argv)
{
  mArguments.clear();
  if (argv == NULL)
    return;
  for(int i=0; i<argc; i++)
  {
    std::string value = argv[i];
    mArguments.push_back(value);
  }
}

const Application::StringVector & Application::getArguments() const
{
  return mArguments;
}
