#pragma once
#include <string>
#include "Player.h"
class Potions {
public:
	Potions();
	~Potions();
	virtual void usePotion();
	virtual std::string getDescription();
	virtual int getMoneyCost();
	virtual int getDuration();
	virtual void potionTimeUp();
protected:
	Player* player;
};
