#pragma once
#include "Skill.h"
#include <string>
class Blackhole : public Skill
{
public:
	Blackhole();
	~Blackhole();
	virtual std::string getDescription();
	virtual int getMoneyCost();
	virtual int getEnergyCost();
	virtual void UseSkill();
	void resetBlackhole();
	bool getBlackholeState();
private:
	bool spawnBlackhole;
};

