#include "SpeedPotion.h"
SpeedPot::SpeedPot() {

}
SpeedPot::~SpeedPot() {

}
std::string SpeedPot::getDescription() {
	return "Increase movement speed by 5";
}
int SpeedPot::getMoneyCost() {
	return 30;
}
int SpeedPot::getDuration() {
	return 30;
}
void SpeedPot::usePotion() {
	player->changeMovementSpeed(5);
}