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


	cSoundController = CSoundController::GetInstance();
	cSoundController->Init();
	cSoundController->LoadSound(FileSystem::getPath("Sound\\damage.ogg"), 1, false);
	cSoundController->LoadSound(FileSystem::getPath("Sound\\sword_swing.ogg"), 2, false);
	cSoundController->LoadSound(FileSystem::getPath("Sound\\enemyHurt.ogg"), 3, false);
	cSoundController->LoadSound(FileSystem::getPath("Sound\\enemyDeath.ogg"), 4, false);

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

	player->SetWeapon(new Rifle());
	GameObject* weapon = FetchGO();
	weapon->type = GameObject::GO_SWORD;
	weapon->pos.SetZero();
	weapon->vel.SetZero();
	weapon->scale.Set(10, 10, 1);
	weapon->angle = 0;
	weapon->color.Set(1, 1, 1);
	weapon->leftwep = false;
	player->GetWeapon()->SetGameObject(weapon);

	cGameManager = GameManger::GetInstance();

	//spawn one enemy
	Enemy* enemy = new Swordsman();
	enemy->Init();
	GameObject* enemyGO = FetchGO();
	enemyGO->type = GameObject::GO_SWORDSMAN;
	enemyGO->pos = Vector3(m_worldWidth / 2, m_worldHeight / 2, 0);
	enemyGO->vel.SetZero();
	enemyGO->scale.Set(10, 10, 1);
	enemyGO->color.Set(1, 1, 1);
	enemyGO->angle = 0;
	enemy->SetWeapon(new Sword());
	enemy->SetGameObject(enemyGO);
	m_enemyList.push_back(enemy);


	GameObject* ewep = FetchGO();
	ewep->type = GameObject::GO_SWORD;
	ewep->vel.SetZero();
	ewep->scale.Set(10, 10, 1);
	ewep->pos = enemyGO->pos;
	ewep->color.Set(1, 1, 1);
	ewep->angle = 0;
	ewep->active = true;
	ewep->leftwep = false;
	enemy->GetWeapon()->SetGameObject(ewep);

	//spawn rifler enemy
	Enemy* enemy2 = new Rifler();
	enemy2->Init();
	GameObject* enemy2GO = FetchGO();
	enemy2GO->type = GameObject::GO_RIFLER;
	enemy2GO->pos = Vector3(m_worldWidth / 2 + 10, m_worldHeight / 2, 0);
	enemy2GO->vel.SetZero();
	enemy2GO->scale.Set(13, 13, 1);
	enemy2GO->color.Set(1, 1, 1);
	enemy2GO->angle = 0;
	enemy2->SetWeapon(new Rifle());
	enemy2->SetGameObject(enemy2GO);
	m_enemyList.push_back(enemy2);

	GameObject* ewep2 = FetchGO();
	ewep2->type = GameObject::GO_RIFLE;
	ewep2->vel.SetZero();
	ewep2->scale.Set(8, 3, 1);
	ewep2->pos = enemy2GO->pos;
	ewep2->color.Set(1, 1, 1);
	ewep2->angle = 0;
	ewep2->active = true;
	ewep2->leftwep = false;
	enemy2->GetWeapon()->SetGameObject(ewep2);

	timer = 0;
	/*offset.Set(weapon->scale.x * 0.2, -weapon->scale.y * 0.4, 0);*/
	//MakeThickWall(10, 40, Vector3(0, 1, 0), Vector3(m_worldWidth / 2, m_worldHeight / 2, 0.f));
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
	static bool oem_5 = false;
	if (Application::IsKeyPressed(VK_OEM_5) && !oem_5)
	{
		cGameManager->dDebug = !cGameManager->dDebug;
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

	if (Application::IsKeyPressed('R') && !e) //go shop
	{
		Application::SetState(3);
		player->getPlayer()->pos.x = m_worldWidth / 2;
		player->getPlayer()->pos.y = 12;
	}
	else if (!Application::IsKeyPressed('E') && e)
		e = false;


	
	if (cGameManager->dDebug)
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

	// Spawn enemy
	//if (timer > 1)
	//{
	//	//spawn one enemy
	//	Enemy* enemy = new Swordsman();
	//	enemy->Init();
	//	GameObject* enemyGO = FetchGO();
	//	enemyGO->type = GameObject::GO_SWORDSMAN;
	//	enemyGO->pos = Vector3(m_worldWidth / 2, m_worldHeight / 2, 0);
	//	enemyGO->vel.SetZero();
	//	enemyGO->scale.Set(10, 10, 1);
	//	enemyGO->color.Set(1, 1, 1);
	//	enemyGO->angle = 0;
	//	enemy->SetWeapon(new Sword());
	//	enemy->SetGameObject(enemyGO);
	//	m_enemyList.push_back(enemy);


	//	GameObject* ewep = FetchGO();
	//	ewep->type = GameObject::GO_SWORD;
	//	ewep->vel.SetZero();
	//	ewep->scale.Set(10, 10, 1);
	//	ewep->pos = enemyGO->pos;
	//	ewep->color.Set(1, 1, 1);
	//	ewep->angle = 0;
	//	ewep->active = true;
	//	ewep->leftwep = false;
	//	enemy->GetWeapon()->SetGameObject(ewep);
	//	timer = 0;
	//}

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
			delete m_enemyList[idx];
			m_enemyList.erase(m_enemyList.begin() + idx);
			continue;
		}
		if (m_enemyList[idx]->IsSpawningBullet())
		{
			//spawn bullet
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
		//spawn bullet for player
		Bullet* bullet = new Bullet;
		GameObject* bulletgo = FetchGO();
		bulletgo->type = GameObject::GO_BULLET;
		bulletgo->pos = player->GetWeapon()->GetGameObject()->pos;
		bulletgo->vel.SetZero();
		bulletgo->scale.Set(2, 2, 1);
		bulletgo->color.Set(1, 1, 1);
		bulletgo->angle = player->GetWeapon()->GetGameObject()->angle;
		bullet->SetGameObject(bulletgo);
		bullet->SetBullet(player->GetWeapon()->GetBulletSpeed(), player->GetWeapon()->GetDamage(), player->GetWeapon()->GetPiercing(), player->GetWeapon()->GetRange(), (mousepos - player->GetGameObject()->pos).Normalize());
		m_pbulletList.push_back(bullet);
	}
	//update bullets
	for (unsigned idx = 0; idx < m_pbulletList.size(); idx++)
	{
		m_pbulletList[idx]->Update(dt);
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

	}
	for (unsigned idx = 0; idx < m_ebulletList.size(); idx++)
	{
		m_ebulletList[idx]->Update(dt);
		if (m_ebulletList[idx]->GetGameObject()->pos.x > m_worldWidth || m_ebulletList[idx]->GetGameObject()->pos.x < 0 || m_ebulletList[idx]->GetGameObject()->pos.y > m_worldHeight || m_ebulletList[idx]->GetGameObject()->pos.y < 0)
		{
			//delete the bullet
			ReturnGO(m_ebulletList[idx]->GetGameObject());
			delete m_ebulletList[idx];
			m_ebulletList.erase(m_ebulletList.begin() + idx);
			continue;
		}
		//check collision
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

	//Physics Simulation Section
	unsigned size = m_goList.size();
	for (unsigned i = 0; i < size; ++i)
	{
		GameObject *go = m_goList[i];
		if(go->active)
		{
			go->pos += go->vel * dt * m_speed;


			if (((go->pos.x - go->scale.x < 0) && go->vel.x < 0 ) || ((go->pos.x + go->scale.x > m_worldWidth) && go->vel.x > 0))
			{
				go->vel.x = -go->vel.x;
			}
			if ((go->pos.y + go->scale.y> m_worldHeight) && go->vel.y > 0)
			{
				go->vel.y = -go->vel.y;
			}

			if (go->pos.y + go->scale.y < 0)
			{
				ReturnGO(go);
				continue;
			}
			
			GameObject* go2 = nullptr;
			for (int j = i; j < size; ++j)
			{
				go2 = m_goList[j];
				if (!go2->checkCollision || !go->checkCollision)
					continue;
				GameObject* actor(go);
				GameObject* actee(go2);

				

				if (go->type != GameObject::GO_BALL)
				{
					actor = go2;
					actee = go;
				}


				
				if (go2->active && CheckCollision(actor, actee))
				{
					//reduce the velocity of the ball when it hits something
					if (actor->type == GameObject::GO_BALL)
						actor->vel *= 0.8;



					CollisionResponse(actor, actee);
					if (actee->disappearWhenHit)
					{

						ReturnGO(actee);
						if (actee->otherGameObjects.size() != 0)
						{
							for (unsigned idx = 0; idx < actee->otherGameObjects.size(); idx++)
							{
								ReturnGO(actee->otherGameObjects[idx]);
							}
							actee->otherGameObjects.clear();
						}
					}
				}
			}		
		}
	}
}

bool SceneCollision::CheckCollision(GameObject* go1, GameObject* go2)
{
	// Prevent non ball vs non ball code
	if (go1->type != GameObject::GO_BALL)
	{
		return false;
	}

	switch (go2->type)
	{
	case GameObject::GO_PILLAR:
	case GameObject::GO_BALL:
	{
		Vector3 relativeVel = go1->vel - go2->vel;
		Vector3 disDiff = go2->pos - go1->pos;

		if (relativeVel.Dot(disDiff) <= 0)
			return false;
		return disDiff.LengthSquared() <= (go1->scale.x + go2->scale.x) * (go1->scale.x + go2->scale.x);
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
	case GameObject::GO_SWORD:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(go->angle, 0, 0, 1);

		if (go->leftwep == false)
		{
			modelStack.Translate(go->scale.x * 0.3, go->scale.y * 0.3, 0);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			meshList[GEO_SWORDL]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_SWORDL], true);
		}

		else
		{
			modelStack.Translate(-go->scale.x * 0.3, go->scale.y * 0.3, 0);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			meshList[GEO_SWORDR]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_SWORDR], true);
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
			meshList[GEO_RIFLE_LEFT]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_RIFLE_LEFT], true);
		}

		else
		{
			modelStack.Translate(-go->scale.x * 0.3, go->scale.y * 0.3, 0);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			meshList[GEO_RIFLE_RIGHT]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
			RenderMesh(meshList[GEO_RIFLE_RIGHT], true);
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
	modelStack.Scale(1000,1000,1000);
	RenderMesh(meshList[GEO_SANDBG], false);
	modelStack.PopMatrix();


	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active && go->visible)
		{
			RenderGO(go);
		}
	}


	//On screen text
	std::ostringstream ss;

	//render the player health
	ss.str("");
	ss << "Health:";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0.5, 57);
	RenderMeshOnScreen(meshList[GEO_HEALTH_UI_BASE], 12, 58.75, 10, 2);
	RenderMeshOnScreen(meshList[GEO_HEALTH_UI_RED], 7 + (double)player->GetHealth() / (double)player->GetMaxHealth() * 5.0f, 58.75, (double)player->GetHealth() / (double)player->GetMaxHealth() * 10.0f, 2);

	if (cGameManager->dDebug)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Object Count:" + std::to_string(m_objectCount), Color(1, 1, 1), 3, 0, 12);

		ss.precision(3);
		ss.str("");
		ss << "Speed: " << m_speed;
		ss << " FPS: " << fps;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 9);
	
	}

	ss.str("");
	ss << "Press r to go shop";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 4, 40);


	if (cGameManager->bPlayerLost)
	{
		ss.str("");
		ss << "You ran out of balls, You Lose";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 20, 24);
		ss.str("");
		ss << "Press 'R' to restart";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 10, 20);
		ss.str("");
		ss << "Press '~' to return to main menu";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 10, 16);
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

}

GameObject* SceneCollision::Checkborder(GameObject* go)
{
	if (go->pos.x + go->scale.x / 2 > m_worldWidth)
	{
		go->pos.x = m_worldWidth - go->scale.x / 2;
	}	
	
	if (go->pos.x - go->scale.x / 2 < 0)
	{
		go->pos.x = 0 + go->scale.x / 2;
	}	
	
	if (go->pos.y + go->scale.y / 2 > m_worldHeight)
	{
		go->pos.y = m_worldHeight - go->scale.y / 2;
	}	
	
	if (go->pos.y - go->scale.y / 2 < 0)
	{
		go->pos.y = 0 + go->scale.y / 2;
	}

	return go;
}
