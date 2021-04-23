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

:: Prepare CMAKE parameters
set CMAKE_INSTALL_PREFIX=%BIN2CPP_SOURCE_DIR%\install
set CMAKE_PREFIX_PATH=
set CMAKE_PREFIX_PATH=%CMAKE_PREFIX_PATH%;%BIN2CPP_SOURCE_DIR%\third_parties\googletest\install
set CMAKE_PREFIX_PATH=%CMAKE_PREFIX_PATH%;%BIN2CPP_SOURCE_DIR%\third_parties\RapidAssist\install

echo ============================================================================
echo Generating bin2cpp...
echo ============================================================================
cd /d "%BIN2CPP_SOURCE_DIR%"
mkdir build >NUL 2>NUL
cd build
cmake -DCMAKE_GENERATOR_PLATFORM=%Platform% -T %PlatformToolset% -DBIN2CPP_BUILD_TEST=ON -DBIN2CPP_BUILD_SAMPLES=ON -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX="%CMAKE_INSTALL_PREFIX%" -DCMAKE_PREFIX_PATH="%CMAKE_PREFIX_PATH%" ..
if %errorlevel% neq 0 exit /b %errorlevel%

echo ============================================================================
echo Compiling bin2cpp library...
echo ============================================================================
cmake --build . --config %Configuration% -- -maxcpucount /m
if %errorlevel% neq 0 exit /b %errorlevel%
echo.

echo ============================================================================
echo Installing bin2cpp library into %BIN2CPP_SOURCE_DIR%\install
echo ============================================================================
cmake --build . --config %Configuration% --target INSTALL
if %errorlevel% neq 0 exit /b %errorlevel%
echo.

::Return to launch folder
cd /d "%~dp0"
