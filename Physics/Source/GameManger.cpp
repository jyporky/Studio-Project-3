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
	bulletExplosionRadius = 20;

	reset = false;
	//self upgrades
	speedUpgradeBought = false;
	healthUpgradeBought = false;
	meleeUpgradeBought = false;
	rangeUpgradeBought = false;

	//skills
	empBought = false;
	hackBought = false;
	doppelgangerBought = false;
	immortalBought = false;
	blackholeBought = false;

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
