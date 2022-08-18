#pragma once
#include <string>
#include "SelfUpgrades.h"
class MovementSpeedUpgrade : public SelfUpgrades {
public:
	virtual void receiveUpgrade();
	virtual int getUpgradeLevel();
	virtual std::string getDescription();
	virtual int getMoneyCost();
	MovementSpeedUpgrade();
	~MovementSpeedUpgrade();
protected:
	int noOfTimesUpgraded;
};
