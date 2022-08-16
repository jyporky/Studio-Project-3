#pragma once
#include "Entity.h"
#include "Player.h"

class Enemy : public Entity
{
public:
	Enemy();
	~Enemy();
	//enemy returns true if the enemy dies
	virtual bool Update(double dt);
	virtual void Init();
	void SetGO(GameObject* newGameObject);
	bool ChangeHealth(int changeInHealth);
protected:
	unsigned health;
	float redTimer;
	float attackdt;
	unsigned movementSpeed;
	unsigned energyDropped;
	unsigned moneyDropped;
	unsigned attackDamage;
	bool affectedByKnockback;
	GameObject* enemyGameObject;
	static Player* PlayerPointer;
};

