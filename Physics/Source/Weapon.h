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
	float GetAttackCast();
	void SetAttackCast(float cd);
	virtual float GetAttackAngle();

protected:
	int cost;
	float damage;
	float range;
	float attack_speed;
	float attack_cast;
	/*unsigned type;*/
};