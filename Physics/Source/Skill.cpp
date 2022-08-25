#include "Skill.h"

void Skill::UseSkill()
{
	return;
}

Skill::Skill()
{
	//player = Player::GetInstance();
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
	return false;
}
void Skill::resetStun() {
	return;
}
bool Skill::getStunState() {
	return false;
}
void Skill::resetBlackhole() {
	return;
}
bool Skill::getBlackholeState() {
	return false;
}
void Skill::resetHack() {
	return;
}
bool Skill::getHackingState() {
	return false;
}
