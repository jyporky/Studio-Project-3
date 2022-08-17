#pragma once
#include "Entity.h"
#include "Player.h"
#include "GameManger.h"
#include "Weapon.h"

class Enemy : public Entity
{
public:
	Enemy();
	~Enemy();
	//enemy returns true if the enemy dies
	virtual bool Update(double dt);
	virtual void Init();
	void SetEnemyGameObject(GameObject* enemyGO);
	GameObject* GetEnemyGameObject();
	void SetGO(GameObject* newGameObject);
	bool ChangeHealth(int changeInHealth);
	virtual void SetWeapon(Weapon* weapon);
	virtual Weapon* GetWeapon();
protected:
	unsigned health;
	float redTimer;
	float greenTimer;
	float attackdt;
	float attackSpeed;
	unsigned movementSpeed;
	unsigned energyDropped;
	unsigned moneyDropped;
	unsigned attackDamage;
	bool affectedByKnockback;
	GameObject* enemyGameObject;
	static Player* PlayerPointer;
	static GameManger* cGameManager;
	Weapon* weapon;
};

