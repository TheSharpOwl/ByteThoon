/*
* The Console Old game since I am making one with DirectX12 drawing 
* Just call RunGame();
* and everthing will work as before
*/

// TODO fix the hit yourself bug (it is not losing when this happens)
#pragma once
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


const int fieldWidth = 50, fieldHeight = 25;
const int screenWidth = 96, screenHeight = 26;
const int games = 3;
const auto TriesAllowed = 3;
bool taken[50][25]; // [i][j] is true if there's a snake part at [i][j]
bool gameOver = false, foodAv = false;
wchar_t* playFieldBuffer;

std::vector<std::pair<int, int>> bodyCoord;
std::pair<int, int> directions[] = { {1,0},{-1,0}, {0,1}, {0,-1} };

void ShowConsoleCursor(const HANDLE& console, bool showFlag)
{
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(console, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(console, &cursorInfo);
}


void start()
{
	// clear the everything if we played before to start over
	bodyCoord.clear();

	// using time as a seed for our random start point
	std::srand((unsigned int)std::time(nullptr));
	int startX = std::rand() % (fieldWidth - 2) + 1;
	int startY = std::rand() % (fieldHeight - 2) + 1;

	bodyCoord.push_back({ startX, startY });
}

enum class Direction
{
	Right = 0,
	Left = 1,
	Up = 2,
	Down = 3
};

// returns true if the snake just got the food
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


void RunGame()
{
	// setting up the console screen
	wchar_t* screen = new wchar_t[screenWidth * screenHeight];
	DWORD writtenBytes = 0;
	for (int i = 0; i < screenWidth * screenHeight; i++)
		screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	ShowConsoleCursor(hConsole, false);

	// filling the player field without the snake
	playFieldBuffer = new wchar_t[fieldWidth * fieldHeight];
	for (int i = 0; i < fieldWidth; i++)
		for (int j = 0; j < fieldHeight; j++)
		{
			if (i == 0 || j == 0 || i + 1 == fieldWidth || j + 1 == fieldHeight)
				playFieldBuffer[j * fieldWidth + i] = L'#';
			else
				playFieldBuffer[j * fieldWidth + i] = L' ';
		}
	for (int i = 0; i < 6; i++)
	{
		screen[screenWidth * 4 + fieldWidth + 7 + i] = L"SCORE:"[i];
	}

	for (int i = 0; i < 11; i++)
	{
		screen[screenWidth * 8 + fieldWidth + 7 + i] = L"TRIES LEFT:"[i];
	}

	start();
	auto food = generateFood();

	// TODO change this line to game over condition "counter" or something else
	int losts = 0, score = 0;
	while (losts < TriesAllowed)
	{
		// copy the playground to the screen buffer
		for (int x = 0; x < fieldWidth; x++)
			for (int y = 0; y < fieldHeight; y++)
				screen[y * screenWidth + x] = playFieldBuffer[y * fieldWidth + x];

		bool onY = false;
		bool justAte = false;

		if (gameOver)
		{
			start();
			gameOver = false;
		}
		else
		{
			justAte = moveSnake(onY, food.first, food.second);
		}



		// copy the snake to the screen buffer and if we lost and hit something mark it differently
		int atHead = 0;
		for (auto a : bodyCoord)
		{
			atHead += (a == bodyCoord[0]);

			if (0 < a.first && a.first + 1 < fieldWidth && 0 < a.second && a.second + 1 < fieldHeight && atHead < 2)
				screen[a.second * screenWidth + a.first] = L'*';
			else
			{
				screen[a.second * screenWidth + a.first] = L'X';
				gameOver = true;
			}
		}

		if (!gameOver && justAte)
		{
			food = generateFood();
			score++;
		}

		screen[food.second * screenWidth + food.first] = L'+';

		printPlayerInfo(screen, score, games - losts);

		WriteConsoleOutputCharacter(hConsole, screen, screenWidth * screenHeight, { 0,0 }, &writtenBytes);

		// sleep for more time when you lose for the player comfort
		float extraTime = (gameOver) ? 1.f : 0.05f;
		if (onY)
			extraTime *= float(screenWidth) / float(screenHeight);
		auto sleepingTime = static_cast<std::chrono::milliseconds>(int(extraTime * 1000) + 50);
		std::this_thread::sleep_for(sleepingTime); // Small Step = 1 Game Tick

		losts += gameOver;
	}

	CloseHandle(hConsole);
}