#pragma once
#include "SingletonTemplate.h"
#include "SpriteAnimation.h"

class GameManger : public CSingletonTemplate<GameManger>
{
public:
	GameManger();
	~GameManger();
	bool bGameWin;
	bool bWaveClear;
	bool bPlayerLost;
	bool bDebug;
	unsigned dWaveNo;
	unsigned dPlayerScore;
	bool waveClear;
	bool outShop;
	bool eButtonState;
};

