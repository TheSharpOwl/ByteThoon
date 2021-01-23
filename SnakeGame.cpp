#include <iostream>
#include <vector>
#include <utility>
#include <random>
#include <ctime>
#include <cstdlib>
#include <Windows.h>
#include <thread>
#include <stdio.h>
#include <chrono>

const int fieldWidth = 70, fieldHeight = 20;
const int screenWidth = 100, screenHeight = 23;

std::vector<std::pair<unsigned int, unsigned int>> bodyCoord;

void draw(int fieldWidth, int fieldHeight)
{
    for (unsigned int i = 0; i < fieldHeight + 1; i++)
    {
        for (unsigned int j = 0; j < fieldWidth + 1; j++)
        {
            if (i == 0 || i == fieldHeight - 1 || j == 0 || j == fieldWidth - 1)
                std::cout << "#";
            else if (i == bodyCoord[0].second && j == bodyCoord[0].first)
                std::cout << "*";
            else
                std::cout << " ";
        }
        std::cout << "\n";
    }
}

void start()
{
    // using time as a seed for our random start point
    std::srand((unsigned int) std::time(nullptr));
    uint32_t startX = std::rand() % fieldWidth + 1;
    uint32_t startY = std::rand() % fieldHeight + 1;

    bodyCoord.push_back({startX, startY});
}

int main()
{
    wchar_t* screen = new wchar_t[screenWidth * screenHeight];
    DWORD writtenBytes = 0;

    for (int i = 0; i < screenWidth * screenHeight; i++)
        screen[i] = L' ';
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);

    wchar_t* playFieldBuffer = new wchar_t[fieldWidth * fieldHeight];

    for(int i = 0; i < fieldWidth;i++)
        for (int j = 0 ; j < fieldHeight; j++)
        {
            if (i == 0 || j == 0 || i + 1 == fieldWidth || j + 1 ==  fieldHeight)
                playFieldBuffer[j * fieldWidth + i] = L'#';
            else
                playFieldBuffer[j * fieldWidth + i] = L' ';
        }

    // TODO change this line to game over condition or something else
    int t = 0;
    while (t < 500)
    {
        t++;
        
        for (int x = 0; x < fieldWidth; x++)
            for (int y = 0; y < fieldHeight; y++)
                screen[y * screenWidth + x] = playFieldBuffer[y * fieldWidth + x];

        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Small Step = 1 Game Tick

        WriteConsoleOutputCharacter(hConsole, screen, screenWidth * screenHeight, { 0,0 }, &writtenBytes);
    }
    //std::cout << dwBytesWritten << std::endl;
    CloseHandle(hConsole);
    return 0;
}