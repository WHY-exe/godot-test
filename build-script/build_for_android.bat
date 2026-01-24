@echo off

set BUILD_DEBUG=0
if "%1"=="--debug" (
    set BUILD_DEBUG=1
)

cmake --preset android-arm64-v8a
cmake --build ./out/build/android-arm64-v8a --target --install

cmake --preset android-armeabi-v7a
cmake --build ./out/build/android-armeabi-v7a --target --install

cmake --preset android-x86
cmake --build ./out/build/android-x86 --target --install

cmake --preset android-x86-64
cmake --build ./out/build/android-x86-64 --target --install

if BUILD_DEBUG==0 (
    cmake --build ./out/build/android-arm64-v8a --target --StripDebugInfo
    cmake --build ./out/build/android-armeabi-v7a --target --StripDebugInfo
    cmake --build ./out/build/android-x86 --target --StripDebugInfo
    cmake --build ./out/build/android-x86-64 --target --StripDebugInfo
)