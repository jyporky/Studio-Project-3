#include "Fist.h"

Fist::Fist()
{
	cost = 0;
	damage = 8;
	range = 1;
	attack_speed = 1;
	attack_angle = 10;
	damage_taken = 1;
}


Fist::~Fist()
{

}

int Fist::GetDamageTaken()
{
	return damage_taken;
}