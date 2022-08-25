#ifndef SCENE_COLLISION_H
#define SCENE_COLLISION_H

#include "GameObject.h"
#include "MeshBuilder.h"
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
#include "BoxingGloves.h"
#include "Swordsman.h"
#include "Bullet.h"
#include "Rifle.h"
#include "RiflerEnemy.h"
#include "Rifle.h"
#include "Necromancer.h"

#include "FlameParticle.h"
#include "Flamethrower.h"
#include "ShieldEnemy.h"
#include "Shield.h"
#include "Crossbow.h"
#include "Arrow.h"


#include "Skill.h"
#include "HackSkill.h"
#include "EMPSkill.h"
#include "OverdriveSkill.h"
#include "HealSkill.h"
#include "ImmortalSkill.h"
#include "DoppelgangerSkill.h"
#include "DoppelgangerAlly.h"
#include "BlackholeSkill.h"

#include "Potions.h"
#include "HealthPotion.h"
#include "StrengthPotion.h"
#include "SpeedPotion.h"

#include "WeaponModifiers.h"
#include "PiercingBulletMod.h"
#include "InventoryManager.h"
#include "MyMath.h"

class SceneCollision : public SceneBase, public CSingletonTemplate<SceneCollision>
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
	void renderWeaponUI(Vector3 pos, Vector3 scale, GameObject* object);

	bool test;
	int test2;
protected:

	//Physics
	std::vector<GameObject *> m_goList;
	std::vector<Entity*> m_enemyList;
	std::vector<Bullet*> m_ebulletList;
	std::vector<Bullet*> m_pbulletList;

	std::vector<FlameParticle*> m_FlameParticle;

	std::vector<Arrow*> m_parrowList;

	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	int m_objectCount;
	CSoundController* cSoundController;
	Player* player;
	DoppelgangerAlly* doppelganger;
	GameManger* cGameManager;
	CInventoryManager* cInventoryManager;
	CInventoryItem* cInventoryItem;
	void RenderWall();
	int wave;
	float rate;
	int totalEnemy;
	int enemyLeft;
	void SpawnEnemy(float rate);
	float SetRate();
	bool NearShop();
	bool arrowCrit;
	void SetWeapon();
	void NewWeapon(int weptype, bool MainWep);
	Skill* HackSkill = new Hack();
	Skill* HealSkill = new Heal();
	Skill* ImmortalitySkill = new Immortal();
	Skill* DoppelgangerSkill = new Doppelganger();
	Skill* BlackholeSkill = new Blackhole();
	Skill* EMPSkill = new EMP();
	GameObject* blackhole;
	Potions* StrengthPotion = new StrengthPot();
	Potions* HealthPotion = new HealthPot();
	Potions* SpeedPotion = new SpeedPot();
	float timer;
	Vector3 color[3];
	int colorsize = 3;

	WeaponMods* PierceMod = new PiercingBulletMod();
	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2;
	float strengthPotTimer;

	float speedPotTimer;

	bool strengthPotUsed;
	bool speedPotUsed;
};

#endif