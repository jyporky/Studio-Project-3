#include "Weapon.h"

Weapon::Weapon()
{
	gameobject = nullptr;
	attack_cast = 0;
	attack_speed = 0;
	cost = 0;
	damage = 0;
	range = 5;
	attackdt = 0;
}

Weapon::~Weapon()
{
	gameobject = nullptr;
}

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

bool Weapon::Update(double dt, Vector3 mousepos, Vector3 movementdirection, GameObject* userGO)
{
	return false;
}

void Weapon::attack()
{
}

float Weapon::GetAttackCast()
{
	return attack_cast;
}

void Weapon::SetAttackCast(float cd)
{
	attack_cast = cd;
}

void Weapon::SetGameObject(GameObject* newGameObject)
{
	gameobject = newGameObject;
}

GameObject* Weapon::GetGameObject()
{
	return gameobject;
}

std::string Weapon::getDescription() {
	return description;
}