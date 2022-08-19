#include "Ranged.h"

Ranged::Ranged()
{
	IsMelee = false;
}

Ranged::~Ranged()
{
}

bool Ranged::GetPiercing()
{
	return piercing;
}

void Ranged::SetPiercing(bool pierce)
{
	piercing = pierce;
}

float Ranged::GetBulletSpeed()
{
	return bullet_speed;
}

void Ranged::SetBulletSpeed(float bs)
{
	bullet_speed = bs;
}

bool Ranged::Update(double dt, Vector3 mousepos, Vector3 movementdirection, GameObject* userGO)
{
	return false;
}

bool Ranged::attack()
{
	return false;
}
