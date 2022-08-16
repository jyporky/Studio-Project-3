#pragma once
#include "Skill.h"
#include <string>
class Hack : public Skill
{
public:
	Hack();
	~Hack();
	virtual int getMoneyCost();
	virtual int getEnegyCost();
	virtual std::string getDescription();	
	virtual void UseSkill();
};

