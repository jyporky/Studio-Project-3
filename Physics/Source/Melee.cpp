#include "Melee.h"

Melee::Melee()
{
	attack_angle = 0;
}

Melee::~Melee()
{
}

float Melee::GetAttackAngle()
{
	return attack_angle;
}

bool Melee::Update(double dt, Vector3 mousepos, Vector3 movementdirection, GameObject* userGO)
{
	return false;
}

void Melee::attack()
{
}
