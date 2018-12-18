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

# Create the root_directory.hpp file from the root_directory.hpp.in template
# This just defines a string containing the path to the root directory of this
# project. Really handy when compiling this project on new machines, since you
# dont have to change any file path's to shaders, OBJs, etc.
configure_file(${CMAKE_SOURCE_DIR}/cmake/root_directory.hpp.in ${CMAKE_SOURCE_DIR}/include/root_directory.hpp)


# Make a list of all the implementation files
set(
    SOURCES
    src/main.cpp
    src/shader.cpp
    src/camera.cpp
    src/mesh.cpp
    src/model.cpp
    src/material.cpp
    src/transform.cpp
    src/resource_loader.cpp
    src/window.cpp

    ext/glad/src/glad.c
    ext/tinyobjloader/tiny_obj_loader.cc
)

# Make a list of all the header files
set(
    INCLUDES
    include/shader.hpp
    include/camera.hpp
    include/common.hpp
    include/mesh.hpp
    include/model.hpp
    include/material.hpp
    include/root_directory.hpp
    include/transform.hpp
    include/resource_loader.hpp
    include/window.hpp

    ext/glad/include/glad/glad.h
    ext/tinyobjloader/tiny_obj_loader.h
)

# Make a list of all of the directories to look in when doing #include "whatever.h"
set(
    INCLUDE_DIRS
    ext/
    include/
    ext/glfw/include
    ext/glad/include
)

set(
    LIBS
    glfw
)

# Actually define what we are trying to produce here (an executable), as
# well as what items are needed to create it (the header and source files)
add_executable(${PROJECT_NAME} ${SOURCES} ${INCLUDES})

# Tell cmake which directories to look in when you #include a file
# Equivalent to the "-I" option for g++
include_directories(${INCLUDE_DIRS})

# Tell cmake which libraries to link to
# Equivalent to the "-l" option for g++
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})

# When compiling this in Debug mode, tack on a "_debug" to the end of the executable name
set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    DEBUG_POSTFIX _debug
)

# For Visual Studio only
if (MSVC)
    # Do a parallel compilation of this project
    target_compile_options(${PROJECT_NAME} PRIVATE "/MP")
    # Have this project be the default one to build when hitting F5 / Ctrl-F5
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()
