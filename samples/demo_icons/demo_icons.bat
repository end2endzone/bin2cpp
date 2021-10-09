@echo off

rmdir /S /Q outdir 1>NUL 2>NUL
mkdir outdir 1>NUL 2>NUL

set path=%cd%\..\..\build\bin\Release;%cd%\..\..\build\bin\Debug;%PATH%

bin2cpp.exe --dir=flat-color-icons --managerfile=IconsFileManager.h --output=.\outdir --chunksize=50

pause
