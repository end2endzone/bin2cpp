@ECHO OFF

REM **************************************
REM  Generate ZIP package
REM **************************************

cd /d %~dp0
set PRODUCT_PLATFORM=Win32


call ..\version_info.bat

set outfile=".\bin\bin2cpp v%PRODUCT_VERSION% %PRODUCT_PLATFORM% portable.zip"

set infiles=
set infiles=%infiles% "..\msvc\%PRODUCT_PLATFORM%\Release\bin2cpp.exe"
set infiles=%infiles% "..\msvc\%PRODUCT_PLATFORM%\Release\bin2cpp_unittest.exe"
set infiles=%infiles% "..\msvc\%PRODUCT_PLATFORM%\Release\generate_test_files.bat"
set infiles=%infiles% "..\msvc\%PRODUCT_PLATFORM%\Release\testfilegenerator.exe"
set infiles=%infiles% "..\AUTHORS"
set infiles=%infiles% "..\CHANGES"
set infiles=%infiles% "..\LICENSE"
set infiles=%infiles% "..\README.md"

echo Generating portable install %outfile%
echo using the following input files:
echo %infiles%
echo.
call "C:\Program Files\7-Zip\7z.exe" a -bd %outfile% %infiles%

pause
