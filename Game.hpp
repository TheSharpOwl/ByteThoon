#pragma once


class Game
{
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
    // returns error code, if no errors then 0
	virtual int Run() = 0;
	virtual int Destroy() = 0;
    virtual ~Game() = default;
};