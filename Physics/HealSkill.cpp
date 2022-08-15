#include "HealSkill.h"
Heal::Heal() {

}
Heal::~Heal() {

}
std::string Heal::getDescription() {
	return "Heal 50 health";
}
int Heal::getMoneyCost() {
	return 400;
}
int Heal::getEnergyCost() {
	return 150;
}
