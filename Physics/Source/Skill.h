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
	virtual void resetStun();
	virtual bool getStunState();
	virtual void resetBlackhole();
	virtual bool getBlackholeState();
	virtual void resetHack();
	virtual bool getHackingState();
protected:
	Player* player;

};

