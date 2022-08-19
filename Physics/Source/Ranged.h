#pragma once
#include "Weapon.h"
#include "Player.h"
class Ranged : public Weapon
{
public:
	Ranged();
	~Ranged();
	virtual bool GetPiercing();
	virtual void SetPiercing(bool pierce);
	virtual float GetBulletSpeed();
	virtual void SetBulletSpeed(float bs);
	virtual bool Update(double dt, Vector3 mousepos, Vector3 movementdirection, GameObject* userGO);
	virtual bool attack();
protected:
	bool piercing;
	float bullet_speed;
	Player* player;
};