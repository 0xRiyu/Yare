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

if "%1"=="regen" set REGEN=TRUE
if "%2"=="regen" set REGEN=TRUE
if "%3"=="regen" set REGEN=TRUE

if "%1"=="release" set RELEASE_MODE=TRUE
if "%2"=="release" set RELEASE_MODE=TRUE
if "%3"=="release" set RELEASE_MODE=TRUE

if "%1"=="run" set RUN=TRUE
if "%2"=="run" set RUN=TRUE
if "%3"=="run" set RUN=TRUE

if %REGEN%==TRUE (
    rm -rf build
    mkdir build
)

cd build

if not defined DevEnvDir (
   call vcvarsall.bat x64
)

if %RELEASE_MODE%==FALSE goto :Debug
if %RELEASE_MODE%==TRUE goto :Release

:Release
echo Building in Release Mode...
cmake -DCMAKE_BUILD_TYPE=Release -GNinja ..
IF %ERRORLEVEL% NEQ 0 EXIT 1
goto :ninja

:Debug
echo Building in Debug Mode...
cmake -DCMAKE_BUILD_TYPE=Debug -GNinja ..
IF %ERRORLEVEL% NEQ 0 EXIT 1
goto :ninja

:ninja
ninja
cd ..
IF %ERRORLEVEL% NEQ 0 EXIT 1
if %RUN%==FALSE goto :eof
echo.
echo Starting Sandbox.exe...
cd build\Sandbox
Sandbox.exe
cd ..\..
