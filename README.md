![bin2cpp logo](https://github.com/end2endzone/bin2cpp/raw/master/docs/bin2cpp-splashscreen.png)


[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Github Releases](https://img.shields.io/github/release/end2endzone/bin2cpp.svg)](https://github.com/end2endzone/bin2cpp/releases)
[![Build status](https://ci.appveyor.com/api/projects/status/q9k0ebb971o475qi/branch/master?svg=true)](https://ci.appveyor.com/project/end2endzone/bin2cpp/branch/master)
[![Tests status](https://img.shields.io/appveyor/tests/end2endzone/bin2cpp/master.svg)](https://ci.appveyor.com/project/end2endzone/bin2cpp/branch/master/tests)

AppVeyor build statistics:

[![Build statistics](https://buildstats.info/appveyor/chart/end2endzone/bin2cpp)](https://ci.appveyor.com/project/end2endzone/bin2cpp/branch/master)



# bin2cpp

bin2cpp is a command line tool for embedding small files (like images, icons or raw data files) into a C++ executable.

It's main features are:

*   Easily converts small files as C++ source code for embedding into a C++ executable.
*   Content extraction supported by a unique function call for each embedded file.
*   Supports a multiple embedded files at once.
*   Makes it harder for resource hacker to modify or steal the embedded files.
*   No third party libraries required for retrieving the data of the embedded files.

bin2cpp is not implemented using [executable resources](http://en.wikipedia.org/wiki/Resource_(Windows)). Instead, bin2cpp creates multiple function calls for retrieving the data which makes it harder to steal the executable's content. It also makes it even harder to replace an existing resource of the executable.

It is designed to be easy to use by developers and to provide easy call functions to get access to the data of the embedded files.

The generated functions that reads and extracts the embedded content does not rely on external libraries so you don't need to setup your projects to use any third party library to start using bin2cpp. All your embedded data can be accessed right away.

# Command Line Usage

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

# Example

### Input File:
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

### Command:
```batchfile
bin2cpp.exe --file=html5skeleton.html --output=.\outdir --headerfile=resourcehtml5skeleton.h
            --identifier=HtmlSample --chunksize=50
```

### Console output

```batchfile
bin2cpp v2.0.0 - Convert binary files into C++ source code.
Copyright (C) 2013-2018 end2endzone.com. All rights reserved.
bin2cpp is open source software, see http://github.com/end2endzone/bin2cpp
Embedding "html5skeleton.html" using chunks of 50 bytes...
Writing file ".\outdir\resourcehtml5skeleton.h"...
Writing file ".\outdir\resourcehtml5skeleton.cpp"...
```

### File resourcehtml5skeleton.h

```cpp
/**
 * This file was generated by bin2cpp v2.0.0
 * Copyright (C) 2013-2018 end2endzone.com. All rights reserved.
 * bin2cpp is open source software, see http://github.com/end2endzone/bin2cpp
 * Source code for file 'html5skeleton.html', last modified 1513117337.
 * Do not modify this file.
 */
#pragma once
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
```

### File resourcehtml5skeleton.cpp

```cpp
/**
 * This file was generated by bin2cpp v2.0.0
 * Copyright (C) 2013-2018 end2endzone.com. All rights reserved.
 * bin2cpp is open source software, see http://github.com/end2endzone/bin2cpp
 * Source code for file 'html5skeleton.html', last modified 1513117337.
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
    ~HtmlSampleFile() {}
    virtual size_t getSize() const { return 238; }
    virtual const char * getFilename() const { return "html5skeleton.html"; }
    virtual const char * getBuffer() const { return mBuffer.c_str(); }
    void build()
    {
      mBuffer.clear();
      mBuffer.reserve(getSize()); //allocate all required memory at once to prevent reallocations
      mBuffer.append("<!DOCTYPE html>\r\n<html lang=\"en\">\r\n<head>\r\n  <meta", 50);
      mBuffer.append(" charset=\"utf-8\">\r\n  <meta name=\"viewport\" content", 50);
      mBuffer.append("=\"width=device-width, initial-scale=1, user-scalab", 50);
      mBuffer.append("le=yes\">\r\n  <title>Hello World!</title>\r\n</head>\r\n", 50);
      mBuffer.append("<body>\r\nHello World!\r\n</body>\r\n</html>", 38);
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

### Code sample
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

# Installing

Please refer to file [INSTALL.md](INSTALL.md) for details on how installing/building the application.

## Testing
bin2cpp comes with unit tests which tests for multiple combinations to make sure that input files are always encoded without errors.

Test are build using the Google Test v1.6.0 framework. For more information on how googletest is working, see the [google test documentation primer](https://github.com/google/googletest/blob/release-1.8.0/googletest/docs/V1_6_Primer.md).  

Test are automatically build when building the solution. Please see the '*build step*' section for details on how to build the software.

Test can be executed from the following two locations:

1) From the Visual Studio IDE:
   1) Select the project '*bin2cpp_unittest*' as StartUp project.
   2) Hit CTRL+F5 (Start Without Debugging)
2) From the output binaries folder:
   1) Open a file navigator and browse to the output folder(for example c:\projects\bin2cpp\msvc\Win32\Release)
   2) Run the '*generate_test_files.bat*' batch script. The script will generate all required input files.
   3) Run the '*bin2cpp_unittest.exe*' executable.

See also the latest test results at the beginning of the document.

# Screenshots

[![bin2cpp v2.0.0 Sample](http://github.com/end2endzone/bin2cpp/raw/master/docs/bin2cpp-v2.0.0-sample.png)](http://github.com/end2endzone/bin2cpp/blob/master/docs/bin2cpp-v2.0.0-sample.png)

bin2cpp v2.0.0 Sample

# Compatible with

bin2cpp is only available for the Windows platform and has been tested with the following version of Windows:

*   Windows XP
*   Windows Vista
*   Windows 7

# Versioning

We use [Semantic Versioning 2.0.0](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/end2endzone/bin2cpp/tags).

# Authors

* **Antoine Beauchamp** - *Initial work* - [end2endzone](https://github.com/end2endzone)

See also the list of [contributors](https://github.com/end2endzone/bin2cpp/blob/master/AUTHORS) who participated in this project.

# License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details