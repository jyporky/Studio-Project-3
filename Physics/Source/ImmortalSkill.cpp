#include "ImmortalSkill.h"
Immortal::Immortal() {

}
Immortal::~Immortal() {

}
std::string Immortal::getDescription() {
	return "Immune to all damage for 3 seconds";
}
int Immortal::getMoneyCost() {
	return 600;
}
int Immortal::getEnergyCost() {
	return 200;
}