#pragma once
#include "Source/Skill.h"
#include <string>
class Hack : public Skill
{
public:
	Hack();
	~Hack();
	int getMoneyCost();
	int getEnegyCost();
	std::string getDescription();	
};

