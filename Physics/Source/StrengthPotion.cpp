#include "StrengthPotion.h"
StrengthPot::StrengthPot() {

}
StrengthPot::~StrengthPot() {

}
std::string StrengthPot::getDescription() {
	return "Increase all damage by 10";
}
int StrengthPot::getMoneyCost() {
	return 40;
}
int StrengthPot::getDuration() {
	return 30;
}
void StrengthPot::usePotion() {
	player->meleeDmgBoost += 10;
	player->rangeDmgBoost += 10;
}
void StrengthPot::potionTimeUp()
{
	player->meleeDmgBoost -= 10;
	player->rangeDmgBoost -= 10;
}