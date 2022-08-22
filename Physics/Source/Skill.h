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
	virtual void resetImmortality();
	virtual bool getState();
protected:
	Player* player;
	bool immortalState;
private:
	
};

