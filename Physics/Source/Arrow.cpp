#include "Arrow.h"

Arrow::Arrow()
{
	arrowspeed = 1;
	damage = 1;
	numOfEntitiesHit = 0;
	penetrationValue = 0;
	direction.SetZero();
	srand(time(NULL));
	critChance = 5;
}

Arrow::~Arrow()
{
}

bool Arrow::Update(double dt)
{
	//move the Arrow 
	gameobject->pos += (direction * arrowspeed * dt);
	gameobject->vel = direction;
	return false;
}

void Arrow::SetArrow(float Arrowspeed, unsigned damage, bool pen, float range, Vector3 direction)
{
	this->arrowspeed = Arrowspeed;
	this->damage = damage;
	penetrationValue = pen;
	this->range = range;
	this->direction = direction;
}

unsigned Arrow::GetDamage()
{
	return damage;
}

bool Arrow::GetPenetrationValue()
{
	return penetrationValue;
}
int Arrow::getCrit() {
	int random = rand() % critChance + 1;
	return random;
}
void Arrow::setCrit(int chance) {
	critChance = chance;
}