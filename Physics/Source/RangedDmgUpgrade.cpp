#include "RangedDmgUpgrade.h"
RangedDmgUpgrade::RangedDmgUpgrade() {
	noOfTimesUpgraded = 1;
}
RangedDmgUpgrade::~RangedDmgUpgrade() {

}
void RangedDmgUpgrade::receiveUpgrade() {
	//player->rangeddamage + 5;
	noOfTimesUpgraded++;
}
std::string RangedDmgUpgrade::getDescription() {
	return "Increase ranged weapon damage by 1";
}
int RangedDmgUpgrade::getMoneyCost() {
	return 75 +(25 * noOfTimesUpgraded);
}