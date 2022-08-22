#include "Shield.h"

Shield::Shield()
{
	cost = 0;
	damage = 3;
	range = 6;
	attack_speed = 1.2;
	attack_angle = 75;
	attack_cast = 0.4;
	attackdt = attack_speed * 2;
	description = "Shield";
	Animate = false;
	WeaponType = SHIELD;
	shieldoffset = 1;
}

Shield::~Shield()
{
}

bool Shield::Update(double dt, Vector3 mousepos, Vector3 movementdirection, GameObject* userGO)
{
	attackdt += dt;
	
	//move the shield around the user
	gameobject->pos = userGO->pos;
	mousepos = mousepos.Normalized() * range * shieldoffset;
	gameobject->pos += mousepos;
	if (Animate)
	{
		static float forwards = 0;
		if (forwards < attack_cast * 0.5f)
		{
			forwards += dt;
			shieldoffset += dt * 5;
		}
		else if (forwards < attack_cast)
		{
			forwards += dt;
			shieldoffset -= dt * 5;
		}
		else
		{
			forwards = 0;
			Animate = false;
		}
	}
	return false;
}

bool Shield::attack()
{
	if (attackdt >= attack_speed)
	{
		attackdt = 0;
		Animate = true;
		return true;
	}
	return false;
}
