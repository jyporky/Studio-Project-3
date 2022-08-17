#include "Crossbow.h"
Crossbow::Crossbow() {
	piercing = false;
	bullet_speed = 50;
	cost = 150;
	damage = 5;
	range = 1000;
	attack_speed = 1;
	critChance = 0.3;
	critDamage = 2;
	description = "Have a 30% chance to headshot a target (Does not apply to tanks)";
}
Crossbow::~Crossbow() {

}