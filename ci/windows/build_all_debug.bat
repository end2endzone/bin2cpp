@echo off

:: Set build configuration parameters
set CONFIGURATION=Debug
set PLATFORM=x64
set PLATFORMTOOLSET=""
echo Building bin2cpp for Windows in %CONFIGURATION%, %PLATFORM% configuration...
echo.

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

:: Return back to scripts folder
cd /d "%~dp0"

:: Call windows scripts one by one.
call "%PRODUCT_SOURCE_DIR%\ci\windows\install_googletest.bat"
if %errorlevel% neq 0 pause && exit /b %errorlevel%
call "%PRODUCT_SOURCE_DIR%\ci\windows\install_rapidassist.bat"
if %errorlevel% neq 0 pause && exit /b %errorlevel%
call "%PRODUCT_SOURCE_DIR%\ci\windows\install_bin2cpp.bat"
if %errorlevel% neq 0 pause && exit /b %errorlevel%
call "%PRODUCT_SOURCE_DIR%\ci\windows\test_script.bat"
if %errorlevel% neq 0 pause && exit /b %errorlevel%

:: Press a key to continue
pause
