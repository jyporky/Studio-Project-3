#pragma once
#include <string>
#include "SelfUpgrades.h"
class MovementSpeedUpgrade : public SelfUpgrades {
	virtual void receiveUpgrade();
	virtual std::string getDescription();
	virtual int getMoneyCost();
	MovementSpeedUpgrade();
	~MovementSpeedUpgrade();
protected:
	int noOfTimesUpgraded;
};
