#pragma once
#include "Weapon.h"
class Ranged : public Weapon
{
public:
	bool GetPiercing();
	void SetPiercing(bool pierce);
	float GetBulletSpeed();
	void SetBulletSpeed(float bs);

private:
	bool piercing;
	float bullet_speed;
};