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

	canInteract = false; //with shop
	canLeave = false; //the shop scene

	inShop = false;

	ShopMenu1 = false;
	ShopMenu2 = false;
	ShopMenu3 = false;
	ShopMenu4 = false;

	weaponType = 'm';
}



void SceneShop::Update(double dt)
{
	SceneBase::Update(dt);
	

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// Moving of player
	Vector3 movementDirection;
	movementDirection.SetZero();

	if (inShop == false)
	{
		if (Application::IsKeyPressed('W'))
		{
			if (m_player->pos.y <= 70)
			{
				movementDirection.y += 1;
			}
		}

		if (Application::IsKeyPressed('S'))
		{
			if (m_player->pos.y >= 12)
			{
				movementDirection.y -= 1;
			}
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

		
	}

	if ((m_player->pos.x >= 70) && (m_player->pos.x <= 100) && (m_player->pos.y <= 12))
	{
		canLeave = true;
		if (Application::IsKeyPressed('E'))
		{
			Application::SetState(2);
		}
	}
	else
	{
		canLeave = false;
	}

	if ((m_player->pos.x >= 29) && (m_player->pos.x <= 43) && (m_player->pos.y >= 66)) //part dealer
	{
		canInteract = true;
		if (Application::IsKeyPressed('E'))
		{
			inShop = true;
			ShopMenu1 = true;
		}
	}
	else if ((m_player->pos.x >= 64) && (m_player->pos.x <= 78) && (m_player->pos.y >= 66)) //weapon dealer
	{
		canInteract = true;
		if (Application::IsKeyPressed('E'))
		{
			inShop = true;
			
			
			ShopMenu2 = true;
		}
	}
	else if ((m_player->pos.x >= 100) && (m_player->pos.x <= 114) && (m_player->pos.y >= 66)) //blacksmith
	{
		canInteract = true;
		if (Application::IsKeyPressed('E'))
		{
			inShop = true;
			ShopMenu3 = true;
		}
	}
	else if ((m_player->pos.x >= 135) && (m_player->pos.x <= 149) && (m_player->pos.y >= 66)) //alchemist
	{
		canInteract = true;
		if (Application::IsKeyPressed('E'))
		{
			inShop = true;
			ShopMenu4 = true;
		}
	}
	else
	{
		canInteract = false;
		
	}

	if((inShop) && (Application::IsKeyPressed('R')))
	{
		inShop = false;
		ShopMenu1 = false;
		ShopMenu2 = false;
		ShopMenu3 = false;
		ShopMenu4 = false;
	}

	if (ShopMenu2 == true)
	{
		if (Application::IsKeyPressed('Z'))
		{
			weaponType = 'm';
		}
		else if (Application::IsKeyPressed('X'))
		{
			weaponType = 'r';
		}
	}

	m_player->pos += movementDirection.Normalize() * 40 * dt;

	m_player = Checkborder(m_player);

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
	

	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_SANDBG], false);
	modelStack.PopMatrix();

	//NPCs
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth / 5, 80, 1);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(15, 15, 1);
	RenderMesh(meshList[GEO_PARTDEALER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth * 2 / 5, 80, 1);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(15, 15, 1);
	RenderMesh(meshList[GEO_WEAPONDEALER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth * 3 / 5, 80, 1);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(15, 15, 1);
	RenderMesh(meshList[GEO_BLACKSMITH], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth * 4 / 5, 80, 1);
	modelStack.Rotate(180, 0, 0, 2);
	modelStack.Scale(15, 15, 1);
	RenderMesh(meshList[GEO_ALCHEMIST], false);
	modelStack.PopMatrix();

	renderEnvironment();
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



	if (ShopMenu1)
	{
		renderShopMenu1();
	}
	else if (ShopMenu2)
	{
		renderShopMenu2();
	}
	else if (ShopMenu3)
	{
		renderShopMenu3();
	}
	else if (ShopMenu4)
	{
		renderShopMenu4();
	}


	//On screen text
	std::ostringstream ss;


	ss.str("");
	ss << "Parts Dealer ";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 10, 55);

	ss.str("");
	ss << "Weapons Dealer ";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 25, 55);

	ss.str("");
	ss << "Blacksmith ";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 43, 55);

	ss.str("");
	ss << "Alchemist ";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 60, 55);

	if ((canInteract) && (inShop == false))
	{
		//On screen text
		std::ostringstream ss;

		ss.str("");
		ss << "Press [E] to interact";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.2, 0.2, 0.2), 3, 30, 8);
	}

	if (canLeave)
	{
		//On screen text
		std::ostringstream ss;

		ss.str("");
		ss << "Press [E] to leave shop";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.2, 0.2, 0.2), 3, 30, 8);
	}

	ss.str("");
	ss << m_player->pos.x;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 30, 17);
}



void SceneShop::Exit()
{
	SceneBase::Exit();
}

void SceneShop::renderEnvironment()
{
	//top
	modelStack.PushMatrix();
	modelStack.Translate(90, 95, 1);
	modelStack.Scale(m_worldWidth + 12, 10, 1);
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
	modelStack.Translate(m_worldWidth - 1, 50, 1);
	modelStack.Scale(5, m_worldHeight, 1);
	RenderMesh(meshList[GEO_CUBE], false);
	modelStack.PopMatrix();

	//bottom left
	modelStack.PushMatrix();
	modelStack.Translate(30, 3, 1);
	modelStack.Scale(m_worldWidth / 2 - 10, 8, 1);
	RenderMesh(meshList[GEO_CUBE], false);
	modelStack.PopMatrix();

	//bottom right
	modelStack.PushMatrix();
	modelStack.Translate(140, 3, 1);
	modelStack.Scale(m_worldWidth / 2 - 10, 8, 1);
	RenderMesh(meshList[GEO_CUBE], false);
	modelStack.PopMatrix();

	//shop counter
	modelStack.PushMatrix();
	modelStack.Translate(88, 72, 1);
	modelStack.Scale(m_worldWidth + 10, 5, 1);
	RenderMesh(meshList[GEO_CUBE], false);
	modelStack.PopMatrix();
}

void SceneShop::renderShopMenu1()
{
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth / 2, 30, 1);
	modelStack.Scale(m_worldWidth - 5, 55, 1);
	RenderMesh(meshList[GEO_SHOPMENUBG], false);
	modelStack.PopMatrix();

	std::ostringstream ss;

	ss.str("");
	ss << "Upgrades to Player";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 4, 30.5);

	ss.str("");
	ss << "[R]";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 75, 31);

	ss.str("");
	ss << "X";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 3, 77, 30.5);
}

void SceneShop::renderShopMenu2()
{
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth / 2, 30, 1);
	modelStack.Scale(m_worldWidth - 5, 55, 1);
	RenderMesh(meshList[GEO_SHOPMENUBG], false);
	modelStack.PopMatrix();

	std::ostringstream ss;

	if (weaponType == 'm')
	{
		ss.str("");
		ss << "Melee Weapons";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 4, 30.5);
	}
	else if (weaponType == 'r')
	{
		ss.str("");
		ss << "Ranged Weapons";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 4, 30.5);
	}

	ss.str("");
	ss << "[Z] / [X]";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 20, 30.5);

	ss.str("");
	ss << "[R]";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 75, 31);

	ss.str("");
	ss << "X";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 3, 77, 30.5);
}

void SceneShop::renderShopMenu3()
{
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth / 2, 30, 1);
	modelStack.Scale(m_worldWidth - 5, 55, 1);
	RenderMesh(meshList[GEO_SHOPMENUBG], false);
	modelStack.PopMatrix();

	std::ostringstream ss;

	ss.str("");
	ss << "Weapon Upgrades";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 4, 30.5);

	ss.str("");
	ss << "[R]";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 75, 31);

	ss.str("");
	ss << "X";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 3, 77, 30.5);
}
void SceneShop::renderShopMenu4()
{
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth / 2, 30, 1);
	modelStack.Scale(m_worldWidth - 5, 55, 1);
	RenderMesh(meshList[GEO_SHOPMENUBG], false);
	modelStack.PopMatrix();

	std::ostringstream ss;

	ss.str("");
	ss << "Potions";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 4, 30.5);

	ss.str("");
	ss << "[R]";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 75, 31);

	ss.str("");
	ss << "X";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 3, 77, 30.5);
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