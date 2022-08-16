#pragma once
#include "SingletonTemplate.h"

class GameManger : public CSingletonTemplate<GameManger>
{
public:
	GameManger();
	~GameManger();
	bool bGameWin;
	bool bWaveClear;
	bool bPlayerLost;
	bool dDebug;
	unsigned dWaveNo;
};

