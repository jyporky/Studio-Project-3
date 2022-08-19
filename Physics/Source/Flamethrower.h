#pragma once
#include "Ranged.h"
class Flamethrower : public Ranged {
public:
	Flamethrower();
	~Flamethrower();
	virtual bool Update(double dt, Vector3 mousepos, Vector3 movementdirection, GameObject* userGO);
	virtual bool attack();
};
