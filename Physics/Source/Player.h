#pragma once
#include "Entity.h"
#include "GameObject.h"
#include "Application.h"
#include "GameManger.h"
class Player : public Entity ,public CSingletonTemplate<Player>
{
public:
	Player();
	void SetGameObject(GameObject* player);
	void Update(double dt);
	void ChangeHealth(int ChangeAmount);
	unsigned GetHealth();
	unsigned GetMaxHealth();
	void SetMaxHealth(unsigned newMaxHealth);
	~Player();
	GameObject* getPlayer();
private:
	GameObject* m_player;
	int health;
	unsigned maxHealth;
	float redTimer;
	float greenTimer;
	GameManger* cGameManager;
};

