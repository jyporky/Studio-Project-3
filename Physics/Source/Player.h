#pragma once
#include "Entity.h"
#include "GameObject.h"
#include "Application.h"
#include "GameManger.h"
#include "Skill.h"
#include "Bullet.h"
#include <cmath>
#include <iostream>
#include "SoundController.h"
class Player : public Entity ,public CSingletonTemplate<Player>
{
public:
	Player();
	~Player();
	void SetGameObject(GameObject* player);
	void Update(double dt, Vector3 mousepos);
	bool ChangeHealth(int ChangeAmount);
	void SetEnemyVector(std::vector<Entity*> m_enemyList);
	unsigned GetHealth();
	unsigned GetMaxHealth();
	void SetMaxHealth(unsigned changedMaxHealth);
	void SetWeapon(Weapon* weapon);
	void Attack(Vector3 mousepos);
	virtual bool IsSpawningBullet();
	Weapon* GetWeapon();
	Weapon* GetSideWeapon();
	GameObject* getPlayer();
	bool iFrame;
	int getMoney();
	void changeMoney(float moneyChange);
	int getEnergy();
	void changeEnergy(float energyChange);
	void SwapWeapon();
	void changeMovementSpeed(float change);
	void SetSideWeapon(Weapon* sidewep);
	void ResetPlayer();
	unsigned GetMovementSpeed()
	{
		return movementspeed;
	}
	unsigned GetDashBoost()
	{
		return dashBoost;
	}
	void setMovementSpeed(int ms);
	int meleeDmgBoost;
	int rangeDmgBoost;
	bool dashing;
	float rotate;
private:
	std::vector<Entity*> m_enemyList;
	std::vector<Entity*> hitlist;
	Vector3 clickMousePos;
	bool isSpawningBullet;
	int health;
	unsigned movementspeed;
	unsigned dashBoost; // increase of movement speed due to dash
	unsigned maxHealth;
	float redTimer;
	float greenTimer;
	float blueTimer;
	GameManger* cGameManager;
	int money;
	int energy;
	Vector3 dashDirection;
	float dashcd;
};

