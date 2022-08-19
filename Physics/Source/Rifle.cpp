#include "Rifle.h"
Rifle::Rifle() {
	piercing = false;
	bullet_speed = 60;
	cost = 80;
	damage = 3;
	range = 1000;
	attack_speed = 0.3;
	attack_cast = attack_speed * 0.5;
	attackdt = attack_speed * 2;
	IsMelee = false;
	description = "RATATATATATATATA";
	WeaponType = RIFLE;
}


Rifle::~Rifle() {

}

bool Rifle::Update(double dt, Vector3 mousepos, Vector3 movementdirection, GameObject* userGO)
{
	attackdt += dt;
	Vector3 offset(0, 0, 0);

	Vector3 direction;
	if (userGO->angle == 0)
	{
		direction = (mousepos - userGO->pos);
	}
	else if (userGO->angle == 180)
	{
		direction = -(mousepos - userGO->pos);
	}

	gameobject->angle = Math::RadianToDegree(atan2f(direction.y, direction.x));

	if (direction.x < 0)
	{
		if (userGO->angle == 0)
		{
			gameobject->leftwep = true;
			userGO->angle = 180;
			gameobject->angle += 180;
			offset = Vector3(gameobject->scale.x * 0.2, -gameobject->scale.y * 0.9);
		}
		else if (userGO->angle == 180)
		{
			gameobject->leftwep = false;
			userGO->angle = 0;
			gameobject->angle -= 180;
			offset = -Vector3(gameobject->scale.x * 0.2, -gameobject->scale.y * 0.9);
		}
	}

	// offset the sword
	if (userGO->angle == 0)
	{
		gameobject->leftwep = false;
		offset = Vector3(0, -gameobject->scale.y * 1);
	}
	else if (userGO->angle == 180)
	{
		gameobject->leftwep = true;
		offset = Vector3(0, -gameobject->scale.y * 1);
	}

	//do the weapon animation
	if (Animate)
	{
		float wp = 10 * dt;
		if (attackdt < attack_cast)
		{
			if (userGO->angle == 0)
				offset = Vector3(-gameobject->scale.x * 0.1, -gameobject->scale.y * 0.8);
			else if (userGO->angle == 180)
				offset = Vector3(-gameobject->scale.x * 0.1, -gameobject->scale.y * 0.8);
		}

		else if (attackdt > attack_cast)
		{
			Animate = false;
		}
	}

	gameobject->pos = userGO->pos;
	gameobject->pos += offset;


	attackdt += dt;
	return false;
}

bool Rifle::attack()
{
	if (attackdt >= attack_speed)
	{
		attackdt = 0;
		Animate = true;
		cSoundController->PlaySoundByID(5);
		return true;
	}
	return false;
}
