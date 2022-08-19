#pragma once
#include "Ranged.h"
class Crossbow : public Ranged {
public:
	Crossbow();
	~Crossbow(); 
	virtual bool Update(double dt, Vector3 mousepos, Vector3 movementdirection, GameObject* userGO);
	virtual bool attack();
private:
	float critChance;
	float critDamage;
};