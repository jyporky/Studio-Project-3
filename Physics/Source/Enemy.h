#pragma once
#include "Entity.h"
#include "Player.h"
#include "GameManger.h"
#include "Weapon.h"
#include "SoundController.h"

class Enemy : public Entity
{
public:
	
	Enemy();
	~Enemy();
	//enemy returns true if the enemy dies
	virtual bool Update(double dt);
	virtual void Init();
	virtual void SetGameObject(GameObject* GO);
	virtual void SetWeapon(Weapon* weapon);
	virtual Weapon* GetWeapon();
	virtual GameObject* GetGameObject();
	void SetGO(GameObject* newGameObject);
	virtual bool IsSpawningBullet();
	virtual bool ChangeHealth(int changeInHealth);
	virtual unsigned GetMoneyDrop();
	virtual unsigned GetEnergyDrop();
	unsigned GetEnemyType();
	virtual float GetAngle();
	virtual bool getStunned();
	virtual void makeEnemyStunned();
protected:
	static std::vector<Entity*> m_enemyList;
	unsigned enemytype;
	bool turned;
	bool isSpawningBullet;
	int health;
	double iFrameTimer;
	float redTimer;
	float greenTimer;
	float attackSpeed;
	unsigned movementSpeed;
	unsigned energyDropped;
	unsigned moneyDropped;
	unsigned attackDamage;
	bool affectedByKnockback;
	static Player* PlayerPointer;
	static GameManger* cGameManager;
	bool isStunned;
};

