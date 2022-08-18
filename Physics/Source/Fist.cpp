#include "Fist.h"

Fist::Fist()
{
	cost = 0;
	damage = 8;
	range = 1;
	attack_speed = 1;
	attack_cast = 0.4;
	attack_angle = 10;
	damage_taken = 1;
	description = "Whats weapon? Just punch them like a man. But you hurt yourself dumbass.";
	WeaponType = FIST;
}


Fist::~Fist()
{
	
}

int Fist::GetDamageTaken()
{
	return damage_taken;
}