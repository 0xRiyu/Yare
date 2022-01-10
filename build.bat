@echo off
REM This script will build Yare using the MSVC and ninja, it requires vcvarsall.bat/ninja to be in the path
REM Example location to find vcvarsall - Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build

if not exist build (
    mkdir build
)
REM default build in debug
set RELEASE_MODE=FALSE
set REGEN=FALSE
set RUN=FALSE
set root=%~dp0

if "%1"=="regen" set REGEN=TRUE
if "%2"=="regen" set REGEN=TRUE
if "%3"=="regen" set REGEN=TRUE

if "%1"=="release" set RELEASE=TRUE
if "%2"=="release" set RELEASE=TRUE
if "%3"=="release" set RELEASE_MODE=TRUE

if "%1"=="run" set RUN=TRUE
if "%2"=="run" set RUN=TRUE
if "%3"=="run" set RUN=TRUE

if %REGEN%==TRUE (
    rm -rf build
)

mkdir build 2>NUL
cd build

if not defined DevEnvDir (
   call "%ProgramFiles(x86)%/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build/vcvarsall.bat" x64
)

if %RELEASE_MODE%==TRUE (
echo Building in Release Mode...
cmake -DCMAKE_BUILD_TYPE=Release -GNinja ..
)
if %RELEASE_MODE%==FALSE (
echo Building in Debug Mode...
cmake -DCMAKE_BUILD_TYPE=Debug -GNinja ..
)
IF %ERRORLEVEL% NEQ 0 goto :exit

ninja
IF %ERRORLEVEL% NEQ 0 goto :exit
if %RUN%==FALSE goto :exit
echo Starting Sandbox.exe...
cd %root%\build\Sandbox
Sandbox.exe
echo Process exited with Error Level: %ERRORLEVEL%

:exit
cd %root%
