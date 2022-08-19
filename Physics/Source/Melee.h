#pragma once
#include "Weapon.h"
#include "Player.h"
class Melee :public Weapon
{
public:
	Melee();
	~Melee();
	virtual float GetAttackAngle();
	virtual bool Update(double dt, Vector3 mousepos, Vector3 movementdirection, GameObject* userGO);
	virtual bool attack();
protected:
	float attack_angle;
	Player* player;
};