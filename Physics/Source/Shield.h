#pragma once
#include "Melee.h"
class Shield : public Melee
{
public:
	Shield();
	~Shield();
	virtual bool Update(double dt, Vector3 mousepos, Vector3 movementdirection, GameObject* userGO);
	virtual bool attack();
private:
	float shieldoffset;
};

