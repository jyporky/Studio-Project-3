#include "HealSkill.h"
Heal::Heal() {
	player = Player::GetInstance();
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
void Heal::UseSkill() {
	player->ChangeHealth(50);
}
