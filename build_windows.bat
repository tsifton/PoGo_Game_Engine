@echo off
REM PoGo Game Engine Windows Build Script
REM This script builds the game engine using CMake with Visual Studio

echo PoGo Game Engine Windows Build Script
echo =====================================

REM Check if CMake is available
cmake --version >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Error: CMake not found. Please install CMake and add it to your PATH.
    echo Download from: https://cmake.org/download/
    pause
    exit /b 1
)

REM Check if vcpkg toolchain file exists
set VCPKG_TOOLCHAIN=""
if exist "%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" (
    set VCPKG_TOOLCHAIN=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake
    echo Using vcpkg toolchain: %VCPKG_TOOLCHAIN%
) else (
    echo Warning: vcpkg not found. Make sure GLFW and GLEW are installed manually.
    echo Consider installing vcpkg and running: vcpkg install glfw3 glew
)

REM Create build directory
if not exist build mkdir build
cd build

REM Configure with CMake
echo Configuring project with CMake...
if not "%VCPKG_TOOLCHAIN%"=="" (
    cmake .. -DCMAKE_TOOLCHAIN_FILE="%VCPKG_TOOLCHAIN%"
) else (
    cmake ..
)

if %ERRORLEVEL% NEQ 0 (
    echo Error: CMake configuration failed.
    echo Make sure GLFW and GLEW are installed.
    pause
    exit /b 1
)

REM Build the project
echo Building project...
cmake --build . --config Release

if %ERRORLEVEL% NEQ 0 (
    echo Error: Build failed.
    pause
    exit /b 1
)

echo.
echo Build completed successfully!
echo Executable location: build\Release\pogo_demo.exe
echo.
echo Press any key to run the demo...
pause >nul

REM Run the demo
if exist "Release\pogo_demo.exe" (
    echo Running demo...
    Release\pogo_demo.exe
) else if exist "pogo_demo.exe" (
    echo Running demo...
    pogo_demo.exe
) else (
    echo Error: Demo executable not found.
)

pause