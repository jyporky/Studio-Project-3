#pragma once
#include "Skill.h"
#include <string>
class Immortal : public Skill
{
public:
	Immortal();
	~Immortal();
	virtual std::string getDescription();
	virtual int getMoneyCost();
	virtual int getEnergyCost();
	virtual void UseSkill();
	virtual void resetImmortality();
	virtual bool getState();
protected:
	bool immortalState;
};

