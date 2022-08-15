#include "SceneCollision.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneCollision::SceneCollision()
{
	basketleft = nullptr;
	basketright = nullptr;
	basket = nullptr;
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

	m_ghost = new GameObject(GameObject::GO_BALL);

	//MakeThinWall(5.0f, 20.0f, Vector3(0, 1, 0), Vector3(m_worldWidth / 2, m_worldHeight / 2, 0));

	/*float angle = Math::QUARTER_PI;
	float wallLength = 30;
	float radius = wallLength * 0.5f / tan(angle * 0.5f);*/
	////create octogonal shape
	//for (int i = 0; i < 8; ++i)
	//{
	//	GameObject* go = FetchGO();
	//	go->type = GameObject::GO_WALL;
	//	go->active = true;
	//	go->scale.Set(2.0f, wallLength + 0.9f, 1.0f);
	//	go->pos = Vector3(radius * cosf(i * angle) + m_worldWidth * 0.5, radius * sinf(i * angle) + m_worldHeight * 0.5, 0.f);
	//	go->normal = Vector3(cosf(i * angle), sinf(i * angle), 0.f);
	//	go->vel.SetZero();
	//}

	
	//create the gun body
	m_gunbody = new GameObject(GameObject::GO_BALL);
	m_gunbody->checkCollision = false;
	m_gunbody->pos.Set(m_worldWidth * 0.5f, m_worldHeight * 0.99f, 0);
	m_gunbody->scale.Set(5, 5, 5);
	m_gunbody->color.Set(107.0f / 255.0f, 119.0f / 255.0f, 138.0f / 255.0f);
	m_gunbody->vel.SetZero();

	//create the gun barrel
	m_gunbarrel = new GameObject(GameObject::GO_WALL);
	m_gunbarrel->checkCollision = false;
	m_gunbarrel->pos.Set(m_worldWidth * 0.5f, m_worldHeight * 0.99f, 0);
	m_gunbarrel->scale.Set(4, 6, 2);
	m_gunbarrel->color.Set(82.0f / 255.0f, 89.0f / 255.0f, 102.0f / 255.0f);
	m_gunbarrel->normal.Set(1, 0, 0);
	m_gunbarrel->vel.SetZero();

	basket = new GameObject(GameObject::GO_WALL);
	basket->checkCollision = true;
	basket->pos.Set(m_worldWidth * 0.5, m_worldHeight * 0.03, 0);
	basket->scale.Set(4, 14, 2);
	basket->color.Set(252.f / 255.f, 160.f / 255.f, 68.f / 255.f);
	basket->affectedByGravity = false;
	basket->normal.Set(0, 1, 0);
	basket->vel.SetZero();
	basket->visible = false;

	//create the basket at the bottom of the map
	basketright = FetchGO();
	basketright->type = GameObject::GO_PILLAR;
	basketright->pos.Set(m_worldWidth * 0.5 + 7, m_worldHeight * 0.05, 0);
	basketright->scale.Set(1, 1, 1);
	basketright->color.Set(252.f / 255.f, 160.f / 255.f, 68.f / 255.f);
	basketright->vel.SetZero();
	basketright->checkCollision = true;
	basketright->affectedByGravity = false;
	basketright->disappearWhenHit = false;

	basketleft = FetchGO();
	basketleft->pos.Set(m_worldWidth * 0.5 - 7, m_worldHeight * 0.05, 0);
	basketleft->scale.Set(1, 1, 1);
	basketleft->type = GameObject::GO_PILLAR;
	basketleft->color.Set(252.f / 255.f, 160.f / 255.f, 68.f / 255.f);
	basketleft->vel.SetZero();
	basketleft->checkCollision = true;
	basketleft->affectedByGravity = false;
	basketleft->disappearWhenHit = false;


	debug = false; shootcooldown = 10; activeballs = 0; redbricksleft = 0;
	Gravity.Set(0, -30, 0); ballsleft = 5; playerlose = false; playerwin = false; levelno = 1;
	gameclear = false; ballselected = 0;

	CreateLevel(5, 5, 2);
	basketshotdt = 0;

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
		if (m_goList[idx] == basketleft || m_goList[idx] == basketright || !m_goList[idx]->active)
			continue;
		ReturnGO(m_goList[idx]);
		m_goList[idx]->ResetValues();
	}
	if (playerwin)
		levelno++;
	else if (playerlose)
		levelno = 1;

	ballsleft = 5 * levelno;
	redbricksleft = 0;
	shootcooldown = 10; activeballs = 0; redbricksleft = 0;
	playerlose = false; playerwin = false;
	basketshotdt = 0;

	basketleft->pos.Set(m_worldWidth * 0.5 - 7, m_worldHeight * 0.05, 0);
	basketright->pos.Set(m_worldWidth * 0.5 + 7, m_worldHeight * 0.05, 0);
	basket->pos.Set(m_worldWidth * 0.5, m_worldHeight * 0.03, 0);

	if (levelno == 1)
	{
		CreateLevel(5, 5, 3);
	}
	else if (levelno == 2)
	{
		CreateLevel(10, 10, 6);
	}
}

void SceneCollision::Update(double dt)
{
	SceneBase::Update(dt);
	if (playerlose || playerwin || gameclear)
	{
		if (Application::IsKeyPressed('R') && !gameclear)
		{
			ResetLevel();
		}
		if (Application::IsKeyPressed(VK_OEM_3))
		{
			Application::SetState(1);
		}
		return;
	}

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	shootcooldown += dt;
	if (ballsleft <= 0 && redbricksleft != 0 && activeballs == 0)
		playerlose = true;
	else if (redbricksleft == 0 && activeballs == 0)
	{
		if (levelno == 2)
		{
			gameclear = true;
		}
		else
			playerwin = true;
	}

	basketshotdt -= dt;
	if (basketshotdt < 0)
	{
		basketshotdt = 0;
	}

	static bool oem_5 = false;
	if (Application::IsKeyPressed(VK_OEM_5) && !oem_5)
	{
		debug = !debug;
		oem_5 = true;
	}
	else if (!Application::IsKeyPressed(VK_OEM_5) && oem_5)
		oem_5 = false;

	static bool q = false, e = false;
	if (Application::IsKeyPressed('Q') && !q)
	{
		if (ballselected == 0)
		{
			ballselected = 2;
		}
		else
			ballselected--;
		q = true;
	}
	else if (!Application::IsKeyPressed('Q') && q)
		q = false;

	if (Application::IsKeyPressed('E') && !e)
	{
		if (ballselected == 2)
		{
			ballselected = 0;
		}
		else
			ballselected++;
		e = true;
	}
	else if (!Application::IsKeyPressed('E') && e)
		e = false;


	//update the basket
	static bool basketmovingleft = true;
	if (basketmovingleft)
	{
		//move basket left
		basket->pos += Vector3(15, 0, 0) * dt;
		basketleft->vel = Vector3(15, 0, 0);
		basketright->vel = Vector3(15, 0, 0);
		if (basketright->pos.x + basketright->scale.x > m_worldWidth)
		{
			basketmovingleft = false;
		}
	}
	else
	{
		//move basket right
		basket->pos += Vector3(-15, 0, 0) * dt;
		basketleft->vel = Vector3(-15, 0, 0);
		basketright->vel = Vector3(-15, 0, 0);

		//check if the basket has reached the edge of the screen
		if (basketleft->pos.x - basketleft->scale.x < 0)
		{
			basketmovingleft = true;
		}
	}

	
	if (debug)
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
	

	//Mouse Section
	double x, y, windowWidth, windowHeight;
	Application::GetCursorPos(&x, &y);
	windowWidth = Application::GetWindowWidth();
	windowHeight = Application::GetWindowHeight();
	Vector3 mousePos(x * (m_worldWidth / windowWidth), (windowHeight - y) * (m_worldHeight / windowHeight), 0);



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
			if (go->affectedByGravity)
				go->vel += Gravity * dt;


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
	pillar1->affectedByGravity = false;
	pillar1->disappearWhenHit = true;
	pillar1->visible = true;


	//4 pillars
	GameObject* pillar2 = FetchGO();
	pillar2->type = GameObject::GO_PILLAR;
	pillar2->color = color;
	pillar2->scale.Set(size, size, 1);
	pillar2->pos = pos + height * 0.48f * tangent - width * 0.48f * normal;
	pillar2->affectedByGravity = false;
	pillar2->disappearWhenHit = true;
	pillar2->visible = true;

	//4 pillars
	GameObject* pillar3 = FetchGO();
	pillar3->type = GameObject::GO_PILLAR;
	pillar3->color = color;
	pillar3->scale.Set(size, size, 1);
	pillar3->pos = pos - height * 0.48f * tangent - width * 0.48f * normal;
	pillar3->affectedByGravity = false;
	pillar3->disappearWhenHit = true;
	pillar3->visible = true;

	//4 pillars
	GameObject* pillar4 = FetchGO();
	pillar4->type = GameObject::GO_PILLAR;
	pillar4->color = color;
	pillar4->scale.Set(size, size, 1);
	pillar4->pos = pos - height * 0.48f * tangent + width * 0.48f * normal;
	pillar4->affectedByGravity = false;
	pillar4->disappearWhenHit = true;
	pillar4->visible = true;

	GameObject* wall1 = FetchGO();
	wall1->type = GameObject::GO_WALL;
	wall1->scale.Set(width, height, 1.f);
	wall1->normal = normal;
	wall1->color = color;
	wall1->pos = pos;
	wall1->affectedByGravity = false;
	wall1->disappearWhenHit = true;
	wall1->visible = true;

	GameObject* wall2 = FetchGO();
	wall2->type = GameObject::GO_WALL;
	wall2->scale.Set(height, width, 1.f);
	wall2->normal = tangent;
	wall2->color = color;
	wall2->pos = pos;
	wall2->visible = false;
	wall2->affectedByGravity = false;
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
		//Exercise 4: render a sphere using scale and pos
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		meshList[GEO_BALL]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
		RenderMesh(meshList[GEO_BALL], true);
		modelStack.PopMatrix();

		//Exercise 11: think of a way to give balls different colors
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
	
	//RenderMesh(meshList[GEO_AXES], false);
	if (m_ghost->active)
		RenderGO(m_ghost);


	RenderGO(m_gunbarrel);
	RenderGO(m_gunbody);
	RenderGO(basket);
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

	if (debug)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Object Count:" + std::to_string(m_objectCount), Color(0, 1, 0), 3, 0, 12);

		ss.precision(3);
		ss.str("");
		ss << "Speed: " << m_speed;
		ss << " FPS: " << fps;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 9);
	
		ss.str("");
		ss << " ACtiveBalls " << activeballs;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 15);
	}

	//render the number of balls left
	ss.str("");
	ss << "Balls Left:" << ballsleft;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

	ss.str("");
	ss << "Red Bricks left:" << redbricksleft;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);

	ss.str("");
	ss << "Level " << levelno;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 0, 56);

	ss.str("");
	ss << "Selected Ball:";
	switch (ballselected)
	{
	case 0:
		ss << "Normal, Cost:1";
		break;
	case 1:
		ss << "Homing ball, Cost:2";
		break;
	case 2:
		ss << "Explosive ball, Cost: 3";
		break;
	}

	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 0, 52);


	if (playerwin)
	{
		ss.str("");
		ss << "You cleared the red bricks, You Win!!";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 10, 24);
		ss.str("");
		ss << "Press 'R' to go to next level";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 10, 20);
		ss.str("");
		ss << "Press '~' to return to main menu";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 10, 16);
	}

	if (playerlose)
	{
		ss.str("");
		ss << "You ran out of balls, You Lose";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 4, 20, 24);
		ss.str("");
		ss << "Press 'R' to restart";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 10, 20);
		ss.str("");
		ss << "Press '~' to return to main menu";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 10, 16);
	}

	if (gameclear)
	{
		ss.str("");
		ss << "You cleared the game!";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 10, 24);
		ss.str("");
		ss << "Press '~' to return to main menu";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 10, 16);
	}

	if (basketshotdt > 0)
	{
		ss.str("");
		ss << "Awesome! Ball +1";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 20, 40);
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
	if(m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}
	if (m_gunbarrel)
	{
		delete m_gunbarrel;
		m_gunbarrel = NULL;
	}
	if (m_gunbody)
	{
		delete m_gunbody;
		m_gunbody = NULL;
	}
	if (basket)
	{
		delete basket;
		basket = NULL;
	}
}
