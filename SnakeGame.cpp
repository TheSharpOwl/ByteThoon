#include "SnakeGame.hpp"


#include <ctime>



void SnakeGame::Initialize()
{
	// clear the everything if we played before to start over
	bodyCoord.clear();

	// using time as a seed for our random start point
	std::srand((unsigned int)std::time(nullptr));
	int startX = std::rand() % (fieldWidth - 2) + 1;
	int startY = std::rand() % (fieldHeight - 2) + 1;

	bodyCoord.push_back({ startX, startY });
}
void SnakeGame::Update()
{
	
}
// returns error code, if no errors then 0
int SnakeGame::Run()
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

	Initialize();
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
			Initialize();
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
	return 0;
}
int SnakeGame::Destroy()
{
	return 0;
}