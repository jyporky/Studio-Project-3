#include "Entity.h"

Entity::Entity()
{
	gameobject = nullptr;
	CurrWeapon = nullptr;
}

Entity::~Entity()
{
}

bool Entity::Update(double dt)
{
	return false;
}

void Entity::Init()
{
}

void Entity::SetGameObject(GameObject* GO)
{
	gameobject = GO;
}

GameObject* Entity::GetGameObject()
{
	return gameobject;
}

bool Entity::ChangeHealth(int changeInHealth)
{
	return CurrWeapon;
}

Weapon* Entity::GetWeapon()
{
	return nullptr;
}

bool Entity::IsSpawningBullet()
{
	return false;
}

void Entity::SetWeapon(Weapon* weapon)
{
	CurrWeapon = weapon;
}
