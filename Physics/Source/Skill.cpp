#include "Skill.h"

void Skill::UseSkill()
{
	return;
}

Skill::Skill()
{
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