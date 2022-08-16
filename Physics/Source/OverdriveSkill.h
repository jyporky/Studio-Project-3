#pragma once
#include "Skill.h"
#include <string>
class Overdrive : public Skill
{
public:
	Overdrive();
	~Overdrive();
	virtual std::string getDescription();
	virtual int getMoneyCost();
	virtual int getEnergyCost();
	virtual void UseSkill();
};

