#ifndef SCENE_SHOP_H
#define SCENE_SHOP_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

class SceneShop : public SceneBase
{
public:
	SceneShop();
	~SceneShop();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void renderEnvironment();
protected:

	float m_worldWidth;
	float m_worldHeight;
	unsigned menubuttonhighlight = 0;
};

#endif