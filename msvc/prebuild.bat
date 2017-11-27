@echo off
:: %1 is $(ConfigurationName)
:: %2 is $(Platform)

set BIN2CPP_EXE=.\%2\%1\bin2cpp.exe
set TESTFILEGENERATOR_EXE=.\%2\%1\testfilegenerator.exe

set TEST_NAME=testSmall10
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
%TESTFILEGENERATOR_EXE% --file=%OUTDIR%\%TEST_NAME%.bin --size=10 --fill=text
%BIN2CPP_EXE% %OUTDIR%\%TEST_NAME%.bin %OUTDIR% _%TEST_NAME%.h %TEST_NAME% 450 -override

set TEST_NAME=testPredictable1000
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
%TESTFILEGENERATOR_EXE% --file=%OUTDIR%\%TEST_NAME%.bin --size=10 --fill=text
%BIN2CPP_EXE% %OUTDIR%\%TEST_NAME%.bin %OUTDIR% _%TEST_NAME%.h %TEST_NAME% 450 -override

set TEST_NAME=testRandom100000
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
%TESTFILEGENERATOR_EXE% --file=%OUTDIR%\%TEST_NAME%.bin --size=100000 --fill=random
%BIN2CPP_EXE% %OUTDIR%\%TEST_NAME%.bin %OUTDIR% _%TEST_NAME%.h %TEST_NAME% 450 -override

set TEST_NAME=testPredictable100000
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
%TESTFILEGENERATOR_EXE% --file=%OUTDIR%\%TEST_NAME%.bin --size=100000 --fill=predictable
%BIN2CPP_EXE% %OUTDIR%\%TEST_NAME%.bin %OUTDIR% _%TEST_NAME%.h %TEST_NAME% 450 -override

set TEST_NAME=testHtml100000
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
%TESTFILEGENERATOR_EXE% --file=%OUTDIR%\%TEST_NAME%.bin --size=100000 --fill=html
%BIN2CPP_EXE% %OUTDIR%\%TEST_NAME%.bin %OUTDIR% _%TEST_NAME%.h %TEST_NAME% 450 -override

set TEST_NAME=testText100000
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
%TESTFILEGENERATOR_EXE% --file=%OUTDIR%\%TEST_NAME%.bin --size=100000 --fill=text
%BIN2CPP_EXE% %OUTDIR%\%TEST_NAME%.bin %OUTDIR% _%TEST_NAME%.h %TEST_NAME% 450 -override

set TEST_NAME=testSequential10MB
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
%TESTFILEGENERATOR_EXE% --file=%OUTDIR%\%TEST_NAME%.bin --size=10485760 --fill=sequential
%BIN2CPP_EXE% %OUTDIR%\%TEST_NAME%.bin %OUTDIR% _%TEST_NAME%.h %TEST_NAME% 450 -override

REM   set TEST_NAME=testSequential20MB
REM   set OUTDIR=.\generated_files\%TEST_NAME%
REM   mkdir %OUTDIR% 1>NUL 2>NUL
REM   %TESTFILEGENERATOR_EXE% --file=%OUTDIR%\%TEST_NAME%.bin --size=20971520 --fill=sequential
REM   %BIN2CPP_EXE% %OUTDIR%\%TEST_NAME%.bin %OUTDIR% _%TEST_NAME%.h %TEST_NAME% 450 -override
REM   
REM   set TEST_NAME=testSequential30MB
REM   set OUTDIR=.\generated_files\%TEST_NAME%
REM   mkdir %OUTDIR% 1>NUL 2>NUL
REM   %TESTFILEGENERATOR_EXE% --file=%OUTDIR%\%TEST_NAME%.bin --size=31457280 --fill=sequential
REM   %BIN2CPP_EXE% %OUTDIR%\%TEST_NAME%.bin %OUTDIR% _%TEST_NAME%.h %TEST_NAME% 450 -override

dir >NUL
