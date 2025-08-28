cmake_minimum_required(VERSION 3.10)
project(PoGo_Game_Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)

# Find GLFW and GLEW
pkg_check_modules(GLFW REQUIRED glfw3)
pkg_check_modules(GLEW REQUIRED glew)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/PoGo_Engine/pogo_core
    ${CMAKE_CURRENT_SOURCE_DIR}/Middleware/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Middleware/glew/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Middleware/glm
    ${GLFW_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
)

# Core engine library
set(POGO_CORE_SOURCES
    PoGo_Engine/pogo_core/engine.cpp
    PoGo_Engine/pogo_core/window.cpp
    PoGo_Engine/pogo_core/engine_config.cpp
    PoGo_Engine/pogo_core/window_config.cpp
)

set(POGO_CORE_HEADERS
    PoGo_Engine/pogo_core/engine.h
    PoGo_Engine/pogo_core/window.h
    PoGo_Engine/pogo_core/engine_config.h
    PoGo_Engine/pogo_core/window_config.h
    PoGo_Engine/pogo_core/application_listener.h
)

# Create core library
add_library(pogo_core ${POGO_CORE_SOURCES} ${POGO_CORE_HEADERS})

# Link core library with dependencies
target_link_libraries(pogo_core 
    ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARIES}
)

target_compile_options(pogo_core PRIVATE ${GLFW_CFLAGS_OTHER} ${GLEW_CFLAGS_OTHER})

# Demo executable
set(DEMO_SOURCES
    PoGo_Engine/PoGo\ Demo/main.cpp
    PoGo_Engine/PoGo\ Demo/game.cpp
)

set(DEMO_HEADERS
    PoGo_Engine/PoGo\ Demo/game.h
)

# Create demo executable
add_executable(pogo_demo ${DEMO_SOURCES} ${DEMO_HEADERS})

# Link demo with core library and dependencies
target_link_libraries(pogo_demo 
    pogo_core
    ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARIES}
)

target_compile_options(pogo_demo PRIVATE ${GLFW_CFLAGS_OTHER} ${GLEW_CFLAGS_OTHER})