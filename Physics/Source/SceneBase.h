#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "GameObject.h"
#include <vector>

class SceneBase : public Scene
{
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_TEXT,
		GEO_HEALTH_UI_BASE,
		GEO_HEALTH_UI_RED,
		GEO_BALL,
		GEO_CUBE,

		GEO_BULLET,
		GEO_FLAME,
		GEO_SHOPMENUBG,
		GEO_SANDBG,
		GEO_HOTBAR,
		GEO_HOTBAR_SELECTED,

		GEO_TELEPORT_PAD,

		// Weapons
		GEO_SWORDL,
		GEO_SWORDR,
		GEO_BOXINGGLOVE,
		GEO_CHICKEN,
		GEO_PAN,

		GEO_RIFLE_LEFT,
		GEO_RIFLE_RIGHT,
		GEO_FLAMETHROWER,
		GEO_FLAMETHROWER_LEFT,
		GEO_SHIELD,
		GEO_CROSSBOW,

		//skills
		GEO_EMP,
		GEO_HACK,
		GEO_HEAL,
		GEO_IMMORTAL,
		GEO_OVERDRIVE,

		//potions
		GEO_SPEEDPOT,
		GEO_STRENGTHPOT,
		GEO_HEALTHPOT,

		//self upgrades
		GEO_SPEEDUP,
		GEO_HEALTHUP,
		GEO_MELEEUP,
		GEO_RANGEDUP,

		//weapon upgrades
		GEO_PIERCINGBULLETS,
		GEO_FASTERFIRING,
		GEO_FASTERBULLET,
		GEO_EXPLOSIVEBULLET,
		GEO_BETTERFUEL,
		GEO_ACCURATEARROWS,
		GEO_FASTERMELEE,

		//NPCs
		GEO_BLACKSMITH, //sell weapon upgrades
		GEO_PARTDEALER, //sell upgrades to player and skills
		GEO_ALCHEMIST, //sell potions
		GEO_WEAPONDEALER, //sell weapons
		
		//player
		GEO_LEFT_PLAYER,
		GEO_RIGHT_PLAYER,

		//enemies
		GEO_LEFT_SWORDSMAN,
		GEO_RIGHT_SWORDSMAN,
		GEO_LEFT_RIFLER,
		GEO_RIGHT_RIFLER,
		NUM_GEOMETRY,
	};
public:
	SceneBase();
	~SceneBase();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderMeshOnScreen(Mesh* mesh, double x, double y, double sizex, double sizey);
	void RenderGO(GameObject *go);
	static bool eButtonState;

	GameObject* FetchGO();
protected:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera camera;
	Vector3 color;
	MS modelStack;
	MS viewStack;
	MS projectionStack;
	Light lights[1];

	bool bLightEnabled;

	float fps;
	int textMaxWidth;
	int textSpacing[256];
};

#endif