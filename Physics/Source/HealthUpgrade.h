#pragma once
#include <string>
#include "SelfUpgrades.h"
class HealthUpgrade : public SelfUpgrades {
public:
	virtual void receiveUpgrade();
	virtual int getUpgradeLevel();
	virtual std::string getDescription();
	virtual int getMoneyCost();
	virtual void resetNoOfTimesUpgraded();
	HealthUpgrade();
	~HealthUpgrade();
protected:
	int noOfTimesUpgraded;
};
