#pragma once
#include "Skill.h"
#include <string>
class Hack : public Skill
{
public:
	Hack();
	~Hack();
	virtual int getMoneyCost();
	virtual int getEnergyCost();
	virtual std::string getDescription();	
	virtual void UseSkill();
	virtual void resetHack();
	virtual bool getHackingState();
protected:
	bool hackState;
};

