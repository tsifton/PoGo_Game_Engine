# PoGo Game Engine

A C++ game engine using OpenGL, GLFW, and GLEW for cross-platform window management and graphics.

## Features

- OpenGL rendering pipeline
- Cross-platform window management with GLFW
- Modular engine architecture with ApplicationListener pattern
- Demo application included

## Building

### Prerequisites

On Ubuntu/Debian:
```bash
sudo apt update
sudo apt install -y libglfw3-dev libglew-dev libgl1-mesa-dev build-essential pkg-config cmake
```

### Build Steps

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

## Notes

- The engine has been updated from the original Visual Studio project to support CMake builds on Linux
- All middleware libraries are included in the repository
- The project builds a static library (libpogo_core.a) and links it with the demo executable