# deferred-starter

## Description
Starter code for students starting on deferred rendering, after they complete their first OpenGL project themselves. This code is designed with the following goals in mind:
1. Provide them with a lot of the grunt work code (model loading, shader loading, etc), to help them actually focus on the graphics part of the project, and making cool scenes
2. Show them how a project could potentially be structured in an extensible way
3. Compile on Linux, Windows, and Mac, without needing to install anything else (hence the use of CMake, GLAD, and GLFW)
4. Show them how they how to setup cross platform projects using CMake

## Installing + Compiling
```
git clone --recursive https://github.com/LiamTyler/deferred-starter.git
cd deferred-starter
mkdir build
cd build
cmake ..
```

To compile it on linux/mac, just do a `make`. With visual studio,
open up the solution file and build it from there like normal. Note that
with Visual Studio, you might want to do `cmake -G "Visual Studio 15 2017 Win64" ..`
instead of `cmake ..` if you want to get the 64-bit version. You can change the
15 and 2017 to be whatever your VS version is, but I have only tested it on that one.
Also with Visual Studio, don't forget that it defaults to Debug mode
(if you want to change it to Release)
