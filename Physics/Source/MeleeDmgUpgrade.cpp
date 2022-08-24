#include "MeleeDmgUpgrade.h"
MeleeDmgUpgrade::MeleeDmgUpgrade() {
	noOfTimesUpgraded = 0;
}
MeleeDmgUpgrade::~MeleeDmgUpgrade() {

}
void MeleeDmgUpgrade::receiveUpgrade() {
	noOfTimesUpgraded++;
}
std::string MeleeDmgUpgrade::getDescription() {
	return "Increase melee weapon damage by 2";
}
int MeleeDmgUpgrade::getMoneyCost() {
	return 75 + (25 * noOfTimesUpgraded);
}
int MeleeDmgUpgrade::getUpgradeLevel()
{
	return noOfTimesUpgraded;
}
void MeleeDmgUpgrade::resetNoOfTimesUpgraded()
{
	noOfTimesUpgraded = 0;
}
