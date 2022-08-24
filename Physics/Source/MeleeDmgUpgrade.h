#pragma once
#include "SelfUpgrades.h"
class MeleeDmgUpgrade : public SelfUpgrades {
public:
	virtual void receiveUpgrade();
	virtual int getUpgradeLevel();
	virtual std::string getDescription();
	virtual int getMoneyCost();
	virtual void resetNoOfTimesUpgraded();
	MeleeDmgUpgrade();
	~MeleeDmgUpgrade();
protected:
	int noOfTimesUpgraded;
};