#include <stdio.h>  // printf
#include <stdlib.h> // getenv
#include <iostream>

#include "IconsFileManager.h"

int main(int argc, char* argv[])
{
  bin2cpp::FileManager & mgr = bin2cpp::FileManager::getInstance();

  //Print information about all files generated with "--managerfile" or --registerfile flags.
  size_t num_files = mgr.getFileCount();
  std::cout << "Found " << num_files << " embedded icons...\n";

  //Listing files.
  for(size_t i=0; i<num_files; i++)
  {
    const bin2cpp::File * file = mgr.getFile(i);
    std::cout << "  File '" << file->getFileName() << "', " << file->getSize() << " bytes\n";
  }

  //Saving content back to files.
  const char * temp_dir = getenv("TEMP");
  std::cout << "Saving embedded icons to directory '" << temp_dir << "'...\n";
  bool saved = mgr.saveFiles(temp_dir);
  if (saved)
    std::cout << "saved\n";
  else
    std::cout << "failed\n";

  return 0;
}
