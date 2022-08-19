#include "RangedDmgUpgrade.h"
RangedDmgUpgrade::RangedDmgUpgrade() {
	noOfTimesUpgraded = 0;
}
RangedDmgUpgrade::~RangedDmgUpgrade() {

}
void RangedDmgUpgrade::receiveUpgrade() {
	noOfTimesUpgraded++;
}
std::string RangedDmgUpgrade::getDescription() {
	return "Increase ranged weapon damage by 2";
}
int RangedDmgUpgrade::getMoneyCost() {
	return 75 +(25 * noOfTimesUpgraded);
}

int RangedDmgUpgrade::getUpgradeLevel()
{
	return noOfTimesUpgraded;
}