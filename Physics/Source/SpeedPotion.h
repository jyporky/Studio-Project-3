#pragma once
#include "Potions.h"
#include <string>
class SpeedPot : public Potions {
public:
	SpeedPot();
	~SpeedPot();
	virtual std::string getDescription();
	virtual int getMoneyCost();
	virtual int getDuration();
	virtual void usePotion();
};
