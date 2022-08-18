#pragma once
#include <string>
#include "SelfUpgrades.h"
class RangedDmgUpgrade : public SelfUpgrades {
public:
	virtual void receiveUpgrade();
	virtual std::string getDescription();
	virtual int getMoneyCost();
	virtual int getUpgradeLevel();
	RangedDmgUpgrade();
	~RangedDmgUpgrade();
protected:
	int noOfTimesUpgraded;
};
