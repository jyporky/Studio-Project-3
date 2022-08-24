#include "GameManger.h"

GameManger::GameManger()
{
	bPlayerLost = false;
	bDebug = false;
	dWaveNo = 1;
	waveClear = true;
	outShop = false;
	eButtonState = false;
	switchWeapon = false;
	buyFirstWep = true;
	weptype = Weapon::NONE;
	sideweptype = Weapon::NONE;

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
