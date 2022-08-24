#pragma once
#include <string>
#include "SelfUpgrades.h"
class MovementSpeedUpgrade : public SelfUpgrades {
public:
	virtual void receiveUpgrade();
	virtual int getUpgradeLevel();
	virtual std::string getDescription();
	virtual int getMoneyCost();
	virtual void resetNoOfTimesUpgraded();
	MovementSpeedUpgrade();
	~MovementSpeedUpgrade();
protected:
	int noOfTimesUpgraded;
};
