#pragma once
#include <string>
class Potions {
public:
	Potions();
	~Potions();
	virtual void usePotion();
	virtual std::string getDescription();
	virtual int getMoneyCost();
	virtual int getDuration();
};
