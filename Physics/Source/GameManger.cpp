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
	switchWeapon = false;
	int weptype = Weapon::NONE;
	int sideweptype = Weapon::NONE;

	pierceBought = false;
	fastfireBought = false;
	fastbulletBought = false;
	explosiveBought = false;
	betterfuelBought = false;
	accuratearrowsBought = false;
	fastmeleeBought = false;
}

GameManger::~GameManger()
{
	// Delete the animated sprites
}
