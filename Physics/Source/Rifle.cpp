#include "Rifle.h"
Rifle::Rifle() {
	piercing = false;
	bullet_speed = 30;
	cost = 80;
	damage = 3;
	range = 1000;
	attack_speed = 0.3;
	description = "RATATATATATATATA";
	WeaponType = RIFLE;
}
Rifle::~Rifle() {

}