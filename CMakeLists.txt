cmake_minimum_required(VERSION 3.10)
project(PoGo_Game_Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
if(WINDOWS_BUILD)
    # On Windows, we'll link with system OpenGL and expect GLFW/GLEW to be installed
    find_package(OpenGL REQUIRED)
    
    # Try to find GLFW and GLEW via find_package (works with vcpkg, etc.)
    find_package(glfw3 QUIET)
    find_package(GLEW QUIET)
    
    if(NOT glfw3_FOUND OR NOT GLEW_FOUND)
        message(STATUS "GLFW or GLEW not found via find_package. Trying manual linking...")
        message(STATUS "Make sure GLFW and GLEW are installed (e.g., via vcpkg)")
        # Fallback to manual library names
        set(GLFW3_LIBRARIES glfw3)
        set(GLEW_LIBRARIES glew32)
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(GLEW_LIBRARIES glew32d)
        endif()
    else()
        set(GLFW3_LIBRARIES glfw)
        set(GLEW_LIBRARIES GLEW::GLEW)
    endif()
else()
    # Unix/Linux build using pkg-config
    find_package(PkgConfig REQUIRED)
    find_package(OpenGL REQUIRED)
    
    pkg_check_modules(GLFW REQUIRED glfw3)
    pkg_check_modules(GLEW REQUIRED glew)
    
    set(GLFW3_LIBRARIES ${GLFW_LIBRARIES})
    set(GLEW_LIBRARIES ${GLEW_LIBRARIES})
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
)

# Platform-specific compilation flags
if(WINDOWS_BUILD)
    target_compile_definitions(pogo_core PRIVATE GLEW_STATIC)
    if(MSVC)
        target_compile_options(pogo_core PRIVATE /W4)
    endif()
else()
    target_compile_options(pogo_core PRIVATE ${GLFW_CFLAGS_OTHER} ${GLEW_CFLAGS_OTHER})
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
)

# Platform-specific compilation flags for demo
if(WINDOWS_BUILD)
    target_compile_definitions(pogo_demo PRIVATE GLEW_STATIC)
    if(MSVC)
        target_compile_options(pogo_demo PRIVATE /W4)
    endif()
else()
    target_compile_options(pogo_demo PRIVATE ${GLFW_CFLAGS_OTHER} ${GLEW_CFLAGS_OTHER})
endif()