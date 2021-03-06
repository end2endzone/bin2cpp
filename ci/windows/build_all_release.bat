@echo off

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

:: Set build configuration parameters
set CONFIGURATION=Release
set PLATFORM=x64
set PLATFORMTOOLSET=""
echo Building RapidAssist for Windows in %CONFIGURATION%, %Platform% configuration...
echo.

:: Return back to scripts folder
cd /d "%~dp0"

:: Call windows scripts one by one.
call "%BIN2CPP_SOURCE_DIR%\ci\windows\install_googletest.bat"
if %errorlevel% neq 0 pause && exit /b %errorlevel%
call "%BIN2CPP_SOURCE_DIR%\ci\windows\install_rapidassist.bat"
if %errorlevel% neq 0 pause && exit /b %errorlevel%
call "%BIN2CPP_SOURCE_DIR%\ci\windows\install_bin2cpp.bat"
if %errorlevel% neq 0 pause && exit /b %errorlevel%
call "%BIN2CPP_SOURCE_DIR%\ci\windows\test_script.bat"
if %errorlevel% neq 0 pause && exit /b %errorlevel%

:: Press a key to continue
pause
