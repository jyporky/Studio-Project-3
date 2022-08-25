#include "BoxingGloves.h"

BoxingGloves::BoxingGloves()
{
	cost = 350;
	damage = 5;
	range = 18;
	attack_speed = 0.3;
	attack_cast = 0.1;
	attack_angle = 30;
	attackdt = attack_speed * 2;
	description = "Knocks back enemies";
	Animate = false;
	WeaponType = BOXING_GLOVES;
}
BoxingGloves::~BoxingGloves()
{

}

bool BoxingGloves::Update(double dt, Vector3 mousepos, Vector3 movementdirection, GameObject* userGO)
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
		if (userGO->angle == 0)
		{
			gameobject->leftwep = false;
			offset += Vector3(gameobject->scale.x * 0.2, 0);
		}
		else if (userGO->angle == 180)
		{
			gameobject->leftwep = true;
			offset += Vector3(-gameobject->scale.x * 0.2, 0);
		}

		if (!gameobject->leftwep)
		{
			if (gameobject->angle > 0 && gameobject->angle <= 90)
				offset += Vector3(0, gameobject->scale.y * 0.2);

			else
				offset += Vector3(0, -gameobject->scale.y * 0.2);
		}

		else
		{
			if (gameobject->angle > 0 && gameobject->angle <= 90)
				offset += Vector3(0, -gameobject->scale.y * 0.2);

			else
				offset += Vector3(0, gameobject->scale.y * 0.2);
		}

		if (attackdt > attack_speed)
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
				gameobject->leftwep = false;
				userGO->angle = 180;
				offset = Vector3(gameobject->scale.x * 0.2, -gameobject->scale.y * 0.4);
			}
			else if (userGO->angle == 180)
			{
				gameobject->leftwep = true;
				userGO->angle = 0;
				offset = -Vector3(gameobject->scale.x * 0.2, -gameobject->scale.y * 0.4);
			}
		}
	}

	gameobject->pos = userGO->pos;
	gameobject->pos += offset;


	return false;
}

bool BoxingGloves::attack()
{
	if (attackdt >= attack_speed)
	{
		attackdt = 0;
		Animate = true;
		cSoundController->StopPlayByID(2);
		cSoundController->PlaySoundByID(2);
		return true;
	}
	return false;
}