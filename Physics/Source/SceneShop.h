#ifndef SCENE_SHOP_H
#define SCENE_SHOP_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "SoundController.h"
#include "InventoryManager.h"
#include "Player.h"

#include "Potions.h"
#include "StrengthPotion.h"
#include "HealthPotion.h"
#include "SpeedPotion.h"

#include "Weapon.h"

#include "Sword.h"
#include "BoxingGloves.h"
#include "RubberChicken.h"
#include "FryingPan.h"

#include "Rifle.h"
#include "Flamethrower.h"
#include "Crossbow.h"

#include "Skill.h"
#include "EMPSkill.h"
#include "HackSkill.h"
#include "HealSkill.h"
#include "ImmortalSkill.h"
#include "OverdriveSkill.h"

#include "SelfUpgrades.h"
#include "HealthUpgrade.h"
#include "MovementSpeedUpgrade.h"
#include "MeleeDmgUpgrade.h"
#include "RangedDmgUpgrade.h"

#include "WeaponModifiers.h"
#include "AccurateArrowsMod.h"
#include "BetterFuelMod.h"
#include "ExplosiveBulletsMod.h"
#include "FasterBulletSpeedMod.h"
#include "FasterFiringRateMod.h"
#include "PiercingBulletMod.h"
#include "FasterMeleeMod.h"


class SceneShop : public SceneBase
{
public:
	SceneShop();
	~SceneShop();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual void CreateBullet(Vector3 pos, Vector3 direction);
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
	char playerUpgradeType;
	int weaponUpgradePage;

	bool ShopMenu1, ShopMenu2, ShopMenu3, ShopMenu4;

	unsigned shopbuttonhighlight = 0;

	//inventory
	CInventoryManager* cInventoryManager;
	CInventoryItem* cInventoryItem;

	//potions
	Potions* StrengthPotion = new StrengthPot();
	Potions* HealthPotion = new HealthPot();
	Potions* SpeedPotion = new SpeedPot();

	//weapons
	Weapon* sword = new Sword();
	Weapon* boxingGlove = new BoxingGloves();
	Weapon* rubberchicken = new RubberChicken();
	Weapon* pan = new FryingPan();

	Weapon* rifle = new Rifle();
	Weapon* flamethrower = new Flamethrower();
	Weapon* crossbow = new Crossbow();

	bool swordBought;
	bool boxingGloveBought;
	bool rubberchickenBought;
	bool panBought;

	bool rifleBought;
	bool flamethrowerBought;
	bool crossbowBought;

	//skills
	Skill* emp = new EMP();
	Skill* hack = new Hack();
	Skill* heal = new Heal();
	Skill* immortal = new Immortal();
	Skill* overdrive = new Overdrive();

	bool empBought;
	bool hackBought;
	bool healBought;
	bool immortalBought;
	bool overdriveBought;

	//self upgrades
	SelfUpgrades* movementspeedupgrade = new MovementSpeedUpgrade();
	SelfUpgrades* healthupgrade = new HealthUpgrade();
	SelfUpgrades* meleedmgupgrade = new MeleeDmgUpgrade();
	SelfUpgrades* rangeddmgupgrade = new RangedDmgUpgrade();

	//weapon upgrades
	WeaponMods* PierceMod = new PiercingBulletMod();
	WeaponMods* FasterFiringMod = new FasterFiringRateMod();
	WeaponMods* FasterBulletMod = new FasterBulletSpeedMod();
	WeaponMods* ExplosiveMod = new ExplosiveBulletsMod();
	WeaponMods* betterFuelMod = new BetterFuelMod();
	WeaponMods* accurateArrowsMod = new AccurateArrowsMod();
	WeaponMods* fasterMeleeMod = new FasterMeleeMod();
};

#endif