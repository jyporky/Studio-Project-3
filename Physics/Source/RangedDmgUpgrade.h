#pragma once
#include <string>
#include "SelfUpgrades.h"
class RangedDmgUpgrade : public SelfUpgrades {
public:
	virtual void receiveUpgrade();
	virtual std::string getDescription();
	virtual int getMoneyCost();
	RangedDmgUpgrade();
	~RangedDmgUpgrade();
protected:
	int noOfTimesUpgraded;
};
