#include "Ranged.h"

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