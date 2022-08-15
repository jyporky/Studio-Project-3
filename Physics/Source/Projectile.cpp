#include "Projectile.h"

Projectile::Projectile()
{
	disappearWhenHit = false; 
	damage = 0;
	distanceTravelled = 0;
	range = 0;
}

bool Projectile::Update()
{
	//check if the bullet has travelled past the range
	if (distanceTravelled >= range)
		return true;
	return false;
}

Projectile::~Projectile()
{
}
