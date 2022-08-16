#pragma once
#include "Entity.h"
#include "GameObject.h"
#include "Application.h"
class Player : public Entity ,public CSingletonTemplate<Player>
{
public:
	Player();
	void SetGameObject(GameObject* player);
	void Update(double dt);
	~Player();
	GameObject* getPlayer();
private:
	GameObject* m_player;
};

