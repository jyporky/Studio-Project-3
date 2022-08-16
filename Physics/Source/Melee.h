#pragma once
#include "Weapon.h"

class Melee :public Weapon
{
public:
	float GetAttackAngle();
protected:
	float attack_angle;
};