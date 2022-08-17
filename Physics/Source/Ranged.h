#pragma once
#include "Weapon.h"
class Ranged : public Weapon
{
public:
	virtual bool GetPiercing();
	virtual void SetPiercing(bool pierce);
	virtual float GetBulletSpeed();
	virtual void SetBulletSpeed(float bs);

protected:
	bool piercing;
	float bullet_speed;
};