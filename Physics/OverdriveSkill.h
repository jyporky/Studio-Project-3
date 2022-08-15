#pragma once
#include "Source/Skill.h"
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

