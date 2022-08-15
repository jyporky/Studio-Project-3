#pragma once
#include "Entity.h"
#include "GameObject.h"
class Player : public Entity ,public CSingletonTemplate<Player>
{
public:
	Player();
	void SetGameObject(GameObject* player);
	void update();
	~Player();
	GameObject* getPlayer();
private:
	GameObject* m_player;
};

