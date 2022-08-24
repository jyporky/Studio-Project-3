#include "DoppelgangerSkill.h"
Doppelganger::Doppelganger() {
	player = Player::GetInstance();
}
Doppelganger::~Doppelganger() {

}
std::string Doppelganger::getDescription() {
	return "Summons a clone of yourself that attacks enemies for the rest of the wave";
}
int Doppelganger::getMoneyCost() {
	return 450;
}
int Doppelganger::getEnergyCost() {
	return 120;
}
void Doppelganger::UseSkill() {
	
}
