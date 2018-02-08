@echo off
set SRC_DIR=%cd%\src

REM Create build directory
REM rmdir /q/s build >NUL 2>NUL
mkdir build >NUL 2>NUL
cd build

cmake -G "Visual Studio 10 2010" %SRC_DIR%

pause
