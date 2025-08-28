# PoGo Game Engine

A C++ game engine using OpenGL, GLFW, and GLEW for cross-platform window management and graphics.

## Features

- OpenGL rendering pipeline
- Cross-platform window management with GLFW
- Modular engine architecture with ApplicationListener pattern
- Demo application included

## Building

### Windows

#### Option 1: Visual Studio (Recommended)

**Prerequisites:**
- Microsoft Visual Studio 2015 or later (Community, Professional, or Enterprise)
- Windows SDK 8.1 or later

**Build Steps:**
1. Clone the repository:
```cmd
git clone <repository-url>
cd PoGo_Game_Engine
```

2. Open `PoGo_Engine/PoGo_Engine.sln` in Visual Studio

3. Build the solution:
   - Select your desired configuration (Debug/Release) and platform (x86/x64)
   - Right-click the solution in Solution Explorer → Build Solution
   - Or use Ctrl+Shift+B

4. Run the demo:
   - Set "PoGo Demo" as the startup project (right-click → Set as StartUp Project)
   - Press F5 to run with debugging, or Ctrl+F5 to run without debugging

**Note:** The Visual Studio projects expect GLEW and GLFW libraries to be available in the Middleware folders. If you encounter linker errors, you may need to build or download these dependencies separately.

#### Option 2: CMake with Visual Studio

**Prerequisites:**
- CMake 3.10 or later
- Microsoft Visual Studio 2015 or later
- vcpkg (recommended for dependency management)

**Install Dependencies using vcpkg:**
```cmd
# Install vcpkg if not already installed
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install

# Install required packages
.\vcpkg install glfw3:x64-windows glew:x64-windows
# For 32-bit: .\vcpkg install glfw3:x86-windows glew:x86-windows
```

**Build Steps:**
1. Clone the repository:
```cmd
git clone <repository-url>
cd PoGo_Game_Engine
```

2. Create build directory and configure:
```cmd
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]\scripts\buildsystems\vcpkg.cmake
```

3. Build the project:
```cmd
cmake --build . --config Release
```

4. Run the demo:
```cmd
Release\pogo_demo.exe
```

#### Option 3: CMake with MinGW

**Prerequisites:**
- CMake 3.10 or later
- MinGW-w64
- MSYS2 (recommended for package management)

**Install Dependencies using MSYS2:**
```bash
# In MSYS2 terminal
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-glfw mingw-w64-x86_64-glew
```

**Build Steps:**
1. Open MSYS2 MinGW 64-bit terminal

2. Clone and build:
```bash
git clone <repository-url>
cd PoGo_Game_Engine
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
mingw32-make -j$(nproc)
```

3. Run the demo:
```bash
./pogo_demo.exe
```

#### Option 4: Quick Build Script (Windows)

For convenience, a build script is provided:

1. Ensure CMake and vcpkg are installed and in your PATH
2. Install dependencies: `vcpkg install glfw3 glew`
3. Run the build script:
```cmd
build_windows.bat
```

This script will automatically configure, build, and run the demo.

### Linux/Unix

#### Prerequisites

On Ubuntu/Debian:
```bash
sudo apt update
sudo apt install -y libglfw3-dev libglew-dev libgl1-mesa-dev build-essential pkg-config cmake
```

#### Build Steps

1. Clone the repository:
```bash
git clone <repository-url>
cd PoGo_Game_Engine
```

2. Create build directory and configure:
```bash
mkdir build
cd build
cmake ..
```

3. Build the project:
```bash
make -j$(nproc)
```

4. Run the demo:
```bash
./pogo_demo
```

## Project Structure

- `PoGo_Engine/pogo_core/` - Core engine library
- `PoGo_Engine/PoGo Demo/` - Demo application
- `Middleware/` - Third-party libraries (GLFW, GLEW, GLM)

## Engine Architecture

- **Engine**: Main engine class managing the game loop
- **Window**: GLFW wrapper for OpenGL window creation
- **ApplicationListener**: Interface for game implementations
- **Game**: Demo implementation of ApplicationListener

The engine follows a simple game loop pattern:
1. Initialize window and OpenGL context
2. Loop while window is open:
   - Update game logic
   - Render frame
   - Swap buffers and poll events

## Running the Application

### Windows
After building, the demo executable will be located at:
- Visual Studio: `PoGo_Engine/Debug/PoGo Demo.exe` or `PoGo_Engine/Release/PoGo Demo.exe`
- CMake builds: `build/Release/pogo_demo.exe` or `build/pogo_demo.exe`

You can run it directly by double-clicking the executable or from the command line:
```cmd
cd build\Release
pogo_demo.exe
```

### Linux
After building, run the demo with:
```bash
cd build
./pogo_demo
```

The application will open an OpenGL window with a simple demo scene. Press ESC or close the window to exit.

## Troubleshooting

### Windows Build Issues

**"GLFW/GLEW not found" errors:**
- Install dependencies using vcpkg: `vcpkg install glfw3 glew`
- Or download pre-built libraries and place them in system paths
- For Visual Studio: ensure Windows SDK is installed

**Visual Studio linker errors:**
- The .vcxproj files expect libraries in `Middleware/glfw/lib-vc2015` and `Middleware/glew/lib/Release/Win32`
- Consider using the CMake build method instead
- Update library paths in project properties if needed

**OpenGL32.lib not found:**
- Install Windows SDK
- For MinGW: install mesa development packages

### Linux Build Issues

**Package not found errors:**
- Ubuntu/Debian: `sudo apt install libglfw3-dev libglew-dev libgl1-mesa-dev`
- Fedora: `sudo dnf install glfw-devel glew-devel mesa-libGL-devel`
- Arch: `sudo pacman -S glfw glew mesa`

## Notes

- The engine has been updated from the original Visual Studio project to support CMake builds on Linux and Windows
- All middleware libraries are included in the repository (headers only)
- The project builds a static library (libpogo_core.a/.lib) and links it with the demo executable
- Windows users can use either the provided Visual Studio solution or the CMake build system