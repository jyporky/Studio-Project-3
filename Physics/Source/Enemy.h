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
	virtual bool getTurnedState();
	virtual void turnEnemy();
	virtual Entity* getTarget();
	virtual bool IsSpawningSwordsman();
	virtual unsigned GetMaxHealth()
	{
		return maxhealth;
	}
	virtual unsigned GetHealth()
	{
		return health;
	}
	static void SetEnemyVector(std::vector<Entity*> m_enemyList);
	virtual void SetUndead();
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
	bool isSpawningSwordsman;
	unsigned maxhealth;
};

