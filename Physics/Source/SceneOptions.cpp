#include "SceneOptions.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneOptions::SceneOptions()
{
	
}

SceneOptions::~SceneOptions()
{
}

static Vector3 RotateVector(const Vector3& vec, float radian)
{
	return Vector3(vec.x * cos(radian) + vec.y * -sin(radian),
		vec.x * sin(radian) + vec.y * cos(radian), 0.f);
}

void SceneOptions::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	bLightEnabled = true;

	Math::InitRNG();
	cSoundController = CSoundController::GetInstance();
}



void SceneOptions::Update(double dt)
{
	SceneBase::Update(dt);
	

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	static float buttonpresstimer = 0;
	buttonpresstimer += dt;
	static bool w = false, s = false;
	if (Application::IsKeyPressed('W') && !w)
	{
		w = true;
		if (menubuttonhighlight == 0)
		{
			menubuttonhighlight = 4;
		}
		else
			menubuttonhighlight--;
	}
	else if (!Application::IsKeyPressed('W') && w)
		w = false;

	if (Application::IsKeyPressed('S') && !s)
	{
		s = true;
		if (menubuttonhighlight == 4)
		{
			menubuttonhighlight = 0;
		}
		else
			menubuttonhighlight++;
	}
	else if (!Application::IsKeyPressed('S') && s)
		s = false;


	static bool aButtonState = false, dButtonState = false;
	switch (menubuttonhighlight)
	{
	case 0:
		if (Application::IsKeyPressed('A') && !aButtonState)
		{
			cSoundController->SetMasterVolume(cSoundController->GetMasterVolume() - 0.05);
			aButtonState = true;
			buttonpresstimer = 0;
		}
		else if (Application::IsKeyPressed('A') && aButtonState && buttonpresstimer >= 1)
		{
			cSoundController->SetMasterVolume(cSoundController->GetMasterVolume() - 0.25 * dt);
			aButtonState = true;
		}
		else if (!Application::IsKeyPressed('A') && aButtonState)
		{
			aButtonState = false;
			buttonpresstimer = 0;
		}


		if (Application::IsKeyPressed('D') && !dButtonState)
		{
			cSoundController->SetMasterVolume(cSoundController->GetMasterVolume() + 0.05);
			dButtonState = true;
			buttonpresstimer = 0;
		}
		else if (Application::IsKeyPressed('D') && dButtonState && buttonpresstimer >= 1)
		{
			cSoundController->SetMasterVolume(cSoundController->GetMasterVolume() + 0.25 * dt);
			dButtonState = true;
		}
		else if (!Application::IsKeyPressed('D') && dButtonState)
		{
			dButtonState = false;
			buttonpresstimer = 0;
		}
		break;
	case 1:
		if (Application::IsKeyPressed('A') && !aButtonState)
		{
			cSoundController->SetSFXVolume(cSoundController->GetSFXVolume() - 0.05);
			aButtonState = true;
			buttonpresstimer = 0;
		}
		else if (Application::IsKeyPressed('A') && aButtonState && buttonpresstimer >= 1)
		{
			cSoundController->SetSFXVolume(cSoundController->GetSFXVolume() - 0.25 * dt);
			aButtonState = true;
		}
		else if (!Application::IsKeyPressed('A') && aButtonState)
		{
			aButtonState = false;
			buttonpresstimer = 0;
		}


		if (Application::IsKeyPressed('D') && !dButtonState)
		{
			cSoundController->SetSFXVolume(cSoundController->GetSFXVolume() + 0.05);
			dButtonState = true;
			buttonpresstimer = 0;
		}
		else if (Application::IsKeyPressed('D') && dButtonState && buttonpresstimer >= 1)
		{
			cSoundController->SetSFXVolume(cSoundController->GetSFXVolume() + 0.25 * dt);
			dButtonState = true;
		}
		else if (!Application::IsKeyPressed('D') && dButtonState)
		{
			dButtonState = false;
			buttonpresstimer = 0;
		}
		break;
	case 2:
		if (Application::IsKeyPressed('A') && !aButtonState)
		{
			cSoundController->SetMusicVolume(cSoundController->GetMusicVolume() - 0.05);
			aButtonState = true;
			buttonpresstimer = 0;
		}
		else if (Application::IsKeyPressed('A') && aButtonState && buttonpresstimer >= 1)
		{
			cSoundController->SetMusicVolume(cSoundController->GetMusicVolume() - 0.25 * dt);
			aButtonState = true;
		}
		else if (!Application::IsKeyPressed('A') && aButtonState)
		{
			aButtonState = false;
			buttonpresstimer = 0;
		}


		if (Application::IsKeyPressed('D') && !dButtonState)
		{
			cSoundController->SetMusicVolume(cSoundController->GetMusicVolume() + 0.05);
			dButtonState = true;
			buttonpresstimer = 0;
		}
		else if (Application::IsKeyPressed('D') && dButtonState && buttonpresstimer >= 1)
		{
			cSoundController->SetMusicVolume(cSoundController->GetMusicVolume() + 0.25 * dt);
			dButtonState = true;
		}
		else if (!Application::IsKeyPressed('D') && dButtonState)
		{
			dButtonState = false;
			buttonpresstimer = 0;
		}
		break;
	}


	if (Application::IsKeyPressed('E') && !eButtonState)
	{
		eButtonState = true;
		switch (menubuttonhighlight)
		{
		case 3:
			Application::SetState(1);
			break;
		case 4:
			Application::SetState(2);
			break;
		}
	}
	else if (!Application::IsKeyPressed('E') && eButtonState)
		eButtonState = false;
	return;
}


void SceneOptions::Render()
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
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 1, 44 - menubuttonhighlight * 4);
	
	ss.str("");
	ss.precision(3);
	ss << "Master Volume: " << cSoundController->GetMasterVolume() * 10;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 4, 44);

	ss.str("");
	ss.precision(3);
	ss << "SFX Volume: " << cSoundController->GetSFXVolume() * 10;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 4, 40);

	ss.str("");
	ss.precision(3);
	ss << "Music Volume: " << cSoundController->GetMusicVolume() * 10;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 4, 36);

	ss.str("");
	ss << "Return to Main Menu";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 4, 32);

	ss.str("");
	ss << "Resume Game";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 4, 28);

	ss.str("");
	ss << "Options";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 8, 4, 52);


	ss.str("");
	ss << "Controls";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 8, 45, 52);

	ss.str("");
	ss << "Movement: WASD";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 45, 44);

	ss.str("");
	ss << "Look: Mouse";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 45, 40);

	ss.str("");
	ss << "Use Weapon: Left CLick";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 45, 36);

	ss.str("");
	ss << "Swap Weapon: Right Click";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 45, 32);

	ss.str("");
	ss << "Use Potions: 1 / 2 / 3";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 45, 28);

	ss.str("");
	ss << "Interact / Buy: E";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 45, 24);

	ss.str("");
	ss << "Pause Menu: `";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 45, 20);

	ss.str("");
	ss << "Use Skill: Q";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 45, 16);
}

void SceneOptions::Exit()
{
	SceneBase::Exit();
}

void SceneOptions::CreateBullet(Vector3 pos, Vector3 direction)
{
}
