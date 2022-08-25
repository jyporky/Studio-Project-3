#include "MovementSpeedUpgrade.h"
MovementSpeedUpgrade::MovementSpeedUpgrade() {
	noOfTimesUpgraded = 0;
}
MovementSpeedUpgrade::~MovementSpeedUpgrade() {

}
void MovementSpeedUpgrade::receiveUpgrade() {
	noOfTimesUpgraded++;
}
std::string MovementSpeedUpgrade::getDescription() {
	return "Increases movement speed by 5";
}
int MovementSpeedUpgrade::getMoneyCost() {
	return 30 + (20 * noOfTimesUpgraded);
}
int MovementSpeedUpgrade::getUpgradeLevel()
{
	return noOfTimesUpgraded;
}
void MovementSpeedUpgrade::resetNoOfTimesUpgraded()
{
	noOfTimesUpgraded = 0;
}
