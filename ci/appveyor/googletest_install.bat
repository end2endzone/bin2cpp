@echo off

echo =======================================================================
echo Finding root folder of repository
echo =======================================================================
cd /d %~dp0
cd ..\..
set REPOSITORY_ROOT=%cd%
echo REPOSITORY_ROOT=%REPOSITORY_ROOT%
echo done.
echo.

echo ======================================================================
echo Deleting googletest repository folder (if any)
echo =======================================================================
if EXIST %REPOSITORY_ROOT%\third_party\googletest (
  rmdir /q/s %REPOSITORY_ROOT%\third_party\googletest
)
echo done.
echo.

echo ======================================================================
echo Cloning googletest repository
echo =======================================================================
git clone https://github.com/google/googletest.git %REPOSITORY_ROOT%\third_party\googletest
cd /d %REPOSITORY_ROOT%\third_party\googletest
git checkout release-1.6.0
echo done.
echo.

echo =======================================================================
echo Generating googletest Visual Studio 2010 solution
echo =======================================================================
cd /d %REPOSITORY_ROOT%\third_party\googletest

echo Deleting previous msvc2010 folder (if any)
rmdir /s/q msvc2010 >NUL 2>NUL
echo done.
echo.

echo Creating msvc2010 folder.
mkdir %REPOSITORY_ROOT%\third_party\googletest\msvc2010
echo done.
echo.

echo Launching cmake...
cd /d %REPOSITORY_ROOT%\third_party\googletest\msvc2010
cmake -G "Visual Studio 10 2010" -Dgtest_force_shared_crt=ON -DCMAKE_CXX_FLAGS_DEBUG=/MDd -DCMAKE_CXX_FLAGS_RELEASE=/MD "%REPOSITORY_ROOT%\third_party\googletest"
echo done.
echo.

echo =======================================================================
echo Setting environnment variables for googletest
echo =======================================================================
set GTEST_DEBUG_LIBRARIES=gtest.lib
set GTEST_RELEASE_LIBRARIES=gtest.lib
set GTEST_INCLUDE=%REPOSITORY_ROOT%\third_party\googletest\include
set GTEST_LIBRARY_DIR=%REPOSITORY_ROOT%\third_party\googletest\msvc2010
REM
setx GTEST_DEBUG_LIBRARIES %GTEST_DEBUG_LIBRARIES%
setx GTEST_RELEASE_LIBRARIES %GTEST_RELEASE_LIBRARIES%
setx GTEST_INCLUDE %GTEST_INCLUDE%
setx GTEST_LIBRARY_DIR %GTEST_LIBRARY_DIR%
REM
set GTEST_
echo done.
echo.
