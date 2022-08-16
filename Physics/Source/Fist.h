#pragma once
#include "Melee.h"
class Fist : public Melee
{
public:
	Fist();
	~Fist();
	int GetDamageTaken();
private:
	int damage_taken;
};