#include "SceneBase.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "LoadTexture.h"
#include <sstream>
#include <fstream>

bool SceneBase::eButtonState = false;

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init()
{
	textMaxWidth = 64; //initial value
	std::ifstream fileStream;
	//for text spacing (check)
	const char* file_path;
	file_path = "Image//FontData.csv"; //name of font data csv file
	fileStream.open(file_path, std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cout << "Impossible to open " << file_path << ". Are you in the right directory ?\n";
	}
	else {
		while (!fileStream.eof()) {
			std::string buf;
			std::getline(fileStream, buf, ' ');
			if (buf == "Char") {
				std::getline(fileStream, buf, ' ');
				unsigned i = stoi(buf);
				std::getline(fileStream, buf, ',');
				if (buf == "Base Width") {
					std::getline(fileStream, buf);
					unsigned width = stoi(buf);
					textSpacing[i] = width;
				}
				else {
					std::getline(fileStream, buf);
				}
			}
			else if (buf == "Cell") {
				std::getline(fileStream, buf, ',');
				if (buf == "Width") {
					std::getline(fileStream, buf);
					textMaxWidth = stoi(buf);
				}
				else {
					std::getline(fileStream, buf);
				}
			}
			else {
				std::getline(fileStream, buf);
			}
		}
	}
	fileStream.close();

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders( "Shader//comg.vertexshader", "Shader//comg.fragmentshader" );
	
	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	
	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);
	
	glUniform1i(m_parameters[U_NUMLIGHTS], 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_BALL] = MeshBuilder::GenerateSphere("ball", Color(1, 1, 1), 10, 10, 1.f);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(0.4, 0.4, 0.3), 1.f);
	
	meshList[GEO_SHOPMENUBG] = MeshBuilder::GenerateCube("shopmenubg", Color(0.3, 0.3, 0.3), 1.f);

	meshList[GEO_HEALTH_UI_BASE] = MeshBuilder::GenerateQuad("healthui", Color(1, 1, 1));
	meshList[GEO_HEALTH_UI_RED] = MeshBuilder::GenerateQuad("healthuired", Color(1, 0, 0));

	meshList[GEO_SANDBG] = MeshBuilder::GenerateQuad("sand bg", Color(1, 1, 1));
	meshList[GEO_SANDBG]->textureID = LoadTGA("Image//sand.tga");	

	meshList[GEO_HOTBAR] = MeshBuilder::GenerateQuad("hotbar", Color(1, 1, 1));
	meshList[GEO_HOTBAR]->textureID = LoadTGA("Image//hotbar.tga");	

	meshList[GEO_HOTBAR_SELECTED] = MeshBuilder::GenerateQuad("hotbar selected", Color(1, 1, 1));
	meshList[GEO_HOTBAR_SELECTED]->textureID = LoadTGA("Image//hotbar_selected.tga");

	meshList[GEO_LEFT_PLAYER] = MeshBuilder::GenerateQuad("player", Color(1, 1, 1));
	meshList[GEO_LEFT_PLAYER]->textureID = LoadTGA("Image//playerleft.tga");	
	
	meshList[GEO_RIGHT_PLAYER] = MeshBuilder::GenerateQuad("player", Color(1, 1, 1));
	meshList[GEO_RIGHT_PLAYER]->textureID = LoadTGA("Image//playerright.tga");

	meshList[GEO_LEFT_SWORDSMAN] = MeshBuilder::GenerateQuad("swordmanL", Color(1, 1, 1));
	meshList[GEO_LEFT_SWORDSMAN]->textureID = LoadTGA("Image//swordsmanL.tga");

	meshList[GEO_RIGHT_SWORDSMAN] = MeshBuilder::GenerateQuad("swordmanR", Color(1, 1, 1));
	meshList[GEO_RIGHT_SWORDSMAN]->textureID = LoadTGA("Image//swordsmanR.tga");

	meshList[GEO_LEFT_RIFLER] = MeshBuilder::GenerateQuad("riflerL", Color(1, 1, 1));
	meshList[GEO_LEFT_RIFLER]->textureID = LoadTGA("Image//riflerL.tga");

	meshList[GEO_RIGHT_RIFLER] = MeshBuilder::GenerateQuad("riflerR", Color(1, 1, 1));
	meshList[GEO_RIGHT_RIFLER]->textureID = LoadTGA("Image//riflerR.tga");

	meshList[GEO_BULLET] = MeshBuilder::GenerateQuad("bullet", Color(1, 1, 1));
	meshList[GEO_BULLET]->textureID = LoadTGA("Image//bullet.tga");

	meshList[GEO_FLAME] = MeshBuilder::GenerateQuad("flame", Color(1, 1, 1));
	meshList[GEO_FLAME]->textureID = LoadTGA("Image//FlameParticle.tga");

	//weapons
	meshList[GEO_SWORDL] = MeshBuilder::GenerateQuad("sword", Color(1, 1, 1));
	meshList[GEO_SWORDL]->textureID = LoadTGA("Image//swordleft.tga");	
	meshList[GEO_SWORDR] = MeshBuilder::GenerateQuad("sword", Color(1, 1, 1));
	meshList[GEO_SWORDR]->textureID = LoadTGA("Image//swordright.tga");

	meshList[GEO_BOXINGGLOVE] = MeshBuilder::GenerateQuad("boxing glove", Color(1, 1, 1));
	meshList[GEO_BOXINGGLOVE]->textureID = LoadTexture("Image//boxing_glove.png");

	meshList[GEO_CHICKEN] = MeshBuilder::GenerateQuad("chicken", Color(1, 1, 1));
	meshList[GEO_CHICKEN]->textureID = LoadTexture("Image//chicken.png");

	meshList[GEO_PAN] = MeshBuilder::GenerateQuad("frying pan", Color(1, 1, 1));
	meshList[GEO_PAN]->textureID = LoadTexture("Image//pan.png");

	meshList[GEO_RIFLE_LEFT] = MeshBuilder::GenerateQuad("rifleL", Color(1, 1, 1));
	meshList[GEO_RIFLE_LEFT]->textureID = LoadTGA("Image//ar_left.tga");

	meshList[GEO_RIFLE_RIGHT] = MeshBuilder::GenerateQuad("rifleR", Color(1, 1, 1));
	meshList[GEO_RIFLE_RIGHT]->textureID = LoadTGA("Image//ar_right.tga");

	meshList[GEO_FLAMETHROWER] = MeshBuilder::GenerateQuad("flamethrower", Color(1, 1, 1));
	meshList[GEO_FLAMETHROWER]->textureID = LoadTexture("Image//flamethrower.png");

	meshList[GEO_FLAMETHROWER_LEFT] = MeshBuilder::GenerateQuad("flamethrower left", Color(1, 1, 1));
	meshList[GEO_FLAMETHROWER_LEFT]->textureID = LoadTexture("Image//flamethrowerleft.png");

	meshList[GEO_SHIELD] = MeshBuilder::GenerateQuad("shield", Color(1, 1, 1));
	meshList[GEO_SHIELD]->textureID = LoadTexture("Image//shield.png");

	meshList[GEO_CROSSBOW] = MeshBuilder::GenerateQuad("crossbow", Color(1, 1, 1));
	meshList[GEO_CROSSBOW]->textureID = LoadTexture("Image//crossbow.png");

	meshList[GEO_CROSSBOW_LEFT] = MeshBuilder::GenerateQuad("crossbowL", Color(1, 1, 1));
	meshList[GEO_CROSSBOW_LEFT]->textureID = LoadTGA("Image//crossbow_left.tga");

	meshList[GEO_CROSSBOW_RIGHT] = MeshBuilder::GenerateQuad("crossbowR", Color(1, 1, 1));
	meshList[GEO_CROSSBOW_RIGHT]->textureID = LoadTGA("Image//crossbow_right.tga");

	meshList[GEO_ARROW] = MeshBuilder::GenerateQuad("arrow", Color(1, 1, 1));
	meshList[GEO_ARROW]->textureID = LoadTGA("Image//arrow.tga");

	//skills
	meshList[GEO_EMP] = MeshBuilder::GenerateQuad("emp", Color(1, 1, 1));
	meshList[GEO_EMP]->textureID = LoadTexture("Image//emp.png");

	meshList[GEO_HACK] = MeshBuilder::GenerateQuad("hack", Color(1, 1, 1));
	meshList[GEO_HACK]->textureID = LoadTexture("Image//hack.png");

	meshList[GEO_HEAL] = MeshBuilder::GenerateQuad("heal", Color(1, 1, 1));
	meshList[GEO_HEAL]->textureID = LoadTexture("Image//heal.png");

	meshList[GEO_IMMORTAL] = MeshBuilder::GenerateQuad("immortal", Color(1, 1, 1));
	meshList[GEO_IMMORTAL]->textureID = LoadTexture("Image//immortal.png");

	meshList[GEO_OVERDRIVE] = MeshBuilder::GenerateQuad("overdrive", Color(1, 1, 1));
	meshList[GEO_OVERDRIVE]->textureID = LoadTexture("Image//overdrive.png");

	//self upgrades
	meshList[GEO_SPEEDUP] = MeshBuilder::GenerateQuad("speedup", Color(1, 1, 1));
	meshList[GEO_SPEEDUP]->textureID = LoadTexture("Image//speedup.png");

	meshList[GEO_HEALTHUP] = MeshBuilder::GenerateQuad("healthup", Color(1, 1, 1));
	meshList[GEO_HEALTHUP]->textureID = LoadTexture("Image//healthup.png");

	meshList[GEO_MELEEUP] = MeshBuilder::GenerateQuad("melee up", Color(1, 1, 1));
	meshList[GEO_MELEEUP]->textureID = LoadTexture("Image//meleeup.png");

	meshList[GEO_RANGEDUP] = MeshBuilder::GenerateQuad("ranged up", Color(1, 1, 1));
	meshList[GEO_RANGEDUP]->textureID = LoadTexture("Image//rangedup.png");

	//weapon upgrades
	meshList[GEO_PIERCINGBULLETS] = MeshBuilder::GenerateQuad("piercing bullets", Color(1, 1, 1));
	meshList[GEO_PIERCINGBULLETS]->textureID = LoadTexture("Image//piercing_bullets.png");

	meshList[GEO_FASTERFIRING] = MeshBuilder::GenerateQuad("faster firing rate", Color(1, 1, 1));
	meshList[GEO_FASTERFIRING]->textureID = LoadTexture("Image//faster_firing_rate.png");

	meshList[GEO_FASTERBULLET] = MeshBuilder::GenerateQuad("faster bullet speed", Color(1, 1, 1));
	meshList[GEO_FASTERBULLET]->textureID = LoadTexture("Image//faster_bullet_speed.png");

	meshList[GEO_EXPLOSIVEBULLET] = MeshBuilder::GenerateQuad("explosive bullet", Color(1, 1, 1));
	meshList[GEO_EXPLOSIVEBULLET]->textureID = LoadTexture("Image//explosive_bullets.png");

	meshList[GEO_BETTERFUEL] = MeshBuilder::GenerateQuad("better fuel", Color(1, 1, 1));
	meshList[GEO_BETTERFUEL]->textureID = LoadTexture("Image//better_fuel.png");

	meshList[GEO_ACCURATEARROWS] = MeshBuilder::GenerateQuad("accurate arrows", Color(1, 1, 1));
	meshList[GEO_ACCURATEARROWS]->textureID = LoadTexture("Image//accurate_arrows.png");

	meshList[GEO_FASTERMELEE] = MeshBuilder::GenerateQuad("faster melee", Color(1, 1, 1));
	meshList[GEO_FASTERMELEE]->textureID = LoadTexture("Image//faster_melee_speed.png");

	//NPCs
	meshList[GEO_BLACKSMITH] = MeshBuilder::GenerateQuad("blacksmith", Color(1, 1, 1));
	meshList[GEO_BLACKSMITH]->textureID = LoadTexture("Image//blacksmith.png");

	meshList[GEO_PARTDEALER] = MeshBuilder::GenerateQuad("partdealer", Color(1, 1, 1));
	meshList[GEO_PARTDEALER]->textureID = LoadTexture("Image//partdealer.png");

	meshList[GEO_WEAPONDEALER] = MeshBuilder::GenerateQuad("weapondealer", Color(1, 1, 1));
	meshList[GEO_WEAPONDEALER]->textureID = LoadTexture("Image//weapondealer.png");

	meshList[GEO_ALCHEMIST] = MeshBuilder::GenerateQuad("alchemist", Color(1, 1, 1));
	meshList[GEO_ALCHEMIST]->textureID = LoadTexture("Image//alchemist.png");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//DimboFont.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);


	//potions
	meshList[GEO_SPEEDPOT] = MeshBuilder::GenerateQuad("speedpot", Color(1, 1, 1));
	meshList[GEO_SPEEDPOT]->textureID = LoadTexture("Image//speed potion.png");

	meshList[GEO_HEALTHPOT] = MeshBuilder::GenerateQuad("healthpot", Color(1, 1, 1));
	meshList[GEO_HEALTHPOT]->textureID = LoadTexture("Image//health potion.png");

	meshList[GEO_STRENGTHPOT] = MeshBuilder::GenerateQuad("strenghtpot", Color(1, 1, 1));
	meshList[GEO_STRENGTHPOT]->textureID = LoadTexture("Image//strength potion.png");


	bLightEnabled = false;
	eButtonState = false;
}

void SceneBase::Update(double dt)
{
	//Keyboard Section
	if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	fps = (float)(1.f / dt);
}

void SceneBase::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	float offset = 0;
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(0.5f + offset, 0.5f, 0);
		offset += (textSpacing[(int)(text[i])] / (float)textMaxWidth);

		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderMesh(Mesh *mesh, bool enableLight)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);
		
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderMeshOnScreen(Mesh* mesh, double x, double y, double sizex, double sizey)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizex, sizey, 1);
	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}


void SceneBase::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SceneBase::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}

