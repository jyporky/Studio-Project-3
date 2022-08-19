#ifndef SCENE_MENU_H
#define SCENE_MENU_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "SoundController.h"
class SceneMenu : public SceneBase
{
public:
	SceneMenu();
	~SceneMenu();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual void CreateBullet(Vector3 pos, Vector3 direction);
protected:
	//sound
	CSoundController* cSoundController;

	float m_worldWidth;
	float m_worldHeight;
	unsigned menubuttonhighlight = 0;
};

#endif