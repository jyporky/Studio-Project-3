#pragma once
#include "Potions.h"
#include <string>
class StrengthPot : public Potions {
public:
	StrengthPot();
	~StrengthPot();
	virtual std::string getDescription();
	virtual int getMoneyCost();
	virtual int getDuration();
	virtual void usePotion();
	virtual void potionTimeUp();
};