#include "EMPSkill.h"
EMP::EMP() {

}
EMP::~EMP() {

}
std::string EMP::getDescription() {
	return "Releases a shockwave that stuns all enemies for 3 seconds.";
}
int EMP::getMoneyCost() {
	return 300;
}
int EMP::getEnergyCost() {
	return 100;
}
void EMP::UseSkill() {
	return;
}