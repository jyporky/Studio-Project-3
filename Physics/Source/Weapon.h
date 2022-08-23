#pragma once
#include "GameObject.h"
#include "Vector3.h"
#include <string>
#include "SoundController.h"

class Weapon
{
public:
	Weapon();
	~Weapon();
	int GetCost();
	void SetCost(int Cost);
	float GetDamage();
	void SetDamage(float dmg);
	float GetRange();
	float GetAttackSpeed();
	void SetAttackSpeed(float as);
	float GetAttackCast();
	void SetAttackCast(float cd);
	void SetGameObject(GameObject* newGameObject);
	virtual bool GetPiercing();
	virtual void SetPiercing(bool pierce);
	virtual float GetBulletSpeed();
	virtual void SetBulletSpeed(float bs);
	GameObject* GetGameObject();
	virtual float GetAttackAngle();
	virtual bool Update(double dt, Vector3 mousepos, Vector3 movementdirection, GameObject* userGO);
	virtual bool attack();
	bool attacktest();


	std::string getDescription();
	bool IsMelee;

	enum WEAPONTYPE{
		NONE,
		SWORD,
		FIST,
		BOXING_GLOVES,
		FRYING_PAN,
		RUBBER_CHICKEN,
		RIFLE,
		CROSSBOW,
		SHIELD,
		FLAMETHROWER,
	};
	bool Animate;
	unsigned WeaponType;
protected:
	int cost;
	float damage;
	float range;
	float attack_speed;
	float attack_cast;
	double attackdt;
	GameObject* gameobject;
	std::string description;
	static CSoundController* cSoundController;
};