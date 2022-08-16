#include "OverdriveSkill.h"
Overdrive::Overdrive() {

}
Overdrive::~Overdrive() {

}
std::string Overdrive::getDescription() {
	return "Attack increased by 30% for 10 second, movement speed increased by 20% for 15 seconds";
}
int Overdrive::getMoneyCost() {
	return 200;
}
int Overdrive::getEnergyCost() {
	return 80;
}
void Overdrive::UseSkill() {
	return;
}