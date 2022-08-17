#pragma once
#include <string>
#include "Player.h"
class SelfUpgrades {
public:
	virtual void receiveUpgrade();
	virtual std::string getDescription();
	virtual int getMoneyCost();
	SelfUpgrades();
	~SelfUpgrades();
protected:
	Player* player;
};
