#pragma once
#include "Skill.h"
#include <string>
class Heal : public Skill
{
	Heal();
	~Heal();
	std::string getDescription();
	int getMoneyCost();
	int getEnergyCost();
};
