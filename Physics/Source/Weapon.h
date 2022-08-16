#pragma once
class Weapon
{
public:
	enum WEAPON_TYPE
	{
		FIST,
		GUN,
		NUM_TYPE,
	};
	int GetCost();
	void SetCost(int Cost);
	float GetDamage();
	void SetDamage(float dmg);
	float GetRange();
	float GetAttackSpeed();
	void SetAttackSpeed(float as);

protected:
	int cost;
	float damage;
	float range;
	float attack_speed;
	/*unsigned type;*/
};