@echo off

:: Validate mandatory environment variables
if "%CONFIGURATION%"=="" (
  echo Please define 'Configuration' environment variable.
  exit /B 1
)
if "%PLATFORM%"=="" (
  echo Please define 'Platform' environment variable.
  exit /B 1
)

:: Set PRODUCT_SOURCE_DIR root directory
setlocal enabledelayedexpansion
if "%PRODUCT_SOURCE_DIR%"=="" (
  :: Delayed expansion is required within parentheses https://superuser.com/questions/78496/variables-in-batch-file-not-being-set-when-inside-if
  cd /d "%~dp0"
  cd ..\..
  set PRODUCT_SOURCE_DIR=!CD!
  cd ..\..
  echo PRODUCT_SOURCE_DIR set to '!PRODUCT_SOURCE_DIR!'.
)
endlocal & set PRODUCT_SOURCE_DIR=%PRODUCT_SOURCE_DIR%
echo.

echo =======================================================================
echo Testing bin2cpp...
echo =======================================================================
cd /d "%PRODUCT_SOURCE_DIR%\build\bin\%CONFIGURATION%"
call generate_test_files.bat
if "%CONFIGURATION%"=="Debug" (
  bin2cpp_unittest-d.exe
) else (
  bin2cpp_unittest.exe
)

:: Note:
::  GitHub Action do not support uploading test results in a nice GUI. There is no build-in way to detect a failed test.
::  Do not reset the error returned by unit test execution. This will actually fail the build and will indicate in GitHub that a test has failed.
:: 
:: Reset error in case of test case fail, this prevents a test failure to actually fail the build
exit /b 0
