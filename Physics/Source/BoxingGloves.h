#pragma once
#include "Melee.h"
class BoxingGloves : public Melee {
public:
	BoxingGloves();
	~BoxingGloves();
	virtual bool Update(double dt, Vector3 mousepos, Vector3 movementdirection, GameObject* userGO);
	virtual bool attack();
};
