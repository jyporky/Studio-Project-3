#include "FlameParticle.h"

FlameParticle::FlameParticle()
{
	bulletspeed = 30;
	damage = 0;
	numOfEntitiesHit = 0;
	penetrationValue = 0;
	distanceTravelled = 0.1;
	range = 40;
}

FlameParticle::~FlameParticle()
{
}

bool FlameParticle::Update(double dt)
{
	//move the bullet 
	float speedmod;
	speedmod = ((range - distanceTravelled)/ range);
	gameobject->vel = direction * bulletspeed * speedmod;
	gameobject->pos += gameobject->vel * dt;
	distanceTravelled += (direction * bulletspeed * dt).Length();
	if (distanceTravelled > range)
	{
		return true;
	}
	return false;
}

void FlameParticle::SetBullet(float bulletspeed, unsigned damage, bool pen, float range, Vector3 direction)
{
	this->bulletspeed = bulletspeed;
	this->damage = damage;
	penetrationValue = true;
	this->range = range;
	this->direction = direction;
}

unsigned FlameParticle::GetDamage()
{
	return damage;
}

bool FlameParticle::GetPenetrationValue()
{
	return penetrationValue;
}

void FlameParticle::HalfRange()
{
	range *= 0.99;
}
