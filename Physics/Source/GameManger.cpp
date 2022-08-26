#include "GameManger.h"

GameManger::GameManger()
{
	bPlayerLost = false;
	bDebug = false;
	dWaveNo = 1;
	highestWave = 0;
	waveClear = true;
	outShop = false;
	eButtonState = false;
	switchWeapon = false;
	buyFirstWep = true;
	weptype = Weapon::NONE;
	sideweptype = Weapon::NONE;
	skilltype = Skill::NONE;
	bulletExplosionRadius = 20;
	showHealthBar = true;

	buyFirstSkill = true;
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

	isImmortal = false;
}

GameManger::~GameManger()
{
}
