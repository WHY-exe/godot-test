@echo off

set BUILD_TYPE=rel
if "%1"=="--debug" (
    set BUILD_TYPE=deb
)

cmake --preset win64-%BUILD_TYPE%
cmake --build --preset win64-%BUILD_TYPE% --target --install
cmake --build --preset win64-%BUILD_TYPE% --target DumpSymForBreakpad

cmake --preset win32-%BUILD_TYPE%
cmake --build --preset win32-%BUILD_TYPE% --target --install
cmake --build --preset win32-%BUILD_TYPE% --target DumpSymForBreakpad
