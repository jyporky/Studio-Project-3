#include "OverdriveSkill.h"
Overdrive::Overdrive() {

}
Overdrive::~Overdrive() {

}
std::string Overdrive::getDescription() {
	return "Attack increased by 30% for 10s, speed increased by 20% for 15s";
}
int Overdrive::getMoneyCost() {
	return 200;
}
int Overdrive::getEnergyCost() {
	return 80;
}
void Overdrive::UseSkill() {
	/*player->getAttack * 1.3;
	player->getMovementSpeed * 1.2;*/
	//player->changeMovementSpeed(15);
}