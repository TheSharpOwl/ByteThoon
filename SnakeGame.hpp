#include "Game.hpp"

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
#include <string>
#include <algorithm>
#include "PairHelpers.h"

// TODO put inside the class
enum class Direction
{
	Right = 0,
	Left = 1,
	Up = 2,
	Down = 3
};

void inline ShowConsoleCursor(const HANDLE& console, bool showFlag)
{
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(console, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(console, &cursorInfo);
}

class SnakeGame : public Game 
{
public:
	void Initialize() override;
	void Update() override;
	// returns error code, if no errors then 0
	int Run() override;
	int Destroy() override;

	virtual ~SnakeGame() override = default;
private:
	// TODO move to cpp
	bool moveSnake(bool& onY, int foodX, int foodY)
	{
		// default direction when we start is right 
		static Direction currentDirection = Direction::Right;

		Direction newDirection = currentDirection;
		// TODO check keyboard input and take it to new direction
		for (int k = 0; k < 4; k++)
			if ((0x8000 & GetAsyncKeyState((unsigned char)("\x44\x41\x53\x57"[k])))) // D A W S
			{
				newDirection = static_cast<Direction>(k);
				onY = k > 1;
			}

		// you can't go right directly from left (and the opposite) nor up then down directly (nor the opposite)
		std::pair<int, int> checkSum = directions[static_cast<int>(currentDirection)] + directions[static_cast<int>(newDirection)];
		if (checkSum.first != 0 && checkSum.second != 0)
			currentDirection = newDirection;
		std::pair<int, int> lastTail = bodyCoord.back();

		//exclude the tail place
		taken[lastTail.first][lastTail.second] = false;

		auto temp = bodyCoord[0];
		for (auto& a : bodyCoord)
		{
			if (a == bodyCoord[0])//the head
				a += directions[static_cast<int>(currentDirection)];
			else
				swap(a, temp);
		}

		//include the new head place
		temp = bodyCoord[0];
		taken[temp.first][temp.second] = true;

		if (temp == std::make_pair(foodX, foodY))
		{
			// return the last tail (it's the new piece)
			bodyCoord.push_back(lastTail);
			return true;
		}
		else
			return false;
	}

	std::pair<int, int> generateFood()
	{

		int foodX, foodY;
		do
		{
			foodX = std::rand() % (fieldWidth - 2) + 1;
			foodY = std::rand() % (fieldHeight - 2) + 1;

		} while (taken[foodX][foodY]);

		foodAv = true;
		return { foodX, foodY };
	}
	void printPlayerInfo(wchar_t* screen, int score, int gamesLeft)
	{
		auto printedScore = std::to_wstring(score);
		auto printedGames = std::to_wstring(gamesLeft);

		for (unsigned int i = 0; i < printedScore.size(); i++)
			screen[screenWidth * 5 + fieldWidth + 7 + i] = printedScore[i];

		for (unsigned int i = 0; i < printedGames.size(); i++)
			screen[screenWidth * 9 + fieldWidth + 7 + i] = printedGames[i];
	}

	const int fieldWidth = 50, fieldHeight = 25;
	const int screenWidth = 96, screenHeight = 26;
	const int games = 3;
	const int TriesAllowed = 3;
	bool taken[50][25]; // [i][j] is true if there's a snake part at [i][j]
	bool gameOver = false, foodAv = false;
	wchar_t* playFieldBuffer;

	std::vector<std::pair<int, int>> bodyCoord;
	std::pair<int, int> directions[4] = { {1,0},{-1,0}, {0,1}, {0,-1} };

};