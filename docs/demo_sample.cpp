#include <stdio.h>
#include <string>
#include "base64.h"
#include "resourcehtml5skeleton.h" //a single include file is all you need

int main(int argc, char* argv[])
{
  //get a reference to the embedded file
  const bin2cpp::File & resource = bin2cpp::getHtmlSampleFile();

  //print information about the file.
  printf("Embedded file '%s' is %d bytes long.\n", resource.getFilename(), resource.getSize());
  printf("The MD5 of the file is %s.\n", resource.getMd5());

  //Saving content back to a file.
  printf("Saving embedded file to 'html5skeleton_copy.html'...\n");
  bool saved = resource.save("html5skeleton_copy.html");
  if (saved)
    printf("saved\n");
  else
    printf("failed\n");

  //encoding content as base64
  char * buffer = resource.newBuffer(); //returns a new buffer with a copy of the file. Ownership is transfered to the local function
  size_t bufferSize = resource.getSize();
  std::string encodedFile = toBase64(buffer, bufferSize); //binary to base64 encoder
  delete buffer; //delete allocatd buffer from newBuffer()
  buffer = NULL;

  //do something with the base64 encoded file
  //...

  return 0;
}
