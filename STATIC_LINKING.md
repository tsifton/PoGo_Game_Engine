# Static Linking Configuration

This document describes the static linking configuration for the PoGo Game Engine.

## Overview

The PoGo Game Engine has been configured for **full static linking** of GLFW and GLEW libraries, ensuring that the built executable has no dependencies on external GLFW/GLEW shared libraries.

## Benefits of Static Linking

- **Self-contained executables**: No need to install GLFW/GLEW on target systems
- **Version consistency**: Eliminates runtime version conflicts
- **Simplified deployment**: Single executable file contains all dependencies
- **Better performance**: Potential for better optimization across library boundaries

## Platform-Specific Configuration

### Windows
- Uses static GLEW library (`glew32s` for release, `glew32sd` for debug)
- Links with required Windows system libraries: `gdi32`, `user32`, `shell32`, `kernel32`, `winmm`
- Defines `GLEW_STATIC` preprocessor macro
- Sets static runtime linking for MSVC (`/MT` for release, `/MTd` for debug)

### Unix/Linux
- Automatically detects and uses static libraries when available
- Links with static GLFW (`libglfw3.a`) and GLEW (`libGLEW.a`)
- Links with required system libraries: `X11`, `Xrandr`, `Xinerama`, `Xcursor`, `Xi`, `pthread`, `dl`, `m`
- Falls back to pkg-config if static libraries are not found

## Build Requirements

### For Unix/Linux Systems
The build system will automatically use static libraries if they are available in standard locations:
- `/usr/lib/x86_64-linux-gnu/libglfw3.a`
- `/usr/lib/x86_64-linux-gnu/libGLEW.a`

If static libraries are not available, install development packages and build static GLFW:
```bash
# Install required packages
sudo apt install libgl1-mesa-dev libglu1-mesa-dev libglew-dev
sudo apt install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev

# Build static GLFW (if not available as package)
git clone https://github.com/glfw/glfw.git
cd glfw && mkdir build && cd build
cmake -DBUILD_SHARED_LIBS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DGLFW_BUILD_WAYLAND=OFF -DGLFW_BUILD_X11=ON ..
make -j$(nproc)
sudo cp src/libglfw3.a /usr/lib/x86_64-linux-gnu/
```

### For Windows Systems
Ensure static versions of GLFW and GLEW are available through:
- vcpkg: `vcpkg install glfw3:x64-windows-static glew:x64-windows-static`
- Manual installation of static libraries

## Verification

To verify that static linking is working correctly:

1. Build the project: `cmake .. && make`
2. Check dynamic dependencies: `ldd pogo_demo`
3. Confirm no GLFW/GLEW dependencies are listed
4. Note the larger executable size (includes static libraries)

## Technical Details

The CMakeLists.txt has been modified to:
1. Detect available static libraries automatically
2. Use appropriate library names for each platform
3. Link with required system libraries for static builds
4. Define necessary preprocessor macros (`GLEW_STATIC`)
5. Configure proper runtime linking options

This ensures minimal code changes while achieving complete static linking across platforms.