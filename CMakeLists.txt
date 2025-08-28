cmake_minimum_required(VERSION 3.10)
project(PoGo_Game_Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set OpenGL preference to avoid warnings
if(POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

# Platform detection
if(WIN32)
    set(WINDOWS_BUILD TRUE)
else()
    set(WINDOWS_BUILD FALSE)
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/PoGo_Engine/pogo_core
    ${CMAKE_CURRENT_SOURCE_DIR}/Middleware/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Middleware/glew/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Middleware/glew-2.0.0/glew-2.0.0/include
    ${CMAKE_CURRENT_SOURCE_DIR}/Middleware/glm
)

# Find required packages
find_package(OpenGL REQUIRED)

if(WINDOWS_BUILD)
    # Windows static linking configuration
    # Try to find GLFW and GLEW via find_package (works with vcpkg, etc.)
    find_package(glfw3 QUIET)
    find_package(GLEW QUIET)
    
    if(NOT glfw3_FOUND OR NOT GLEW_FOUND)
        message(STATUS "GLFW or GLEW not found via find_package. Using static library names...")
        message(STATUS "Make sure GLFW and GLEW static libraries are installed (e.g., via vcpkg)")
        # Use static library names for Windows
        set(GLFW3_LIBRARIES glfw3)
        set(GLEW_LIBRARIES glew32s)  # Static version
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(GLEW_LIBRARIES glew32sd)  # Static debug version
        endif()
        # Add Windows system libraries required for static GLFW
        set(GLFW_SYSTEM_LIBS gdi32 user32 shell32 kernel32 winmm)
    else()
        set(GLFW3_LIBRARIES glfw)
        set(GLEW_LIBRARIES GLEW::GLEW)
        # Add Windows system libraries for static linking
        set(GLFW_SYSTEM_LIBS gdi32 user32 shell32 kernel32 winmm)
    endif()
else()
    # Unix/Linux static linking configuration
    # Use static libraries directly instead of pkg-config
    message(STATUS "Configuring for static linking on Unix/Linux...")
    
    # Check for static libraries
    find_library(GLFW3_STATIC_LIB 
        NAMES libglfw3.a glfw3
        PATHS /usr/lib/x86_64-linux-gnu /usr/local/lib /usr/lib
        NO_DEFAULT_PATH
    )
    find_library(GLEW_STATIC_LIB 
        NAMES libGLEW.a GLEW
        PATHS /usr/lib/x86_64-linux-gnu /usr/local/lib /usr/lib
        NO_DEFAULT_PATH
    )
    
    if(GLFW3_STATIC_LIB AND GLEW_STATIC_LIB)
        message(STATUS "Found static libraries: ${GLFW3_STATIC_LIB}, ${GLEW_STATIC_LIB}")
        set(GLFW3_LIBRARIES ${GLFW3_STATIC_LIB})
        set(GLEW_LIBRARIES ${GLEW_STATIC_LIB})
        # Add system libraries required for static GLFW on Linux
        set(GLFW_SYSTEM_LIBS X11 Xrandr Xinerama Xcursor Xi pthread dl m)
    else()
        message(STATUS "Static libraries not found, falling back to pkg-config...")
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(GLFW REQUIRED glfw3)
        pkg_check_modules(GLEW REQUIRED glew)
        set(GLFW3_LIBRARIES ${GLFW_LIBRARIES})
        set(GLEW_LIBRARIES ${GLEW_LIBRARIES})
        set(GLFW_SYSTEM_LIBS "")
    endif()
endif()

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
    ${GLFW3_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLFW_SYSTEM_LIBS}
)

# Platform-specific compilation flags
if(WINDOWS_BUILD)
    target_compile_definitions(pogo_core PRIVATE GLEW_STATIC)
    if(MSVC)
        target_compile_options(pogo_core PRIVATE /W4)
        # Set static runtime for MSVC
        target_compile_options(pogo_core PRIVATE /MT$<$<CONFIG:Debug>:d>)
    endif()
else()
    # Define GLEW_STATIC for Unix/Linux static builds
    if(GLFW3_STATIC_LIB AND GLEW_STATIC_LIB)
        target_compile_definitions(pogo_core PRIVATE GLEW_STATIC)
    else()
        target_compile_options(pogo_core PRIVATE ${GLFW_CFLAGS_OTHER} ${GLEW_CFLAGS_OTHER})
    endif()
endif()

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
    ${GLFW3_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLFW_SYSTEM_LIBS}
)

# Platform-specific compilation flags for demo
if(WINDOWS_BUILD)
    target_compile_definitions(pogo_demo PRIVATE GLEW_STATIC)
    if(MSVC)
        target_compile_options(pogo_demo PRIVATE /W4)
        # Set static runtime for MSVC
        target_compile_options(pogo_demo PRIVATE /MT$<$<CONFIG:Debug>:d>)
    endif()
else()
    # Define GLEW_STATIC for Unix/Linux static builds
    if(GLFW3_STATIC_LIB AND GLEW_STATIC_LIB)
        target_compile_definitions(pogo_demo PRIVATE GLEW_STATIC)
    else()
        target_compile_options(pogo_demo PRIVATE ${GLFW_CFLAGS_OTHER} ${GLEW_CFLAGS_OTHER})
    endif()
endif()