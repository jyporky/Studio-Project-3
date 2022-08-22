#include "GameManger.h"

GameManger::GameManger()
{
	bGameWin = false;
	bWaveClear = false;
	bPlayerLost = false;
	bDebug = false;
	dWaveNo = 1;
	dPlayerScore = 1000;
	waveClear = false;
	outShop = false;
	eButtonState = false;
}

GameManger::~GameManger()
{
}
