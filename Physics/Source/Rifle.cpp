#include "Rifle.h"
Rifle::Rifle() {
	piercing = false;
	bullet_speed = 60;
	cost = 80;
	damage = 3;
	range = 1000;
	attack_speed = 0.3;
	attack_cast = 0.1;
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

	// offset the sword
	if (userGO->angle == 0)
	{
		gameobject->leftwep = false;
		offset = Vector3(gameobject->scale.x * 0.2, -gameobject->scale.y * 1);
	}
	else if (userGO->angle == 180)
	{
		gameobject->leftwep = true;
		offset = Vector3(-gameobject->scale.x * 0.2, -gameobject->scale.y * 1);
	}

	//do the weapon animation
	if (Animate)
	{
		float wp = 20 / (attack_cast / 2) * dt;
		if (attackdt < (attack_cast / 2))
		{
			if (userGO->angle == 0)
				offset = Vector3(-gameobject->scale.x * 0.1, -gameobject->scale.y * 0.8);
			else if (userGO->angle == 180)
				offset = Vector3(-gameobject->scale.x * 0.1, -gameobject->scale.y * 0.8);
		}

		else if (attackdt < attack_cast)
		{
			if (userGO->angle == 0)
				gameobject->angle += wp;
			else if (userGO->angle == 180)
				gameobject->angle -= wp;
		}

		else if (attackdt > attack_cast)
		{
			Animate = false;
		}
	}
	else
	{
		Vector3 direction;
		float angle_offset;
		if (userGO->angle == 0)
		{
			angle_offset = -10;
			direction = (mousepos - userGO->pos);
		}
		else if (userGO->angle == 180)
		{
			angle_offset = 10;
			direction = -(mousepos - userGO->pos);
		}

		gameobject->angle = Math::RadianToDegree(atan2f(direction.y, direction.x)) + angle_offset;

		if (direction.x < 0 && movementdirection == 0)
		{
			if (userGO->angle == 0)
			{
				gameobject->leftwep = true;
				userGO->angle = 180;
				offset = Vector3(gameobject->scale.x * 0.2, -gameobject->scale.y * 0.9);
			}
			else if (userGO->angle == 180)
			{
				gameobject->leftwep = false;
				userGO->angle = 0;
				offset = -Vector3(gameobject->scale.x * 0.2, -gameobject->scale.y * 0.9);
			}
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
		cSoundController->PlaySoundByID(2);
		return true;
	}
	return false;
}
