#include "MovementSpeedUpgrade.h"
MovementSpeedUpgrade::MovementSpeedUpgrade() {
	noOfTimesUpgraded = 1;
}
MovementSpeedUpgrade::~MovementSpeedUpgrade() {

}
void MovementSpeedUpgrade::receiveUpgrade() {
	//player->getmovementspeed + 5;
	noOfTimesUpgraded++;
}
std::string MovementSpeedUpgrade::getDescription() {
	return "Increases movement speed by 1";
}
int MovementSpeedUpgrade::getMoneyCost() {
	return 30 + (20 * noOfTimesUpgraded);
}