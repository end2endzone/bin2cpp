[![Build status](https://ci.appveyor.com/api/projects/status/q9k0ebb971o475qi/branch/master?svg=true)](https://ci.appveyor.com/project/end2endzone/bin2cpp/branch/master)
[![Tests status](http://teststatusbadge.azurewebsites.net/api/status/end2endzone/bin2cpp)](https://ci.appveyor.com/project/end2endzone/bin2cpp)

# bin2cpp

bin2cpp is a command line tool for embedding small files (like images, icons or raw data files) into a C++ executable.

It's main features are:

*   Easily converts small files as C++ source code for embedding into a C++ executable.
*   Content extraction supported by a unique function call for each embedded file.
*   Supports a multiple embedded files at once.
*   Makes it harder for resource hacker to modify or steal the embedded files.
*   No third party libraries required for retrieving the data of the embedded files.

bin2cpp is not implemented using [executable resources](http://en.wikipedia.org/wiki/Resource_(Windows)). Instead, bin2cpp creates multiple function calls for retrieving the data which makes it harder to steal the executable's content. It also makes it even harder to replace an existing resource of the executable.

It is designed to be easy to use by developers and to provide easy call functions to get access to the data of the embedded files.

The generated functions that reads and extracts the embedded content does not rely on external libraries so you don't need to setup your projects to use any third party library to start using bin2cpp. All your embedded data can be accessed right away.

# Usage

**Usage:** bin2cpp [inputfile] [output folder] [header filename] [function identifier] [chunk size] [-override].

*   **inputfile**: Path to the target file to embed in a C++ source code.
*   **output folder**: Output path to create generated source code. ie: .\generated_files\
*   **header filename**: File name of the C++ Header file. ie: SplashScreen.h
*   **function identifier**: Identifier of the function name that is used to get an instance of the file. ie: SplashScreen
*   **chunk size** (optional): Size of each string segments. Defaults to 200]
*   **override** (optional): Tells bin2cpp to over write the destination files.

# Screenshots

[![bin2cpp v1.3 Sample](http://www.end2endzone.com/wp-content/uploads/2015/01/bin2cpp-v1.3-done.png)](http://www.end2endzone.com/wp-content/uploads/2015/01/bin2cpp-v1.3-done.png) bin2cpp v1.3 Sample

[![bin2cpp v1.3 Usage](http://www.end2endzone.com/wp-content/uploads/2015/01/bin2cpp-v1.3-usage.png)](http://www.end2endzone.com/wp-content/uploads/2015/01/bin2cpp-v1.3-usage.png) bin2cpp v1.3 Usage

# Code sample

Here are some C++ code samples for embedding a Visual Studio 2008 project file generated with the following command line:

_bin2cpp.exe bin2cppTest.vcproj outdir EmbededProjectFile.h Vcproj 250 -override_

<pre class="lang:c++ decode:true" title="EmbededProjectFile.cpp">TODO: SHOW GENERATED *.h SAMPLE HERE</pre>

<pre class="lang:c++ decode:true" title="EmbededProjectFile.cpp">TODO: SHOW GENERATED *.cpp SAMPLE HERE</pre>

# Compatible with

bin2cpp is only available for the Windows platform and has been tested with the following version of Windows:

*   Windows XP
*   Windows Vista
*   Windows 7

# License

MIT License

Copyright (c) 2017 end2endzone

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

# History of changes
Version 1.3 - First public release