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

	reset = false;
	//self upgrades
	speedUpgradeBought = false;
	healthUpgradeBought = false;
	meleeUpgradeBought = false;
	rangeUpgradeBought = false;

	//skills
	empBought = false;
	hackBought = false;
	healBought = false;
	immortalBought = false;
	overdriveBought = false;

	//weapons
	boxingGloveBought = false;

	rifleBought = false;
	flamethrowerBought = false;
	crossbowBought = false;

	//weapon upgrades
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
