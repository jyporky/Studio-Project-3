#pragma once
#include <string>
class WeaponMods {
public:
	WeaponMods();
	~WeaponMods();
	std::string getDescription();
	int getMoneyCost();
protected:
	std::string description;
	int moneyCost;

};