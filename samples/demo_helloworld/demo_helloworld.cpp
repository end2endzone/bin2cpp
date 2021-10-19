#include <stdio.h>
#include <string>
#include <iostream>

#include "helloworld.h" //a single include file is all you need

int main(int argc, char* argv[])
{
  //get a reference to the embedded file
  const bin2cpp::File & resource = bin2cpp::getHelloworldHtmlFile();

  //print information about the file.
  std::cout << "Embedded file '" << resource.getFileName() << "' is " << resource.getSize() << " bytes long.\n";

  //Saving content back to a file.
  std::cout << " Saving embedded file to 'helloworld_copy.html'...\n";
  bool saved = resource.save("helloworld_copy.html");
  if (saved)
    std::cout << "saved\n";
  else
    std::cout << "failed\n";

  //Get the internal buffer and do something with the binary data
  const char * buffer = resource.getBuffer();
  size_t bufferSize = resource.getSize();
  //...
  
  return 0;
}
