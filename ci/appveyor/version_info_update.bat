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

REM **************** version_info.bat ****************
set OUTPUTFILE=%REPOSITORY_ROOT%\version_info.bat
echo Updating %OUTPUTFILE%...
echo set PRODUCT_VERSION=%APPVEYOR_BUILD_VERSION%>%OUTPUTFILE%
set OUTPUTFILE=
echo done.
echo.

REM **************** version_info.h ****************
set OUTPUTFILE=%REPOSITORY_ROOT%\version_info.h
echo Updating %OUTPUTFILE%...
echo #ifndef BINCPP_VERSION_H>%OUTPUTFILE%
echo #define BINCPP_VERSION_H>>%OUTPUTFILE%
echo #define BINCPP_VERSION "%APPVEYOR_BUILD_VERSION%">>%OUTPUTFILE%
echo #endif //BINCPP_VERSION_H>>%OUTPUTFILE%
set OUTPUTFILE=
echo done.
echo.

REM **************** version_info.nsh ****************
set OUTPUTFILE=%REPOSITORY_ROOT%\version_info.nsh
echo Updating %OUTPUTFILE%...
echo !define PRODUCT_VERSION "%APPVEYOR_BUILD_VERSION%">%OUTPUTFILE%
set OUTPUTFILE=
echo done.
echo.
