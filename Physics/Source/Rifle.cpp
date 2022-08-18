#include "Rifle.h"
Rifle::Rifle() {
	piercing = false;
	bullet_speed = 30;
	cost = 80;
	damage = 3;
	range = 1000;
	attack_speed = 0.3;
	IsMelee = false;
	description = "RATATATATATATATA";
	WeaponType = RIFLE;
}


Rifle::~Rifle() {

}

bool Rifle::Update(double dt, Vector3 mousepos, Vector3 movementdirection, GameObject* userGO)
{
	attackdt += dt;
	gameobject->pos = userGO->pos;
	return false;
}

bool Rifle::attack()
{
	if (attackdt > attack_speed)
	{
		attackdt = 0;
		return true;
	}
	return false;
}
