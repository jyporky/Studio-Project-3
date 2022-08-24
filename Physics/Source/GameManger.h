#pragma once
#include "SingletonTemplate.h"
#include "SpriteAnimation.h"
#include "Weapon.h"

class GameManger : public CSingletonTemplate<GameManger>
{
public:
	GameManger();
	~GameManger();
	bool bPlayerLost;
	bool bDebug;
	unsigned dWaveNo;
	bool waveClear;
	bool outShop;
	bool eButtonState;
	bool switchWeapon;
	int weptype;
	int sideweptype;
	bool buyFirstWep;

	bool reset;

	//self upgrade
	bool speedUpgradeBought;
	bool healthUpgradeBought;
	bool meleeUpgradeBought;
	bool rangeUpgradeBought;

	//skills
	bool empBought;
	bool hackBought;
	bool healBought;
	bool immortalBought;
	bool overdriveBought;

	//weapons
	bool boxingGloveBought;

	bool rifleBought;
	bool flamethrowerBought;
	bool crossbowBought;

	//weapon upgrades
	bool pierceBought;
	bool fastfireBought;
	bool fastbulletBought;
	bool explosiveBought;
	bool betterfuelBought;
	bool accuratearrowsBought;
	bool fastmeleeBought;
};

