#pragma once
#include "Entity.h"
class Enemy : public Entity
{
public:
	Enemy();
	~Enemy();
	virtual bool Update();
	virtual bool ChangeHealth(int changeInHealth);
private:
	unsigned health;
	float redTimer;
};

