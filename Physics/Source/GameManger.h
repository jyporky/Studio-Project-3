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

	//weapon upgrades
	bool pierceBought;
	bool fastfireBought;
	bool fastbulletBought;
	bool explosiveBought;
	bool betterfuelBought;
	bool accuratearrowsBought;
	bool fastmeleeBought;
};

