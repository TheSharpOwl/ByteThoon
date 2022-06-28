# ByteThoon
    
### Console Snake Game using C++ Win32 API
<br />
<div align="center">
<img src="https://file.asciiart.cc/cover/6fbd2fc56e496c14.png!cover_jpg" alt="Console Snake Image"> 
</div>
<br />

## Requirements:
### To Play?
Just grab the exe from Releases :sunglasses:

### To Build
1. Windows OS
2. MS C++ Compiler (better use VS 2019 or later)
3. CMake 3.18 or newer

## Build and Run:
```bash
git clone https://github.com/TheSharpOwl/SnakeGame
cd SnakeGame
mkdir build & cd build & cmake -A x64 .. & cmake --build . --config Release
```
* If your default generator isn't Visual Studio, you have to specify `-G "Visual Studio 15 2017"` (for example) to cmake. Thus, the last line becomes: 
```bash
mkdir build & cd build & cmake -A x64 -G "Visual Studio 15 2017 .. & cmake --build . --config Release
```
Finally the game exe will be inside `build/Release`

## Possible Improvments:
1. Refactor the code
2. Make it multi-player with networking
