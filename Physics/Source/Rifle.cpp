#include "Rifle.h"
Rifle::Rifle() {
	piercing = false;
	bullet_speed = 30;
	cost = 80;
	damage = 3;
	range = 1000;
	attack_speed = 0.3;
	description = "RATATATATATATATA";
	WeaponType = RIFLE;
}
Rifle::~Rifle() {

}

bool Rifle::Update(double dt, Vector3 mousepos, Vector3 movementdirection, GameObject* userGO)
{
	//attackdt += dt;
	//Vector3 offset(0, 0, 0);

	//// offset the sword
	//if (userGO->angle == 0)
	//{
	//	gameobject->leftwep = false;
	//	offset = Vector3(gameobject->scale.x * 0.2, -gameobject->scale.y * 0.4);
	//}
	//else if (userGO->angle == 180)
	//{
	//	gameobject->leftwep = true;
	//	offset = Vector3(-gameobject->scale.x * 0.2, -gameobject->scale.y * 0.4);
	//}

	////do the weapon animation
	//if (Animate)
	//{
	//	float wp = attack_angle / (attack_cast / 2) * dt;
	//	if (attackdt < (attack_cast / 2))
	//	{
	//		if (userGO->angle == 0)
	//			gameobject->angle -= wp;
	//		else if (userGO->angle == 180)
	//			gameobject->angle += wp;
	//	}

	//	else if (attackdt < attack_cast)
	//	{
	//		if (userGO->angle == 0)
	//			gameobject->angle += wp;
	//		else if (userGO->angle == 180)
	//			gameobject->angle -= wp;
	//	}

	//	else if (attackdt > attack_cast)
	//	{
	//		Animate = false;
	//	}
	//}

	//else
	//{
	//	Vector3 direction;
	//	float angle_offset;
	//	if (userGO->angle == 0)
	//	{
	//		angle_offset = -10;
	//		direction = (mousepos - userGO->pos);
	//	}
	//	else if (userGO->angle == 180)
	//	{
	//		angle_offset = 10;
	//		direction = -(mousepos - userGO->pos);
	//	}

	//	gameobject->angle = Math::RadianToDegree(atan2f(direction.y, direction.x)) + angle_offset;

	//	if (direction.x < 0 && movementdirection == 0)
	//	{
	//		if (userGO->angle == 0)
	//		{
	//			gameobject->leftwep = true;
	//			userGO->angle = 180;
	//			offset = Vector3(gameobject->scale.x * 0.2, -gameobject->scale.y * 0.4);
	//		}
	//		else if (userGO->angle == 180)
	//		{
	//			gameobject->leftwep = false;
	//			userGO->angle = 0;
	//			offset = -Vector3(gameobject->scale.x * 0.2, -gameobject->scale.y * 0.4);
	//		}
	//	}
	//}

	//gameobject->pos = userGO->pos;
	//gameobject->pos += offset;


	return false;
}

bool Rifle::attack()
{
	//if (attackdt >= attack_speed)
	//{
	//	attackdt = 0;
	//	Animate = true;
	//	cSoundController->PlaySoundByID(2);
	//	return true;
	//}
	return false;
}