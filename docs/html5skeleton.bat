@echo off

mkdir outdir

set path=%PATH%;%cd%\..\msvc\Win32\Release

bin2cpp.exe --file=html5skeleton.html --output=.\outdir --headerfile=resourcehtml5skeleton.h --identifier=HtmlSample --chunksize=50 --override

pause
