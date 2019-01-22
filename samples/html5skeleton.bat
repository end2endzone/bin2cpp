@echo off

rmdir /S /Q outdir
mkdir outdir

set path=%PATH%;%cd%\..\build\bin\Release

bin2cpp.exe --file=html5skeleton.html --output=.\outdir --headerfile=file_html5skeleton.h --identifier=HtmlSample --chunksize=50

pause
