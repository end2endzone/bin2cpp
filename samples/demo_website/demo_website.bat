@echo off

rmdir /S /Q outdir 1>NUL 2>NUL
mkdir outdir 1>NUL 2>NUL

set path=%cd%\..\..\build\bin\Release;%cd%\..\..\build\bin\Debug;%PATH%

bin2cpp.exe --dir=www --managerfile=PagesFileManager.h --namespace=www --output=.\outdir --chunksize=50 --keepdirs

pause
