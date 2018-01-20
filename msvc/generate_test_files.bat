@echo off

set TEST_NAME=testText10
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=10 --fill=text
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME%  --chunksize=450 --override

set TEST_NAME=testText1000
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=1000 --fill=text
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME%  --chunksize=450 --override

set TEST_NAME=testText100000
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=100000 --fill=text
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME%  --chunksize=450 --override

set TEST_NAME=testRandom1
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=100000 --fill=random --seed=1
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME%  --chunksize=450 --override

set TEST_NAME=testRandom2
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=100000 --fill=random --seed=2
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME%  --chunksize=450 --override

set TEST_NAME=testRandom3
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=100000 --fill=random --seed=3
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME%  --chunksize=450 --override

set TEST_NAME=testRandom4
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=1000 --fill=random --seed=4
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME% --chunksize=200 --override

set TEST_NAME=testRandom5
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=1000 --fill=random --seed=5
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME% --chunksize=200 --override

set TEST_NAME=testRandom6
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=1000 --fill=random --seed=6
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME% --chunksize=200 --override

set TEST_NAME=testRandom7
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=1000 --fill=random --seed=7
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME% --chunksize=200 --override

set TEST_NAME=testRandom8
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=1000 --fill=random --seed=8
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME% --chunksize=200 --override

set TEST_NAME=testHtml100000
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=100000 --fill=html
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME%  --chunksize=450 --override

set TEST_NAME=testSequential1000
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=1000 --fill=sequential
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME%  --chunksize=180 --override

set TEST_NAME=testSequential10MB
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=10485760 --fill=sequential
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME%  --chunksize=450 --override

REM   set TEST_NAME=testSequential20MB
REM   set OUTDIR=.\generated_files\%TEST_NAME%
REM   mkdir %OUTDIR% 1>NUL 2>NUL
REM   testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=20971520 --fill=sequential
REM   bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME%  --chunksize=450 --override
REM   
REM   set TEST_NAME=testSequential30MB
REM   set OUTDIR=.\generated_files\%TEST_NAME%
REM   mkdir %OUTDIR% 1>NUL 2>NUL
REM   testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=31457280 --fill=sequential
REM   bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME%  --chunksize=450 --override

set TEST_NAME=testGeneratorArray10000
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=10000 --fill=sequential
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME%  --chunksize=450 --generator=array --override

set TEST_NAME=testGeneratorString10000
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=10000 --fill=sequential
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME%  --chunksize=450 --generator=string --override

set TEST_NAME=testGeneratorSegment10000
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=10000 --fill=sequential
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME%  --chunksize=450 --generator=segment --override

set TEST_NAME=testNamespace
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=10000 --fill=sequential
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME%  --chunksize=450 --generator=segment --override --namespace=foobar

set TEST_NAME=testBaseClass
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=10000 --fill=sequential
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME%  --chunksize=450 --generator=segment --override --baseclass=Resource

set TEST_NAME=testEncodingOct
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=300 --fill=sequential
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME%  --chunksize=40 --generator=segment --override --encoding=OcT

set TEST_NAME=testEncodingHex
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=300 --fill=sequential
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME%  --chunksize=40 --generator=segment --override --encoding=HeX

set TEST_NAME=testIssue12
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=100000 --fill=random --seed=3
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME%  --chunksize=450 --override

set TEST_NAME=testIssue12Mini
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=100 --fill=random --seed=3 --skip=47240
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME% --chunksize=10 --override

set TEST_NAME=testIssue13
set OUTDIR=.\generated_files\%TEST_NAME%
mkdir %OUTDIR% 1>NUL 2>NUL
echo testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=1000 --fill=random --seed=1
testfilegenerator.exe --file=%OUTDIR%\%TEST_NAME%.bin --size=1000 --fill=random --seed=1
bin2cpp.exe --noheader --file=%OUTDIR%\%TEST_NAME%.bin --output=%OUTDIR% --headerfile=_%TEST_NAME%.h --identifier=%TEST_NAME% --chunksize=100 --override

dir >NUL
