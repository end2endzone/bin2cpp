@echo off

:: Validate mandatory environment variables
if "%CONFIGURATION%"=="" (
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
set CMAKE_INSTALL_PREFIX=%BIN2CPP_SOURCE_DIR%\third_parties\googletest\install
set CMAKE_PREFIX_PATH=
set CMAKE_PREFIX_PATH=%CMAKE_PREFIX_PATH%;

echo ============================================================================
echo Cloning googletest into %BIN2CPP_SOURCE_DIR%\third_parties\googletest
echo ============================================================================
mkdir "%BIN2CPP_SOURCE_DIR%\third_parties" >NUL 2>NUL
cd "%BIN2CPP_SOURCE_DIR%\third_parties"
git clone "https://github.com/google/googletest.git"
cd googletest
echo.

echo Checking out version 1.8.0...
git -c advice.detachedHead=false checkout release-1.8.0
echo.

echo ============================================================================
echo Compiling googletest...
echo ============================================================================
mkdir build >NUL 2>NUL
cd build
cmake -DCMAKE_GENERATOR_PLATFORM=%Platform% -T %PlatformToolset% -Dgtest_force_shared_crt=ON -DBUILD_GMOCK=OFF -DBUILD_GTEST=ON -DCMAKE_INSTALL_PREFIX="%CMAKE_INSTALL_PREFIX%" -DCMAKE_PREFIX_PATH="%CMAKE_PREFIX_PATH%" -DCMAKE_CXX_FLAGS=/D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING ..
if %errorlevel% neq 0 exit /b %errorlevel%
cmake --build . --config %CONFIGURATION% -- -maxcpucount /m
if %errorlevel% neq 0 exit /b %errorlevel%
echo.

echo ============================================================================
echo Installing googletest into %BIN2CPP_SOURCE_DIR%\third_parties\googletest\install
echo ============================================================================
cmake --build . --config %CONFIGURATION% --target INSTALL
if %errorlevel% neq 0 exit /b %errorlevel%
echo.

::Return to launch folder
cd /d "%~dp0"
