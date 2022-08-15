#pragma once
#include "Skill.h"
#include <string>
class EMP : public Skill
{
public:
	EMP();
	~EMP();
	std::string getDescription();
	int getMoneyCost();
	int getEnergyCost();
};

