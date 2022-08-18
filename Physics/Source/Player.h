#pragma once
#include "Entity.h"
#include "GameObject.h"
#include "Application.h"
#include "GameManger.h"
#include "Weapon.h"
#include <cmath>
#include <iostream>
#include "SoundController.h"
class Player : public Entity ,public CSingletonTemplate<Player>
{
public:
	Player();
	void SetGameObject(GameObject* player);
	void Update(double dt, Vector3 mousepos);
	bool ChangeHealth(int ChangeAmount);
	void SetEnemyVector(std::vector<Entity*> m_enemyList);
	unsigned GetHealth();
	unsigned GetMaxHealth();
	void SetMaxHealth(unsigned newMaxHealth);
	void SetWeapon(Weapon* weapon);
	void Attack(Vector3 mousepos);
	Weapon* GetWeapon();
	~Player();
	GameObject* getPlayer();
	bool iFrame;
	int getMoney();
	void changeMoney(float moneyChange);
private:
	std::vector<Entity*> m_enemyList;
	int health;
	unsigned movementspeed;
	unsigned maxHealth;
	float redTimer;
	float greenTimer;
	float blueTimer;
	GameManger* cGameManager;
	CSoundController* cSoundController;
	int money;
};

