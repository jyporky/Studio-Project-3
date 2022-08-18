#include "SelfUpgrades.h"
SelfUpgrades::SelfUpgrades() {
	player = Player::GetInstance();
}
SelfUpgrades::~SelfUpgrades() {

}
void SelfUpgrades::receiveUpgrade() {
	return;
}
std::string SelfUpgrades::getDescription() {
	return "";
}
int SelfUpgrades::getMoneyCost() {
	return 0;
}
int SelfUpgrades::getUpgradeLevel()
{
	return noOfTimesUpgraded;
}