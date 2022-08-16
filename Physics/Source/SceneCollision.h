#ifndef SCENE_COLLISION_H
#define SCENE_COLLISION_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "SoundController.h"
#include "filesystem.h"
#include "Entity.h"
#include "Player.h"
#include "GameManger.h"

class SceneCollision : public SceneBase
{
public:
	SceneCollision();
	~SceneCollision();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();
	void ReturnGO(GameObject *go);

	void ResetLevel();

	bool SceneCollision::CheckCollision(GameObject* go1, GameObject* go2);
	void SceneCollision::CollisionResponse(GameObject* go1, GameObject* go2);
	void MakeThickWall(float width, float height, const Vector3& normal, const Vector3& pos, const Vector3& color);
	GameObject* Checkborder(GameObject* go);

protected:

	//Physics
	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	GameObject *m_ghost;
	GameObject* m_player;
	int m_objectCount;
	bool debug;
	unsigned levelno;
	bool playerwin;
	bool playerlose;
	bool gameclear;
	CSoundController* cSoundController;
	Player* player;
	GameManger* cGameManager;

	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2;
};

#endif