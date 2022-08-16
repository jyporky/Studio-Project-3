#pragma once
#include <string>
class Skill
{
public:
	virtual void UseSkill();
	virtual std::string getDescription();
	virtual int getMoneyCost();
	virtual int getEnergyCost();
	Skill();
	~Skill();

private:
	
};

