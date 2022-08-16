#pragma once
#include "Potions.h"
#include <string>
class HealthPot : public Potions {
public:
	HealthPot();
	~HealthPot();
	virtual std::string getDescription();
	virtual int getMoneyCost();
	virtual void usePotion();
};
