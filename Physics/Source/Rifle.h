#pragma once
#include "Ranged.h"
class Rifle : public Ranged {
public:
	Rifle();
	~Rifle();
	virtual bool Update(double dt, Vector3 mousepos, Vector3 movementdirection, GameObject* userGO);
	virtual bool attack();
};