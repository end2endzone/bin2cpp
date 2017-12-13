echo ======================================================================
echo Running unit tests
echo =======================================================================
cd /d c:\projects\bin2cpp\msvc
set path=%PATH%;c:\projects\bin2cpp\msvc\Win32\Release
bin2cpp_unittest.exe

echo ======================================================================
echo Uploading test results to AppVeyor
echo =======================================================================
set TEST_RESULT_URL=https://ci.appveyor.com/api/testresults/junit/%APPVEYOR_JOB_ID%
set TEST_RESULT_FILE=%CD%\bin2cppTest.x86.release.xml
echo TEST_RESULT_URL=%TEST_RESULT_URL%
echo TEST_RESULT_FILE=%TEST_RESULT_FILE%
powershell "(New-Object 'System.Net.WebClient').UploadFile($($env:TEST_RESULT_URL), $($env:TEST_RESULT_FILE))"
