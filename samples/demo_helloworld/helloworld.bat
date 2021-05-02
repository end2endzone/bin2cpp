@echo off

rmdir /S /Q outdir 1>NUL 2>NUL
mkdir outdir 1>NUL 2>NUL

set path=%cd%\..\..\build\bin\Release;%PATH%

bin2cpp.exe --file=helloworld.html --output=.\outdir --headerfile=generated_helloworld.h --identifier=HelloWorldHtml --chunksize=50

pause
