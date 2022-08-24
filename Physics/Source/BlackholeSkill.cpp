#include "BlackholeSkill.h"
Blackhole::Blackhole() {
	spawnBlackhole = false;
}
Blackhole::~Blackhole() {

}
std::string Blackhole::getDescription() {
	return "Creates a blackhole that sucks in enemies.";
}
int Blackhole::getMoneyCost() {
	return 350;
}
int Blackhole::getEnergyCost() {
	return 80;
}
void Blackhole::UseSkill() {
	spawnBlackhole = true;
}
void Blackhole::resetBlackhole() {
	spawnBlackhole = false;
}
bool Blackhole::getBlackholeState() {
	return spawnBlackhole;
}