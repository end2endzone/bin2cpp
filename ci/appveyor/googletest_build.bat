echo =======================================================================
echo Building googletest solution: %REPOSITORY_ROOT%\third_party\googletest\msvc2010\gtest.sln
echo =======================================================================
cd /d %REPOSITORY_ROOT%\third_party\googletest\msvc2010
msbuild "gtest.sln" /m /verbosity:minimal
echo done.
echo.
echo.
