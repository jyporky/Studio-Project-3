#include "Weapon.h"

int Weapon::GetCost()
{
	return cost;
}

void Weapon::SetCost(int Cost)
{
	cost = Cost;
}

float Weapon::GetDamage()
{
	return damage;
}

void Weapon::SetDamage(float dmg)
{
	damage = dmg;
}

float Weapon::GetRange()
{
	return range;
}

float Weapon::GetAttackSpeed()
{
	return attack_speed;
}

void Weapon::SetAttackSpeed(float as)
{
	attack_speed = as;
}

float Weapon::GetAttackAngle()
{
	return 0;
}

float Weapon::GetAttackCast()
{
	return attack_cast;
}

void Weapon::SetAttackCast(float cd)
{
	attack_cast = cd;
}