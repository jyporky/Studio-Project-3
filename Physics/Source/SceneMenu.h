#ifndef SCENE_MENU_H
#define SCENE_MENU_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

class SceneMenu : public SceneBase
{
public:
	SceneMenu();
	~SceneMenu();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
protected:

	float m_worldWidth;
	float m_worldHeight;
	unsigned menubuttonhighlight = 0;
};

#endif