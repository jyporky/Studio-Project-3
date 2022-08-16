#ifndef SCENE_SHOP_H
#define SCENE_SHOP_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Player.h"

#include "Potions.h"
#include "StrengthPotion.h"
#include "HealthPotion.h"
#include "SpeedPotion.h"

class SceneShop : public SceneBase
{
public:
	SceneShop();
	~SceneShop();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	GameObject* Checkborder(GameObject* go);
	void renderEnvironment();

	void renderShopMenu1(); //part dealer
	void renderShopMenu2(); //weapon dealer
	void renderShopMenu3(); //blacksmith
	void renderShopMenu4(); //alchemist

protected:
	Player* player;
	float m_worldWidth;
	float m_worldHeight;
	GameObject* m_player;
	unsigned menubuttonhighlight = 0;

	bool canInteract;
	bool canLeave;
	bool inShop;

	char weaponType;

	bool ShopMenu1, ShopMenu2, ShopMenu3, ShopMenu4;

	Potions* StrengthPotion = new StrengthPot();
	Potions* HealthPotion = new HealthPot();
	Potions* SpeedPotion = new SpeedPot();
};

#endif