#pragma once
#include <string>
#include "Player.h"
class SelfUpgrades {
public:
	virtual void receiveUpgrade();
	virtual int getUpgradeLevel();
	virtual std::string getDescription();
	virtual int getMoneyCost();
	SelfUpgrades();
	~SelfUpgrades();
protected:
	int noOfTimesUpgraded;
	Player* player;
};
