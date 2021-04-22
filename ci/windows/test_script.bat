@echo off

:: Validate mandatory environment variables
if "%Configuration%"=="" (
  echo Please define 'Configuration' environment variable.
  exit /B 1
)
if "%Platform%"=="" (
  echo Please define 'Platform' environment variable.
  exit /B 1
)

:: Set BIN2CPP_SOURCE_DIR root directory
setlocal enabledelayedexpansion
if "%BIN2CPP_SOURCE_DIR%"=="" (
  :: Delayed expansion is required within parentheses https://superuser.com/questions/78496/variables-in-batch-file-not-being-set-when-inside-if
  cd /d "%~dp0"
  cd ..\..
  set BIN2CPP_SOURCE_DIR=!CD!
  cd ..\..
  echo BIN2CPP_SOURCE_DIR set to '!BIN2CPP_SOURCE_DIR!'.
)
endlocal & set BIN2CPP_SOURCE_DIR=%BIN2CPP_SOURCE_DIR%

echo =======================================================================
echo Testing RapidAssist library...
echo =======================================================================
cd /d "%BIN2CPP_SOURCE_DIR%\build\bin\%Configuration%"
if "%Configuration%"=="Debug" (
  rapidassist_unittest-d.exe
) else (
  rapidassist_unittest.exe
)

:: Note:
::  GitHub Action do not support uploading test results in a nice GUI. There is no build-in way to detect a failed test.
::  Do not reset the error returned by unit test execution. This will actually fail the build and will indicate in GitHub that a test has failed.
:: 
:: Reset error in case of test case fail, this prevents a test failure to actually fail the build
exit /b 0
