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


// CPP standard can u please add + operator to std::pair))
// call me crazy I don't care
template<typename T, typename U>
std::pair<T, U> operator+(const std::pair<T, U>& l, const std::pair<T, U>& r)
{
    return { l.first + r.first, l.second + r.second };
}

template<typename T, typename U>
std::pair<T, U>& operator+=(std::pair<T, U>& l, const std::pair<T, U>& r)
{
	l.first += r.first;
	l.second += r.second;
	return l;
}

const int fieldWidth = 70, fieldHeight = 20;
const int screenWidth = 100, screenHeight = 23;

bool gameOver = false;
wchar_t* playFieldBuffer;

std::vector<std::pair<int,int>> bodyCoord;
std::pair<int, int> directions[] = { {1,0},{-1,0}, {0,1}, {0,-1} };

void draw(int fieldWidth, int fieldHeight)
{
    for (int i = 0; i < fieldHeight + 1; i++)
    {
        for (int j = 0; j < fieldWidth + 1; j++)
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
    // clear the everything if we played before to start over
    bodyCoord.clear();

    // using time as a seed for our random start point
    std::srand((unsigned int) std::time(nullptr));
    uint32_t startX = std::rand() % (fieldWidth - 2)  + 1;
    uint32_t startY = std::rand() % (fieldHeight - 2) + 1;

    bodyCoord.push_back({startX, startY});
}

enum class Direction
{
	Right = 0,
	Left = 1,
	Up = 2,
	Down = 3
};

void moveSnake()
{
    // default direction when we start is right 
    static Direction currentDirection = Direction::Right;

    Direction newDirection = currentDirection;
    // TODO check keyboard input and take it to new direction

    // you can't go right directly from left (and the opposite) nor up then down directly (nor the opposite)
    std::pair<int,int> checkSum = directions[static_cast<int>(currentDirection)] + directions[static_cast<int>(newDirection)];
    if (checkSum.first != 0 && checkSum.second != 0)
        currentDirection = newDirection;

    auto temp = bodyCoord[0];
    for (auto& a : bodyCoord)
    {
        if (a == bodyCoord[0])//the head
            a += directions[static_cast<int>(currentDirection)];
        else
            swap(a, temp);
    }
}
int main()
{
    // setting up the console screen
    wchar_t* screen = new wchar_t[screenWidth * screenHeight];
    DWORD writtenBytes = 0;
    for (int i = 0; i < screenWidth * screenHeight; i++)
        screen[i] = L' ';
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);

    // filling the player field without the snake
    playFieldBuffer = new wchar_t[fieldWidth * fieldHeight];
    for(int i = 0; i < fieldWidth;i++)
        for (int j = 0 ; j < fieldHeight; j++)
        {
            if (i == 0 || j == 0 || i + 1 == fieldWidth || j + 1 ==  fieldHeight)
                playFieldBuffer[j * fieldWidth + i] = L'#';
            else
                playFieldBuffer[j * fieldWidth + i] = L' ';
        }

    start();

    // TODO change this line to game over condition or something else
    int t = 0;
    while (t < 500)
    {
        t++;

		// copy the playground to the screen buffer
		for (int x = 0; x < fieldWidth; x++)
			for (int y = 0; y < fieldHeight; y++)
				screen[y * screenWidth + x] = playFieldBuffer[y * fieldWidth + x];

        if (gameOver)
        {
            start();
            gameOver = false;
        }
        else
            moveSnake();

		// copy the snake to the screen buffer and if we lost and hit something mark it differently
        int atHead = 0;
        for (auto a : bodyCoord)
        {
            atHead += (a == bodyCoord[0]);

			if (0 < a.first && a.first + 1 < fieldWidth  && 0 < a.second && a.second + 1 < fieldHeight && atHead < 2)
				screen[a.second * screenWidth + a.first] = L'*';
			else
			{
				screen[a.second * screenWidth + a.first] = L'X';
				gameOver = true;
			}
        }


       WriteConsoleOutputCharacter(hConsole, screen, screenWidth * screenHeight, { 0,0 }, &writtenBytes);

       // sleep for more time when you lose for the player comfort
       int sleepingTime = gameOver ? 1000 : 50;
       std::this_thread::sleep_for(std::chrono::milliseconds(sleepingTime)); // Small Step = 1 Game Tick
    }
    //std::cout << dwBytesWritten << std::endl;
    CloseHandle(hConsole);
    return 0;
}