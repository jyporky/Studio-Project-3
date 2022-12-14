#include "HealthUpgrade.h"
HealthUpgrade::HealthUpgrade() {
	noOfTimesUpgraded = 0;
}
HealthUpgrade::~HealthUpgrade() {

}
void HealthUpgrade::receiveUpgrade() {
	player->SetMaxHealth(player->GetMaxHealth() + 5);
	noOfTimesUpgraded++;
}
std::string HealthUpgrade::getDescription() {
	return "Increases max health by 5";
}
int HealthUpgrade::getMoneyCost() {
	return 50 + (25 * noOfTimesUpgraded);
}

int HealthUpgrade::getUpgradeLevel()
{
	return noOfTimesUpgraded;
}
void HealthUpgrade::resetNoOfTimesUpgraded()
{
	noOfTimesUpgraded = 0;
}
