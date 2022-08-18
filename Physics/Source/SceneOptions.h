#ifndef SCENE_OPTIONS_H
#define SCENE_OPTIONS_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "SoundController.h"

class SceneOptions : public SceneBase
{
public:
	SceneOptions();
	~SceneOptions();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual void CreateBullet(Vector3 pos, Vector3 direction);
protected:

	CSoundController *cSoundController;
	float m_worldWidth;
	float m_worldHeight;
	unsigned menubuttonhighlight = 0;
};

#endif