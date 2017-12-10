@echo off

set TEST_NAME=testSmall10
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=10 --fill=text
bin2cpp.exe %OUTDIR%\%TEST_NAME%.bin %OUTDIR% _%TEST_NAME%.h %TEST_NAME% 450 -override

set TEST_NAME=testPredictable1000
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=10 --fill=text
bin2cpp.exe %OUTDIR%\%TEST_NAME%.bin %OUTDIR% _%TEST_NAME%.h %TEST_NAME% 450 -override

set TEST_NAME=testRandom100000
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=100000 --fill=random
bin2cpp.exe %OUTDIR%\%TEST_NAME%.bin %OUTDIR% _%TEST_NAME%.h %TEST_NAME% 450 -override

set TEST_NAME=testPredictable100000
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=100000 --fill=predictable
bin2cpp.exe %OUTDIR%\%TEST_NAME%.bin %OUTDIR% _%TEST_NAME%.h %TEST_NAME% 450 -override

set TEST_NAME=testHtml100000
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=100000 --fill=html
bin2cpp.exe %OUTDIR%\%TEST_NAME%.bin %OUTDIR% _%TEST_NAME%.h %TEST_NAME% 450 -override

set TEST_NAME=testText100000
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=100000 --fill=text
bin2cpp.exe %OUTDIR%\%TEST_NAME%.bin %OUTDIR% _%TEST_NAME%.h %TEST_NAME% 450 -override

set TEST_NAME=testSequential10MB
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=10485760 --fill=sequential
bin2cpp.exe %OUTDIR%\%TEST_NAME%.bin %OUTDIR% _%TEST_NAME%.h %TEST_NAME% 450 -override

REM   set TEST_NAME=testSequential20MB
REM   set OUTDIR=.\generated_files\%TEST_NAME%
REM   mkdir %OUTDIR% 1>NUL 2>NUL
REM   testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=20971520 --fill=sequential
REM   bin2cpp.exe %OUTDIR%\%TEST_NAME%.bin %OUTDIR% _%TEST_NAME%.h %TEST_NAME% 450 -override
REM   
REM   set TEST_NAME=testSequential30MB
REM   set OUTDIR=.\generated_files\%TEST_NAME%
REM   mkdir %OUTDIR% 1>NUL 2>NUL
REM   testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=31457280 --fill=sequential
REM   bin2cpp.exe %OUTDIR%\%TEST_NAME%.bin %OUTDIR% _%TEST_NAME%.h %TEST_NAME% 450 -override

dir >NUL
