![bin2cpp logo](https://github.com/end2endzone/bin2cpp/raw/master/docs/bin2cpp-splashscreen.png)


# bin2cpp #
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Github Releases](https://img.shields.io/github/release/end2endzone/bin2cpp.svg)](https://github.com/end2endzone/bin2cpp/releases)

bin2cpp is a command line tool for embedding small files (like images, icons or raw data files) into a C++ executable.


## Status ##

Build:

| Service | Build | Tests |
|----|-------|-------|
| AppVeyor | [![Build status](https://img.shields.io/appveyor/ci/end2endzone/bin2cpp/master.svg?logo=appveyor)](https://ci.appveyor.com/project/end2endzone/bin2cpp) | [![Tests status](https://img.shields.io/appveyor/tests/end2endzone/bin2cpp/master.svg?logo=appveyor)](https://ci.appveyor.com/project/end2endzone/bin2cpp/branch/master/tests) |
| Travis CI | [![Build Status](https://img.shields.io/travis/end2endzone/bin2cpp/master.svg?logo=travis&style=flat)](https://travis-ci.org/end2endzone/bin2cpp) |  |

Statistics:

| AppVeyor | Travic CI |
|----------|-----------|
| [![Statistics](https://buildstats.info/appveyor/chart/end2endzone/bin2cpp)](https://ci.appveyor.com/project/end2endzone/bin2cpp/branch/master) | [![Statistics](https://buildstats.info/travisci/chart/end2endzone/bin2cpp)](https://travis-ci.org/end2endzone/bin2cpp) |




# Purpose #

bin2cpp is not implemented using [executable resources](http://en.wikipedia.org/wiki/Resource_(Windows)). Instead, bin2cpp creates multiple function calls for retrieving the data which makes it harder to steal the executable's content. It also makes it even harder to replace an existing resource of the executable.

It is designed to be easy to use by developers and to provide easy call functions to get access to the data of the embedded files.

The generated functions that reads and extracts the embedded content does not rely on external libraries so you don't need to setup your projects to use any third party library to start using bin2cpp. All your embedded data can be accessed right away.




# Features #

The main features of the project are:

* Easily converts small files as C++ source code for embedding into a C++ executable.
* Content extraction supported by a unique function call for each embedded file.
* Supports a multiple embedded files at once.
* Makes it harder for resource hacker to modify or steal the embedded files.
* No third party libraries required for retrieving the data of the embedded files.



## Use cases ##

The following list shows situations where bin2cpp is useful:

* Embedding default configuration files if none are provided.
* Embedding GLSL shaders into the executable.
* Embedding text or XML databases (gui layout, names, dictionaries or localization strings)
* A portable alternative to C++11 raw string literals.
* Prevent stealing copyrighted resources.
* Embedding images/icons/sounds for a GUI application. The application executable can be shipped/copied as a single file. 
* Embedding a long PL/SQL code string.
* Allowing an executable to be downloaded from an intranet server as a single file.
* Distributing an application without an installer package. All configurations files and resources can be embedded and extracted at first launch of the application.




# Usage #

The following show how to use bin2cpp with code examples:



## Command Line Usage ##

```
bin2cpp --file=<path> --output=<path> --headerfile=<name> --identifier=<name>
        [--generator=<name>] [--encoding=<name>] [--chunksize=<value>] [--namespace=<value>]
        [--baseclass=<value>] [--override] [--noheader] [--quiet]
bin2cpp --help
bin2cpp --version
```

| Argument             | Description                                                                                                |
|----------------------|------------------------------------------------------------------------------------------------------------|
| --help               | Display this help message.                                                                                 |
| --version            | Display this application version.                                                                          |
| --file=\<path>       | Path of the input file used for embedding as a C++ source code.                                            |
| --output=\<path>     | Output folder where to create generated code. ie: .\generated_files                                        |
| --headerfile=\<name> | File name of the generated C++ Header file. ie: SplashScreen.h                                             |
| --generator=\<name>  | Name of the generator to use. Possible values are 'segment', 'string' and 'array'. [default: segment].     |
| --encoding=\<name>   | Name of the binary to string literal encoding to use. Possible values are 'oct' and 'hex'. [default: oct]. |
| --identifier=\<name> | Identifier of the function name that is used to get an instance of the file. ie: SplashScreen              |
| --chunksize=\<value> | Size in bytes of each string segments (bytes per row). [default: 200].                                     |
| --baseclass=\<value> | The name of the interface for embedded files. [default: File].                                             |
| --namespace=\<value> | The namespace of the generated source code [default: bin2cpp].                                             |
| --override           | Tells bin2cpp to overwrite the destination files.                                                          |
| --noheader           | Do not print program header to standard output.                                                            |
| --quiet              | Do not log any message to standard output.                                                                 |

## Example ##



### Input File: ###
html5skeleton.html
```html
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=yes">
  <title>Hello World!</title>
</head>
<body>
Hello World!
</body>
</html>
```



### Command: ###
```batchfile
bin2cpp.exe --file=html5skeleton.html --output=.\outdir --headerfile=resourcehtml5skeleton.h
            --identifier=HtmlSample --chunksize=50
```



### Console output ###

```batchfile
bin2cpp v2.2.0 - Convert binary files into C++ source code.
Copyright (C) 2013-2019 end2endzone.com. All rights reserved.
bin2cpp is open source software, see http://github.com/end2endzone/bin2cpp
Embedding "html5skeleton.html" using chunks of 50 bytes...
Writing file ".\outdir\resourcehtml5skeleton.h"...
Writing file ".\outdir\resourcehtml5skeleton.cpp"...
```



### File resourcehtml5skeleton.h ###

```cpp
/**
 * This file was generated by bin2cpp v2.2.0
 * Copyright (C) 2013-2019 end2endzone.com. All rights reserved.
 * bin2cpp is open source software, see http://github.com/end2endzone/bin2cpp
 * Source code for file 'html5skeleton.html', last modified 1547463633.
 * Do not modify this file.
 */
#ifndef RESOURCEHTML5SKELETON_H
#define RESOURCEHTML5SKELETON_H

#include <stddef.h>

namespace bin2cpp
{
  #ifndef BIN2CPP_EMBEDDEDFILE_CLASS
  #define BIN2CPP_EMBEDDEDFILE_CLASS
  class File
  {
  public:
    virtual size_t getSize() const = 0;
    virtual const char * getFilename() const = 0;
    virtual const char * getBuffer() const = 0;
    virtual bool save(const char * iFilename) const = 0;
  };
  #endif
  const File & getHtmlSampleFile();
}; //bin2cpp

#endif //RESOURCEHTML5SKELETON_H
```

### File resourcehtml5skeleton.cpp ###

```cpp
/**
 * This file was generated by bin2cpp v2.2.0
 * Copyright (C) 2013-2019 end2endzone.com. All rights reserved.
 * bin2cpp is open source software, see http://github.com/end2endzone/bin2cpp
 * Source code for file 'html5skeleton.html', last modified 1547463633.
 * Do not modify this file.
 */
#include "resourcehtml5skeleton.h"
#include <stdio.h> //for FILE
#include <string> //for memcpy
namespace bin2cpp
{
  class HtmlSampleFile : public virtual bin2cpp::File
  {
  public:
    HtmlSampleFile() { build(); }
    virtual ~HtmlSampleFile() {}
    virtual size_t getSize() const { return 228; }
    virtual const char * getFilename() const { return "html5skeleton.html"; }
    virtual const char * getBuffer() const { return mBuffer.c_str(); }
    void build()
    {
      mBuffer.clear();
      mBuffer.reserve(getSize()); //allocate all required memory at once to prevent reallocations
      mBuffer.append("<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n  <meta ch", 50);
      mBuffer.append("arset=\"utf-8\">\n  <meta name=\"viewport\" content=\"wi", 50);
      mBuffer.append("dth=device-width, initial-scale=1, user-scalable=y", 50);
      mBuffer.append("es\">\n  <title>Hello World!</title>\n</head>\n<body>\n", 50);
      mBuffer.append("Hello World!\n</body>\n</html>", 28);
    }
    virtual bool save(const char * iFilename) const
    {
      FILE * f = fopen(iFilename, "wb");
      if (!f) return false;
      size_t fileSize = getSize();
      const char * buffer = getBuffer();
      fwrite(buffer, 1, fileSize, f);
      fclose(f);
      return true;
    }
  private:
    std::string mBuffer;
  };
  const File & getHtmlSampleFile() { static HtmlSampleFile _instance; return _instance; }
}; //bin2cpp
```

### Code sample (using embedded files) ###
```cpp
#include <stdio.h>
#include <string>
#include "resourcehtml5skeleton.h" //a single include file is all you need

int main(int argc, char* argv[])
{
  //get a reference to the embedded file
  const bin2cpp::File & resource = bin2cpp::getHtmlSampleFile();

  //print information about the file.
  printf("Embedded file '%s' is %d bytes long.\n", resource.getFilename(), resource.getSize());

  //Saving content back to a file.
  printf("Saving embedded file to 'html5skeleton_copy.html'...\n");
  bool saved = resource.save("html5skeleton_copy.html");
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
```



## Screenshots ##

[![bin2cpp v2.0.0 Sample](https://raw.githubusercontent.com/end2endzone/bin2cpp/master/docs/bin2cpp-v2.0.0-sample.png)](https://raw.githubusercontent.com/end2endzone/bin2cpp/master/docs/bin2cpp-v2.0.0-sample.png)

bin2cpp v2.0.0 Sample




# Build #

Please refer to file [INSTALL.md](INSTALL.md) for details on how installing/building the application.




# Platform #

bin2cpp has been tested with the following platform:

*   Linux x86/x64
*   Windows x86/x64




# Versioning #

We use [Semantic Versioning 2.0.0](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/end2endzone/bin2cpp/tags).




# Authors #

* **Antoine Beauchamp** - *Initial work* - [end2endzone](https://github.com/end2endzone)

See also the list of [contributors](https://github.com/end2endzone/bin2cpp/blob/master/AUTHORS) who participated in this project.




# License #

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

