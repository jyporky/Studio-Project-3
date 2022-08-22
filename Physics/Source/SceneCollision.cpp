#include "SceneCollision.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

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

void SceneCollision::Init()
{
	SceneBase::Init();
	
	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 1.f;
	bLightEnabled = true;

	Math::InitRNG();

	//Exercise 1: initialize m_objectCount
	m_objectCount = 0;

	wave = 1;

	rate = SetRate();

	timer = -3;

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
	cSoundController->LoadSound(FileSystem::getPath("Sound\\menuBGM.ogg"), 8, false, CSoundInfo::BGM);
	cSoundController->LoadSound(FileSystem::getPath("Sound\\gameplayBGM.ogg"), 9, false, CSoundInfo::BGM);
	cSoundController->LoadSound(FileSystem::getPath("Sound\\buyItem.ogg"), 10, false);
	cSoundController->LoadSound(FileSystem::getPath("Sound\\Shieldblock.ogg"), 11, false);


	cInventoryManager = CInventoryManager::GetInstance();
	//weapons
	cInventoryItem = cInventoryManager->Add("Fist", 1, 1);
	cInventoryItem = cInventoryManager->Add("sword", 1, 1);
	cInventoryItem = cInventoryManager->Add("boxingglove", 1, 0);	
	cInventoryItem = cInventoryManager->Add("rubberchicken", 1, 0);
	cInventoryItem = cInventoryManager->Add("fryingpan", 1, 0);
	
	cInventoryItem = cInventoryManager->Add("rifle", 1, 1);
	cInventoryItem = cInventoryManager->Add("flamethrower", 1, 0);
	cInventoryItem = cInventoryManager->Add("crossbow", 1, 0);

	//potions
	cInventoryItem = cInventoryManager->Add("healthpotion", 10, 0);
	cInventoryItem = cInventoryManager->Add("strengthpotion", 10, 0);
	cInventoryItem = cInventoryManager->Add("speedpotion", 10, 0);

	//skills
	cInventoryItem = cInventoryManager->Add("emp", 1, 0);
	cInventoryItem = cInventoryManager->Add("hack", 1, 0);
	cInventoryItem = cInventoryManager->Add("heal", 1, 0);
	cInventoryItem = cInventoryManager->Add("immortal", 1, 0);
	cInventoryItem = cInventoryManager->Add("overdrive", 1, 0);

	cGameManager = GameManger::GetInstance();

	GameObject* m_player = FetchGO();
	m_player->type = GameObject::GO_PLAYER;
	m_player->pos.Set(m_worldWidth/2, m_worldHeight/2, 1);
	m_player->vel.SetZero();
	m_player->scale.Set(10, 10, 1);
	m_player->color.Set(1, 1, 1);
	m_player->angle = 0;
	m_player->active = true;
	player = Player::GetInstance();
	player->SetGameObject(m_player);
	player->SetWeapon(new Flamethrower());
	GameObject* weapon1 = FetchGO();
	weapon1->type = GameObject::GO_FLAMETHROWER;
	weapon1->pos.SetZero();
	weapon1->vel.SetZero();
	weapon1->scale.Set(8, 3, 1);
	weapon1->angle = 0;
	weapon1->color.Set(1, 1, 1);
	weapon1->leftwep = false;
	player->GetWeapon()->SetGameObject(weapon1);
	player->SwapWeapon();

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
	player->GetWeapon()->SetGameObject(weapon2);

	////spawn one enemy
	//Enemy* enemy = new Swordsman();
	//enemy->Init();
	//GameObject* enemyGO = FetchGO();
	//enemyGO->type = GameObject::GO_SWORDSMAN;
	//enemyGO->pos = Vector3(m_worldWidth / 2, m_worldHeight / 2, 0);
	//enemyGO->vel.SetZero();
	//enemyGO->scale.Set(10, 10, 1);
	//enemyGO->color.Set(1, 1, 1);
	//enemyGO->angle = 0;
	//enemy->SetWeapon(new Sword());
	//enemy->SetGameObject(enemyGO);
	//m_enemyList.push_back(enemy);

	//GameObject* ewep = FetchGO();
	//ewep->type = GameObject::GO_SWORD;
	//ewep->vel.SetZero();
	//ewep->scale.Set(10, 10, 1);
	//ewep->pos = enemyGO->pos;
	//ewep->color.Set(1, 1, 1);
	//ewep->angle = 0;
	//ewep->active = true;
	//ewep->leftwep = false;
	//enemy->GetWeapon()->SetGameObject(ewep);

	//// spawn rifler enemy
	//Enemy* enemy2 = new Rifler();
	//enemy2->Init();
	//GameObject* enemy2GO = FetchGO();
	//enemy2GO->type = GameObject::GO_RIFLER;
	//enemy2GO->pos = Vector3(m_worldWidth / 2 + 10, m_worldHeight / 2, 0);
	//enemy2GO->vel.SetZero();
	//enemy2GO->scale.Set(13, 13, 1);
	//enemy2GO->color.Set(1, 1, 1);
	//enemy2GO->angle = 0;
	//enemy2->SetWeapon(new Rifle());
	//enemy2->SetGameObject(enemy2GO);
	//m_enemyList.push_back(enemy2);

	//GameObject* ewep2 = FetchGO();
	//ewep2->type = GameObject::GO_RIFLE;
	//ewep2->vel.SetZero();
	//ewep2->scale.Set(8, 3, 1);
	//ewep2->pos = enemy2GO->pos;
	//ewep2->color.Set(1, 1, 1);
	//ewep2->angle = 0;
	//ewep2->active = true;
	//ewep2->leftwep = false;
	//enemy2->GetWeapon()->SetGameObject(ewep2);*/


	/*offset.Set(weapon->scale.x * 0.2, -weapon->scale.y * 0.4, 0);*/
	//MakeThickWall(10, 40, Vector3(0, 1, 0), Vector3(m_worldWidth / 2, m_worldHeight / 2, 0.f));

	colorsize = 3;
	for (unsigned i = 0; i < colorsize; ++i)
	{
		color[i] = Vector3(1,1,1);
	}

	meshList[GEO_TELEPORT_PAD]->material.kAmbient.Set(1, 1, 1);
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
	if (cGameManager->bWaveClear)
		cGameManager->dWaveNo++;
	else if (cGameManager->bPlayerLost)
		cGameManager->dWaveNo = 1;

	cGameManager->bPlayerLost = false;
	cGameManager->bGameWin = false;
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
	std::cout << ImmortalitySkill->getState() << std::endl;
	if (cGameManager->bPlayerLost || cGameManager->bWaveClear || cGameManager->bGameWin)
	{
		/*if (Application::IsKeyPressed('R') && !cGameManager->bGameWin)
		{
			ResetLevel();
		}*/
		if (Application::IsKeyPressed(VK_OEM_3))
		{
			Application::SetState(1);
		}
		return;
	}

	if (cGameManager->outShop)
	{
		cGameManager->outShop = false;
		rate = SetRate();
		wave++;
		player->GetGameObject()->pos.Set(m_worldWidth / 2, m_worldHeight / 2, 1);
		timer = -3;
		for (unsigned i = 0; i < colorsize; ++i)
		{
			color[i].Set(1, 1, 1);
		}
	}

	if (totalEnemy > 0 && timer > 0)
		SpawnEnemy(rate);

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
	}
	else if (!Application::IsKeyPressed('E') && e)
		e = false;
	
	if (cGameManager->bDebug)
	{
		if(Application::IsKeyPressed('9'))
		{
			m_speed = Math::Max(0.f, m_speed - 0.1f);
		}
		if(Application::IsKeyPressed('0'))
		{
			m_speed += 0.1f;
		}
	}
	cSoundController->PlaySoundByID(9);
	static bool switch_weapon = false;
	if (Application::IsMousePressed(1) && !switch_weapon && player->GetSideWeapon() != nullptr)
	{
		player->GetWeapon()->GetGameObject()->visible = false;
		player->SwapWeapon();
		player->GetWeapon()->GetGameObject()->visible = true;
		switch_weapon = true;
	}
	else if (!Application::IsMousePressed(1) && switch_weapon)
		switch_weapon = false;


	static bool ubutton;
	bool dealdamage = false;
	if (Application::IsKeyPressed('U') && !ubutton)
	{
		ubutton = true;
		dealdamage = true;
		HealSkill->UseSkill();
	}
	else if (!Application::IsKeyPressed('U') && ubutton)
		ubutton = false;

	//update enemy
	for (unsigned idx = 0; idx < m_enemyList.size(); idx++)
	{
		if (m_enemyList[idx]->Update(dt))
		{
			//delete the enemy
			ReturnGO(m_enemyList[idx]->GetGameObject());
			ReturnGO(m_enemyList[idx]->GetWeapon()->GetGameObject());
			player->changeEnergy(m_enemyList[idx]->GetEnergyDrop());
			player->changeMoney(m_enemyList[idx]->GetMoneyDrop());
			cGameManager->dPlayerScore += 100;
			delete m_enemyList[idx];
			m_enemyList.erase(m_enemyList.begin() + idx);
			enemyLeft--;
			continue;
		}
		if (m_enemyList[idx]->IsSpawningBullet())
		{
			Vector3 shootPlayer = player->getPlayer()->pos - m_enemyList[idx]->GetGameObject()->pos;
			Bullet* bullet = new Bullet;
			GameObject* bulletgo = FetchGO();
			bulletgo->type = GameObject::GO_BULLET;
			bulletgo->pos = m_enemyList[idx]->GetGameObject()->pos;
			bulletgo->pos.z = 1;
			bulletgo->vel.SetZero();
			bulletgo->scale.Set(2, 2, 1);
			bulletgo->color.Set(1, 1, 1);
			bulletgo->angle = m_enemyList[idx]->GetWeapon()->GetGameObject()->angle;
			bullet->SetGameObject(bulletgo);
			bullet->SetBullet(m_enemyList[idx]->GetWeapon()->GetBulletSpeed(), m_enemyList[idx]->GetWeapon()->GetDamage() + player->rangeDmgBoost, m_enemyList[idx]->GetWeapon()->GetPiercing(), m_enemyList[idx]->GetWeapon()->GetRange(), shootPlayer.Normalize());
			m_ebulletList.push_back(bullet);
		}
		//if (dealdamage)
		//{
		//	if (m_enemyList[idx]->ChangeHealth(-1))
		//	{
		//		//delete the enemy
		//		ReturnGO(m_enemyList[idx]->GetGameObject());
		//		ReturnGO(m_enemyList[idx]->GetWeapon()->GetGameObject());
		//		delete m_enemyList[idx];
		//		m_enemyList.erase(m_enemyList.begin() + idx);
		//	}
		//}
	}
	
	//update the player
	player->SetEnemyVector(m_enemyList);
	player->Update(dt, mousePos);
	Checkborder(player->getPlayer());
	if (player->IsSpawningBullet())
	{
		Application::GetCursorPos(&x, &y);
		Vector3 mousepos = Vector3((x / width) * m_worldWidth, ((height - y) / height) * m_worldHeight, 0);
		if (player->GetWeapon()->WeaponType == Weapon::FLAMETHROWER) //if weapon is a flamethrower
		{
			FlameParticle* flame = new FlameParticle;
			GameObject* flamego = FetchGO();
			flamego->type = GameObject::GO_FLAME;
			flamego->pos = player->GetGameObject()->pos;
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
			arrowgo->scale.Set(4, 4, 1);
			arrowgo->color.Set(1, 1, 1);
			arrowgo->angle = player->GetWeapon()->GetGameObject()->angle;
			arrow->SetGameObject(arrowgo);
			arrow->SetArrow(player->GetWeapon()->GetBulletSpeed(), player->GetWeapon()->GetDamage(), player->GetWeapon()->GetPiercing(), player->GetWeapon()->GetRange(), (mousepos - player->GetGameObject()->pos).Normalize());
			m_parrowList.push_back(arrow);
		}
		else
		{
			//spawn bullet for player
			Bullet* bullet = new Bullet;
			GameObject* bulletgo = FetchGO();
			bulletgo->type = GameObject::GO_BULLET;
			bulletgo->pos = player->GetGameObject()->pos;
			bulletgo->vel.SetZero();
			bulletgo->pos.z = 0;
			bulletgo->scale.Set(2, 2, 1);
			bulletgo->color.Set(1, 1, 1);
			bulletgo->angle = player->GetWeapon()->GetGameObject()->angle;
			bullet->SetGameObject(bulletgo);
			bullet->SetBullet(player->GetWeapon()->GetBulletSpeed(), player->GetWeapon()->GetDamage(), player->GetWeapon()->GetPiercing(), player->GetWeapon()->GetRange(), (mousepos - player->GetGameObject()->pos).Normalize());
			m_pbulletList.push_back(bullet);
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
				m_enemyList[idx1]->ChangeHealth(-m_pbulletList[idx]->GetDamage());
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
			if (ImmortalitySkill->getState() == true) {
				player->ChangeHealth(m_ebulletList[idx]->GetDamage());
			}
			else {
				player->ChangeHealth(-m_ebulletList[idx]->GetDamage());
			}
			if (!m_ebulletList[idx]->GetPenetrationValue()) {
				ReturnGO(m_ebulletList[idx]->GetGameObject());
				delete m_ebulletList[idx];
				m_ebulletList.erase(m_ebulletList.begin() + idx);

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
					m_enemyList[idx1]->ChangeHealth(-m_parrowList[idx]->GetDamage() * 2);
				}
				else {
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
	case GameObject::GO_PLAYER:
	case GameObject::GO_SWORDSMAN:
	case GameObject::GO_RIFLER:
	case GameObject::GO_SHIELDMAN:
	{
		Vector3 relativeVel = go1->vel - go2->vel;
		Vector3 disDiff = go2->pos - go1->pos;

		if (relativeVel.Dot(disDiff) <= 0)
			return false;
		return disDiff.LengthSquared() <= (go1->scale.x + go2->scale.x) * (go1->scale.x + go2->scale.x) * 0.35;
	}

	case GameObject::GO_WALL:
	{
		Vector3 diff = go1->pos - go2->pos;
		Vector3 axisX = go2->normal;
		Vector3 axisY = Vector3(-go2->normal.y, go2->normal.x, 0);
		float projectedDist = diff.Dot(axisX);

		//if it is a thick wall
		if (go2->otherGameObjects.size() != 0)
		{
			if (Math::FAbs(projectedDist) / go2->scale.x < Math::FAbs(diff.Dot(axisY)) / go2->otherGameObjects[0]->scale.x)
			{
				return false;
			}
		}


		if (projectedDist > 0)
			axisX = -axisX;

		return go1->vel.Dot(axisX) >= 0 && // Check 1: Travelling towards the wall?
			go2->scale.x * 0.5 + go1->scale.x > -diff.Dot(axisX) &&// Check 2: Radius + Thickness vs Distance
			go2->scale.y * 0.5 > fabs(diff.Dot(axisY)); // Check 3: Length Check
	}
	}
	return false;
}

void SceneCollision::CollisionResponse(GameObject* go1, GameObject* go2)
{
	u1 = go1->vel;
	u2 = go2->vel;
	m1 = go1->mass;
	m2 = go2->mass;


	switch (go2->type)
	{
		case GameObject::GO_BALL:
		{
			//2D Version 2
			Vector3 n = go1->pos - go2->pos;
			Vector3 vec = (u1 - u2).Dot(n) / (n).LengthSquared() * n;
			go1->vel = u1 - (2 * m2 / (m1 + m2)) * vec;
			go2->vel = u2 - (2 * m1 / (m2 + m1)) * -vec;
			break;
		}
		case GameObject::GO_WALL:
		{
			go1->vel = u1 - (2.0 * u1.Dot(go2->normal)) * go2->normal;
			break;
		}
		case GameObject::GO_PILLAR:
		{
			Vector3 n = (go2->pos - go1->pos).Normalize();
			go1->vel = u1 - (2.0 * u1.Dot(n) * n);
			break;
		}
	}
}


void SceneCollision::MakeThickWall(float width, float height, const Vector3& normal, const Vector3& pos, const Vector3& color)
{
	Vector3 tangent(-normal.y, normal.x);

	float size = 0.1f;

	//4 pillars
	GameObject* pillar1 = FetchGO();
	pillar1->type = GameObject::GO_PILLAR;
	pillar1->color = color;
	pillar1->scale.Set(size, size, 1);
	pillar1->pos = pos + height * 0.48f * tangent + width * 0.48f * normal;
	pillar1->disappearWhenHit = true;
	pillar1->visible = true;


	//4 pillars
	GameObject* pillar2 = FetchGO();
	pillar2->type = GameObject::GO_PILLAR;
	pillar2->color = color;
	pillar2->scale.Set(size, size, 1);
	pillar2->pos = pos + height * 0.48f * tangent - width * 0.48f * normal;
	pillar2->disappearWhenHit = true;
	pillar2->visible = true;

	//4 pillars
	GameObject* pillar3 = FetchGO();
	pillar3->type = GameObject::GO_PILLAR;
	pillar3->color = color;
	pillar3->scale.Set(size, size, 1);
	pillar3->pos = pos - height * 0.48f * tangent - width * 0.48f * normal;
	pillar3->disappearWhenHit = true;
	pillar3->visible = true;

	//4 pillars
	GameObject* pillar4 = FetchGO();
	pillar4->type = GameObject::GO_PILLAR;
	pillar4->color = color;
	pillar4->scale.Set(size, size, 1);
	pillar4->pos = pos - height * 0.48f * tangent + width * 0.48f * normal;
	pillar4->disappearWhenHit = true;
	pillar4->visible = true;

	GameObject* wall1 = FetchGO();
	wall1->type = GameObject::GO_WALL;
	wall1->scale.Set(width, height, 1.f);
	wall1->normal = normal;
	wall1->color = color;
	wall1->pos = pos;
	wall1->disappearWhenHit = true;
	wall1->visible = true;

	GameObject* wall2 = FetchGO();
	wall2->type = GameObject::GO_WALL;
	wall2->scale.Set(height, width, 1.f);
	wall2->normal = tangent;
	wall2->color = color;
	wall2->pos = pos;
	wall2->visible = false;
	wall2->disappearWhenHit = true;


	//add the game objects to each other
	wall1->otherGameObjects.push_back(wall2);
	wall1->otherGameObjects.push_back(pillar1);
	wall1->otherGameObjects.push_back(pillar2);
	wall1->otherGameObjects.push_back(pillar3);
	wall1->otherGameObjects.push_back(pillar4);
	wall2->otherGameObjects.push_back(wall1);
	wall2->otherGameObjects.push_back(pillar1);
	wall2->otherGameObjects.push_back(pillar2);
	wall2->otherGameObjects.push_back(pillar3);
	wall2->otherGameObjects.push_back(pillar4);
	pillar1->otherGameObjects.push_back(wall1);
	pillar1->otherGameObjects.push_back(wall2);
	pillar1->otherGameObjects.push_back(pillar2);
	pillar1->otherGameObjects.push_back(pillar3);
	pillar1->otherGameObjects.push_back(pillar4);
	pillar2->otherGameObjects.push_back(wall1);
	pillar2->otherGameObjects.push_back(wall2);
	pillar2->otherGameObjects.push_back(pillar1);
	pillar2->otherGameObjects.push_back(pillar3);
	pillar2->otherGameObjects.push_back(pillar4);
	pillar3->otherGameObjects.push_back(wall1);
	pillar3->otherGameObjects.push_back(wall2);
	pillar3->otherGameObjects.push_back(pillar1);
	pillar3->otherGameObjects.push_back(pillar2);
	pillar3->otherGameObjects.push_back(pillar4);
	pillar4->otherGameObjects.push_back(wall1);
	pillar4->otherGameObjects.push_back(wall2);
	pillar4->otherGameObjects.push_back(pillar1);
	pillar4->otherGameObjects.push_back(pillar2);
	pillar4->otherGameObjects.push_back(pillar3);
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
	case GameObject::GO_PLAYER:
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
			RenderMesh(meshList[GEO_SWORDR], true);
		}

		else
		{
			modelStack.Translate(-go->scale.x * 0.3, go->scale.y * 0.3, 0);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			meshList[GEO_SWORDL]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_SWORDL], true);
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
			RenderMesh(meshList[GEO_RIFLE_RIGHT], true);
		}

		else
		{
			modelStack.Translate(-go->scale.x * 0.3, go->scale.y * 0.3, 0);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
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
			RenderMesh(meshList[GEO_CROSSBOW_RIGHT], true);
		}

		else
		{
			modelStack.Translate(-go->scale.x * 0.3, go->scale.y * 0.6, 0);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
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
			RenderMesh(meshList[GEO_FLAMETHROWER], true);
		}

		else
		{
			modelStack.Translate(-go->scale.x * 0.3, go->scale.y * 0.3, 0);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
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
		meshList[GEO_ARROW]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
		RenderMesh(meshList[GEO_ARROW], true);
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

	RenderTextOnScreen(meshList[GEO_TEXT], "Wave:" + std::to_string(wave), Color(1, 1, 1), 3, 0, 21);
	RenderTextOnScreen(meshList[GEO_TEXT], "Enemies Left:" + std::to_string(enemyLeft), Color(1, 1, 1), 3, 0, 18);

	if (cGameManager->waveClear && timer < 3)
	{
		if (NearShop())
		{
			ss.str("");
			ss << "Press e to go shop";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 4, 40);
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
		ss << "You Died";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 30, 24);
		ss.str("");
		ss << "Press 'R' to restart";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 20, 20);
		ss.str("");
		ss << "Press '~' to return to main menu";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 20, 16);
	}

	if (cGameManager->bGameWin)
	{
		ss.str("");
		ss << "You cleared the game!";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 10, 24);
		ss.str("");
		ss << "Press '~' to return to main menu";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 10, 16);
	}
}

void SceneCollision::renderUI()
{
	//On screen text
	std::ostringstream ss;

	//render the player health
	ss.str("");
	ss << "Health:";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0.5, 57);
	RenderMeshOnScreen(meshList[GEO_HEALTH_UI_BASE], 12, 58.75, 10, 2);
	RenderMeshOnScreen(meshList[GEO_HEALTH_UI_RED], 7 + (double)player->GetHealth() / (double)player->GetMaxHealth() * 5.0f, 58.75, (double)player->GetHealth() / (double)player->GetMaxHealth() * 10.0f, 2);

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


	//render score
	ss.str("");
	ss << "Score:" << cGameManager->dPlayerScore;
	modelStack.PushMatrix();
	modelStack.Translate(177 - ss.str().size() * 1.2, 90, 1);
	modelStack.Scale(18 + ss.str().size() * 0.7, 5, 1);
	RenderMesh(meshList[GEO_SHOPMENUBG], false);
	modelStack.PopMatrix();

	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 80 - ss.str().size(), 52.5);

	//render energy
	ss.str("");
	ss << "Energy:";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0.5, 53);

	if (player->GetHealth() <= player->GetMaxHealth() * 0.3)
	{
		if (timer > 0.5)
		{
			ss.str("");
			ss << "Low HP!";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.9, 0, 0.1), 3, 0.5, 49);
		}
	}

	ss.str("");
	ss << player->getEnergy();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 8, 52.8);


	Vector3 wep1 = Vector3(6, 24, 1);
	Vector3 wep2 = Vector3(16, 24, 1);
	Vector3 scale = Vector3(10, 10, 1);
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
	//add equipped skill code
	//if (player->getEnergy() >= 100)
	//{
	//	modelStack.PushMatrix();
	//	modelStack.Translate(26, 90, 1);
	//	modelStack.Scale(7, 7, 1);
	//	RenderMesh(meshList[GEO_EMP], false);
	//	modelStack.PopMatrix();
	//}
	//if (player->getEnergy() >= 150)
	//{
	//	modelStack.PushMatrix();
	//	modelStack.Translate(26, 90, 1);
	//	modelStack.Scale(7, 7, 1);
	//	RenderMesh(meshList[GEO_HACK], false);
	//	modelStack.PopMatrix();
	//}
	//if (player->getEnergy() >= 150)
	//{
	//	modelStack.PushMatrix();
	//	modelStack.Translate(26, 90, 1);
	//	modelStack.Scale(7, 7, 1);
	//	RenderMesh(meshList[GEO_HEAL], false);
	//	modelStack.PopMatrix();
	//}
	//if (player->getEnergy() >= 200)
	//{
	//	modelStack.PushMatrix();
	//	modelStack.Translate(26, 90, 1);
	//	modelStack.Scale(7, 7, 1);
	//	RenderMesh(meshList[GEO_IMMORTAL], false);
	//	modelStack.PopMatrix();
	//}
	//if (player->getEnergy() >= 80)
	//{
	//	modelStack.PushMatrix();
	//	modelStack.Translate(26, 90, 1);
	//	modelStack.Scale(7, 7, 1);
	//	RenderMesh(meshList[GEO_OVERDRIVE], false);
	//	modelStack.PopMatrix();
	//}
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
	while (m_FlameParticle.size() > 0)
	{
		FlameParticle* go = m_FlameParticle.back();
		delete go;
		m_FlameParticle.pop_back();
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
	float offset = 10;
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
		int type = Math::RandIntMinMax(1, 3);

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

	if (wave < 3)
	{
		frequency = 3;
		totalEnemy = Math::RandIntMinMax(8, 12);
		enemyLeft = totalEnemy;
	}

	else if (wave < 6)
	{
		frequency = 2.5;
		totalEnemy = Math::RandIntMinMax(13, 18);
		enemyLeft = totalEnemy;
	}
	else if (wave < 10)
	{
		frequency = 2;
		totalEnemy = Math::RandIntMinMax(22, 30);
		enemyLeft = totalEnemy;
	}

	else
	{
		frequency = 1;
		totalEnemy = Math::RandIntMinMax(45, 80);
		enemyLeft = totalEnemy;
	}

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
		RenderMesh(meshList[GEO_SWORDR], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_RIFLE:
		modelStack.PushMatrix();
		modelStack.Translate(pos.x, pos.y, pos.z);
		modelStack.Scale(scale.x - 2, scale.y/3, scale.z);
		RenderMesh(meshList[GEO_RIFLE_RIGHT], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_FLAMETHROWER:
		modelStack.PushMatrix();
		modelStack.Translate(pos.x, pos.y, pos.z);
		modelStack.Scale(scale.x - 2, scale.y/3, scale.z);
		RenderMesh(meshList[GEO_FLAMETHROWER], true);
		modelStack.PopMatrix();
		break;
	}
}