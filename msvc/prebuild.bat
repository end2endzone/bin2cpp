@echo off
:: %1 is $(ConfigurationName)
:: %2 is $(ConfigurationName)

set BIN2CPP_EXE=..\%2\%1\bin2cpp.exe
%BIN2CPP_EXE% splashscreen.png   . EmbededSplashScreen.h splashScreen -override 250
%BIN2CPP_EXE% bin2cppTest.vcproj . EmbededProjectFile.h Vcproj 250 -override
