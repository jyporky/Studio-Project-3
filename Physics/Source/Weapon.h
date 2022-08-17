#pragma once
#include "GameObject.h"
#include "Vector3.h"
#include <string>

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
	GameObject* GetGameObject();
	virtual float GetAttackAngle();
	virtual bool Update(double dt, Vector3 mousepos, Vector3 movementdirection, GameObject* userGO);
	virtual void attack();


	std::string getDescription();
	bool IsMelee;
protected:

	int cost;
	float damage;
	float range;
	float attack_speed;
	float attack_cast;
	bool Animate;
	double attackdt;
	GameObject* gameobject;
	std::string description;
};