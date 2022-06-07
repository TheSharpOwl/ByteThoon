# Snake Game
    
Snake game using command line and ASCII symbols

## Requirements:
1. Windows OS
2. MS C++ Compiler (better use VS 2019 or later)
3. CMake 3.18 or newer

## How to build and run:
```bash
git clone https://github.com/TheSharpOwl/SnakeGame
cd SnakeGame
mkdir build & cd build & cmake -A x64 .. & cmake --build . --config Release
```
* If your default generator isn't Visual Studio, you have to specify `-G "Visual Studio 15 2017"` for example to cmake, the last line becomes: 
```bash
mkdir build & cd build & cmake -A x64 -G "Visual Studio 15 2017 .. & cmake --build . --config Release
```
Finally the game exe will be inside `build/Release`

## Possible Improvments:
1. Using a graphics API such as Direct3D, OpenGL or Vulkan
2. Make it multi-player with networking
