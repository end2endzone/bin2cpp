@echo off

rmdir /S /Q include\bin2cpp 1>NUL 2>NUL
mkdir include\bin2cpp 1>NUL 2>NUL

set path=%cd%\..\..\build\bin\Release;%cd%\..\..\build\bin\Debug;%PATH%

bin2cpp.exe --file=res\icon.ico         --output=include --headerfile=bin2cpp\file_icon.h         --identifier=icon  
bin2cpp.exe --file=res\sinus.dat        --output=include --headerfile=bin2cpp\file_sinus.h        --identifier=SinusTable
bin2cpp.exe --file=res\splashscreen.png --output=include --headerfile=bin2cpp\file_splashscreen.h --identifier=splashscreen

pause
