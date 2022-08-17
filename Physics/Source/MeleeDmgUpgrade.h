#pragma once
#include "SelfUpgrades.h"
class MeleeDmgUpgrade : SelfUpgrades {
public:
	virtual void receiveUpgrade();
	virtual std::string getDescription();
	virtual int getMoneyCost();
	MeleeDmgUpgrade();
	~MeleeDmgUpgrade();
protected:
	int noOfTimesUpgraded;
};