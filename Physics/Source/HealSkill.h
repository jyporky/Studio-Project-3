#pragma once
#include "Skill.h"
#include "Player.h"
#include <string>
class Heal : public Skill
{
public:
	Heal();
	~Heal();
	virtual std::string getDescription();
	virtual int getMoneyCost();
	virtual int getEnergyCost();
	virtual void UseSkill();
protected:
	Player* player;
};

