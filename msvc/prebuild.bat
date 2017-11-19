@echo off
:: %1 is $(ConfigurationName)
:: %2 is $(ConfigurationName)

set BIN2CPP_EXE=.\%2\%1\bin2cpp.exe

::testBinaryFile prebuild
set OUTDIR=.\generated_files\testBinaryFile
mkdir %OUTDIR% 1>NUL 2>NUL
%BIN2CPP_EXE% ..\docs\icon.xcf %OUTDIR% EmbeddedBinaryFile.h icon -override 250

::testTextFile prebuild
set OUTDIR=.\generated_files\testTextFile
mkdir %OUTDIR% 1>NUL 2>NUL
%BIN2CPP_EXE% ..\README.md %OUTDIR% EmbeddedTextFile.h readme -override 250
