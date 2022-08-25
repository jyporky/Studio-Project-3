
#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue) 
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f)
{
	angle = 0;
	visible = true;
	leftwep = false;
	time2disappear = 0;
}

GameObject::~GameObject()
{
}

void GameObject::ResetValues()
{
	type = GO_BALL;
	pos.SetZero();
	vel.SetZero();
	scale.Set(1, 1, 1);
	angle = 0;
	active = false;
	visible = true;
	mass = 1;
	color.SetZero();
	normal.SetZero();
	direction.SetZero();
	leftwep = false;
	time2disappear = 0;
}
