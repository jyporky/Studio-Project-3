#include "SceneShop.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>


SceneShop::SceneShop()
{
	
}

SceneShop::~SceneShop()
{
}

static Vector3 RotateVector(const Vector3& vec, float radian)
{
	return Vector3(vec.x * cos(radian) + vec.y * -sin(radian),
		vec.x * sin(radian) + vec.y * cos(radian), 0.f);
}

void SceneShop::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	bLightEnabled = true;


	

	player = Player::GetInstance();
	m_player = player->getPlayer();
	player->SetGameObject(m_player);

	Math::InitRNG();

}



void SceneShop::Update(double dt)
{
	SceneBase::Update(dt);
	

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	static bool w = false, s = false;
	if (Application::IsKeyPressed('W') && !w)
	{
		w = true;
		if (menubuttonhighlight == 0)
		{
			menubuttonhighlight = 1;
		}
		else
			menubuttonhighlight--;
	}
	else if (!Application::IsKeyPressed('W') && w)
		w = false;

	if (Application::IsKeyPressed('S') && !s)
	{
		s = true;
		if (menubuttonhighlight == 1)
		{
			menubuttonhighlight = 0;
		}
		else
			menubuttonhighlight++;
	}
	else if (!Application::IsKeyPressed('S') && s)
		s = false;

	if (Application::IsKeyPressed('P'))
	{
		switch (menubuttonhighlight)
		{
		case 0:
			Application::SetState(2);
			break;
		case 1:
			Application::SetState(0);
			break;
		}
	}	

	// Moving of player
	Vector3 movementDirection;
	movementDirection.SetZero();
	if (Application::IsKeyPressed('W'))
	{
		movementDirection.y += 1;
	}

	if (Application::IsKeyPressed('S'))
	{
		movementDirection.y -= 1;
	}

	if (Application::IsKeyPressed('A'))
	{
		movementDirection.x -= 1;
	}

	if (Application::IsKeyPressed('D'))
	{
		movementDirection.x += 1;
	}

	if (movementDirection.x > 0)
	{
		m_player->angle = 0;
	}

	else if (movementDirection.x < 0)
	{
		m_player->angle = 180;
	}

	m_player->pos += movementDirection.Normalize() * 40 * dt;

	m_player = Checkborder(m_player);

	// Put this after all changes is made to player
	player->SetGameObject(m_player);
	return;


	

}


void SceneShop::Render()
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
	


	//On screen text
	std::ostringstream ss;
	

	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_SANDBG], false);
	modelStack.PopMatrix();


	//player
	modelStack.PushMatrix();
	modelStack.Translate(m_player->pos.x, m_player->pos.y, m_player->pos.z);
	modelStack.Scale(m_player->scale.x, m_player->scale.y, m_player->scale.z);
	if (m_player->angle == 180)
	{
		meshList[GEO_LEFT_PLAYER]->material.kAmbient.Set(m_player->color.x, m_player->color.y, m_player->color.z);
		RenderMesh(meshList[GEO_LEFT_PLAYER], true);
	}

	else if (m_player->angle == 0)
	{
		meshList[GEO_RIGHT_PLAYER]->material.kAmbient.Set(m_player->color.x, m_player->color.y, m_player->color.z);
		RenderMesh(meshList[GEO_RIGHT_PLAYER], true);
	}
	modelStack.PopMatrix();

	//NPCs

	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth/2, 80, 1);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(15, 15, 1);
	RenderMesh(meshList[GEO_BLACKSMITH], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth / 2 - 40, 75, 1);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(15, 15, 1);
	RenderMesh(meshList[GEO_PARTDEALER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth / 2 + 40, 75, 1);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(15, 15, 1);
	RenderMesh(meshList[GEO_ALCHEMIST], false);
	modelStack.PopMatrix();

	renderEnvironment();



	ss.str("");
	ss << "Shop ";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 3, 55);
}



void SceneShop::Exit()
{
	SceneBase::Exit();
}

void SceneShop::renderEnvironment()
{
	//top
	modelStack.PushMatrix();
	modelStack.Translate(88, 98, 1);
	modelStack.Scale(m_worldWidth +10, 5, 1);
	RenderMesh(meshList[GEO_CUBE], false);
	modelStack.PopMatrix();

	//left
	modelStack.PushMatrix();
	modelStack.Translate(2, 50, 1);
	modelStack.Scale(5, m_worldHeight, 1);
	RenderMesh(meshList[GEO_CUBE], false);
	modelStack.PopMatrix();

	//right
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth - 3, 50, 1);
	modelStack.Scale(5, m_worldHeight, 1);
	RenderMesh(meshList[GEO_CUBE], false);
	modelStack.PopMatrix();
}


GameObject* SceneShop::Checkborder(GameObject* go)
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