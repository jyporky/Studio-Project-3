#include "Skill.h"

void Skill::UseSkill()
{
	return;
}

Skill::Skill()
{
	immortalState = false;
	player = Player::GetInstance();
}

Skill::~Skill()
{
}

std::string Skill::getDescription() {
	return "";
}

int Skill::getEnergyCost() {
	return 0;
}

int Skill::getMoneyCost() {
	return 0;
}
void Skill::resetImmortality() {
	return;
}
bool Skill::getState() {
	return immortalState;
}