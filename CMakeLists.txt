# Set the minimum required version of cmake for this project
# Set to 3.1 so we can use the "CMAKE_CXX_STANDARD" below
cmake_minimum_required (VERSION 3.1)

# Require c++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create a project called 'deferred_starter'
project(deferred_starter)

# Also disable building some of the extra things GLFW has (examples, tests, docs)
set(GLFW_BUILD_EXAMPLES  OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_TESTS     OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_DOCS      OFF CACHE BOOL " " FORCE)

# Now actually run cmake on the CMakeLists.txt file found inside of the GLFW directory
add_subdirectory(ext/glfw)

# Make a list of all the implementation files
set(
    SOURCES
    src/main.cpp
    src/shader.cpp
    src/mesh.cpp
    src/model.cpp
    src/material.cpp

    ext/glad/glad.c
    ext/tinyobjloader/tiny_obj_loader.cc
)

# Make a list of all the header files
set(
    INCLUDES
    include/shader.hpp
    include/common.hpp
    include/mesh.hpp
    include/model.hpp
    include/material.hpp

    ext/glad/glad.h
    ext/tinyobjloader/tiny_obj_loader.h
)
 
# Make a list of all of the directories to look in when doing #include "whatever.h"
set(
    INCLUDE_DIRS
    ext/
    include/
    ext/glfw/include
)

# Actually define what we are trying to produce here (an executable), as
# well as what items are needed to create it (the header and source files)
add_executable(${PROJECT_NAME} ${SOURCES} ${INCLUDES})

# Tell cmake which directories to look in when you #include a file
# Equivalent to the "-I" option for g++
include_directories(${INCLUDE_DIRS})

# Tell cmake which libraries to link to
# Equivalent to the "-l" option for g++
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

# If using visual studio, have this project be the default one to build when hitting F5 / Ctrl-F5
if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()
