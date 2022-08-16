#include "HackSkill.h"
Hack::Hack() {

}
Hack::~Hack() {

}
std::string Hack::getDescription() {
	return "Hacks surrounding AI to fight for you for 10 seconds.";
}
int Hack::getMoneyCost() {
	return 400;
}
int Hack::getEnegyCost() {
	return 150;
}
void Hack::UseSkill() {
	return;
}