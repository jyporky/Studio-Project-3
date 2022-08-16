#include "HealthPotion.h"
HealthPot::HealthPot() {

}
HealthPot::~HealthPot() {

}
std::string HealthPot::getDescription() {
	return "Heals 30 health";
}
int HealthPot::getMoneyCost() {
	return 30;
}
void HealthPot::usePotion() {
	return;
}