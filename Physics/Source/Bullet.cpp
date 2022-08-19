#include "Bullet.h"

Bullet::Bullet()
{
	bulletspeed = 1;
	damage = 1;
	numOfEntitiesHit = 0;
	penetrationValue = 0;
	direction.SetZero();
}

Bullet::~Bullet()
{
}

bool Bullet::Update(double dt)
{
	//move the bullet 
	gameobject->pos += (direction * bulletspeed * dt);
	gameobject->vel = direction;
	return false;
}

void Bullet::SetBullet(float bulletspeed, unsigned damage, bool pen, float range, Vector3 direction)
{
	this->bulletspeed = bulletspeed;
	this->damage = damage;
	penetrationValue = pen;
	this->range = range;
	this->direction = direction;
}

unsigned Bullet::GetDamage()
{
	return damage;
}

bool Bullet::GetPenetrationValue()
{
	return penetrationValue;
}
