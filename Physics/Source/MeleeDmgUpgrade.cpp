#include "MeleeDmgUpgrade.h"
MeleeDmgUpgrade::MeleeDmgUpgrade() {
	noOfTimesUpgraded = 1;
}
MeleeDmgUpgrade::~MeleeDmgUpgrade() {

}
void MeleeDmgUpgrade::receiveUpgrade() {
	//player->meleedamage + 5
	noOfTimesUpgraded++;
}
std::string MeleeDmgUpgrade::getDescription() {
	return "Increase melee weapon damage by 1";
}
int MeleeDmgUpgrade::getMoneyCost() {
	return 75 + (25 * noOfTimesUpgraded);
}