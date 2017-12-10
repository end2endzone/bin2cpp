REM ======================================================================
REM Cloning google test repository
REM =======================================================================
git clone https://github.com/google/googletest.git c:\projects\third_party\googletest
cd /d c:\projects\third_party\googletest
git checkout release-1.6.0

REM =======================================================================
REM Generating google test Visual Studio 2010 solution
REM =======================================================================
mkdir msvc2010
cd msvc2010
cmake -G "Visual Studio 10 2010" -Dgtest_force_shared_crt=ON -DCMAKE_CXX_FLAGS_DEBUG=/MDd -DCMAKE_CXX_FLAGS_RELEASE=/MD "c:\projects\third_party\googletest"
cd..

REM =======================================================================
REM Displaying Environnment Variables
REM =======================================================================
set

REM =======================================================================
REM Building google test library
REM =======================================================================
msbuild "msvc2010\gtest.sln" /m /verbosity:minimal /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll"

REM =======================================================================
REM Done
REM =======================================================================
