#ifndef SCENE_SHOP_H
#define SCENE_SHOP_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Player.h"

class SceneShop : public SceneBase
{
public:
	SceneShop();
	~SceneShop();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	GameObject* Checkborder(GameObject* go);
	void renderEnvironment();
protected:
	Player* player;
	float m_worldWidth;
	float m_worldHeight;
	GameObject* m_player;
	unsigned menubuttonhighlight = 0;
};

#endif