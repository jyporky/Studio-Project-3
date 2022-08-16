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
#include "Enemy.h"
#include "GameManger.h"
<<<<<<< HEAD
#include "Weapon.h"
#include "Fist.h"
#include "Sword.h"
=======
#include "Swordsman.h"

#include "Skill.h"
#include "HackSkill.h"
#include "EMPSkill.h"
#include "OverdriveSkill.h"
#include "HealSkill.h"
#include "ImmortalSkill.h"

#include "Potions.h"
#include "HealthPotion.h"
#include "StrengthPotion.h"
#include "SpeedPotion.h"
>>>>>>> 78685c1f0494b66d6ad065fd1e5d0e7e0c7af0d1

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
	std::vector<Enemy*> m_enemyList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	int m_objectCount;
	CSoundController* cSoundController;
	Player* player;
	GameManger* cGameManager;
<<<<<<< HEAD
	Weapon* sword;
	GameObject* weapon;
	float timer;
	Vector3 offset;

=======
	Skill* HackSkill = new Hack();
	Potions* StrengthPotion = new StrengthPot();
>>>>>>> 78685c1f0494b66d6ad065fd1e5d0e7e0c7af0d1
	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2;
};

#endif