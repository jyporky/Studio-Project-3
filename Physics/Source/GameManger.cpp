#include "GameManger.h"

GameManger::GameManger()
{
	bGameWin = false;
	bWaveClear = false;
	bPlayerLost = false;
	bDebug = false;
	dWaveNo = 1;
	waveClear = false;
	outShop = false;
	eButtonState = false;
	switchWeapon = false;
	int weptype = Weapon::NONE;
	int sideweptype = Weapon::NONE;
}

GameManger::~GameManger()
{
	// Delete the animated sprites
}
