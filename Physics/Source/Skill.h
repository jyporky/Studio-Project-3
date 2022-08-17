#pragma once
#include <string>
#include "Player.h"
class Skill
{
public:
	virtual void UseSkill();
	virtual std::string getDescription();
	virtual int getMoneyCost();
	virtual int getEnergyCost();
	Skill();
	~Skill();

protected:
	Player* player;
};

