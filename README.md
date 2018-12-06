# deferred-starter

## Description

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