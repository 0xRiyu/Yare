@echo off
REM This script will build Yarezo using the MSVC, it requires the vcvarsall.bat to be in the path
REM Example location to find vcvarsall - Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build

if "%2"=="regen" (
    rm -rf build
    mkdir build
)

cd build
if not defined DevEnvDir (
  vcvarsall.bat x64
)

if "%1"=="debug" goto :Debug
if "%1"=="release" goto :Release
echo Please provide a build type, debug/release
goto :End

:Release
cmake -DCMAKE_BUILD_TYPE=Release -GNinja ..
ninja
goto :End

:Debug
cmake -DCMAKE_BUILD_TYPE=Debug -GNinja ..
ninja
goto :End

:End
cd ..