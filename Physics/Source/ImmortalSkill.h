#pragma once
#include "Skill.h"
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

