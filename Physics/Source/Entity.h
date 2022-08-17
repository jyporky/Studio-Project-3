#pragma once
#include "SingletonTemplate.h"'
#include "Vector3.h"
#include "GameObject.h"
#include "Weapon.h"
class Entity
{
public:
	Entity();
	~Entity();
	virtual bool Update(double dt);
	virtual void Init();
	virtual void SetGameObject(GameObject* GO);
	virtual GameObject* GetGameObject();
	virtual bool ChangeHealth(int changeInHealth);
	virtual Weapon* GetWeapon();
	virtual void SetWeapon(Weapon* weapon);
protected:
	GameObject* gameobject;
	Weapon* CurrWeapon;
};

