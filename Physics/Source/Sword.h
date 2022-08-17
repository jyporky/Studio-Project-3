#pragma once
#include "Melee.h"
class Sword : public Melee
{
public:
	Sword();
	~Sword();
	virtual bool Update(double dt, Vector3 mousepos, Vector3 movementdirection, GameObject* userGO);
	virtual void attack();
private:
};