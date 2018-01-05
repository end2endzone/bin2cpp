@echo off

echo Change directory to output directory
cd /d %~dp0
cd ..\..\msvc\Win32\Release

echo =======================================================================
echo Generating test files...
echo =======================================================================
echo Deleting previous folder...
rmdir /q/s generated_files >NUL 2>NUL
echo Creating new folder...
mkdir generated_files
echo Creating files...
call generate_test_files.bat
echo all test files generated...
echo.

echo =======================================================================
echo Running unit tests...
echo =======================================================================
bin2cpp_unittest.exe
echo done
echo.

echo =======================================================================
echo Uploading test results to AppVeyor
echo =======================================================================
set TEST_RESULT_URL=https://ci.appveyor.com/api/testresults/junit/%APPVEYOR_JOB_ID%
set TEST_RESULT_FILE=%CD%\bin2cppTest.x86.release.xml
echo TEST_RESULT_URL=%TEST_RESULT_URL%
echo TEST_RESULT_FILE=%TEST_RESULT_FILE%
powershell "(New-Object 'System.Net.WebClient').UploadFile($($env:TEST_RESULT_URL), $($env:TEST_RESULT_FILE))"
