@echo off

cd /d %~dp0
cd ..\..
set APPVEYOR=True
set APPVEYOR_BUILD_FOLDER=%cd%
set CONFIGURATION=Debug
REM set PLATFORM=Win32
set PLATFORM=x64
set PLATFORMTOOLSET=""
REM set PLATFORMTOOLSET=v100
REM set PLATFORMTOOLSET=Windows7.1SDK

echo Emulating appveyor's environment.
echo APPVEYOR_BUILD_FOLDER set to '%APPVEYOR_BUILD_FOLDER%'
echo.

:: Return back to original folder
cd /d %~dp0

:: Leave the command prompt open
cmd.exe /k echo AppVeyor ready...
