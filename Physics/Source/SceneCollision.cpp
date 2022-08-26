#include "SceneCollision.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include <cmath>

SceneCollision::SceneCollision()
{

}

SceneCollision::~SceneCollision()
{
}

static Vector3 RotateVector(const Vector3& vec, float radian)
{
	return Vector3(vec.x * cos(radian) + vec.y * -sin(radian),
		vec.x * sin(radian) + vec.y * cos(radian), 0.f);
}

int SolveQuadratic(float a, float b, float c, float& root1, float& root2)
{
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
	{
		root1 = INFINITY;
		root2 = -root1;
		return 0;
	}

	root1 = (-b + sqrt(discriminant)) / (2 * a);
	root2 = (-b - sqrt(discriminant)) / (2 * a);
	if (discriminant > 0)
		return 2;
	else
		return 1;
}

//calculate the angle between vector in radian
float RadainAngleBetweenVector(Vector3 a, Vector3 b)
{
	float a1 = atan2f(a.y, a.x);
	float a2 = atan2f(b.y, b.x);
	
	a1 = a1 - a2;
	if (a1 > 180)
		a1 -= 360;
	else if (a1 < -180)
		a1 += 360;

	if (a1 >= 0)
		return a1;
	else
		return -a1;
}

bool InterceptionDirection(Vector3 a, Vector3 b, Vector3 vA, float sB, Vector3& result)
{
	Vector3 aToB = b - a;
	float dC = aToB.Length();
	float alpha = RadainAngleBetweenVector(aToB, vA);
	float sA = vA.Length();
	float r = sA / sB;
	float root1, root2;
	if (SolveQuadratic(1 - r * r, 2 * r * dC * cos(alpha), -(dC * dC), root1, root2) == 0)
	{
		result = Vector3(0, 0, 0);
		return false;
	}
	float dA = max(root1, root2);
	float t = dA / sB;
	Vector3 c = a + vA * t;
	result = (c - b).Normalize();
	return true;
}

void SceneCollision::Init()
{
	SceneBase::Init();
	
	cGameManager = GameManger::GetInstance();
	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 1.f;
	bLightEnabled = true;

	Math::InitRNG();

	//Exercise 1: initialize m_objectCount
	m_objectCount = 0;

	cGameManager->dWaveNo = 1;
	

	rate = SetRate();

	timer = -3;

	arrowCrit = false;

	blackhole = nullptr;
	doppelganger = nullptr;
	test = false;
	test2 = 0;
	enemyHacked = false;
	empTimer = 0;
	blackholeTimer = 0;
	immortalTimer = 0;
	hackTimer = 0;
	//load sound
	cSoundController = CSoundController::GetInstance();
	cSoundController->Init();
	cSoundController->LoadSound(FileSystem::getPath("Sound\\damage.ogg"), 1, false);
	cSoundController->LoadSound(FileSystem::getPath("Sound\\sword_swing.ogg"), 2, false);
	cSoundController->LoadSound(FileSystem::getPath("Sound\\enemyHurt.ogg"), 3, false);
	cSoundController->LoadSound(FileSystem::getPath("Sound\\enemyDeath.ogg"), 4, false);
	cSoundController->LoadSound(FileSystem::getPath("Sound\\shoot.ogg"), 5, false);
	cSoundController->LoadSound(FileSystem::getPath("Sound\\playerDash.ogg"), 6, false);
	cSoundController->LoadSound(FileSystem::getPath("Sound\\flamethrower.ogg"), 7, true);
	cSoundController->LoadSound(FileSystem::getPath("Sound\\menuBGM.ogg"), 8, true, CSoundInfo::BGM);
	cSoundController->LoadSound(FileSystem::getPath("Sound\\gameplayBGM.ogg"), 9, true, CSoundInfo::BGM);
	cSoundController->LoadSound(FileSystem::getPath("Sound\\buyItem.ogg"), 10, false);
	cSoundController->LoadSound(FileSystem::getPath("Sound\\Shieldblock.ogg"), 11, false);
	cSoundController->LoadSound(FileSystem::getPath("Sound\\switch_weapon.ogg"), 12, false);
	cSoundController->LoadSound(FileSystem::getPath("Sound\\errorSound.ogg"), 13, false);


	cInventoryManager = CInventoryManager::GetInstance();
	//weapons
	cInventoryItem = cInventoryManager->Add("sword", 1, 1);
	cInventoryItem = cInventoryManager->Add("boxingglove", 1, 0);	
	
	cInventoryItem = cInventoryManager->Add("rifle", 1, 0);
	cInventoryItem = cInventoryManager->Add("flamethrower", 1, 0);
	cInventoryItem = cInventoryManager->Add("crossbow", 1, 0);

	//potions
	cInventoryItem = cInventoryManager->Add("healthpotion", 99, 0);
	cInventoryItem = cInventoryManager->Add("strengthpotion", 99, 0);
	cInventoryItem = cInventoryManager->Add("speedpotion", 99, 0);

	//skills
	cInventoryItem = cInventoryManager->Add("emp", 1, 0);
	cInventoryItem = cInventoryManager->Add("hack", 1, 0);
	cInventoryItem = cInventoryManager->Add("doppelganger", 1, 0);
	cInventoryItem = cInventoryManager->Add("immortal", 1, 0);
	cInventoryItem = cInventoryManager->Add("blackhole", 1, 0);

	

	GameObject* m_player = FetchGO();
	m_player->type = GameObject::GO_PLAYER;
	m_player->pos.Set(m_worldWidth / 2, m_worldHeight / 2, 1);
	m_player->vel.SetZero();
	m_player->scale.Set(10, 10, 1);
	m_player->color.Set(1, 1, 1);
	m_player->angle = 0;
	m_player->active = true;
	player = Player::GetInstance();
	player->SetGameObject(m_player);
	Sword* sword = new Sword();
	player->SetWeapon(sword);
	GameObject* weapon2 = FetchGO();
	weapon2->type = GameObject::GO_SWORD;
	weapon2->pos.SetZero();
	weapon2->vel.SetZero();
	weapon2->scale.Set(10, 10, 1);
	weapon2->angle = 0;
	weapon2->color.Set(1, 1, 1);
	weapon2->leftwep = false;
	cGameManager->weptype = Weapon::SWORD;
	player->GetWeapon()->SetGameObject(weapon2);

	colorsize = 3;
	for (unsigned i = 0; i < colorsize; ++i)
	{
		color[i] = Vector3(1,1,1);
	}

	meshList[GEO_TELEPORT_PAD]->material.kAmbient.Set(1, 1, 1);

	strengthPotTimer = 30;
	speedPotTimer = 30;

	strengthPotUsed = false;
	speedPotUsed = false;

	//Enemy* enemy4;
	//GameObject* enemyGO4;
	//enemy4 = new Necromancer();
	//enemy4->Init();
	//enemyGO4 = FetchGO();
	//enemyGO4->type = GameObject::GO_NECROMANCER;
	//enemyGO4->pos = (m_worldWidth / 2, m_worldHeight / 2, 1);
	//enemyGO4->vel.SetZero();
	//enemyGO4->scale.Set(13, 10, 1);
	//enemyGO4->color.Set(1, 1, 1);
	//enemyGO4->angle = 0;
	//enemy4->SetGameObject(enemyGO4);
	//m_enemyList.push_back(enemy4);
	//enemyLeft++;
}

GameObject* SceneCollision::FetchGO()
{
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (go->active)
			continue;
		++m_objectCount;
		go->ResetValues();
		go->active = true;
		return go;
	}
	int prevSize = m_goList.size();
	for (int i = 0; i < 10; ++i)
	{
		m_goList.push_back(new GameObject(GameObject::GO_BALL));
	}
	m_goList.at(prevSize)->active = true;
	++m_objectCount;
	return m_goList[prevSize];
}

void SceneCollision::ReturnGO(GameObject *go)
{
	//Exercise 3: implement ReturnGO()
	if (go->active)
	{
		go->active = false;
		--m_objectCount;
	}
}

float Clamp(float value, float minvalue, float maxvalue)
{
	if (value > maxvalue)
		return maxvalue;
	else if (value < minvalue)
		return minvalue;
	else
		return value;
}

void SceneCollision::ResetLevel()
{
	for (unsigned idx = 0; idx < m_goList.size(); idx++)
	{
		if (!m_goList[idx]->active)
			continue;
		ReturnGO(m_goList[idx]);
		m_goList[idx]->ResetValues();
	}

	while (m_enemyList.size() > 0)
	{
		Entity* go = m_enemyList.back();
		delete go;
		m_enemyList.pop_back();
	}
	while (m_pbulletList.size() > 0)
	{
		Bullet* go = m_pbulletList.back();
		delete go;
		m_pbulletList.pop_back();
	}
	while (m_ebulletList.size() > 0)
	{
		Bullet* go = m_ebulletList.back();
		delete go;
		m_ebulletList.pop_back();
	}
	while (m_FlameParticle.size() > 0)
	{
		FlameParticle* go = m_FlameParticle.back();
		delete go;
		m_FlameParticle.pop_back();
	}

	while (m_parrowList.size() > 0)
	{
		Arrow* go = m_parrowList.back();
		delete go;
		m_parrowList.pop_back();
	}

	player->ResetPlayer();
	GameObject* m_player = FetchGO();
	m_player->type = GameObject::GO_PLAYER;
	m_player->pos.Set(m_worldWidth / 2, m_worldHeight / 2, 1);
	m_player->vel.SetZero();
	m_player->scale.Set(10, 10, 1);
	m_player->color.Set(1, 1, 1);
	m_player->angle = 0;
	m_player->active = true;
	player->SetGameObject(m_player);
	Sword* sword = new Sword();
	player->SetWeapon(sword);
	GameObject* weapon2 = FetchGO();
	weapon2->type = GameObject::GO_SWORD;
	weapon2->pos.SetZero();
	weapon2->vel.SetZero();
	weapon2->scale.Set(10, 10, 1);
	weapon2->angle = 0;
	weapon2->color.Set(1, 1, 1);
	weapon2->leftwep = false;
	cGameManager->weptype = Weapon::SWORD;
	player->GetWeapon()->SetGameObject(weapon2);


	cGameManager->bPlayerLost = false;
	cGameManager->dWaveNo = 1;
	cGameManager->sideweptype = Weapon::NONE;
	cInventoryItem = cInventoryManager->GetItem("boxingglove");
	cInventoryItem->Remove(1);	
	cInventoryItem = cInventoryManager->GetItem("rifle");
	cInventoryItem->Remove(1);	
	cInventoryItem = cInventoryManager->GetItem("flamethrower");
	cInventoryItem->Remove(1);	
	cInventoryItem = cInventoryManager->GetItem("crossbow");
	cInventoryItem->Remove(1);	
	cInventoryItem = cInventoryManager->GetItem("healthpotion");
	cInventoryItem->Remove(10);	
	cInventoryItem = cInventoryManager->GetItem("strengthpotion");
	cInventoryItem->Remove(10);	
	cInventoryItem = cInventoryManager->GetItem("speedpotion");
	cInventoryItem->Remove(10);
	cInventoryItem = cInventoryManager->GetItem("emp");
	cInventoryItem->Remove(1);
	cInventoryItem = cInventoryManager->GetItem("hack");
	cInventoryItem->Remove(1);
	cInventoryItem = cInventoryManager->GetItem("doppelganger");
	cInventoryItem->Remove(1);
	cInventoryItem = cInventoryManager->GetItem("immortal");
	cInventoryItem->Remove(1);
	cInventoryItem = cInventoryManager->GetItem("blackhole");
	cInventoryItem->Remove(1);

	rate = SetRate();
	timer = -3;

	colorsize = 3;
	for (unsigned i = 0; i < colorsize; ++i)
	{
		color[i] = Vector3(1, 1, 1);
	}

	meshList[GEO_TELEPORT_PAD]->material.kAmbient.Set(1, 1, 1);

	player->setMovementSpeed(40);
	player->SetMaxHealth(100);
	player->meleeDmgBoost = 0;
	player->rangeDmgBoost = 0;



	strengthPotTimer = 30;
	speedPotTimer = 30;

	strengthPotUsed = false;
	speedPotUsed = false;

	cGameManager->reset = true;



	cGameManager->empBought = false;
	cGameManager->hackBought = false;
	cGameManager->doppelgangerBought = false;
	cGameManager->immortalBought = false;
	cGameManager->blackholeBought = false;

	cGameManager->boxingGloveBought = false;
	cGameManager->rifleBought = false;
	cGameManager->flamethrowerBought = false;
	cGameManager->crossbowBought = false;

	cGameManager->pierceBought = false;
	cGameManager->fastfireBought = false;
	cGameManager->fastbulletBought = false;
	cGameManager->explosiveBought = false;
	cGameManager->betterfuelBought = false;
	cGameManager->accuratearrowsBought = false;
	cGameManager->fastmeleeBought = false;
}

void SceneCollision::Update(double dt)
{
	timer += dt;

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	double x, y;
	float width = Application::GetWindowWidth();
	float height = Application::GetWindowHeight();
	Application::GetCursorPos(&x, &y);
	Vector3 mousePos = Vector3((x / width) * m_worldWidth, ((height - y) / height) * m_worldHeight, 0);
	
	
	SceneBase::Update(dt);
	//std::cout << ImmortalitySkill->getState() << std::endl;
	if (cGameManager->bPlayerLost)
	{
		if (Application::IsKeyPressed('R'))
		{
			ResetLevel();
		}
		if (Application::IsKeyPressed(VK_OEM_3))
		{
			Application::SetState(1);
		}
		return;
	}

	//to go shop for debugging
	if (Application::IsKeyPressed('H'))
	{
		Application::SetState(3);
	}

	//pause
	static bool oem_3 = false;
	if ((Application::IsKeyPressed(VK_OEM_3)) && (!oem_3))
	{
		Application::SetState(4);
		oem_3 = true;
	}
	else if ((!Application::IsKeyPressed(VK_OEM_3)) && (oem_3))
	{
		oem_3 = false;
	}

	if (cGameManager->outShop)
	{
		cGameManager->outShop = false;
		SetWeapon();
		cGameManager->dWaveNo++;
		rate = SetRate();
		player->GetGameObject()->pos.Set(m_worldWidth / 2, m_worldHeight / 2, 1);
		timer = -3;
		for (unsigned i = 0; i < colorsize; ++i)
		{
			color[i].Set(1, 1, 1);
		}
	}

	if (timer > -0.1 && timer < 0)
		timer = 10;

	if (totalEnemy > 0 && timer > 0)
		SpawnEnemy(rate);
	else if (totalEnemy <= 0 && timer > rate / 3)
	{
		for (unsigned i = 0; i < colorsize; ++i)
		{
			color[i].Set(1, 1, 1);
		}
	}

	if (timer >= 0)
	{
		if (enemyLeft <= 0)
		{
			cGameManager->waveClear = true;
			timer = 0;
		}
	}

	static bool oem_5 = false;
	if (Application::IsKeyPressed(VK_OEM_5) && !oem_5)
	{
		cGameManager->bDebug = !cGameManager->bDebug;
		oem_5 = true;
	}
	else if (!Application::IsKeyPressed(VK_OEM_5) && oem_5)
		oem_5 = false;

	static bool q = false, e = false;
	if (Application::IsKeyPressed('Q') && !q)
	{
		q = true;
	}
	else if (!Application::IsKeyPressed('Q') && q)
		q = false;

	if (Application::IsKeyPressed('E') && cGameManager->waveClear && !e && NearShop()) //go shop
	{
		Application::SetState(3);
		player->getPlayer()->pos.x = m_worldWidth / 2;
		player->getPlayer()->pos.y = 12;
		cGameManager->eButtonState;
		
		//clear the projectiles
		while (m_ebulletList.size() != 0)
		{
			ReturnGO(m_ebulletList.back()->GetGameObject());
			delete m_ebulletList.back();
			m_ebulletList.pop_back();
		}
		while (m_pbulletList.size() != 0)
		{
			ReturnGO(m_pbulletList.back()->GetGameObject());
			delete m_pbulletList.back();
			m_pbulletList.pop_back();
		}
		while (m_parrowList.size() != 0)
		{
			ReturnGO(m_parrowList.back()->GetGameObject());
			delete m_parrowList.back();
			m_parrowList.pop_back();
		}
		while (m_FlameParticle.size() != 0)
		{
			ReturnGO(m_FlameParticle.back()->GetGameObject());
			delete m_FlameParticle.back();
			m_FlameParticle.pop_back();
		}


		//remove the particles
		for (unsigned idx = 0; idx < m_goList.size(); idx++)
		{
			if (!m_goList[idx]->active)
				continue;

			if (m_goList[idx]->type != GameObject::GO_DEATH_PARTICLE)
				continue;

			ReturnGO(m_goList[idx]);
		}

	}
	else if (!Application::IsKeyPressed('E') && e)
		e = false;

	if (cGameManager->bDebug)
	{
		if (Application::IsKeyPressed('9'))
		{
			m_speed = Math::Max(0.f, m_speed - 0.1f);
		}
		if (Application::IsKeyPressed('0'))
		{
			m_speed += 0.1f;
		}
	}
	cSoundController->PlaySoundByID(9);

	//skills
	//remove heal skill later
	static bool ubutton;
	if (Application::IsKeyPressed('U') && !ubutton)
	{
		ubutton = true;
		HealSkill->UseSkill();
	}
	else if (!Application::IsKeyPressed('U') && ubutton)
	{
		ubutton = false;
	}
	if (EMPSkill->getStunState())
		empTimer += dt;
	if (empTimer >= 3) {
		empTimer = 0;
		EMPSkill->resetStun();
	}

	
	if(ImmortalitySkill->getState())
	   immortalTimer += dt;
	if (immortalTimer >= 2)
	{
		cGameManager->isImmortal = false;
		ImmortalitySkill->resetImmortality();
		empTimer = 0;
	}
	if (HackSkill->getHackingState()) {
		enemyHacked = true;
		HackSkill->resetHack();
	}
	if (enemyHacked)
		hackTimer += dt;
	if (blackhole) 
	{
		blackholeTimer += dt;
		if (blackholeTimer >= 3)
		{
		
			ReturnGO(blackhole);
			BlackholeSkill->resetBlackhole();
			blackhole = nullptr;
			blackholeTimer = 0;
		}
	}

	static bool qbutton = false;
	if ((Application::IsKeyPressed('Q')) && (!qbutton))
	{
		qbutton = true;
		switch (cGameManager->skilltype)
		{
		case Skill::EMP:
			if ((player->getEnergy() >= EMPSkill->getEnergyCost()) && (empTimer == 0))
			{
				EMPSkill->UseSkill();
				player->changeEnergy(-EMPSkill->getEnergyCost());
			}
			else
			{
				cSoundController->PlaySoundByID(13);
			}
			
			break;
		case Skill::HACK:
			if ((player->getEnergy() >= HackSkill->getEnergyCost()) && (hackTimer <= 0))
			{
				HackSkill->UseSkill();
				player->changeEnergy(-HackSkill->getEnergyCost());
			}
			else
			{
				cSoundController->PlaySoundByID(13);
			}
			break;
		case Skill::DOPPELGANGER:
			if (player->getEnergy() >= DoppelgangerSkill->getEnergyCost())
			{
				spawnDoppelganger();
				player->changeEnergy(-DoppelgangerSkill->getEnergyCost());
			}
			else
			{
				cSoundController->PlaySoundByID(13);
			}
			break;
		case Skill::IMMORTAL:
			if ((player->getEnergy() >= ImmortalitySkill->getEnergyCost()) && (immortalTimer <= 0))
			{
				ImmortalitySkill->UseSkill();
				cGameManager->isImmortal = true;
				player->changeEnergy(-ImmortalitySkill->getEnergyCost());
			}
			else
			{
				cSoundController->PlaySoundByID(13);
			}
			break;
		case Skill::BLACKHOLE:
			if ((player->getEnergy() >= BlackholeSkill->getEnergyCost()) && (blackholeTimer <= 0))
			{
				BlackholeSkill->UseSkill();
				spawnBlackhole();
				player->changeEnergy(-BlackholeSkill->getEnergyCost());
				
			}
			else
			{
				cSoundController->PlaySoundByID(13);
			}
			break;
		case Skill::NONE:
			cSoundController->PlaySoundByID(13);
			break;
		}
	}
	else if ((!Application::IsKeyPressed('Q')) && (qbutton))
	{
		qbutton = false;
	}


	
	if (test && test2 < 2) {
	/*	doppelganger = new DoppelgangerAlly();
		doppelganger->Init();
		GameObject* enemy2GO = FetchGO();
		enemy2GO->type = GameObject::GO_DOPPELGANGER;
		enemy2GO->pos = Vector3(m_worldWidth / 2 + 10, m_worldHeight / 2, 0);
		enemy2GO->vel.SetZero();
		enemy2GO->scale.Set(10, 10, 1);
		enemy2GO->color.Set(1, 1, 1);
		enemy2GO->angle = 0;
		doppelganger->SetWeapon(new Sword());
		doppelganger->SetGameObject(enemy2GO);
		m_enemyList.push_back(doppelganger);

		GameObject* ewep2 = FetchGO();
		ewep2->type = GameObject::GO_SWORD;
		ewep2->vel.SetZero();
		ewep2->scale.Set(10, 10, 1);
		ewep2->pos = enemy2GO->pos;
		ewep2->color.Set(1, 1, 1);
		ewep2->angle = 0;
		ewep2->active = true;
		ewep2->leftwep = false;
		doppelganger->GetWeapon()->SetGameObject(ewep2);*/
		blackhole = FetchGO();
		blackhole->type = GameObject::GO_BLACKHOLE;
		blackhole->pos = player->GetGameObject()->pos;
		blackhole->vel.SetZero();
		blackhole->scale.Set(12, 12, 1);
		blackhole->color.Set(1, 1, 1);
		test = false;
	}
	if (Application::IsKeyPressed('O') && blackhole) {
		ReturnGO(blackhole);
		blackhole = nullptr;
	}


	//use health potion
	static bool button1;
	if (Application::IsKeyPressed('1') && !button1)
	{

		cInventoryItem = cInventoryManager->GetItem("healthpotion");
		if (cInventoryItem->GetCount() > 0)
		{
			button1 = true;
			HealthPotion->usePotion();
			cInventoryItem->Remove(1);
		}
	}
	else if (!Application::IsKeyPressed('1') && button1)
	{
		button1 = false;
	}
	//use strength potion
	static bool button2;
	if (Application::IsKeyPressed('2') && !button2)
	{
		cInventoryItem = cInventoryManager->GetItem("strengthpotion");
		if ((cInventoryItem->GetCount() > 0) && (strengthPotTimer >= 30))
		{
			button2 = true;
			StrengthPotion->usePotion();
			cInventoryItem->Remove(1);
			strengthPotTimer = 0;
			strengthPotUsed = true;
		}
	}
	else if (!Application::IsKeyPressed('2') && button2)
	{
		button2 = false;
	}
	if (strengthPotUsed)
		strengthPotTimer += dt;
	if ((strengthPotTimer >= 30) && (strengthPotUsed == true))
	{
		StrengthPotion->potionTimeUp();
		strengthPotUsed = false;
	}
	
	//use speed potion
	static bool button3;
	if (Application::IsKeyPressed('3') && !button3)
	{
		cInventoryItem = cInventoryManager->GetItem("speedpotion");
		if ((cInventoryItem->GetCount() > 0) && (speedPotTimer >= 30))
		{
			button3 = true;
			SpeedPotion->usePotion();
			cInventoryItem->Remove(1);
			speedPotTimer = 0;
			speedPotUsed = true;
		}
	}
	else if (!Application::IsKeyPressed('3') && button3)
	{
		button3 = false;
	}
	speedPotTimer += dt;
	if ((speedPotTimer == 30) && (speedPotUsed == true))
	{
		SpeedPotion->potionTimeUp();
		speedPotUsed = false;
	}

	//update doppelganger when it is spawned
	if (doppelganger) {
		doppelganger->SetEnemyVector(m_enemyList);
		doppelganger->Update(dt);
	}

	
	//update the player
	Vector3 temppos = player->GetGameObject()->pos;
	player->SetEnemyVector(m_enemyList);
	player->Update(dt, mousePos);
	Checkborder(player->getPlayer());
	if (!player->dashing)
		player->GetGameObject()->vel = (player->GetGameObject()->pos - temppos).Normalize() * player->GetMovementSpeed();
	else
	{
		player->GetGameObject()->vel = (player->GetGameObject()->pos - temppos).Normalize() * (player->GetMovementSpeed() - player->GetDashBoost());
	}
	if (player->IsSpawningBullet())
	{
		Application::GetCursorPos(&x, &y);
		Vector3 mousepos = Vector3((x / width) * m_worldWidth, ((height - y) / height) * m_worldHeight, 0);
		if (player->GetWeapon()->WeaponType == Weapon::FLAMETHROWER) //if weapon is a flamethrower
		{
			FlameParticle* flame = new FlameParticle;
			GameObject* flamego = FetchGO();
			flamego->type = GameObject::GO_FLAME;
			Vector3 offset;
			offset.SetZero();
			if (player->GetGameObject()->angle == 180)
				flamego->pos = player->GetWeapon()->GetGameObject()->pos - RotateVector(Vector3(player->GetWeapon()->GetGameObject()->scale.x, -player->GetWeapon()->GetGameObject()->scale.y * 0.5, 0) + Vector3(), Math::DegreeToRadian(player->GetWeapon()->GetGameObject()->angle));
			else
				flamego->pos = player->GetWeapon()->GetGameObject()->pos + RotateVector(Vector3(player->GetWeapon()->GetGameObject()->scale.x, player->GetWeapon()->GetGameObject()->scale.y * 0.5, 0), Math::DegreeToRadian(player->GetWeapon()->GetGameObject()->angle));

			flamego->pos.z = 0;
			flamego->vel.SetZero();
			flamego->scale.Set(7, 7, 1);
			flamego->color.Set(1, 1, 1);
			flamego->angle = 0;
			flame->SetGameObject(flamego);
			flame->SetBullet(player->GetWeapon()->GetBulletSpeed(), player->GetWeapon()->GetDamage(), player->GetWeapon()->GetPiercing(), player->GetWeapon()->GetRange(), (mousepos - player->GetGameObject()->pos).Normalize());
			m_FlameParticle.push_back(flame);
		}
		else if (player->GetWeapon()->WeaponType == Weapon::CROSSBOW) {
			Arrow* arrow = new Arrow;
			GameObject* arrowgo = FetchGO();
			arrowgo->type = GameObject::GO_ARROW;
			arrowgo->pos = player->GetGameObject()->pos;
			arrowgo->vel.SetZero();
			arrowgo->scale.Set(5, 2, 1);
			arrowgo->color.Set(1, 1, 1);
			arrowgo->angle = player->GetWeapon()->GetGameObject()->angle;
			arrow->SetGameObject(arrowgo);
			if (cGameManager->accuratearrowsBought)
			{
				arrow->setCrit(3);
			}
			else
			{
				arrow->setCrit(5);
			}
			arrow->SetArrow(player->GetWeapon()->GetBulletSpeed(), player->GetWeapon()->GetDamage(), player->GetWeapon()->GetPiercing(), player->GetWeapon()->GetRange(), (mousepos - player->GetGameObject()->pos).Normalize());
			m_parrowList.push_back(arrow);
		}
		else
		{
			//spawn bullet for player
			Bullet* bullet = new Bullet;
			GameObject* bulletgo = FetchGO();
			bulletgo->type = GameObject::GO_BULLET;

			if (player->GetGameObject()->angle == 180)
				bulletgo->pos = player->GetWeapon()->GetGameObject()->pos - RotateVector(Vector3(player->GetWeapon()->GetGameObject()->scale.x, -player->GetWeapon()->GetGameObject()->scale.y * 0.5, 0) + Vector3(), Math::DegreeToRadian(player->GetWeapon()->GetGameObject()->angle));
			else
				bulletgo->pos = player->GetWeapon()->GetGameObject()->pos + RotateVector(Vector3(player->GetWeapon()->GetGameObject()->scale.x, player->GetWeapon()->GetGameObject()->scale.y * 0.5, 0), Math::DegreeToRadian(player->GetWeapon()->GetGameObject()->angle));
			bulletgo->vel.SetZero();
			bulletgo->pos.z = 0;
			bulletgo->scale.Set(2, 1, 1);
			bulletgo->color.Set(1, 1, 1);
			bulletgo->angle = player->GetWeapon()->GetGameObject()->angle;
			bullet->SetGameObject(bulletgo);
			if (cGameManager->pierceBought)
			{
				bullet->SetBullet(player->GetWeapon()->GetBulletSpeed(), player->GetWeapon()->GetDamage(), true, player->GetWeapon()->GetRange(), (mousepos - player->GetGameObject()->pos).Normalize(), cGameManager->explosiveBought, cGameManager->bulletExplosionRadius);
			}
			else
			{
				bullet->SetBullet(player->GetWeapon()->GetBulletSpeed(), player->GetWeapon()->GetDamage(), false, player->GetWeapon()->GetRange(), (mousepos - player->GetGameObject()->pos).Normalize(), cGameManager->explosiveBought, cGameManager->bulletExplosionRadius);
			}

			m_pbulletList.push_back(bullet);
		}
	}
	//update enemy
	
	for (unsigned idx = 0; idx < m_enemyList.size(); idx++)
	{
		if (EMPSkill->getStunState() == true) {
			m_enemyList[idx]->makeEnemyStunned();
		}
		else {
			m_enemyList[idx]->resetEnemyStunned();
		}
		Enemy::SetEnemyVector(m_enemyList);
		if (m_enemyList[idx]->Update(dt))
		{
			unsigned particlespeed = 4;
			unsigned particlescale = 2;
			//create the particle effect
			GameObject* particle = FetchGO();
			particle->type = GameObject::GO_DEATH_PARTICLE;
			particle->pos = m_enemyList[idx]->GetGameObject()->pos;
			particle->color.Set(1, 1, 1);
			particle->vel = Vector3(1, 0, 0) * particlespeed;
			particle->scale.Set(particlescale, particlescale, 1);
			particle->time2disappear = 2;

			particle = FetchGO();
			particle->type = GameObject::GO_DEATH_PARTICLE;
			particle->pos = m_enemyList[idx]->GetGameObject()->pos;
			particle->color.Set(1, 1, 1);
			particle->vel = RotateVector(Vector3(0,1,0), -Math::PI * 0.3) * particlespeed;
			particle->scale.Set(particlescale, particlescale, 1);
			particle->time2disappear = 2;

			particle = FetchGO();
			particle->type = GameObject::GO_DEATH_PARTICLE;
			particle->pos = m_enemyList[idx]->GetGameObject()->pos;
			particle->color.Set(1, 1, 1);
			particle->vel = RotateVector(Vector3(0, 1, 0), Math::PI * 0.3) * particlespeed;
			particle->scale.Set(particlescale, particlescale, 1);
			particle->time2disappear = 2;

			particle = FetchGO();
			particle->type = GameObject::GO_DEATH_PARTICLE;
			particle->pos = m_enemyList[idx]->GetGameObject()->pos;
			particle->color.Set(1, 1, 1);
			particle->vel = Vector3(-1, 0, 0) * particlespeed;
			particle->scale.Set(particlescale, particlescale, 1);
			particle->time2disappear = 2;


			//delete the enemy
			ReturnGO(m_enemyList[idx]->GetGameObject());
			if(m_enemyList[idx]->GetWeapon())
				ReturnGO(m_enemyList[idx]->GetWeapon()->GetGameObject());
			player->changeEnergy(m_enemyList[idx]->GetEnergyDrop());
			player->changeMoney(m_enemyList[idx]->GetMoneyDrop());
			if (m_enemyList[idx]->GetGameObject()->type == GameObject::GO_DOPPELGANGER)
				doppelganger = nullptr;
			delete m_enemyList[idx];
			m_enemyList.erase(m_enemyList.begin() + idx);
			enemyLeft--;

			continue;
		}
		//ensure that the enemy does not move out of the map
		Checkborder(m_enemyList[idx]->GetGameObject());
		if (m_enemyList[idx]->IsSpawningBullet())
		{
			Vector3 shootPlayer;
			if (!InterceptionDirection(m_enemyList[idx]->getTarget()->GetGameObject()->pos, m_enemyList[idx]->GetGameObject()->pos, m_enemyList[idx]->getTarget()->GetGameObject()->vel, m_enemyList[idx]->GetWeapon()->GetBulletSpeed(), shootPlayer))
			{
				shootPlayer = (m_enemyList[idx]->getTarget()->GetGameObject()->pos - m_enemyList[idx]->GetGameObject()->pos).Normalize();
			}

			Bullet* bullet = new Bullet;
			GameObject* bulletgo = FetchGO();
			bulletgo->type = GameObject::GO_BULLET;
			if (m_enemyList[idx]->GetGameObject()->angle == 180)
				bulletgo->pos = m_enemyList[idx]->GetWeapon()->GetGameObject()->pos - RotateVector(Vector3(m_enemyList[idx]->GetWeapon()->GetGameObject()->scale.x, -m_enemyList[idx]->GetWeapon()->GetGameObject()->scale.y * 0.5, 0) + Vector3(), Math::DegreeToRadian(m_enemyList[idx]->GetWeapon()->GetGameObject()->angle));
			else
				bulletgo->pos = m_enemyList[idx]->GetWeapon()->GetGameObject()->pos + RotateVector(Vector3(m_enemyList[idx]->GetWeapon()->GetGameObject()->scale.x, m_enemyList[idx]->GetWeapon()->GetGameObject()->scale.y * 0.5, 0), Math::DegreeToRadian(m_enemyList[idx]->GetWeapon()->GetGameObject()->angle));


			bulletgo->pos = m_enemyList[idx]->GetGameObject()->pos;
			bulletgo->pos.z = 1;
			bulletgo->vel.SetZero();
			bulletgo->scale.Set(2, 2, 1);
			bulletgo->color.Set(1, 1, 1);
			bulletgo->angle = m_enemyList[idx]->GetWeapon()->GetGameObject()->angle;
			bullet->SetGameObject(bulletgo);
			bullet->SetBullet(m_enemyList[idx]->GetWeapon()->GetBulletSpeed(), m_enemyList[idx]->GetWeapon()->GetDamage(), m_enemyList[idx]->GetWeapon()->GetPiercing(), m_enemyList[idx]->GetWeapon()->GetRange(), shootPlayer);
			if (m_enemyList[idx]->getTurnedState())
				m_pbulletList.push_back(bullet);
			else
				m_ebulletList.push_back(bullet);
		}
		if (m_enemyList[idx]->IsSpawningSwordsman()) {
			Vector3 thepos[3] = { Vector3(0,5,0), Vector3(5,0,0), Vector3(-5,0,0) };

			for (unsigned i = 0; i < 3; ++i)
			{
				Enemy* enemy;
				GameObject* enemyGO;
				GameObject* ewep;

				enemy = new Swordsman();
				enemy->Init();
				enemy->SetUndead();
				enemyGO = FetchGO();
				enemyGO->type = GameObject::GO_SWORDSMAN;
				enemyGO->pos = m_enemyList[idx]->GetGameObject()->pos + thepos[i];
				enemyGO->vel.SetZero();
				enemyGO->scale.Set(10, 10, 1);
				enemyGO->color.Set(0.8, 0.16, 0.16);
				enemyGO->angle = 0;
				enemy->SetWeapon(new Sword());
				enemy->SetGameObject(enemyGO);
				m_enemyList.push_back(enemy);

				ewep = FetchGO();
				ewep->type = GameObject::GO_SWORD;
				ewep->vel.SetZero();
				ewep->scale.Set(10, 10, 1);
				ewep->color.Set(1, 1, 1);
				ewep->angle = 0;
				ewep->active = true;
				ewep->leftwep = false;
				enemy->GetWeapon()->SetGameObject(ewep);
				++enemyLeft;
			}
		}
		if (blackhole) {
			if (m_enemyList[idx]->GetGameObject()->type != GameObject::GO_DOPPELGANGER) {
				if (m_enemyList[idx]->GetGameObject()->pos.DistanceSquared(blackhole->pos) < 500.0f) {
					Vector3 distance = blackhole->pos - m_enemyList[idx]->GetGameObject()->pos;
					m_enemyList[idx]->GetGameObject()->pos += (distance.Normalized() * 0.5);
				}
			}
		}
		if (HackSkill->getHackingState()) {
			if (m_enemyList[idx]->GetGameObject()->type != GameObject::GO_DOPPELGANGER) {
				if (m_enemyList[idx]->GetGameObject()->pos.DistanceSquared(player->GetGameObject()->pos) < 800.0f) {
					m_enemyList[idx]->turnEnemy();
				}
			}
		}
		if (hackTimer >= 3) {
			for (unsigned idx2 = 0; idx2 < m_enemyList.size(); idx2++) {
				m_enemyList[idx2]->resetEnemyTurned();
			}
			hackTimer = 0;
			enemyHacked = false;
		}
	}
	
	

	//update bullets
	for (unsigned idx = 0; idx < m_pbulletList.size(); idx++)
	{
		if (m_pbulletList[idx]->Update(dt)) //if bullet exceeds the range
		{
			//delete the bullet
			ReturnGO(m_pbulletList[idx]->GetGameObject());
			delete m_pbulletList[idx];
			m_pbulletList.erase(m_pbulletList.begin() + idx);
			continue;
		}
		//check if the bullet has exited the screen
		if (m_pbulletList[idx]->GetGameObject()->pos.x > m_worldWidth || m_pbulletList[idx]->GetGameObject()->pos.x < 0 || m_pbulletList[idx]->GetGameObject()->pos.y > m_worldHeight || m_pbulletList[idx]->GetGameObject()->pos.y < 0)
		{
			//delete the bullet
			ReturnGO(m_pbulletList[idx]->GetGameObject());
			delete m_pbulletList[idx];
			m_pbulletList.erase(m_pbulletList.begin() + idx);
			continue;
		}
		//check collision
		for (unsigned idx1 = 0; idx1 < m_enemyList.size(); idx1++)
		{
			//for shield enemy
			if (m_enemyList[idx1]->GetEnemyType() == Enemy::SHIELDMAN)
			{
				//check if the attack is blocked
				if (Entity::CheckShieldCollision(m_pbulletList[idx], m_enemyList[idx1]))
				{
					//delete the bullet
					ReturnGO(m_pbulletList[idx]->GetGameObject());
					delete m_pbulletList[idx];
					m_pbulletList.erase(m_pbulletList.begin() + idx);
					break;
				}
			}
			if (CheckCollision(m_pbulletList[idx]->GetGameObject(), m_enemyList[idx1]->GetGameObject()))
			{
				if (m_enemyList[idx1]->ChangeHealth(-m_pbulletList[idx]->GetDamage() - player->rangeDmgBoost))
				{
					//for explosive bullets
					if (m_pbulletList[idx]->isExplosive)
					{
						for (unsigned idx2 = 0; idx2 < m_enemyList.size(); idx2++)
						{
							//check distance of the bullet to the enemy
							if ((m_enemyList[idx2]->GetGameObject()->pos - m_pbulletList[idx]->GetGameObject()->pos).LengthSquared() <= m_pbulletList[idx]->explosionRadius * m_pbulletList[idx]->explosionRadius)
							{
								//deal damage to the enemy
								m_enemyList[idx2]->ChangeHealth(-m_pbulletList[idx]->GetDamage() - player->rangeDmgBoost);
							}
						}
					}
				}
				if (!m_pbulletList[idx]->GetPenetrationValue())
				{
					//delete the bullet
					ReturnGO(m_pbulletList[idx]->GetGameObject());
					delete m_pbulletList[idx];
					m_pbulletList.erase(m_pbulletList.begin() + idx);
					break;
				}
			}
		}
	}
	for (unsigned idx = 0; idx < m_ebulletList.size(); idx++)
	{
		if (m_ebulletList[idx]->Update(dt))
		{
			//delete the bullet
			ReturnGO(m_ebulletList[idx]->GetGameObject());
			delete m_ebulletList[idx];
			m_ebulletList.erase(m_ebulletList.begin() + idx);
			continue;
		}
		if (m_ebulletList[idx]->GetGameObject()->pos.x > m_worldWidth || m_ebulletList[idx]->GetGameObject()->pos.x < 0 || m_ebulletList[idx]->GetGameObject()->pos.y > m_worldHeight || m_ebulletList[idx]->GetGameObject()->pos.y < 0)
		{
			//delete the bullet
			ReturnGO(m_ebulletList[idx]->GetGameObject());
			delete m_ebulletList[idx];
			m_ebulletList.erase(m_ebulletList.begin() + idx);
			continue;
		}
		//check collision
		if (CheckCollision(m_ebulletList[idx]->GetGameObject(), player->GetGameObject())) {
			if (cGameManager->isImmortal == true) {
				player->ChangeHealth(m_ebulletList[idx]->GetDamage());
			}
			else {
				if (!player->iFrame)
					player->ChangeHealth(-m_ebulletList[idx]->GetDamage());
			}
			if (!m_ebulletList[idx]->GetPenetrationValue()) {
				ReturnGO(m_ebulletList[idx]->GetGameObject());
				delete m_ebulletList[idx];
				m_ebulletList.erase(m_ebulletList.begin() + idx);

			}
		}
		else if (doppelganger) {
			if (CheckCollision(m_ebulletList[idx]->GetGameObject(), doppelganger->GetGameObject())) {
			doppelganger->ChangeHealth(-m_ebulletList[idx]->GetDamage());
				if (!m_ebulletList[idx]->GetPenetrationValue()) {
					ReturnGO(m_ebulletList[idx]->GetGameObject());
					delete m_ebulletList[idx];
					m_ebulletList.erase(m_ebulletList.begin() + idx);
				}
			}
		}
	}
	//update the flame particles
	for (unsigned idx = 0; idx < m_FlameParticle.size(); idx++)
	{
		if (m_FlameParticle[idx]->Update(dt))
		{
			//delete the bullet
			ReturnGO(m_FlameParticle[idx]->GetGameObject());
			delete m_FlameParticle[idx];
			m_FlameParticle.erase(m_FlameParticle.begin() + idx);
			continue;
		}
		bool hit = false, deleted = false;
		for (unsigned idx1 = 0; idx1 < m_enemyList.size(); idx1++)
		{
			//for shield enemy
			if (m_enemyList[idx1]->GetEnemyType() == Enemy::SHIELDMAN)
			{
				//check if the attack is blocked
				if (Entity::CheckShieldCollision(m_FlameParticle[idx], m_enemyList[idx1]))
				{
					//delete the bullet
					ReturnGO(m_FlameParticle[idx]->GetGameObject());
					delete m_FlameParticle[idx];
					m_FlameParticle.erase(m_FlameParticle.begin() + idx);
					deleted = true;
					break;
				}
			}
			if (CheckCollision(m_FlameParticle[idx]->GetGameObject(), m_enemyList[idx1]->GetGameObject()))
			{
				m_enemyList[idx1]->ChangeHealth(-m_FlameParticle[idx]->GetDamage());
				hit = true;
				if (!m_FlameParticle[idx]->GetPenetrationValue())
				{
					//delete the bullet
					ReturnGO(m_FlameParticle[idx]->GetGameObject());
					delete m_FlameParticle[idx];
					m_FlameParticle.erase(m_FlameParticle.begin() + idx);
					deleted = true;
					break;
				}
			}
		}
		//reduce range of the flame particles
		if (deleted == false)
		{
			if (hit && m_FlameParticle[idx])
			{
				m_FlameParticle[idx]->HalfRange();
			}
		}
	}


	for (unsigned idx = 0; idx < m_parrowList.size(); idx++) {
		m_parrowList[idx]->Update(dt);
		if (m_parrowList[idx]->GetGameObject()->pos.x > m_worldWidth || m_parrowList[idx]->GetGameObject()->pos.x < 0 || m_parrowList[idx]->GetGameObject()->pos.y > m_worldHeight || m_parrowList[idx]->GetGameObject()->pos.y < 0)
		{
			//delete the arrow
			ReturnGO(m_parrowList[idx]->GetGameObject());
			delete m_parrowList[idx];
			m_parrowList.erase(m_parrowList.begin() + idx);
			continue;
		}
		for (unsigned idx1 = 0; idx1 < m_enemyList.size(); idx1++)
		{
			if (CheckCollision(m_parrowList[idx]->GetGameObject(), m_enemyList[idx1]->GetGameObject()))
			{
				if (m_parrowList[idx]->getCrit() == 1) {
					arrowCrit = true;
					m_enemyList[idx1]->ChangeHealth(-m_parrowList[idx]->GetDamage() * 20);

				}
				else {
					arrowCrit = false;
					m_enemyList[idx1]->ChangeHealth(-m_parrowList[idx]->GetDamage());

				}
				if (!m_parrowList[idx]->GetPenetrationValue())
				{
					//delete the bullet
					ReturnGO(m_parrowList[idx]->GetGameObject());
					delete m_parrowList[idx];
					m_parrowList.erase(m_parrowList.begin() + idx);
					break;
				}
			}
		}
	}


	static bool bLButtonState = false;
	if(!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		
	}
	else if(bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
	}
	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
	}
	else if(bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
	}

	//Update the particles
	for (unsigned idx = 0; idx < m_goList.size(); idx++)
	{
		if (!m_goList[idx]->active)
			continue;
		
		if (m_goList[idx]->type != GameObject::GO_DEATH_PARTICLE)
			continue;

		m_goList[idx]->pos += m_goList[idx]->vel * dt;
		m_goList[idx]->time2disappear -= dt;
		if (m_goList[idx]->time2disappear <= 0)
		{
			ReturnGO(m_goList[idx]);
		}
	}
}

bool SceneCollision::CheckCollision(GameObject* go1, GameObject* go2)
{
	// Prevent non ball vs non ball code
	if (!(go1->type == GameObject::GO_BALL || go1->type == GameObject::GO_BULLET || go1->type == GameObject::GO_FLAME || go1->type == GameObject::GO_ARROW))
	{
		return false;
	}

	switch (go2->type)
	{
	case GameObject::GO_PILLAR:
	case GameObject::GO_BALL:
	case GameObject::GO_BULLET:
	case GameObject::GO_ARROW:
	case GameObject::GO_FLAME:
	{
		Vector3 relativeVel = go1->vel - go2->vel;
		Vector3 disDiff = go2->pos - go1->pos;

		if (relativeVel.Dot(disDiff) <= 0)
			return false;
		return disDiff.LengthSquared() <= (go1->scale.x + go2->scale.x) * (go1->scale.x + go2->scale.x);
	}
	case GameObject::GO_NECROMANCER:
	case GameObject::GO_DOPPELGANGER:
	case GameObject::GO_PLAYER:
	case GameObject::GO_SWORDSMAN:
	case GameObject::GO_RIFLER:
	case GameObject::GO_SHIELDMAN:
	{
		Vector3 disDiff = go2->pos - go1->pos;

		return disDiff.LengthSquared() <= (go1->scale.x + go2->scale.x) * (go1->scale.x + go2->scale.x) * 0.4f;
	}

	case GameObject::GO_WALL:
	{
		Vector3 diff = go1->pos - go2->pos;
		Vector3 axisX = go2->normal;
		Vector3 axisY = Vector3(-go2->normal.y, go2->normal.x, 0);
		float projectedDist = diff.Dot(axisX);

		if (projectedDist > 0)
			axisX = -axisX;

		return go1->vel.Dot(axisX) >= 0 && // Check 1: Travelling towards the wall?
			go2->scale.x * 0.5 + go1->scale.x > -diff.Dot(axisX) &&// Check 2: Radius + Thickness vs Distance
			go2->scale.y * 0.5 > fabs(diff.Dot(axisY)); // Check 3: Length Check
	}
	}
	return false;
}

void SceneCollision::RenderGO(GameObject *go)
{
	switch(go->type)
	{
	case GameObject::GO_PILLAR:
	case GameObject::GO_BALL:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		meshList[GEO_BALL]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
		RenderMesh(meshList[GEO_BALL], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_DEATH_PARTICLE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		meshList[GEO_PARTICLE]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
		RenderMesh(meshList[GEO_PARTICLE], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_PLAYER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		modelStack.Rotate(player->rotate, 0, 0, 1);
		if (player->dashing)
		{
			if (go->angle == 180)
			{
				meshList[GEO_LEFT_DASH]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
				RenderMesh(meshList[GEO_LEFT_DASH], true);
			}

			else if (go->angle == 0)
			{
				meshList[GEO_RIGHT_DASH]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
				RenderMesh(meshList[GEO_RIGHT_DASH], true);
			}
		}
		else
		{
			if (go->angle == 180)
			{
				meshList[GEO_LEFT_PLAYER]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
				RenderMesh(meshList[GEO_LEFT_PLAYER], true);
			}

			else if (go->angle == 0)
			{
				meshList[GEO_RIGHT_PLAYER]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
				RenderMesh(meshList[GEO_RIGHT_PLAYER], true);
			}
		}
		modelStack.PopMatrix();
		break;
	case GameObject::GO_SWORDSMAN:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		if (go->angle == 180)
		{
			meshList[GEO_LEFT_SWORDSMAN]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_LEFT_SWORDSMAN], true);
		}

		else if (go->angle == 0)
		{
			meshList[GEO_RIGHT_SWORDSMAN]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_RIGHT_SWORDSMAN], true);
		}
		modelStack.PopMatrix();
		break;
	case GameObject::GO_RIFLER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		if (go->angle == 180)
		{
			meshList[GEO_LEFT_RIFLER]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_LEFT_RIFLER], true);
		}

		else if (go->angle == 0)
		{
			meshList[GEO_RIGHT_RIFLER]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_RIGHT_RIFLER], true);
		}
		modelStack.PopMatrix();
		break;
	case GameObject::GO_SHIELDMAN:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		if (go->angle == 180)
		{
			meshList[GEO_LEFT_SHIELDMAN]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_LEFT_SHIELDMAN], true);
		}

		else if (go->angle == 0)
		{
			meshList[GEO_RIGHT_SHIELDMAN]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_RIGHT_SHIELDMAN], true);
		}
		modelStack.PopMatrix();
		break;
	case GameObject::GO_SHIELD:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(go->angle, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		meshList[GEO_SHIELD]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
		RenderMesh(meshList[GEO_SHIELD], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_SWORD:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(go->angle, 0, 0, 1);

		if (go->leftwep == false)
		{
			modelStack.Translate(go->scale.x * 0.3, go->scale.y * 0.3, 0);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			meshList[GEO_SWORDR]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			meshList[GEO_SWORDR]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_SWORDR], true);
		}

		else
		{
			modelStack.Translate(-go->scale.x * 0.3, go->scale.y * 0.3, 0);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			meshList[GEO_SWORDL]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			meshList[GEO_SWORDL]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_SWORDL], true);
		}
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BOXINGGLOVES:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(go->angle, 0, 0, 1);

		if (go->leftwep == false)
		{
			modelStack.Translate(go->scale.x * 0.3, go->scale.y * 0.3, 0);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			meshList[GEO_BOXINGGLOVE_RIGHT]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			meshList[GEO_BOXINGGLOVE_RIGHT]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_BOXINGGLOVE_RIGHT], true);
		}

		else
		{
			modelStack.Translate(-go->scale.x * 0.3, go->scale.y * 0.3, 0);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			meshList[GEO_BOXINGGLOVE_LEFT]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			meshList[GEO_BOXINGGLOVE_LEFT]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_BOXINGGLOVE_LEFT], true);
		}
		modelStack.PopMatrix();
		break;

	case GameObject::GO_RIFLE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(go->angle, 0, 0, 1);

		if (go->leftwep == false)
		{
			modelStack.Translate(go->scale.x * 0.3, go->scale.y * 0.3, 0);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			meshList[GEO_RIFLE_RIGHT]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			meshList[GEO_RIFLE_RIGHT]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_RIFLE_RIGHT], true);
		}

		else
		{
			modelStack.Translate(-go->scale.x * 0.3, go->scale.y * 0.3, 0);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			meshList[GEO_RIFLE_LEFT]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			meshList[GEO_RIFLE_LEFT]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_RIFLE_LEFT], true);
		}
		modelStack.PopMatrix();
		break;
	case GameObject::GO_CROSSBOW:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(go->angle, 0, 0, 1);

		if (go->leftwep == false)
		{
			modelStack.Translate(go->scale.x * 0.3, go->scale.y * 0.6, 0);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			meshList[GEO_CROSSBOW_RIGHT]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			meshList[GEO_CROSSBOW_RIGHT]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_CROSSBOW_RIGHT], true);
		}

		else
		{
			modelStack.Translate(-go->scale.x * 0.3, go->scale.y * 0.6, 0);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			meshList[GEO_CROSSBOW_LEFT]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			meshList[GEO_CROSSBOW_LEFT]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_CROSSBOW_LEFT], true);
		}
		modelStack.PopMatrix();
		break;
	case GameObject::GO_FLAMETHROWER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(go->angle, 0, 0, 1);

		if (go->leftwep == false)
		{
			modelStack.Translate(go->scale.x * 0.3, go->scale.y * 0.3, 0);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			meshList[GEO_FLAMETHROWER]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			meshList[GEO_FLAMETHROWER]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_FLAMETHROWER], true);
		}

		else
		{
			modelStack.Translate(-go->scale.x * 0.3, go->scale.y * 0.3, 0);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			meshList[GEO_FLAMETHROWER_LEFT]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			meshList[GEO_FLAMETHROWER_LEFT]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_FLAMETHROWER_LEFT], true);
		}
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BULLET:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(go->angle, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		meshList[GEO_BULLET]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
		RenderMesh(meshList[GEO_BULLET], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ARROW:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(go->angle, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		if (arrowCrit == false) {
			meshList[GEO_ARROWRED]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_ARROWRED], true);
		}
		else {
			meshList[GEO_ARROWBLUE]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_ARROWBLUE], true);
		}
		modelStack.PopMatrix();
		break;

	case GameObject::GO_FLAME:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(go->angle, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		meshList[GEO_FLAME]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
		RenderMesh(meshList[GEO_FLAME], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_WALL:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2f(go->normal.y, go->normal.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		meshList[GEO_CUBE]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
		RenderMesh(meshList[GEO_CUBE], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_DOPPELGANGER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		if (go->angle == 180)
		{
			meshList[GEO_LEFT_PLAYER]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_LEFT_PLAYER], true);
		}

		else if (go->angle == 0)
		{
			meshList[GEO_RIGHT_PLAYER]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_RIGHT_PLAYER], true);
		}
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BLACKHOLE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(go->angle, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		meshList[GEO_BLACKHOLE]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
		RenderMesh(meshList[GEO_BLACKHOLE], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_NECROMANCER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		if (go->angle == 180)
		{
			meshList[GEO_LEFT_NECROMANCER]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_LEFT_NECROMANCER], true);
		}

		else if (go->angle == 0)
		{
			meshList[GEO_RIGHT_NECROMANCER]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_RIGHT_NECROMANCER], true);
		}
		modelStack.PopMatrix();
		break;
	}

}

void SceneCollision::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	//render the sand bg
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, 0);
	modelStack.Scale(360,240,1);
	float reduce_red = 0;
	float reduce = 0;
	if (player->GetHealth() <= player->GetMaxHealth() * 0.6)
	{
		reduce_red = player->GetMaxHealth() - player->GetHealth();
		reduce_red /= player->GetMaxHealth();
		reduce = reduce_red;
		reduce_red /= 8;
		reduce /= 2;
	}

	meshList[GEO_SANDBG]->material.kAmbient.Set(1 - reduce_red, 1 - reduce, 1 - reduce);
	RenderMesh(meshList[GEO_SANDBG], true);
	modelStack.PopMatrix();


	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active && go->visible)
		{
			RenderGO(go);
		}
	}

	//render the Health bar of the enemies
	if (cGameManager->showHealthBar)
	{
		float ratiox, ratioy;
		ratiox = 80 / m_worldWidth;
		ratioy = 60 / m_worldHeight;
		for (unsigned idx = 0; idx < m_enemyList.size(); idx++)
		{
			RenderMeshOnScreen(meshList[GEO_HEALTH_UI_RED], m_enemyList[idx]->GetGameObject()->pos.x * ratiox, (m_enemyList[idx]->GetGameObject()->pos.y + 6.5) * ratioy, 5, 1.25);
			if (m_enemyList[idx]->GetHealth() <= m_enemyList[idx]->GetMaxHealth())
				RenderMeshOnScreen(meshList[GEO_HEALTH_UI_GREEN], m_enemyList[idx]->GetGameObject()->pos.x * ratiox, (m_enemyList[idx]->GetGameObject()->pos.y + 6.5) * ratioy, 5.0f * ((double)m_enemyList[idx]->GetHealth() / (double)m_enemyList[idx]->GetMaxHealth()), 1.25);
		}
	}

	RenderWall();
	renderUI();

	//On screen text
	std::ostringstream ss;
	
	if (cGameManager->bDebug)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Object Count:" + std::to_string(m_objectCount), Color(1, 1, 1), 3, 0, 9);

		ss.precision(3);
		ss.str("");
		ss << "Speed: " << m_speed;
		ss << " FPS: " << fps;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 6);
	
	}

	RenderTextOnScreen(meshList[GEO_TEXT], "Wave:" + std::to_string(cGameManager->dWaveNo), Color(1, 1, 1), 3, 37, 57);
	RenderTextOnScreen(meshList[GEO_TEXT], "Enemies Left:" + std::to_string(enemyLeft), Color(1, 1, 1), 3, 0.5, 49);
	if (cGameManager->waveClear && timer < 3)
	{
		if (NearShop())
		{
			ss.str("");
			ss << "Press [E] to go to shop";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.2, 0.2, 0.2), 3, 30, 8);
		}

		ss.str("");
		ss << "Wave Cleared!";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 6, 30, 30);
		ss.str("");
		ss << "Enter the top door to go to shop";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 5, 20, 24);
	}

	if (timer < 0)
	{
		ss.str("");
		ss.precision(1);
		int countdown = timer * -1 + 1;
		ss << countdown;
		if (timer > -1)
		{
			ss.str("");
			ss << "GAME START!";
		}
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 6, 30, 30);
	}

	if (cGameManager->bPlayerLost)
	{
		ss.str("");
		ss << "You Died, Highest Wave: " << cGameManager->highestWave;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 4, 20, 24);
		ss.str("");
		ss << "Press 'R' to restart";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 4, 20, 20);
		ss.str("");
		ss << "Press '~' to return to main menu";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 4, 20, 16);
	}
}

void SceneCollision::renderUI()
{
	//On screen text
	std::ostringstream ss;
	std::ostringstream ss2;

	//render the player health
	ss.str("");
	ss << "Health:";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0.5, 57);
	
	RenderMeshOnScreen(meshList[GEO_HEALTH_UI_BASE], 12, 58.75, 10, 2);
	RenderMeshOnScreen(meshList[GEO_HEALTH_UI_RED], 7 + (double)player->GetHealth() / (double)player->GetMaxHealth() * 5.0f, 58.75, (double)player->GetHealth() / (double)player->GetMaxHealth() * 10.0f, 2);

	ss.str("");
	ss << player->GetHealth();
	ss2.str("");
	ss2 << player->GetMaxHealth();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str() + "/" + ss2.str(), Color(0, 0, 0), 2.5, 9, 57.5);

	//render money
	modelStack.PushMatrix();
	modelStack.Translate(169, 97, 1);
	modelStack.Scale(14, 5, 1);
	RenderMesh(meshList[GEO_SHOPMENUBG], false);
	modelStack.PopMatrix();

	ss.str("");
	ss << "$";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.1, 1, 0.1), 3, 73, 56.7);

	ss.str("");
	ss << player->getMoney();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 74.5, 56.7);

	ss.str("");
	ss << "[`] Pause";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 72.5, 53.7);

	//render energy
	ss.str("");
	ss << "Energy:";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0.5, 53);

	ss.str("");
	ss << player->getEnergy();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 8, 52.8);


	if (player->GetHealth() <= player->GetMaxHealth() * 0.3)
	{
		if (timer > 0.5)
		{
			ss.str("");
			ss << "Low HP!";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.9, 0, 0.1), 3, 0.5, 49);
		}
	}

	

	Vector3 wep1 = Vector3(16, 7, 1);
	Vector3 wep2 = Vector3(26, 7, 1);
	Vector3 skillVec = Vector3(36, 7, 1);
	Vector3 scale = Vector3(10, 10, 1);
	Vector3 scale2 = Vector3(7, 7, 1);
	// render hotbar
	modelStack.PushMatrix();
	modelStack.Translate(wep1.x, wep1.y, wep1.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderMesh(meshList[GEO_HOTBAR_SELECTED], false);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(wep2.x, wep2.y, wep2.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderMesh(meshList[GEO_HOTBAR], false);
	modelStack.PopMatrix();

	if (player->GetWeapon() != nullptr)
		renderWeaponUI(wep1, scale, player->GetWeapon()->GetGameObject());
	if (player->GetSideWeapon() != nullptr)
		renderWeaponUI(wep2, scale, player->GetSideWeapon()->GetGameObject());

	//render skill UI
	modelStack.PushMatrix();
	modelStack.Translate(skillVec.x, skillVec.y, skillVec.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderMesh(meshList[GEO_HOTBAR], false);
	modelStack.PopMatrix();

	if (cGameManager->skilltype != 0)
		renderSkillUI(skillVec, scale2, cGameManager->skilltype);

	ss.str("");
	ss << "[Q]";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 14, 5);


	//potions
	modelStack.PushMatrix();
	modelStack.Translate(140, wep1.y, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderMesh(meshList[GEO_HOTBAR], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(150, wep1.y, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderMesh(meshList[GEO_HOTBAR], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(160, wep1.y, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderMesh(meshList[GEO_HOTBAR], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(140, wep1.y, 1);
	modelStack.Scale(5, 5, 1);
	RenderMesh(meshList[GEO_HEALTHPOT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(150, wep1.y, 1);
	modelStack.Scale(5, 5, 1);
	RenderMesh(meshList[GEO_STRENGTHPOT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(160, wep1.y, 1);
	modelStack.Scale(5, 5, 1);
	RenderMesh(meshList[GEO_SPEEDPOT], false);
	modelStack.PopMatrix();

	ss.str("");
	ss << "[1]";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 61, 5);

	ss.str("");
	ss << "[2]";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 65.5, 5);

	ss.str("");
	ss << "[3]";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 70, 5);

	ss.str("");
	cInventoryItem = cInventoryManager->GetItem("healthpotion");
	ss << cInventoryItem->GetCount();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 2, 62.7, 0.9);

	ss.str("");
	cInventoryItem = cInventoryManager->GetItem("strengthpotion");
	ss << cInventoryItem->GetCount();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 2, 67.2, 0.9);

	ss.str("");
	cInventoryItem = cInventoryManager->GetItem("speedpotion");
	ss << cInventoryItem->GetCount();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 2, 71.7, 0.9);
}
void SceneCollision::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	while (m_enemyList.size() > 0)
	{
		Entity* go = m_enemyList.back();
		delete go;
		m_enemyList.pop_back();
	}
	while (m_pbulletList.size() > 0)
	{
		Bullet* go = m_pbulletList.back();
		delete go;
		m_pbulletList.pop_back();
	}
	while (m_ebulletList.size() > 0)
	{
		Bullet* go = m_ebulletList.back();
		delete go;
		m_ebulletList.pop_back();
	}
	while (m_parrowList.size() > 0)
	{
		Arrow* go = m_parrowList.back();
		delete go;
		m_parrowList.pop_back();
	}
	while (m_FlameParticle.size() > 0)
	{
		FlameParticle* go = m_FlameParticle.back();
		delete go;
		m_FlameParticle.pop_back();
	}

	while (m_parrowList.size() > 0)
	{
		Arrow* go = m_parrowList.back();
		delete go;
		m_parrowList.pop_back();
	}

	if (player)
	{
		delete player;
		player = nullptr;
	}
	if (cGameManager)
	{
		delete cGameManager;
		cGameManager = nullptr;
	}

	if (HackSkill)
	{
		delete HackSkill;
		HackSkill = nullptr;
	}
	if (HealSkill)
	{
		delete HealSkill;
		HealSkill = nullptr;
	}
	if (StrengthPotion)
	{
		delete StrengthPotion;
		StrengthPotion = nullptr;
	}
	if (HealthPotion)
	{
		delete HealthPotion;
		HealthPotion = nullptr;
	}
	if (PierceMod)
	{
		delete PierceMod;
		PierceMod = nullptr;
	}
}

GameObject* SceneCollision::Checkborder(GameObject* go)
{
	float offset = 7.5;
	if (go->pos.x + go->scale.x / 2 > m_worldWidth - offset)
	{
		go->pos.x = m_worldWidth - go->scale.x / 2 - offset;
	}	
	
	if (go->pos.x - go->scale.x / 2 < 0 + offset)
	{
		go->pos.x = 0 + go->scale.x / 2 + offset;
	}	
	
	if (go->pos.y + go->scale.y / 2 > m_worldHeight - offset)
	{
		go->pos.y = m_worldHeight - go->scale.y / 2 - offset;
	}	
	
	if (go->pos.y - go->scale.y / 2 < 0 + offset)
	{
		go->pos.y = 0 + go->scale.y / 2 + offset;
	}

	return go;
}

void SceneCollision::RenderWall()
{
	if (cGameManager->waveClear)
	{
		//top left
		modelStack.PushMatrix();
		modelStack.Translate(30, m_worldHeight - 3, 1);
		modelStack.Scale(m_worldWidth / 2 - 10, 6, 1);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();

		//top right
		modelStack.PushMatrix();
		modelStack.Translate(140, m_worldHeight - 3, 1);
		modelStack.Scale(m_worldWidth / 2 - 10, 6, 1);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();

		//bottom
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth/2, 3, 1);
		modelStack.Scale(m_worldWidth, 6, 1);
		meshList[GEO_CUBE]->material.kAmbient.Set(1, 0, 0);
		RenderMesh(meshList[GEO_CUBE], true);
		modelStack.PopMatrix();

		//left
		modelStack.PushMatrix();
		modelStack.Translate(3, m_worldHeight/2, 1);
		modelStack.Scale(6, m_worldHeight, 1);
		RenderMesh(meshList[GEO_CUBE], true);
		modelStack.PopMatrix();

		//right
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth - 3, m_worldHeight / 2, 1);
		modelStack.Scale(6, m_worldHeight, 1);
		RenderMesh(meshList[GEO_CUBE], true);
		modelStack.PopMatrix();
	}

	else
	{
		//top
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth/2, m_worldHeight - 3, 1);
		modelStack.Scale(m_worldWidth, 6, 1);
		meshList[GEO_CUBE]->material.kAmbient.Set(1, 0, 0);
		RenderMesh(meshList[GEO_CUBE], true);
		modelStack.PopMatrix();


		//bottom left
		modelStack.PushMatrix();
		modelStack.Translate(35, 3, 1);
		modelStack.Scale(m_worldWidth / 2 - 10, 6, 1);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();

		//bottom right
		modelStack.PushMatrix();
		modelStack.Translate(145, 3, 1);
		modelStack.Scale(m_worldWidth / 2 - 10, 6, 1);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();


		//right top
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth - 3, 85, 1);
		modelStack.Scale(6, m_worldHeight / 2 - 15, 1);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();

		//right bottom
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth - 3, 20, 1);
		modelStack.Scale(6, m_worldHeight / 2 - 15, 1);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();


		//left top
		modelStack.PushMatrix();
		modelStack.Translate(3, 85, 1);
		modelStack.Scale(6, m_worldHeight / 2 - 15, 1);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();

		//left bottom
		modelStack.PushMatrix();
		modelStack.Translate(3, 20, 1);
		modelStack.Scale(6, m_worldHeight / 2 - 15, 1);
		RenderMesh(meshList[GEO_CUBE], false);
		modelStack.PopMatrix();

		// Teleport pad left
		modelStack.PushMatrix();
		modelStack.Translate(4, m_worldHeight/2, 1);
		modelStack.Scale(8, 8, 1);
		meshList[GEO_TELEPORT_PAD]->material.kAmbient.Set(color[0].x, color[0].y, color[0].z);
		RenderMesh(meshList[GEO_TELEPORT_PAD], true);
		modelStack.PopMatrix();

		// Teleport pad bottom
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth / 2, 4, 1);
		modelStack.Scale(8, 8, 1);
		meshList[GEO_TELEPORT_PAD]->material.kAmbient.Set(color[1].x, color[1].y, color[1].z);
		RenderMesh(meshList[GEO_TELEPORT_PAD], true);
		modelStack.PopMatrix();

		// Teleport pad right
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth - 4, m_worldHeight / 2, 1);
		modelStack.Scale(8, 8, 1);
		meshList[GEO_TELEPORT_PAD]->material.kAmbient.Set(color[2].x, color[2].y, color[2].z);
		RenderMesh(meshList[GEO_TELEPORT_PAD], true);
		modelStack.PopMatrix();
	}
}

void SceneCollision::SpawnEnemy(float rate)
{
	static bool loc = false;
	static int location;
	static Vector3 pos;

	if (totalEnemy <= 0)
	{
		for (unsigned i = 0; i < colorsize; ++i)
		{
			color[i].Set(1, 1, 1);
		}
	}

	else if (!loc && timer > rate/3)
	{
		location = Math::RandIntMinMax(1, 3);
		switch (location)
		{
			float x, y;
		case 1:
			x = 10;
			y = m_worldHeight / 2;
			pos.Set(x, y, 1);
			break;
		case 2:
			x = m_worldWidth / 2;
			y = 4;
			pos.Set(x, y, 1);
			break;
		case 3:
			x = m_worldWidth - 10;
			y = m_worldHeight / 2;
			pos.Set(x, y, 1);
			break;
		}

		for (unsigned i = 0; i < colorsize; ++i)
		{
			color[i].Set(1, 1, 1);
		}
		color[location - 1].Set(1, 0, 1);

		loc = true;
	}

	else if (timer > rate)
	{
		int type;
		if (cGameManager->dWaveNo < 4)
			type = 1;

		else if (cGameManager->dWaveNo < 7)
		{
			int ran = Math::RandIntMinMax(1, 10);
			if (ran < 8)
				type = 1;
			else
				type = 3;
		}
		
		else if (cGameManager->dWaveNo < 12)
		{
			int ran = Math::RandIntMinMax(1, 10);
			if (ran < 6)
				type = 1;
			else if (ran < 8)
				type = 2;
			else
				type = 3;
		}

		else if (cGameManager->dWaveNo < 20)
		{
			int ran = Math::RandIntMinMax(1, 10);
			if (ran < 4)
				type = 1;
			else if (ran < 6)
				type = 2;
			else if (ran < 10)
				type = 3;
			else
				type = 4;
		}

		else
		{
			type = Math::RandIntMinMax(1, 4);
		}
		Enemy* enemy;
		GameObject* enemyGO;
		GameObject* ewep;
		switch (type)
		{
		case 1:
			enemy = new Swordsman();
			enemy->Init();
			enemyGO = FetchGO();
			enemyGO->type = GameObject::GO_SWORDSMAN;
			enemyGO->pos = pos;
			enemyGO->vel.SetZero();
			enemyGO->scale.Set(10, 10, 1);
			enemyGO->color.Set(1, 1, 1);
			enemyGO->angle = 0;
			enemy->SetWeapon(new Sword());
			enemy->SetGameObject(enemyGO);
			m_enemyList.push_back(enemy);

			ewep = FetchGO();
			ewep->type = GameObject::GO_SWORD;
			ewep->vel.SetZero();
			ewep->scale.Set(10, 10, 1);
			ewep->color.Set(1, 1, 1);
			ewep->angle = 0;
			ewep->active = true;
			ewep->leftwep = false;
			enemy->GetWeapon()->SetGameObject(ewep);

			break;
		case 2:
			enemy = new Rifler();
			enemy->Init();
			enemyGO = FetchGO();
			enemyGO->type = GameObject::GO_RIFLER;
			enemyGO->pos = pos;
			enemyGO->vel.SetZero();
			enemyGO->scale.Set(10, 10, 1);
			enemyGO->color.Set(1, 1, 1);
			enemyGO->angle = 0;
			enemy->SetWeapon(new Rifle());
			enemy->SetGameObject(enemyGO);
			m_enemyList.push_back(enemy);

			ewep = FetchGO();
			ewep->type = GameObject::GO_RIFLE;
			ewep->vel.SetZero();
			ewep->scale.Set(8, 3, 1);
			ewep->color.Set(1, 1, 1);
			ewep->angle = 0;
			ewep->active = true;
			ewep->leftwep = false;
			enemy->GetWeapon()->SetGameObject(ewep);
			break;
		case 3:
			enemy = new ShieldEnemy();
			enemy->Init();
			enemyGO = FetchGO();
			enemyGO->type = GameObject::GO_SHIELDMAN;
			enemyGO->pos = pos;
			enemyGO->vel.SetZero();
			enemyGO->scale.Set(10, 10, 1);
			enemyGO->color.Set(1, 1, 1);
			enemyGO->angle = 0;
			enemy->SetWeapon(new Shield());
			enemy->SetGameObject(enemyGO);
			m_enemyList.push_back(enemy);

			ewep = FetchGO();
			ewep->type = GameObject::GO_SHIELD;
			ewep->vel.SetZero();
			ewep->scale.Set(10, 10, 1);
			ewep->color.Set(1, 1, 1);
			ewep->angle = 0;
			ewep->active = true;
			ewep->leftwep = false;
			enemy->GetWeapon()->SetGameObject(ewep);
			break;
		case 4:
			enemy = new Necromancer();
			enemy->Init();
			enemyGO = FetchGO();
			enemyGO->type = GameObject::GO_NECROMANCER;
			enemyGO->pos = (m_worldWidth / 2, m_worldHeight / 2, 1);
			enemyGO->vel.SetZero();
			enemyGO->scale.Set(13, 10, 1);
			enemyGO->color.Set(1, 1, 1);
			enemyGO->angle = 0;
			enemy->SetGameObject(enemyGO);
			m_enemyList.push_back(enemy);
			break;
		}
		totalEnemy--;
		timer = 0;
		loc = false;
		for (unsigned i = 0; i < colorsize; ++i)
		{
			color[i].Set(1, 1, 1);
		}

		color[location - 1].Set(1, 0, 0);
	}

	//else if (timer > rate/3)
	//{
	//	for (unsigned i = 0; i < colorsize; ++i)
	//	{
	//		color[i].Set(1, 1, 1);
	//	}
	//}
}

float SceneCollision::SetRate()
{
	float frequency;
	totalEnemy = cGameManager->dWaveNo * 3;
	if (cGameManager->dWaveNo >= 5)
	{
		totalEnemy += 3 * (cGameManager->dWaveNo - 4);
	}

	totalEnemy += (cGameManager->dWaveNo / 10) * 10;
	frequency = 3 / (cGameManager->dWaveNo * 0.5);
	if (frequency < 0.1)
	{
		frequency = 0.1;
	}

	enemyLeft = totalEnemy;
	return frequency;
}

bool SceneCollision::NearShop()
{
	if (player->GetGameObject()->pos.y >= 85)
	{
		if (player->GetGameObject()->pos.x > m_worldWidth / 2 - 20 && player->GetGameObject()->pos.x < m_worldWidth / 2 + 20)
		{
			return true;
		}
	}

	return false;
}

void SceneCollision::renderWeaponUI(Vector3 pos, Vector3 scale, GameObject* object)
{
	switch (object->type)
	{
	case GameObject::GO_SWORD:
		modelStack.PushMatrix();
		modelStack.Translate(pos.x, pos.y, pos.z);
		modelStack.Scale(scale.x, scale.y, scale.z);
		meshList[GEO_SWORDR]->material.kAmbient.Set(1, 1, 1);
		RenderMesh(meshList[GEO_SWORDR], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_RIFLE:
		modelStack.PushMatrix();
		modelStack.Translate(pos.x, pos.y, pos.z);
		modelStack.Scale(scale.x - 2, scale.y/3, scale.z);
		meshList[GEO_RIFLE_RIGHT]->material.kAmbient.Set(1, 1, 1);
		RenderMesh(meshList[GEO_RIFLE_RIGHT], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_FLAMETHROWER:
		modelStack.PushMatrix();
		modelStack.Translate(pos.x, pos.y, pos.z);
		modelStack.Scale(scale.x - 2, scale.y/3, scale.z);
		meshList[GEO_FLAMETHROWER]->material.kAmbient.Set(1, 1, 1);
		RenderMesh(meshList[GEO_FLAMETHROWER], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_CROSSBOW:
		modelStack.PushMatrix();
		modelStack.Translate(pos.x, pos.y, pos.z);
		modelStack.Scale(scale.x - 2, scale.y - 2, scale.z);
		meshList[GEO_CROSSBOW]->material.kAmbient.Set(1, 1, 1);
		RenderMesh(meshList[GEO_CROSSBOW], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BOXINGGLOVES:
		modelStack.PushMatrix();
		modelStack.Translate(pos.x, pos.y, pos.z);
		modelStack.Scale(scale.x - 2, scale.y - 2, scale.z);
		meshList[GEO_BOXINGGLOVE_RIGHT]->material.kAmbient.Set(1, 1, 1);
		RenderMesh(meshList[GEO_BOXINGGLOVE_RIGHT], true);
		modelStack.PopMatrix();
		break;
	}
}

void SceneCollision::renderSkillUI(Vector3 pos, Vector3 scale, int object)
{
	switch (object)
	{
	case Skill::EMP:
		modelStack.PushMatrix();
		modelStack.Translate(pos.x, pos.y, pos.z);
		modelStack.Scale(scale.x, scale.y, scale.z);
		meshList[GEO_EMP]->material.kAmbient.Set(1, 1, 1);
		RenderMesh(meshList[GEO_EMP], true);
		modelStack.PopMatrix();
		break;
	case Skill::HACK:
		modelStack.PushMatrix();
		modelStack.Translate(pos.x, pos.y, pos.z);
		modelStack.Scale(scale.x, scale.y, scale.z);
		meshList[GEO_HACK]->material.kAmbient.Set(1, 1, 1);
		RenderMesh(meshList[GEO_HACK], true);
		modelStack.PopMatrix();
		break;
	case Skill::DOPPELGANGER:
		modelStack.PushMatrix();
		modelStack.Translate(pos.x, pos.y, pos.z);
		modelStack.Scale(scale.x, scale.y, scale.z);
		meshList[GEO_DOPPELGANGER]->material.kAmbient.Set(1, 1, 1);
		RenderMesh(meshList[GEO_DOPPELGANGER], true);
		modelStack.PopMatrix();
		break;
	case Skill::IMMORTAL:
		modelStack.PushMatrix();
		modelStack.Translate(pos.x, pos.y, pos.z);
		modelStack.Scale(scale.x, scale.y, scale.z);
		meshList[GEO_IMMORTAL]->material.kAmbient.Set(1, 1, 1);
		RenderMesh(meshList[GEO_IMMORTAL], true);
		modelStack.PopMatrix();
		break;
	case Skill::BLACKHOLE:
		modelStack.PushMatrix();
		modelStack.Translate(pos.x, pos.y, pos.z);
		modelStack.Scale(scale.x, scale.y, scale.z);
		meshList[GEO_BLACKHOLE]->material.kAmbient.Set(1, 1, 1);
		RenderMesh(meshList[GEO_BLACKHOLE], true);
		modelStack.PopMatrix();
		break;
	}
}

void SceneCollision::SetWeapon()
{
	if (player->GetWeapon()->WeaponType != cGameManager->weptype)
	{
		ReturnGO(player->GetWeapon()->GetGameObject());
		NewWeapon(cGameManager->weptype, true);
		cGameManager->weptype = player->GetWeapon()->WeaponType;
	}

	if (player->GetSideWeapon() != nullptr)
	{
		if (player->GetSideWeapon()->WeaponType != cGameManager->sideweptype)
		{
			ReturnGO(player->GetSideWeapon()->GetGameObject());
			NewWeapon(cGameManager->sideweptype, false);
			cGameManager->sideweptype = player->GetSideWeapon()->WeaponType;
		}
	}
	else if (cGameManager->sideweptype != 0)
	{
		NewWeapon(cGameManager->sideweptype, false);
		cGameManager->sideweptype = player->GetSideWeapon()->WeaponType;
	}
}

void SceneCollision::NewWeapon(int weptype, bool MainWep)
{
	Weapon* wep;
	GameObject* weapon = FetchGO();
	switch (weptype)
	{
	case Weapon::SWORD:
		wep = new Sword;
		weapon->type = GameObject::GO_SWORD;
		weapon->scale.Set(10, 10, 1);
		if (cGameManager->fastmeleeBought)
		{
			wep->SetAttackSpeed(0.6);
		}
		break;
	case Weapon::BOXING_GLOVES:
		wep = new BoxingGloves;
		weapon->type = GameObject::GO_BOXINGGLOVES;
		weapon->scale.Set(6, 6, 1);
		if (cGameManager->fastmeleeBought)
		{
			wep->SetAttackSpeed(0.3);
		}
		break;
	case Weapon::RIFLE:
		wep = new Rifle;
		weapon->type = GameObject::GO_RIFLE;
		weapon->scale.Set(8, 3, 1);
		if (cGameManager->fastfireBought)
		{
			wep->SetAttackSpeed(0.07);
		}
		if (cGameManager->fastbulletBought)
		{
			wep->SetBulletSpeed(100);
		}
		break;
	case Weapon::FLAMETHROWER:
		wep = new Flamethrower;
		weapon->type = GameObject::GO_FLAMETHROWER;
		weapon->scale.Set(8, 3, 1);
		if (cGameManager->betterfuelBought)
		{
			wep->SetDamage(6);
		}
		break;
	case Weapon::CROSSBOW:
		wep = new Crossbow;
		weapon->type = GameObject::GO_CROSSBOW;
		weapon->scale.Set(8, 8, 1);
		if (cGameManager->fastfireBought)
		{
			wep->SetAttackSpeed(0.5);
		}
		if (cGameManager->fastbulletBought)
		{
			wep->SetBulletSpeed(90);
		}
		break;
	}


	weapon->angle = 0;
	weapon->pos.SetZero();
	weapon->vel.SetZero();
	weapon->color.Set(1, 1, 1);
	weapon->leftwep = false;

	if (MainWep)
	{
		player->SetWeapon(wep);
		player->GetWeapon()->SetGameObject(weapon);
	}

	else
	{
		player->SetSideWeapon(wep);
		player->GetSideWeapon()->SetGameObject(weapon);
	}
}

void SceneCollision::spawnBlackhole()
{
	blackhole = FetchGO();
	blackhole->type = GameObject::GO_BLACKHOLE;
	blackhole->pos = player->GetGameObject()->pos;
	blackhole->vel.SetZero();
	blackhole->scale.Set(12, 12, 1);
	blackhole->color.Set(1, 1, 1);
	blackhole->angle = 0;
	//blackhole->active = true;
}
void SceneCollision::spawnDoppelganger() {
	doppelganger = new DoppelgangerAlly();
	doppelganger->Init();
	GameObject* doppelgangerGO = FetchGO();
	doppelgangerGO->type = GameObject::GO_DOPPELGANGER;
	doppelgangerGO->pos = player->GetGameObject()->pos;
	doppelgangerGO->vel.SetZero();
	doppelgangerGO->scale.Set(8, 8, 1);
	doppelgangerGO->color.Set(1, 1, 1);
	doppelgangerGO->angle = 0;
	doppelganger->SetWeapon(new Sword());
	doppelganger->SetGameObject(doppelgangerGO);
	m_enemyList.push_back(doppelganger);

	GameObject* dwep = FetchGO();
	dwep->type = GameObject::GO_SWORD;
	dwep->vel.SetZero();
	dwep->scale.Set(8, 8, 1);
	dwep->pos = doppelgangerGO->pos;
	dwep->color.Set(1, 1, 1);
	dwep->angle = 0;
	dwep->active = true;
	dwep->leftwep = false;
	doppelganger->GetWeapon()->SetGameObject(dwep);
}