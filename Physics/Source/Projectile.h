#pragma once
#include "Entity.h"
class Projectile : public Entity
{
public:
	Projectile();
	bool Update();
	~Projectile();
private:
	bool disappearWhenHit;
	unsigned damage;
	float distanceTravelled;
	float range;
};

