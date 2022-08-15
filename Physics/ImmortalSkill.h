#pragma once
#include "Source/Skill.h"
#include <string>
class Immortal : public Skill
{
public:
	Immortal();
	~Immortal();
	std::string getDescription();
	int getMoneyCost();
	int getEnergyCost();
};

