#pragma once
#include "Ranged.h"
class Crossbow : public Ranged {
public:
	Crossbow();
	~Crossbow();
private:
	float critChance;
	float critDamage;
};