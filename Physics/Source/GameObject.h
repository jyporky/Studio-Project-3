#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include <vector>

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CUBE,
		GO_PILLAR,
		GO_WALL,
		GO_PLAYER,
		GO_SWORD,
		GO_SWORDSMAN,
		GO_BULLET,
		GO_FLAME,
		GO_SHIELD,
		GO_SHIELDMAN,
		GO_FLAMETHROWER,
		GO_RIFLER,
		GO_RIFLE,
		GO_CROSSBOW,
		GO_ARROW,
		GO_TOTAL, //must be last
	};
	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	float angle;
	bool active;
	bool visible;
	float mass;
	Vector3 color;
	Vector3 normal;
	bool leftwep;

	//Rotation variables
	Vector3 direction;
	float momentOfInertia;
	float angularVelocity;

	std::vector <GameObject*> otherGameObjects;

	bool checkCollision;
	bool disappearWhenHit;
	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();

	void ResetValues();
};

#endif