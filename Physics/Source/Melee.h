#pragma once
#include "Weapon.h"

class Melee :public Weapon
{
public:
	virtual float GetAttackAngle();
protected:
	float attack_angle;
};