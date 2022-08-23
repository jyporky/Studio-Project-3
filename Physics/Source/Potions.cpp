#include "Potions.h"
Potions::Potions() {
	player = Player::GetInstance();
}
Potions::~Potions() {

}
void Potions::usePotion() {
	return;
}
std::string Potions::getDescription() {
	return "";
}
int Potions::getDuration() {
	return 0;
}
int Potions::getMoneyCost() {
	return 0;
}
void Potions::potionTimeUp() {
	return;		
}