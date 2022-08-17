#include "Sword.h"

Sword::Sword()
{
	cost = 100;
	damage = 10;
	range = 5;
	attack_speed = 1.2;
	attack_angle = 75;
	attack_cast = 0.4;
	description = "SAMURAI TIME!";
	Animate = false;
}


Sword::~Sword()
{

}

bool Sword::Update(double dt, Vector3 mousepos, Vector3 movementdirection, GameObject* userGO)
{
	attackdt += dt;
	float offset = 0;
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

		if (userGO->angle == 0)
			direction = (mousepos - userGO->pos);
		else if (userGO->angle == 180)
			direction = -(mousepos - userGO->pos);

		gameobject->angle = Math::RadianToDegree(atan2f(direction.y, direction.x));

		if (direction.x < 0 && movementdirection == 0)
		{
			if (userGO->angle == 0)
			{
				userGO->angle = 180;
				offset = -(gameobject->scale.x * 0.2);
			}
			else if (userGO->angle == 180)
			{
				gameobject->angle = 0;
				offset = gameobject->scale.x * 0.2;
			}
		}
	}

	gameobject->pos = userGO->pos;
	gameobject->pos.x += offset;

	return false;
}

void Sword::attack()
{
	attackdt = 0;
	Animate = true;
}
