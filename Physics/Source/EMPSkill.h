#pragma once
#include "Skill.h"
#include <string>
class EMP : public Skill
{
public:
	EMP();
	~EMP();
	virtual std::string getDescription();
	virtual int getMoneyCost();
	virtual int getEnergyCost();
	virtual void UseSkill();
};

