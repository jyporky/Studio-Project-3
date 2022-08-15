#pragma once
#include "Skill.h"
#include <string>
class Overdrive : public Skill
{
public:
	Overdrive();
	~Overdrive();
	std::string getDescription();
	int getMoneyCost();
	int getEnergyCost();
};

