#include <stdio.h>  // printf
#include <stdlib.h> // getenv
#include <iostream>
#include <string>

#include "PagesFileManager.h"

int main(int argc, char* argv[])
{
  www::FileManager & mgr = www::FileManager::getInstance();

  //Print information about all files generated with "--managerfile" or --registerfile flags.
  size_t num_files = mgr.getFileCount();
  std::cout << "Found " << num_files << " embedded web pages...\n";

  //Listing files.
  for(size_t i=0; i<num_files; i++)
  {
    const www::File * file = mgr.getFile(i);
    std::cout << "  File '" << file->getFilePath() << "', " << file->getSize() << " bytes\n";
  }

  //Saving content back to files preserving the original directory structure.
  std::string temp_dir = getenv("TEMP");
#ifdef _WIN32
  temp_dir += "\\";
#else
  temp_dir += "/";
#endif
  temp_dir += "www";
  std::cout << "Saving embedded web pages to directory '" << temp_dir << "'...\n";
  bool saved = mgr.saveFiles(temp_dir.c_str());
  if (saved)
    std::cout << "saved\n";
  else
    std::cout << "failed\n";

  return 0;
}
