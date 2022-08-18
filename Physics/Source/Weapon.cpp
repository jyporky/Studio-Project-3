#include "Weapon.h"

CSoundController* Weapon::cSoundController = CSoundController::GetInstance();

Weapon::Weapon()
{
	gameobject = nullptr;
	attack_cast = 0;
	attack_speed = 0;
	cost = 0;
	damage = 0;
	range = 5;
	attackdt = 0;
	IsMelee = false;
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

bool Weapon::attack()
{
	return false;
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

bool Weapon::GetPiercing()
{
	return false;
}

void Weapon::SetPiercing(bool pierce)
{
}

float Weapon::GetBulletSpeed()
{
	return 0.0f;
}

void Weapon::SetBulletSpeed(float bs)
{
}

GameObject* Weapon::GetGameObject()
{
	return gameobject;
}

std::string Weapon::getDescription() {
	return description;
}