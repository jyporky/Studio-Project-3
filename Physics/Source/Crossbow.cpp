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
	description = "30% chance to headshot and CRIT	";
	WeaponType = CROSSBOW;
}
Crossbow::~Crossbow() {

}