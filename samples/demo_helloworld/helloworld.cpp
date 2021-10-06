#include <stdio.h>
#include <string>
#include "generated_helloworld.h" //a single include file is all you need

int main(int argc, char* argv[])
{
  //get a reference to the embedded file
  const bin2cpp::File & resource = bin2cpp::getHelloWorldHtmlFile();

  //print information about the file.
  printf("Embedded file '%s' is %llu bytes long.\n", resource.getFileName(), resource.getSize());

  //Saving content back to a file.
  printf("Saving embedded file to 'helloworld_copy.html'...\n");
  bool saved = resource.save("helloworld_copy.html");
  if (saved)
    printf("saved\n");
  else
    printf("failed\n");

  //Get the internal buffer and do something with the binary data
  const char * buffer = resource.getBuffer();
  size_t bufferSize = resource.getSize();
  //...
  
  return 0;
}
