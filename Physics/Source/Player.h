#pragma once
#include "Entity.h"
#include "GameObject.h"
#include "Application.h"
#include "GameManger.h"
#include "Weapon.h"
#include "SoundController.h"
class Player : public Entity ,public CSingletonTemplate<Player>
{
public:
	Player();
	void SetGameObject(GameObject* player);
	void Update(double dt, Vector3 mousepos);
	void ChangeHealth(int ChangeAmount);
	unsigned GetHealth();
	unsigned GetMaxHealth();
	void SetMaxHealth(unsigned newMaxHealth);
	void SetWeapon(Weapon* weapon);
	Weapon* GetWeapon();
	~Player();
	GameObject* getPlayer();
private:
	GameObject* m_player;
	int health;
	unsigned maxHealth;
	float redTimer;
	float greenTimer;
	Weapon* weapon;
	GameManger* cGameManager;
	CSoundController* cSoundController;
};

