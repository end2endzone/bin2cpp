#include <stdio.h>  // printf
#include <stdlib.h> // getenv

#include "IconsFileManager.h"

int main(int argc, char* argv[])
{
  bin2cpp::FileManager & mgr = bin2cpp::FileManager::getInstance();

  //Print information about all files generated with "--managerfile" or --registerfile flags.
  size_t num_files = mgr.getFileCount();
  printf("Found %lu embedded icons...\n", num_files);

  //Listing files.
  for(size_t i=0; i<num_files; i++)
  {
    const bin2cpp::File * file = mgr.getFile(i);
    printf("  File '%s', %lu bytes\n", file->getFilename(), file->getSize());
  }

  //Saving content back to files.
  const char * temp_dir = getenv("TEMP");
  printf("Saving embedded icons to directory '%s'...\n", temp_dir);
  bool saved = mgr.saveFiles(temp_dir);
  if (saved)
    printf("saved\n");
  else
    printf("failed\n");

  return 0;
}
