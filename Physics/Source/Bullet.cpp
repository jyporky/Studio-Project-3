#include "Bullet.h"

Bullet::Bullet()
{
	bulletspeed = 1;
	damage = 1;
	numOfEntitiesHit = 0;
	penetrationValue = 0;
	direction.SetZero();
	isExplosive = false;
	explosionRadius = 0;
}

Bullet::~Bullet()
{
}

bool Bullet::Update(double dt)
{
	//move the bullet 
	gameobject->pos += (direction * bulletspeed * dt);
	distanceTravelled += (direction * bulletspeed * dt).Length();
	gameobject->vel = direction;
	if (distanceTravelled > range)
	{
		return true;
	}
	return false;
}

void Bullet::SetBullet(float bulletspeed, unsigned damage, bool pen, float range, Vector3 direction)
{
	this->bulletspeed = bulletspeed;
	this->damage = damage;
	penetrationValue = pen;
	this->range = range;
	this->direction = direction;
	isExplosive = false;
	explosionRadius = 0;
}

void Bullet::SetBullet(float bulletspeed, unsigned damage, bool pen, float range, Vector3 direction, bool isExplosive, float explosionRad)
{
	this->bulletspeed = bulletspeed;
	this->damage = damage;
	penetrationValue = pen;
	this->range = range;
	this->direction = direction;
	this->isExplosive = isExplosive;
	explosionRadius = explosionRad;
}

unsigned Bullet::GetDamage()
{
	return damage;
}

bool Bullet::GetPenetrationValue()
{
	return penetrationValue;
}
