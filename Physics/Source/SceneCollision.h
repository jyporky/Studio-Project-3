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
#include "Weapon.h"
#include "Fist.h"
#include "Sword.h"
#include "Swordsman.h"
#include "Bullet.h"
#include "Rifle.h"
#include "RiflerEnemy.h"
#include "Rifle.h"

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

#include "WeaponModifiers.h"
#include "PiercingBulletMod.h"
#include "InventoryManager.h"

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

	void renderUI();
protected:

	//Physics
	std::vector<GameObject *> m_goList;
	std::vector<Entity*> m_enemyList;
	std::vector<Bullet*> m_ebulletList;
	std::vector<Bullet*> m_pbulletList;
	std::vector<Weapon*> m_weaponList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	int m_objectCount;
	CSoundController* cSoundController;
	Player* player;
	GameManger* cGameManager;
	CInventoryManager* cInventoryManager;
	CInventoryItem* cInventoryItem;

	Skill* HackSkill = new Hack();
	Skill* HealSkill = new Heal();
	Potions* StrengthPotion = new StrengthPot();
	Potions* HealthPotion = new HealthPot();
	float timer;

	WeaponMods* PierceMod = new PiercingBulletMod();
	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2;
};

#endif