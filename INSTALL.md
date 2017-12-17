# Installing

This section explains how to compile and build the software and how to get a development environment running.

## Compatible with

bin2cpp is only available for the Windows platform and has been tested with the following version of Windows:

*   Windows XP
*   Windows Vista
*   Windows 7

## Prerequisites

The following software must be installed on the system for compiling source code:

* Visual Studio 2010 (or newer) (There are plans to migrate the software to other platform. See [issue #9](https://github.com/end2endzone/bin2cpp/issues/9))
* [Google C++ Testing Framework v1.6.0](https://github.com/google/googletest/tree/release-1.6.0) (untested with other versions)
* [CMake](http://www.cmake.org/) for compilation of Google C++ Testing Framework. (Tested with CMake 3.9.6)
* [bin2cpp source code](https://github.com/end2endzone/bin2cpp/tags)

The following software must be installed on the system for building the deploy packages:

* [7-Zip](http://www.7-zip.org/) for building the win32 portable package. Tested with version 9.20.
* [NSIS (Nullsoft Scriptable Install System)](http://nsis.sourceforge.net/) for building Windows Self-Extracting Setup Installer (setup.exe). Tested with version 3.0a1.

## Build steps

### Google C++ testing framework

1) Download googletest source code to your computer using one of the following:
   1) Download googletest as a [zip file](https://github.com/google/googletest/archive/release-1.6.0.zip) and extract to a temporary directory (for example c:\projects\third_party\googletest).
   2) Clone the git repository using the following commands:
      * git clone https://github.com/google/googletest.git c:\projects\third_party\googletest
      * cd /d c:\projects\third_party\googletest
      * git checkout release-1.6.0

2) Generate googletest Visual Studio 2010 solution using cmake. Enter the following commands:
   * cd c:\projects\third_party\googletest
   * mkdir msvc2010
   * cd msvc2010
   * cmake -G "Visual Studio 10 2010" -Dgtest_force_shared_crt=ON -DCMAKE_CXX_FLAGS_DEBUG=/MDd -DCMAKE_CXX_FLAGS_RELEASE=/MD "c:\projects\third_party\googletest"

3) Open the generated Visual Studio 2010 solution file located in 
   ***c:\projects\third_party\googletest\msvc2010\gtest.sln***

4) Build the solution.

### Define environment variables
Note: this step need to be executed once.

bin2cpp needs to know where the libraries of googletest are located (debug & release).
Define the following environement variables:

| Name                     | Value                                        |
|--------------------------|----------------------------------------------|
|  GTEST_DEBUG_LIBRARIES   | gtest.lib                                    |
|  GTEST_RELEASE_LIBRARIES | gtest.lib                                    |
|  GTEST_INCLUDE           | c:\projects\third_party\googletest\include   |
|  GTEST_LIBRARY_DIR       | c:\projects\third_party\googletest\msvc2010  |
 
### bin2cpp

1) Download the [bin2cpp source code](https://github.com/end2endzone/bin2cpp/tags) and extract the content to a temporary directory (for example c:\projects\bin2cpp).

2) Open the Visual Studio 2010 solution file located in 
   ***c:\projects\bin2cpp\msvc\bin2cpp.sln***

3) Build the solution.

### Deploy packages

The application support two types of deploy package:

* Portable (*.zip)
* Self-Extracting Setup Installer (setup.exe).

The installer packages are automatically build from the Visual Studio 2010 solution. See the '*install*' project.

To manually build the installer packages execute the following steps:

1) Open the Visual Studio 2010 solution file located in 
   ***c:\projects\bin2cpp\msvc\bin2cpp.sln***

2) Build the solution in '*Release*' configuration.

3) Nagivate to the '*nsis*' folder.

4) Run '*build_portable.bat*' to build the portable installer or
   run '*build_setup.bat*' to build the setup installer.

5) The packages will be generated in the '*nsis/bin*' folder.
