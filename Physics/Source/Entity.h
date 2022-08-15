#pragma once
#include "SingletonTemplate.h"'
#include "Vector3.h"

class Entity
{
public:
	Entity();
	~Entity();
protected:
	Vector3 position;
};

