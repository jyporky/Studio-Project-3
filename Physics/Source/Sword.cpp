#include "Sword.h"

Sword::Sword()
{
	cost = 100;
	damage = 10;
	range = 10;
	attack_speed = 1.2;
	attack_angle = 75;
	attack_cast = 0.4;
	description = "SAMURAI TIME!";
	Animate = false;
	WeaponType = SWORD;
}


Sword::~Sword()
{

}

bool Sword::Update(double dt, Vector3 mousepos, Vector3 movementdirection, GameObject* userGO)
{
	attackdt += dt;
	Vector3 offset(0, 0, 0);

	// offset the sword
	if (userGO->angle == 0)
	{
		gameobject->leftwep = false;
		offset = Vector3(gameobject->scale.x * 0.2, -gameobject->scale.y * 0.4);
	}
	else if (userGO->angle == 180)
	{
		gameobject->leftwep = true;
		offset = Vector3(-gameobject->scale.x * 0.2, -gameobject->scale.y * 0.4);
	}

	//do the weapon animation
	if (Animate)
	{
		float wp = attack_angle / (attack_cast / 2) * dt;
		if (attackdt < (attack_cast / 2))
		{
			if (userGO->angle == 0)
				gameobject->angle -= wp;
			else if (userGO->angle == 180)
				gameobject->angle += wp;
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
			angle_offset = -40;
			direction = (mousepos - userGO->pos);
		}
		else if (userGO->angle == 180)
		{
			angle_offset = 40;
			direction = -(mousepos - userGO->pos);
		}

		gameobject->angle = Math::RadianToDegree(atan2f(direction.y, direction.x)) + angle_offset;

		if (direction.x < 0 && movementdirection == 0)
		{
			if (userGO->angle == 0)
			{
				gameobject->leftwep = true;
				userGO->angle = 180;
				offset = Vector3(gameobject->scale.x * 0.2, -gameobject->scale.y * 0.4);
			}
			else if (userGO->angle == 180)
			{
				gameobject->leftwep = false;
				userGO->angle = 0;
				offset = -Vector3(gameobject->scale.x * 0.2, -gameobject->scale.y * 0.4);
			}
		}
	}

	gameobject->pos = userGO->pos;
	gameobject->pos += offset;


	return false;
}

bool Sword::attack()
{
	if (attackdt >= attack_speed)
	{
		attackdt = 0;
		Animate = true;
		return true;
	}
	return false;
}
