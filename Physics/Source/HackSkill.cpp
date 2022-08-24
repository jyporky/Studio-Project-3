#include "HackSkill.h"
Hack::Hack() {
	hackState = false;
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
	hackState = true;
}
void Hack::resetHack() {
	hackState = false;
}
bool Hack::getHackingState() {
	return hackState;
}