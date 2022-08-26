#include "ImmortalSkill.h"
Immortal::Immortal() {
	immortalState = false;
}
Immortal::~Immortal() {

}
std::string Immortal::getDescription() {
	return "Heal instead of taking damage for 2s";
}
int Immortal::getMoneyCost() {
	return 600;
}
int Immortal::getEnergyCost() {
	return 200;
}
void Immortal::UseSkill() {
	immortalState = true;
}
void Immortal::resetImmortality() {
	immortalState = false;
}
bool Immortal::getState() {
	return immortalState;
}