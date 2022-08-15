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

	if (Application::IsKeyPressed('E'))
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
	
	ss.str("");
	ss << ">";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 0, 44 - menubuttonhighlight * 4);
	
	ss.str("");
	ss << "Start";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 4, 44);


	ss.str("");
	ss << "Quit";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 4, 4, 40);


	ss.str("");
	ss << "Fake Peggle";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 8, 4, 52);

}

void SceneShop::Exit()
{
	SceneBase::Exit();
}
