@echo off

REM **************************************
REM  Build Installer (setup) package
REM **************************************

cd /d %~dp0
set PRODUCT_PLATFORM=win32

"C:\Program Files (x86)\NSIS\makensis.exe" /DPLATFORM_INCLUDE=%PRODUCT_PLATFORM%.nsh main.nsi

pause
