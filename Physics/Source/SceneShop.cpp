#include "SceneShop.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>


SceneShop::SceneShop()
{
	
}

SceneShop::~SceneShop()
{
	
}


float Clamp2(float value, float minvalue, float maxvalue)
{
	if (value > maxvalue)
		return maxvalue;
	else if (value < minvalue)
		return minvalue;
	else
		return value;
}

static Vector3 RotateVector(const Vector3& vec, float radian)
{
	return Vector3(vec.x * cos(radian) + vec.y * -sin(radian),
		vec.x * sin(radian) + vec.y * cos(radian), 0.f);
}

void SceneShop::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	bLightEnabled = true;

	cInventoryManager = CInventoryManager::GetInstance();

	cSoundController = CSoundController::GetInstance();

	cGameManager = GameManger::GetInstance();

	cSceneCollision = SceneCollision::GetInstance();
	
	player = Player::GetInstance();
	player->getPlayer();

	Math::InitRNG();

	canInteract = false; //with shop
	canLeave = false; //the shop scene
	canInteractComputer = false;
	inComputer = false;

	inShop = false;

	ShopMenu1 = false;
	ShopMenu2 = false;
	ShopMenu3 = false;
	ShopMenu4 = false;

	playerUpgradeType = 'u'; //u -> upgrade
	weaponUpgradePage = 1; //shop 1
	computerPage = 1; //computer

	empBought = false;
	hackBought = false;
	healBought = false;
	immortalBought = false;
	overdriveBought = false;

	swordBought = false;
	boxingGloveBought = false;

	rifleBought = false;
	flamethrowerBought = false;
	crossbowBought = false;


}



void SceneShop::Update(double dt)
{
	SceneBase::Update(dt);
	

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// Moving of player
	Vector3 movementDirection;
	movementDirection.SetZero();

	double x, y;
	float width = Application::GetWindowWidth();
	float height = Application::GetWindowHeight();
	Application::GetCursorPos(&x, &y);
	Vector3 mousePos = Vector3((x / width) * m_worldWidth, ((height - y) / height) * m_worldHeight, 0);


	if ((inShop == false) && (inComputer == false))
	{
		player->Update(dt, mousePos); //movement

		//collision
		if (player->getPlayer()->pos.y >= 70)
		{
			player->getPlayer()->pos.y = 70;
		}
		if (player->getPlayer()->pos.y <= 12)
		{
			player->getPlayer()->pos.y = 12;
		}
	}

	if ((player->getPlayer()->pos.x >= 70) && (player->getPlayer()->pos.x <= 100) && (player->getPlayer()->pos.y <= 12))
	{
		canLeave = true;
		if (Application::IsKeyPressed('E'))
		{
			cGameManager->waveClear = false;
			cGameManager->outShop = true;
			Application::SetState(2);
		}
	}
	else
	{
		canLeave = false;
	}

	if ((player->getPlayer()->pos.x >= 29) && (player->getPlayer()->pos.x <= 43) && (player->getPlayer()->pos.y >= 66)) //part dealer
	{
		canInteract = true;
		if (Application::IsKeyPressed('E'))
		{
			if (playerUpgradeType == 'u')
			{
				shopbuttonhighlight = Clamp2(shopbuttonhighlight, 0, 3);
			}
			else if (playerUpgradeType == 's')
			{
				shopbuttonhighlight = Clamp2(shopbuttonhighlight, 0, 4);
			}
			inShop = true;
			ShopMenu1 = true;
		}
	}
	else if ((player->getPlayer()->pos.x >= 64) && (player->getPlayer()->pos.x <= 78) && (player->getPlayer()->pos.y >= 66)) //weapon dealer
	{
		canInteract = true;
		if (Application::IsKeyPressed('E'))
		{
			shopbuttonhighlight = Clamp2(shopbuttonhighlight, 0, 3);	
			inShop = true;
			ShopMenu2 = true;
		}
	}
	else if ((player->getPlayer()->pos.x >= 100) && (player->getPlayer()->pos.x <= 114) && (player->getPlayer()->pos.y >= 66)) //blacksmith
	{
		canInteract = true;
		if (Application::IsKeyPressed('E'))
		{
			if (weaponUpgradePage == 1)
			{
				shopbuttonhighlight = Clamp2(shopbuttonhighlight, 0, 3);
			}
			else if (weaponUpgradePage == 2)
			{
				shopbuttonhighlight = Clamp2(shopbuttonhighlight, 0, 2);
			}

			inShop = true;
			ShopMenu3 = true;
		}
	}
	else if ((player->getPlayer()->pos.x >= 135) && (player->getPlayer()->pos.x <= 149) && (player->getPlayer()->pos.y >= 66)) //alchemist
	{
		canInteract = true;
		if (Application::IsKeyPressed('E')) 
		{
			shopbuttonhighlight = Clamp2(shopbuttonhighlight, 0, 2);
			inShop = true;
			ShopMenu4 = true;
		}
	}

	else
	{
		canInteract = false;
	}

	if ((player->getPlayer()->pos.x >= 10) && (player->getPlayer()->pos.x <= 20) && (player->getPlayer()->pos.y >= 66)) //computer
	{
		canInteractComputer = true;
		if (Application::IsKeyPressed('E'))
		{
			shopbuttonhighlight = Clamp2(shopbuttonhighlight, 0, 4);
			inComputer = true;		
		}
	}
	else
	{
		canInteractComputer = false;
	}

	if(Application::IsKeyPressed('R'))
	{
		if (inShop)
		{
			inShop = false;
			ShopMenu1 = false;
			ShopMenu2 = false;
			ShopMenu3 = false;
			ShopMenu4 = false;
		}
		if (inComputer)
		{
			inComputer = false;
		}
	}

	if (inComputer)
	{
		if (Application::IsKeyPressed('Z'))
		{
			shopbuttonhighlight = 0;
			computerPage = 1;
		}
		else if (Application::IsKeyPressed('X'))
		{
			shopbuttonhighlight = 0;
			computerPage = 2;
		}
	}

	if (inComputer == true)
	{
		if (computerPage == 1)
		{
			static bool w = false, s = false;
			if (Application::IsKeyPressed('W') && !w)
			{
				w = true;
				if (shopbuttonhighlight == 0)
				{
					shopbuttonhighlight = 4;
				}
				else
					shopbuttonhighlight--;
			}
			else if (!Application::IsKeyPressed('W') && w)
				w = false;

			if (Application::IsKeyPressed('S') && !s)
			{
				s = true;
				if (shopbuttonhighlight == 4)
				{
					shopbuttonhighlight = 0;
				}
				else
					shopbuttonhighlight++;
			}
			else if (!Application::IsKeyPressed('S') && s)
				s = false;

			if (Application::IsKeyPressed('E') && !eButtonState)
			{
				eButtonState = true;
				switch (shopbuttonhighlight)
				{
				case 0:
					cInventoryItem = cInventoryManager->GetItem("sword");
					if (CheckEquip(Weapon::SWORD) && cInventoryItem->GetCount() > 0)
					{
						cGameManager->weptype = Weapon::SWORD;
					}
					break;
				case 1:
					cInventoryItem = cInventoryManager->GetItem("boxingglove");
					if (CheckEquip(Weapon::BOXING_GLOVES) && cInventoryItem->GetCount() > 0)
					{
						cGameManager->weptype = Weapon::BOXING_GLOVES;
					}
					break;
				case 2:
					cInventoryItem = cInventoryManager->GetItem("rifle");
					if (CheckEquip(Weapon::RIFLE) && cInventoryItem->GetCount() > 0)
					{
						cGameManager->weptype = Weapon::RIFLE;
					}
					break;
				case 3:
					cInventoryItem = cInventoryManager->GetItem("flamethrower");
					if (CheckEquip(Weapon::FLAMETHROWER) && cInventoryItem->GetCount() > 0)
					{
						cGameManager->weptype = Weapon::FLAMETHROWER;
					}
					break;
				case 4:
					cInventoryItem = cInventoryManager->GetItem("crossbow");
					if (CheckEquip(Weapon::CROSSBOW) && cInventoryItem->GetCount() > 0)
					{
						cGameManager->weptype = Weapon::CROSSBOW;
					}
					break;
				}
			}
			else if (!Application::IsKeyPressed('E') && eButtonState)
			{
				eButtonState = false;
			}
		}
		else if (computerPage == 2)
		{
			static bool w = false, s = false;
			if (Application::IsKeyPressed('W') && !w)
			{
				w = true;
				if (shopbuttonhighlight == 0)
				{
					shopbuttonhighlight = 4;
				}
				else
					shopbuttonhighlight--;
			}
			else if (!Application::IsKeyPressed('W') && w)
				w = false;

			if (Application::IsKeyPressed('S') && !s)
			{
				s = true;
				if (shopbuttonhighlight == 4)
				{
					shopbuttonhighlight = 0;
				}
				else
					shopbuttonhighlight++;
			}
			else if (!Application::IsKeyPressed('S') && s)
				s = false;

			if (Application::IsKeyPressed('E') && !eButtonState)
			{
				eButtonState = true;
				switch (shopbuttonhighlight)
				{
				case 0:

					break;
				case 1:

					break;
				case 2:

					break;
				case 3:

					break;
				case 4:

					break;
				}
			}
			else if (!Application::IsKeyPressed('E') && eButtonState)
			{
				eButtonState = false;
			}
		}
	}

	//parts dealer to switch between upgrades and skills
	if (ShopMenu1 == true)
	{
		if (Application::IsKeyPressed('Z'))
		{
			shopbuttonhighlight = 0;
			playerUpgradeType = 'u'; //upgrade (for player)
		}
		else if (Application::IsKeyPressed('X'))
		{
			shopbuttonhighlight = 0;
			playerUpgradeType = 's'; //skills
		}

		if (playerUpgradeType == 'u')
		{
			static bool w = false, s = false;
			if (Application::IsKeyPressed('W') && !w)
			{
				w = true;
				if (shopbuttonhighlight == 0)
				{
					shopbuttonhighlight = 3;
				}
				else
					shopbuttonhighlight--;
			}
			else if (!Application::IsKeyPressed('W') && w)
				w = false;

			if (Application::IsKeyPressed('S') && !s)
			{
				s = true;
				if (shopbuttonhighlight == 3)
				{
					shopbuttonhighlight = 0;
				}
				else
					shopbuttonhighlight++;
			}
			else if (!Application::IsKeyPressed('S') && s)
				s = false;

			if (Application::IsKeyPressed('E') && !eButtonState)
			{
				eButtonState = true;
				switch (shopbuttonhighlight)
				{
				case 0:
					//buy speed
					if ((player->getMoney() >= movementspeedupgrade->getMoneyCost()) && (movementspeedupgrade->getUpgradeLevel() < 3))
					{
						player->changeMoney(-movementspeedupgrade->getMoneyCost());
						movementspeedupgrade->receiveUpgrade();
						player->changeMovementSpeed(10); 
						cSoundController->PlaySoundByID(10);
					}
					break;
				case 1:
					//buy health
					if ((player->getMoney() >= healthupgrade->getMoneyCost()) && (healthupgrade->getUpgradeLevel() < 6))
					{
						player->changeMoney(-healthupgrade->getMoneyCost());
						healthupgrade->receiveUpgrade();
						cSoundController->PlaySoundByID(10);
					}
					break;
				case 2:
					//buy melee up
					if ((player->getMoney() >= meleedmgupgrade->getMoneyCost()) && (meleedmgupgrade->getUpgradeLevel() < 5))
					{
						player->changeMoney(-meleedmgupgrade->getMoneyCost());
						meleedmgupgrade->receiveUpgrade();
						cSoundController->PlaySoundByID(10);
						player->meleeDmgBoost += 2;
					}
					break;
				case 3:
					//buy ranged up
					if ((player->getMoney() >= rangeddmgupgrade->getMoneyCost()) && (rangeddmgupgrade->getUpgradeLevel() < 5))
					{
						player->changeMoney(-rangeddmgupgrade->getMoneyCost());
						rangeddmgupgrade->receiveUpgrade();
						cSoundController->PlaySoundByID(10);
						player->rangeDmgBoost += 2;
					}
					break;
				}
			}
			else if (!Application::IsKeyPressed('E') && eButtonState)
			{
				eButtonState = false;
			}
		}
		else if (playerUpgradeType == 's')
		{
			static bool w = false, s = false;
			if (Application::IsKeyPressed('W') && !w)
			{
				w = true;
				if (shopbuttonhighlight == 0)
				{
					shopbuttonhighlight = 4;
				}
				else
					shopbuttonhighlight--;
			}
			else if (!Application::IsKeyPressed('W') && w)
				w = false;

			if (Application::IsKeyPressed('S') && !s)
			{
				s = true;
				if (shopbuttonhighlight == 4)
				{
					shopbuttonhighlight = 0;
				}
				else
					shopbuttonhighlight++;
			}
			else if (!Application::IsKeyPressed('S') && s)
				s = false;

			if (Application::IsKeyPressed('E') && !eButtonState)
			{
				eButtonState = true;

				switch (shopbuttonhighlight)
				{
				case 0:
					//buy emp
					if ((player->getMoney() >= emp->getMoneyCost()) && (empBought == false))
					{
						player->changeMoney(-emp->getMoneyCost());
						empBought = true;
						cInventoryItem = cInventoryManager->GetItem("emp");
						cSoundController->PlaySoundByID(10);
						cInventoryItem->Add(1);
					}
					break;
				case 1:
					//buy hack
					if ((player->getMoney() >= hack->getMoneyCost()) && (hackBought == false))
					{
						player->changeMoney(-hack->getMoneyCost());
						hackBought = true;
						cInventoryItem = cInventoryManager->GetItem("hack");
						cSoundController->PlaySoundByID(10);
						cInventoryItem->Add(1);
					}
					break;
				case 2:
					//buy heal
					if ((player->getMoney() >= heal->getMoneyCost()) && (healBought == false))
					{
						player->changeMoney(-heal->getMoneyCost());
						healBought = true;
						cInventoryItem = cInventoryManager->GetItem("heal");
						cSoundController->PlaySoundByID(10);
						cInventoryItem->Add(1);
					}
					break;
				case 3:
					//buy immortal
					if ((player->getMoney() >= immortal->getMoneyCost()) && (immortalBought == false))
					{
						player->changeMoney(-immortal->getMoneyCost());
						immortalBought = true;
						cInventoryItem = cInventoryManager->GetItem("immortal");
						cSoundController->PlaySoundByID(10);
						cInventoryItem->Add(1);
					}
					break;
				case 4:
					//buy overdrive
					if ((player->getMoney() >= overdrive->getMoneyCost()) && (overdriveBought == false))
					{
						player->changeMoney(-overdrive->getMoneyCost());
						overdriveBought = true;
						cInventoryItem = cInventoryManager->GetItem("overdrive");
						cSoundController->PlaySoundByID(10);
						cInventoryItem->Add(1);
					}
					break;
				}
			}
			else if (!Application::IsKeyPressed('E') && eButtonState)
			{
				eButtonState = false;
			}
		}
	}
	//weapons dealer to switch between ranged and melee
	if (ShopMenu2 == true) 
	{
		static bool w = false, s = false;
		if (Application::IsKeyPressed('W') && !w)
		{
			w = true;
			if (shopbuttonhighlight == 0)
			{
				shopbuttonhighlight = 3;
			}
			else
				shopbuttonhighlight--;
		}
		else if (!Application::IsKeyPressed('W') && w)
			w = false;

		if (Application::IsKeyPressed('S') && !s)
		{
			s = true;
			if (shopbuttonhighlight == 3)
			{
				shopbuttonhighlight = 0;
			}
			else
				shopbuttonhighlight++;
		}
		else if (!Application::IsKeyPressed('S') && s)
			s = false;

		if (Application::IsKeyPressed('E') && !eButtonState)
		{
			eButtonState = true;

			switch (shopbuttonhighlight)
			{
			case 0:
				//buy boxing glove
				if ((player->getMoney() >= boxingGlove->GetCost()) && (boxingGloveBought == false))
				{
					player->changeMoney(-boxingGlove->GetCost());
					boxingGloveBought = true;
					cInventoryItem = cInventoryManager->GetItem("boxingglove");
					cSoundController->PlaySoundByID(10);
					cInventoryItem->Add(1);
				}
				break;
			case 1:
				//buy rifle
				if ((player->getMoney() >= rifle->GetCost()) && (rifleBought == false))
				{
					player->changeMoney(-rifle->GetCost());
					rifleBought = true;
					cInventoryItem = cInventoryManager->GetItem("rifle");
					cSoundController->PlaySoundByID(10);
					cInventoryItem->Add(1);
				}
				break;
			case 2:
				//buy flamethrower
				if ((player->getMoney() >= flamethrower->GetCost()) && (flamethrowerBought == false))
				{
					player->changeMoney(-flamethrower->GetCost());
					flamethrowerBought = true;
					cInventoryItem = cInventoryManager->GetItem("flamethrower");
					cSoundController->PlaySoundByID(10);
					cInventoryItem->Add(1);
				}
				break;
			case 3:
				//buy crossbow
				if ((player->getMoney() >= crossbow->GetCost()) && (crossbowBought == false))
				{
					player->changeMoney(-crossbow->GetCost());
					crossbowBought = true;
					cInventoryItem = cInventoryManager->GetItem("crossbow");
					cSoundController->PlaySoundByID(10);
					cInventoryItem->Add(1);
				}
				break;
			}
		}
		else if (!Application::IsKeyPressed('E') && eButtonState)
		{
			eButtonState = false;
		}
	}

	//weapon upgrades
	if (ShopMenu3)
	{
		if (Application::IsKeyPressed('Z'))
		{
			weaponUpgradePage = 1; //page 1
			shopbuttonhighlight = 0;
		}
		else if (Application::IsKeyPressed('X'))
		{
			weaponUpgradePage = 2; //page 2
			shopbuttonhighlight = 0;
		}

		if (weaponUpgradePage == 1)
		{
			static bool w = false, s = false;
			if (Application::IsKeyPressed('W') && !w)
			{
				w = true;
				if (shopbuttonhighlight == 0)
				{
					shopbuttonhighlight = 3;
				}
				else
					shopbuttonhighlight--;
			}
			else if (!Application::IsKeyPressed('W') && w)
				w = false;

			if (Application::IsKeyPressed('S') && !s)
			{
				s = true;
				if (shopbuttonhighlight == 3)
				{
					shopbuttonhighlight = 0;
				}
				else
					shopbuttonhighlight++;
			}
			else if (!Application::IsKeyPressed('S') && s)
				s = false;

			if (Application::IsKeyPressed('E') && !eButtonState)
			{
				eButtonState = true;

				switch (shopbuttonhighlight)
				{
				case 0:
					//buy piercing bullet
					if ((player->getMoney() >= PierceMod->getMoneyCost()) && (cGameManager->pierceBought == false))
					{
						player->changeMoney(-PierceMod->getMoneyCost());
						cSoundController->PlaySoundByID(10);
						cGameManager->pierceBought = true;
					}
					break;
				case 1:
					//buy faster firing
					if ((player->getMoney() >= FasterFiringMod->getMoneyCost()) && (cGameManager->fastfireBought == false))
					{
						player->changeMoney(-FasterFiringMod->getMoneyCost());
						cSoundController->PlaySoundByID(10);
						cGameManager->fastfireBought = true;
					}
					break;
				case 2:
					//buy faster bullet
					if ((player->getMoney() >= FasterBulletMod->getMoneyCost()) && (cGameManager->fastbulletBought == false))
					{
						player->changeMoney(-FasterBulletMod->getMoneyCost());
						cSoundController->PlaySoundByID(10);
						cGameManager->fastbulletBought = true;
					}
					break;
				case 3:
					//buy explosive bullet
					if ((player->getMoney() >= ExplosiveMod->getMoneyCost()) && (cGameManager->explosiveBought == false))
					{
						player->changeMoney(-ExplosiveMod->getMoneyCost());
						cSoundController->PlaySoundByID(10);
						cGameManager->explosiveBought = true;
					}
					break;
				}
			}
			else if (!Application::IsKeyPressed('E') && eButtonState)
			{
				eButtonState = false;
			}
		}
		else if (weaponUpgradePage == 2)
		{
			static bool w = false, s = false;
			if (Application::IsKeyPressed('W') && !w)
			{
				w = true;
				if (shopbuttonhighlight == 0)
				{
					shopbuttonhighlight = 2;
				}
				else
					shopbuttonhighlight--;
			}
			else if (!Application::IsKeyPressed('W') && w)
				w = false;

			if (Application::IsKeyPressed('S') && !s)
			{
				s = true;
				if (shopbuttonhighlight == 2)
				{
					shopbuttonhighlight = 0;
				}
				else
					shopbuttonhighlight++;
			}
			else if (!Application::IsKeyPressed('S') && s)
				s = false;

			if (Application::IsKeyPressed('E') && !eButtonState)
			{
				eButtonState = true;

				switch (shopbuttonhighlight)
				{
				case 0:
					//buy better fuel
					if ((player->getMoney() >= betterFuelMod->getMoneyCost()) && (cGameManager->betterfuelBought == false))
					{
						player->changeMoney(-betterFuelMod->getMoneyCost());
						cSoundController->PlaySoundByID(10);
						cGameManager->betterfuelBought = true;
					}
					break;
				case 1:
					//buy accurate arrows
					if ((player->getMoney() >= accurateArrowsMod->getMoneyCost()) && (cGameManager->accuratearrowsBought== false))
					{
						player->changeMoney(-accurateArrowsMod->getMoneyCost());
						cSoundController->PlaySoundByID(10);
						cGameManager->accuratearrowsBought = true;
					}
					break;
				case 2:
					//buy faster melee
					if ((player->getMoney() >= fasterMeleeMod->getMoneyCost()) && (cGameManager->fastmeleeBought == false))
					{
						player->changeMoney(-fasterMeleeMod->getMoneyCost());
						cSoundController->PlaySoundByID(10);
						cGameManager->fastmeleeBought = true;
					}
					break;
				}
			}
			else if (!Application::IsKeyPressed('E') && eButtonState)
			{
				eButtonState = false;
			}
		}
	}

	//potion shop
	if (ShopMenu4)
	{
		static bool w = false, s = false;
		if (Application::IsKeyPressed('W') && !w)
		{
			w = true;
			if (shopbuttonhighlight == 0)
			{
				shopbuttonhighlight = 2;
			}
			else
				shopbuttonhighlight--;
		}
		else if (!Application::IsKeyPressed('W') && w)
			w = false;

		if (Application::IsKeyPressed('S') && !s)
		{
			s = true;
			if (shopbuttonhighlight == 2)
			{
				shopbuttonhighlight = 0;
			}
			else
				shopbuttonhighlight++;
		}
		else if (!Application::IsKeyPressed('S') && s)
			s = false;

		if (Application::IsKeyPressed('E') && !eButtonState)
		{
			eButtonState = true;

			//if shop menu == 4 
			switch (shopbuttonhighlight)
			{
			case 0:
				//buy health pot
				if (player->getMoney() >= HealthPotion->getMoneyCost())
				{
					player->changeMoney(-HealthPotion->getMoneyCost());
					cInventoryItem = cInventoryManager->GetItem("healthpotion");
					cSoundController->PlaySoundByID(10);
					cInventoryItem->Add(1);
				}
				break;
			case 1:
				//buy strength pot
				if (player->getMoney() >= StrengthPotion->getMoneyCost())
				{
					player->changeMoney(-StrengthPotion->getMoneyCost());
					cInventoryItem = cInventoryManager->GetItem("strengthpotion");
					cSoundController->PlaySoundByID(10);
					cInventoryItem->Add(1);
				}
				break;
			case 2:
				//buy speed pot
				if (player->getMoney() >= SpeedPotion->getMoneyCost())
				{
					player->changeMoney(-SpeedPotion->getMoneyCost());
					cInventoryItem = cInventoryManager->GetItem("speedpotion");
					cSoundController->PlaySoundByID(10);
					cInventoryItem->Add(1);
				}
				break;
			}
		}
		else if (!Application::IsKeyPressed('E') && eButtonState)
			eButtonState = false;
	}

	if ((ShopMenu1) || (ShopMenu2))
	{
		if (Application::IsKeyPressed('E'))
		{
			
		}
	}
	player->getPlayer()->pos += movementDirection.Normalize() * 40 * dt;

	Checkborder(player->getPlayer());

	return;

	
}


void SceneShop::Render()
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
	
	//sand background
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, 0);
	modelStack.Scale(360, 240, 1);
	RenderMesh(meshList[GEO_SANDBG], false);
	modelStack.PopMatrix();



	//NPCs
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth / 5, 80, 1);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(15, 15, 1);
	RenderMesh(meshList[GEO_PARTDEALER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth * 2 / 5, 80, 1);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(15, 15, 1);
	RenderMesh(meshList[GEO_WEAPONDEALER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth * 3 / 5, 80, 1);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(15, 15, 1);
	RenderMesh(meshList[GEO_BLACKSMITH], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth * 4 / 5, 80, 1);
	modelStack.Rotate(180, 0, 0, 2);
	modelStack.Scale(15, 15, 1);
	RenderMesh(meshList[GEO_ALCHEMIST], false);
	modelStack.PopMatrix();

	renderEnvironment();

	//computer
	modelStack.PushMatrix();
	modelStack.Translate(15, 75, 1);
	modelStack.Scale(9, 9, 1);
	RenderMesh(meshList[GEO_COMPUTER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(170, 97, 1);
	modelStack.Scale(13, 5, 1);
	RenderMesh(meshList[GEO_SHOPMENUBG], false);
	modelStack.PopMatrix();

	std::ostringstream ss;

	

	modelStack.PushMatrix();
	modelStack.Translate(player->GetGameObject()->pos.x, player->GetGameObject()->pos.y, player->GetGameObject()->pos.z);
	modelStack.Scale(player->GetGameObject()->scale.x, player->GetGameObject()->scale.y, player->GetGameObject()->scale.z);
	modelStack.Rotate(player->rotate, 0, 0, 1);
	if (player->dashing)
	{
		if (player->GetGameObject()->angle == 180)
		{
			meshList[GEO_LEFT_DASH]->material.kAmbient.Set(player->GetGameObject()->color.x, player->GetGameObject()->color.y, player->GetGameObject()->color.z);
			RenderMesh(meshList[GEO_LEFT_DASH], true);
		}

		else if (player->GetGameObject()->angle == 0)
		{
			meshList[GEO_RIGHT_DASH]->material.kAmbient.Set(player->GetGameObject()->color.x, player->GetGameObject()->color.y, player->GetGameObject()->color.z);
			RenderMesh(meshList[GEO_RIGHT_DASH], true);
		}
	}
	else
	{
		if (player->GetGameObject()->angle == 180)
		{
			meshList[GEO_LEFT_PLAYER]->material.kAmbient.Set(player->GetGameObject()->color.x, player->GetGameObject()->color.y, player->GetGameObject()->color.z);
			RenderMesh(meshList[GEO_LEFT_PLAYER], true);
		}

		else if (player->GetGameObject()->angle == 0)
		{
			meshList[GEO_RIGHT_PLAYER]->material.kAmbient.Set(player->GetGameObject()->color.x, player->GetGameObject()->color.y, player->GetGameObject()->color.z);
			RenderMesh(meshList[GEO_RIGHT_PLAYER], true);
		}
	}
	modelStack.PopMatrix();

	renderUI();

	if (ShopMenu1)
	{
		renderShopMenu1();
	}
	else if (ShopMenu2)
	{
		renderShopMenu2();
	}
	else if (ShopMenu3)
	{
		renderShopMenu3();
	}
	else if (ShopMenu4)
	{
		renderShopMenu4();
	}

	if (inComputer)
	{
		renderComputerMenu();
	}

	if (inComputer)
	{
		if (computerPage == 1)
		{
			cInventoryItem = cInventoryManager->GetItem("sword");
			if (cInventoryItem->GetCount() == 1)
			{
				ss.str("");
				ss << "Owned";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 32, 43);
			}
			else
			{
				ss.str("");
				ss << "Not Owned";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2.5, 32, 43);
			}

			if (!CheckEquip(Weapon::SWORD))
			{
				ss << "(Equipped)";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 32, 43);
			}

			cInventoryItem = cInventoryManager->GetItem("boxingglove");
			if (cInventoryItem->GetCount() == 1)
			{
				ss.str("");
				ss << "Owned";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 32, 36);
			}
			else
			{
				ss.str("");
				ss << "Not Owned";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2.5, 32, 36);
			}

			if (!CheckEquip(Weapon::BOXING_GLOVES))
			{
				ss << "(Equipped)";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 32, 36);
			}

			cInventoryItem = cInventoryManager->GetItem("rifle");
			if (cInventoryItem->GetCount() == 1)
			{
				ss.str("");
				ss << "Owned";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 32, 29);
			}
			else
			{
				ss.str("");
				ss << "Not Owned";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2.5, 32, 29);
			}

			if (!CheckEquip(Weapon::RIFLE))
			{
				ss << "(Equipped)";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 32, 29);
			}

			cInventoryItem = cInventoryManager->GetItem("flamethrower");
			if (cInventoryItem->GetCount() == 1)
			{
				ss.str("");
				ss << "Owned";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 32, 22);
			}
			else
			{
				ss.str("");
				ss << "Not Owned";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2.5, 32, 22);
			}

			if (!CheckEquip(Weapon::FLAMETHROWER))
			{
				ss << "(Equipped)";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 32, 22);
			}

			cInventoryItem = cInventoryManager->GetItem("crossbow");
			if (cInventoryItem->GetCount() == 1)
			{
				ss.str("");
				ss << "Owned";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 32, 15);
			}
			else
			{
				ss.str("");
				ss << "Not Owned";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2.5, 32, 15);
			}

			if (!CheckEquip(Weapon::CROSSBOW))
			{
				ss << "(Equipped)";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 32, 15);
			}
	
		}
		else if (computerPage == 2)
		{
			cInventoryItem = cInventoryManager->GetItem("emp");
			if (cInventoryItem->GetCount() == 1)
			{
				ss.str("");
				ss << "Owned";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 32, 43);
			}
			else
			{
				ss.str("");
				ss << "Not Owned";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2.5, 32, 43);
			}

			cInventoryItem = cInventoryManager->GetItem("hack");
			if (cInventoryItem->GetCount() == 1)
			{
				ss.str("");
				ss << "Owned";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 32, 36);
			}
			else
			{
				ss.str("");
				ss << "Not Owned";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2.5, 32, 36);
			}

			cInventoryItem = cInventoryManager->GetItem("heal");
			if (cInventoryItem->GetCount() == 1)
			{
				ss.str("");
				ss << "Owned";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 32, 29);
			}
			else
			{
				ss.str("");
				ss << "Not Owned";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2.5, 32, 29);
			}

			cInventoryItem = cInventoryManager->GetItem("immortal");
			if (cInventoryItem->GetCount() == 1)
			{
				ss.str("");
				ss << "Owned";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 32, 22);
			}
			else
			{
				ss.str("");
				ss << "Not Owned";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2.5, 32, 22);
			}

			cInventoryItem = cInventoryManager->GetItem("overdrive");
			if (cInventoryItem->GetCount() == 1)
			{
				ss.str("");
				ss << "Owned";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 32, 15);
			}
			else
			{
				ss.str("");
				ss << "Not Owned";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2.5, 32, 15);
			}
		}
	}

	ss.str("");
	ss << "Parts Dealer ";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 10, 55);

	ss.str("");
	ss << "Weapons Dealer ";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 25, 55);

	ss.str("");
	ss << "Blacksmith ";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 43, 55);

	ss.str("");
	ss << "Alchemist ";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 60, 55);

	if ((canInteract) && (inShop == false))
	{
		//On screen text
		std::ostringstream ss;

		ss.str("");
		ss << "Press [E] to interact";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.2, 0.2, 0.2), 3, 30, 8);
	}

	if ((canInteractComputer) && (inComputer == false))
	{
		//On screen text
		std::ostringstream ss;

		ss.str("");
		ss << "Press [E] to access weapons/skills";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.2, 0.2, 0.2), 3, 25, 8);
	}

	if (canLeave)
	{
		//On screen text
		std::ostringstream ss;

		ss.str("");
		ss << "Press [E] to leave shop";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.2, 0.2, 0.2), 3, 30, 8);
	}


}


void SceneShop::Exit()
{
	SceneBase::Exit();
}

void SceneShop::CreateBullet(Vector3 pos, Vector3 direction)
{
}

void SceneShop::renderEnvironment()
{
	//top
	modelStack.PushMatrix();
	modelStack.Translate(90, 95, 1);
	modelStack.Scale(m_worldWidth + 12, 10, 1);
	RenderMesh(meshList[GEO_CUBE], false);
	modelStack.PopMatrix();

	//left
	modelStack.PushMatrix();
	modelStack.Translate(2, 50, 1);
	modelStack.Scale(5, m_worldHeight, 1);
	RenderMesh(meshList[GEO_CUBE], false);
	modelStack.PopMatrix();

	//right
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth - 1, 50, 1);
	modelStack.Scale(5, m_worldHeight, 1);
	RenderMesh(meshList[GEO_CUBE], false);
	modelStack.PopMatrix();

	//bottom left
	modelStack.PushMatrix();
	modelStack.Translate(30, 3, 1);
	modelStack.Scale(m_worldWidth / 2 - 10, 8, 1);
	RenderMesh(meshList[GEO_CUBE], false);
	modelStack.PopMatrix();

	//bottom right
	modelStack.PushMatrix();
	modelStack.Translate(140, 3, 1);
	modelStack.Scale(m_worldWidth / 2 - 10, 8, 1);
	RenderMesh(meshList[GEO_CUBE], false);
	modelStack.PopMatrix();

	//shop counter
	modelStack.PushMatrix();
	modelStack.Translate(88, 72, 1);
	modelStack.Scale(m_worldWidth + 10, 5, 1);
	RenderMesh(meshList[GEO_CUBE], false);
	modelStack.PopMatrix();
}

void SceneShop::renderShopMenu1()
{
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth / 2, 40, 1);
	modelStack.Scale(m_worldWidth - 30, 55, 1);
	RenderMesh(meshList[GEO_SHOPMENUBG], false);
	modelStack.PopMatrix();

	std::ostringstream ss;

	ss.str("");
	ss << "[R]";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 69, 37);

	ss.str("");
	ss << "X";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 3, 71, 36.5);


	if (playerUpgradeType == 'u') //upgrades
	{
		ss.str("");
		ss << "Player Upgrades [Z]/[X]";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 10, 37);

		ss.str("");
		ss << "Cost";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 27, 37);

		ss.str("");
		ss << "Description";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 36, 37);

		ss.str("");
		ss << "Level";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 57, 37);

		//speed up 
		modelStack.PushMatrix();
		modelStack.Translate(46, 53.5, 1);
		modelStack.Scale(7, 7, 1);
		RenderMesh(meshList[GEO_SPEEDUP], false);
		modelStack.PopMatrix();

		ss.str("");
		ss << "Speed Upgrade";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 10, 31);
		ss.str("");
		ss << movementspeedupgrade->getMoneyCost();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 28, 31);
		ss.str("");
		ss << movementspeedupgrade->getDescription();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.6, 0.7, 1), 2, 35, 31);
		ss.str("");
		ss << movementspeedupgrade->getUpgradeLevel();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 58, 31);

		if (movementspeedupgrade->getUpgradeLevel() == 3)
		{
			modelStack.PushMatrix();
			modelStack.Translate(80, 53, 1);
			modelStack.Scale(120, 0.5, 1);
			RenderMesh(meshList[GEO_SOLDOUT], false);
			modelStack.PopMatrix();
		}
		//health up
		modelStack.PushMatrix();
		modelStack.Translate(46, 43.5, 1);
		modelStack.Scale(7, 7, 1);
		RenderMesh(meshList[GEO_HEALTHUP], false);
		modelStack.PopMatrix();

		ss.str("");
		ss << "Health Upgrade";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 10, 25);
		ss.str("");
		ss << healthupgrade->getMoneyCost();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 28, 25);
		ss.str("");
		ss << healthupgrade->getDescription();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.6, 0.7, 1), 2, 35, 25);
		ss.str("");
		ss << healthupgrade->getUpgradeLevel();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 58, 25);

		if (healthupgrade->getUpgradeLevel() == 6)
		{
			modelStack.PushMatrix();
			modelStack.Translate(80, 43, 1);
			modelStack.Scale(120, 0.5, 1);
			RenderMesh(meshList[GEO_SOLDOUT], false);
			modelStack.PopMatrix();
		}
		//melee upgrade
		modelStack.PushMatrix();
		modelStack.Translate(46, 33.5, 1);
		modelStack.Scale(7, 7, 1);
		RenderMesh(meshList[GEO_MELEEUP], false);
		modelStack.PopMatrix();

		ss.str("");
		ss << "Melee Upgrade";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 10, 19);
		ss.str("");
		ss << meleedmgupgrade->getMoneyCost();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 28, 19);
		ss.str("");
		ss << meleedmgupgrade->getDescription();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.6, 0.7, 1), 2, 35, 19);
		ss.str("");
		ss << meleedmgupgrade->getUpgradeLevel();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 58, 19);

		if (meleedmgupgrade->getUpgradeLevel() == 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(80, 33, 1);
			modelStack.Scale(120, 0.5, 1);
			RenderMesh(meshList[GEO_SOLDOUT], false);
			modelStack.PopMatrix();
		}
		
		//range upgrade
		modelStack.PushMatrix();
		modelStack.Translate(47, 23.5, 1);
		modelStack.Scale(7, 7, 1);
		RenderMesh(meshList[GEO_RANGEDUP], false);
		modelStack.PopMatrix();

		ss.str("");
		ss << "Ranged Upgrade";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 10, 13);
		ss.str("");
		ss << rangeddmgupgrade->getMoneyCost();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 28, 13);
		ss.str("");
		ss << rangeddmgupgrade->getDescription();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.6, 0.7, 1), 2, 35, 13);
		ss.str("");
		ss << rangeddmgupgrade->getUpgradeLevel();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 58, 13);

		if (rangeddmgupgrade->getUpgradeLevel() == 5)
		{
			modelStack.PushMatrix();
			modelStack.Translate(80, 23, 1);
			modelStack.Scale(120, 0.5, 1);
			RenderMesh(meshList[GEO_SOLDOUT], false);
			modelStack.PopMatrix();
		}

		//selctor
		ss.str("");
		ss << ">";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.1, 0.7, 1), 4, 8, 30 - shopbuttonhighlight * 6);
	}
	else if (playerUpgradeType == 's') //skills
	{
		ss.str("");
		ss << "Skills [Z]/[X]";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 10, 37);

		ss.str("");
		ss << "Cost";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 24, 37);

		ss.str("");
		ss << "Description";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 36, 37);

		ss.str("");
		ss << "Energy Cost";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 57, 37);


		modelStack.PushMatrix();
		modelStack.Translate(38, 55.5, 1);
		modelStack.Scale(7, 7, 1);
		RenderMesh(meshList[GEO_EMP], false);
		modelStack.PopMatrix();

		ss.str("");
		ss << "EMP Skill";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 10, 32);
		ss.str("");
		ss << emp->getMoneyCost();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 24, 32);
		ss.str("");
		ss << emp->getDescription();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.6, 0.7, 1), 1.5, 31, 32);
		ss.str("");
		ss << emp->getEnergyCost();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 60, 32);

		if (empBought)
		{
			modelStack.PushMatrix();
			modelStack.Translate(80, 55, 1);
			modelStack.Scale(120, 0.5, 1);
			RenderMesh(meshList[GEO_SOLDOUT], false);
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();
		modelStack.Translate(39, 46.5, 1);
		modelStack.Scale(7, 7, 1);
		RenderMesh(meshList[GEO_HACK], false);
		modelStack.PopMatrix();

		ss.str("");
		ss << "Hack Skill";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 10, 27);
		ss.str("");
		ss << hack->getMoneyCost();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 24, 27);
		ss.str("");
		ss << hack->getDescription();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.1, 0.9, 0.1), 1.5, 31, 27);
		ss.str("");
		ss << hack->getEnergyCost();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 60, 27);

		if (hackBought)
		{
			modelStack.PushMatrix();
			modelStack.Translate(80, 46.5, 1);
			modelStack.Scale(120, 0.5, 1);
			RenderMesh(meshList[GEO_SOLDOUT], false);
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();
		modelStack.Translate(49, 38.5, 1);
		modelStack.Scale(6, 6, 1);
		RenderMesh(meshList[GEO_DOPPELGANGER], false);
		modelStack.PopMatrix();

		ss.str("");
		ss << "Doppelganger Skill";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 10, 22);
		ss.str("");
		ss << heal->getMoneyCost();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 24, 22);
		ss.str("");
		ss << heal->getDescription();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0.1, 0.1), 1.5, 31, 22);
		ss.str("");
		ss << heal->getEnergyCost();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 60, 22);

		if (healBought)
		{
			modelStack.PushMatrix();
			modelStack.Translate(80, 38.5, 1);
			modelStack.Scale(120, 0.5, 1);
			RenderMesh(meshList[GEO_SOLDOUT], false);
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();
		modelStack.Translate(44, 30.5, 1);
		modelStack.Scale(6, 6, 1);
		RenderMesh(meshList[GEO_IMMORTAL], false);
		modelStack.PopMatrix();

		ss.str("");
		ss << "Immortal Skill";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 10, 17);
		ss.str("");
		ss << immortal->getMoneyCost();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 24, 17);
		ss.str("");
		ss << immortal->getDescription();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.8, 0.8, 0.1), 1.5, 31, 17);
		ss.str("");
		ss << immortal->getEnergyCost();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 60, 17);

		if (immortalBought)
		{
			modelStack.PushMatrix();
			modelStack.Translate(80, 30, 1);
			modelStack.Scale(120, 0.5, 1);
			RenderMesh(meshList[GEO_SOLDOUT], false);
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();
		modelStack.Translate(44, 22.5, 1);
		modelStack.Scale(6, 6, 1);
		RenderMesh(meshList[GEO_OVERDRIVE], false);
		modelStack.PopMatrix();

		ss.str("");
		ss << "Overdrive Skill";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 10, 12);
		ss.str("");
		ss << overdrive->getMoneyCost();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 24, 12);
		ss.str("");
		ss << overdrive->getDescription();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0.1), 1.5, 31, 12);
		ss.str("");
		ss << overdrive->getEnergyCost();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 60, 12);

		if (overdriveBought)
		{
			modelStack.PushMatrix();
			modelStack.Translate(80, 21.5, 1);
			modelStack.Scale(120, 0.5, 1);
			RenderMesh(meshList[GEO_SOLDOUT], false);
			modelStack.PopMatrix();
		}

		//selctor
		ss.str("");
		ss << ">";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.1, 0.7, 1), 4, 8, 31 - shopbuttonhighlight * 5);
	}

}

void SceneShop::renderShopMenu2()
{
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth / 2, 40, 1);
	modelStack.Scale(m_worldWidth - 20, 55, 1);
	RenderMesh(meshList[GEO_SHOPMENUBG], false);
	modelStack.PopMatrix();

	std::ostringstream ss;
	ss.str("");
	ss << "Cost";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 24, 36);

	ss.str("");
	ss << "Damage";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 30, 36);

	ss.str("");
	ss << "Attack Speed";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 38, 36);

	ss.str("");
	ss << "Description";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 49, 36);

	ss.str("");
	ss << "Range";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 63, 36);

	ss.str("");
	ss << "Weapons";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 8, 36);

	modelStack.PushMatrix();
	modelStack.Translate(41, 52.5, 1);
	modelStack.Scale(7, 7, 1);
	RenderMesh(meshList[GEO_BOXINGGLOVE], false);
	modelStack.PopMatrix();

	ss.str("");
	ss << "Boxing Glove";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 9, 31);

	ss.str("");
	ss << boxingGlove->GetCost();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 24.5, 31);
	ss.str("");
	ss << boxingGlove->GetDamage();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2, 31, 31);
	ss.str("");
	ss << boxingGlove->GetAttackSpeed();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 40, 31);
	ss.str("");
	ss << boxingGlove->getDescription();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 1.5, 49, 31);
	ss.str("");
	ss << boxingGlove->GetRange();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 64, 31);

	if (boxingGloveBought)
	{
		modelStack.PushMatrix();
		modelStack.Translate(85, 53, 1);
		modelStack.Scale(135, 0.5, 1);
		RenderMesh(meshList[GEO_SOLDOUT], false);
		modelStack.PopMatrix();
	}

	//rifle
	modelStack.PushMatrix();
	modelStack.Translate(35, 42.5, 1);
	modelStack.Scale(12, 4, 1);
	RenderMesh(meshList[GEO_RIFLE_RIGHT], false);
	modelStack.PopMatrix();

	ss.str("");
	ss << "Rifle";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 9, 25);

	ss.str("");
	ss << rifle->GetCost();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 24.5, 25);
	ss.str("");
	ss << rifle->GetDamage();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2, 31, 25);
	ss.str("");
	ss << rifle->GetAttackSpeed();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 40, 25);
	ss.str("");
	ss << rifle->getDescription();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 1.5, 48, 25);
	ss.str("");
	ss << rifle->GetRange();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 64, 25);

	if (rifleBought)
	{
		modelStack.PushMatrix();
		modelStack.Translate(85, 43, 1);
		modelStack.Scale(135, 0.5, 1);
		RenderMesh(meshList[GEO_SOLDOUT], false);
		modelStack.PopMatrix();
	}

	//flamethrower
	modelStack.PushMatrix();
	modelStack.Translate(45, 32.5, 1);
	modelStack.Scale(12, 6, 1);
	RenderMesh(meshList[GEO_FLAMETHROWER], false);
	modelStack.PopMatrix();

	ss.str("");
	ss << "Flamethrower";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 9, 19);

	ss.str("");
	ss << flamethrower->GetCost();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 24.5, 19);
	ss.str("");
	ss << flamethrower->GetDamage();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2, 31, 19);
	ss.str("");
	ss << flamethrower->GetAttackSpeed();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 40, 19);
	ss.str("");
	ss << flamethrower->getDescription();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 1.5, 48, 19);
	ss.str("");
	ss << flamethrower->GetRange();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 64, 19);

	if (flamethrowerBought)
	{
		modelStack.PushMatrix();
		modelStack.Translate(85, 33, 1);
		modelStack.Scale(135, 0.5, 1);
		RenderMesh(meshList[GEO_SOLDOUT], false);
		modelStack.PopMatrix();
	}

	//crossbow
	modelStack.PushMatrix();
	modelStack.Translate(40, 23.5, 1);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(10, 8, 1);
	RenderMesh(meshList[GEO_CROSSBOW], false);
	modelStack.PopMatrix();

	ss.str("");
	ss << "Crossbow";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 9, 13);

	ss.str("");
	ss << crossbow->GetCost();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 24.5, 13);
	ss.str("");
	ss << crossbow->GetDamage();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2, 31, 13);
	ss.str("");
	ss << crossbow->GetAttackSpeed();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 40, 13);
	ss.str("");
	ss << crossbow->getDescription();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 1.5, 48, 13);
	ss.str("");
	ss << crossbow->GetRange();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 64, 13);

	if (crossbowBought)
	{
		modelStack.PushMatrix();
		modelStack.Translate(85, 23, 1);
		modelStack.Scale(135, 0.5, 1);
		RenderMesh(meshList[GEO_SOLDOUT], false);
		modelStack.PopMatrix();
	}
	//selctor
	ss.str("");
	ss << ">";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.1, 0.7, 1), 4, 7, 30 - shopbuttonhighlight * 6);

	ss.str("");
	ss << "[R]";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 71, 37);

	ss.str("");
	ss << "X";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 3, 73, 36.5);
}

void SceneShop::renderShopMenu3()
{
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth / 2, 40, 1);
	modelStack.Scale(m_worldWidth - 30, 55, 1);
	RenderMesh(meshList[GEO_SHOPMENUBG], false);
	modelStack.PopMatrix();

	std::ostringstream ss;

	ss.str("");
	ss << "[R]";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 69, 37);

	ss.str("");
	ss << "X";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 3, 71, 36.5);

	ss.str("");
	ss << "Weapon Upgrades [Z]/[X]";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 10, 37);

	ss.str("");
	ss << "Cost";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 27, 37);

	ss.str("");
	ss << "Description";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 36, 37);

	ss.str("");
	ss << "Applies to";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 57, 37);

	if (weaponUpgradePage == 1)
	{
		modelStack.PushMatrix();
		modelStack.Translate(46, 53.5, 1);
		modelStack.Scale(7, 7, 1);
		RenderMesh(meshList[GEO_PIERCINGBULLETS], false);
		modelStack.PopMatrix();

		ss.str("");
		ss << "Piercing bullets";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 10, 31);

		ss.str("");
		ss << PierceMod->getMoneyCost();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 27, 31);
		ss.str("");
		ss << PierceMod->getDescription();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.6, 0.7, 1), 2, 35, 31);
		ss.str("");
		ss << "Rifle, Crossbow";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 56, 31);

		if (cGameManager->pierceBought)
		{
			modelStack.PushMatrix();
			modelStack.Translate(85, 53, 1);
			modelStack.Scale(130, 0.5, 1);
			RenderMesh(meshList[GEO_SOLDOUT], false);
			modelStack.PopMatrix();
		}
		modelStack.PushMatrix();
		modelStack.Translate(46, 43.5, 1);
		modelStack.Scale(7, 7, 1);
		RenderMesh(meshList[GEO_FASTERFIRING], false);
		modelStack.PopMatrix();

		ss.str("");
		ss << "Faster firing";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 10, 25);

		ss.str("");
		ss << FasterFiringMod->getMoneyCost();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 27, 25);
		ss.str("");
		ss << FasterFiringMod->getDescription();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.6, 0.7, 1), 2, 35, 25);
		ss.str("");
		ss << "Rifle, Crossbow";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 56, 25);

		if (cGameManager->fastfireBought)
		{
			modelStack.PushMatrix();
			modelStack.Translate(85, 43, 1);
			modelStack.Scale(130, 0.5, 1);
			RenderMesh(meshList[GEO_SOLDOUT], false);
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();
		modelStack.Translate(46, 33.5, 1);
		modelStack.Scale(7, 7, 1);
		RenderMesh(meshList[GEO_FASTERBULLET], false);
		modelStack.PopMatrix();

		ss.str("");
		ss << "Faster bullets";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 10, 19);

		ss.str("");
		ss << FasterBulletMod->getMoneyCost();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 27, 19);
		ss.str("");
		ss << FasterBulletMod->getDescription();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.6, 0.7, 1), 2, 35, 19);
		ss.str("");
		ss << "Rifle, Crossbow";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 56, 19);

		if (cGameManager->fastbulletBought)
		{
			modelStack.PushMatrix();
			modelStack.Translate(85, 33, 1);
			modelStack.Scale(130, 0.5, 1);
			RenderMesh(meshList[GEO_SOLDOUT], false);
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();
		modelStack.Translate(46, 23.5, 1);
		modelStack.Scale(7, 7, 1);
		RenderMesh(meshList[GEO_EXPLOSIVEBULLET], false);
		modelStack.PopMatrix();

		ss.str("");
		ss << "Explosive bullets";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 10, 13);

		ss.str("");
		ss << ExplosiveMod->getMoneyCost();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 27, 13);
		ss.str("");
		ss << ExplosiveMod->getDescription();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.6, 0.7, 1), 2, 35, 13);
		ss.str("");
		ss << "Rifle, Crossbow";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 56, 13);

		if (cGameManager->explosiveBought)
		{
			modelStack.PushMatrix();
			modelStack.Translate(85, 23, 1);
			modelStack.Scale(130, 0.5, 1);
			RenderMesh(meshList[GEO_SOLDOUT], false);
			modelStack.PopMatrix();
		}
		//selctor
		ss.str("");
		ss << ">";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.1, 0.7, 1), 4, 8, 30 - shopbuttonhighlight * 6);
	}
	else if (weaponUpgradePage == 2)
	{
		modelStack.PushMatrix();
		modelStack.Translate(40, 53.5, 1);
		modelStack.Scale(7, 7, 1);
		RenderMesh(meshList[GEO_BETTERFUEL], false);
		modelStack.PopMatrix();

		ss.str("");
		ss << "Better Fuel";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 10, 31);

		ss.str("");
		ss << betterFuelMod->getMoneyCost();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 27, 31);
		ss.str("");
		ss << betterFuelMod->getDescription();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.6, 0.7, 1), 2, 35, 31);
		ss.str("");
		ss << "Flamethrower";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 56, 31);

		if (cGameManager->betterfuelBought)
		{
			modelStack.PushMatrix();
			modelStack.Translate(85, 53, 1);
			modelStack.Scale(130, 0.5, 1);
			RenderMesh(meshList[GEO_SOLDOUT], false);
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();
		modelStack.Translate(49, 40.5, 1);
		modelStack.Scale(7, 7, 1);
		RenderMesh(meshList[GEO_ACCURATEARROWS], false);
		modelStack.PopMatrix();

		ss.str("");
		ss << "Accurate Arrows";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 10, 23);

		ss.str("");
		ss << accurateArrowsMod->getMoneyCost();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 27, 23);
		ss.str("");
		ss << accurateArrowsMod->getDescription();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.6, 0.7, 1), 2, 35, 23);
		ss.str("");
		ss << "Crossbow";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 56, 23);

		if (cGameManager->accuratearrowsBought)
		{
			modelStack.PushMatrix();
			modelStack.Translate(85, 40, 1);
			modelStack.Scale(130, 0.5, 1);
			RenderMesh(meshList[GEO_SOLDOUT], false);
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();
		modelStack.Translate(52, 26.5, 1);
		modelStack.Scale(7, 7, 1);
		RenderMesh(meshList[GEO_FASTERMELEE], false);
		modelStack.PopMatrix();

		ss.str("");
		ss << "Faster Melee Attacks";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 10, 15);

		ss.str("");
		ss << fasterMeleeMod->getMoneyCost();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 27, 15);
		ss.str("");
		ss << fasterMeleeMod->getDescription();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.6, 0.7, 1), 2, 35, 15);
		ss.str("");
		ss << "Melee weapons";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 56, 15);

		if (cGameManager->fastmeleeBought)
		{
			modelStack.PushMatrix();
			modelStack.Translate(85, 26.5, 1);
			modelStack.Scale(130, 0.5, 1);
			RenderMesh(meshList[GEO_SOLDOUT], false);
			modelStack.PopMatrix();
		}

		//selector
		ss.str("");
		ss << ">";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.1, 0.7, 1), 4, 8, 30 - shopbuttonhighlight * 8.2);
	}
}
void SceneShop::renderShopMenu4()
{
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth / 2, 40, 1);
	modelStack.Scale(m_worldWidth - 30, 55, 1);
	RenderMesh(meshList[GEO_SHOPMENUBG], false);
	modelStack.PopMatrix();

	std::ostringstream ss;

	ss.str("");
	ss << "Potions";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 10, 36);

	ss.str("");
	ss << "Cost";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 24, 36);

	ss.str("");
	ss << "Description";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 33, 36);

	ss.str("");
	ss << "Duration";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 57, 36);

	ss.str("");
	ss << "[R]";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 68, 37);

	ss.str("");
	ss << "X";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 3, 70, 36.5);

	//health
	modelStack.PushMatrix();
	modelStack.Translate(44, 51.5, 1);
	modelStack.Scale(5, 5, 1);
	RenderMesh(meshList[GEO_HEALTHPOT], false);
	modelStack.PopMatrix();
	
	ss.str("");
	ss << "Health Potion";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 10, 30);
	ss.str("");
	ss << HealthPotion->getMoneyCost();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 25, 30);
	ss.str("");
	ss << HealthPotion->getDescription();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2, 33, 30);
	ss.str("");
	ss << "Instant";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 58, 30);

	//strength
	modelStack.PushMatrix();
	modelStack.Translate(47, 38, 1);
	modelStack.Scale(5, 5, 1);
	RenderMesh(meshList[GEO_STRENGTHPOT], false);
	modelStack.PopMatrix();

	ss.str("");
	ss << "Strength Potion";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 10, 22);
	ss.str("");
	ss << StrengthPotion->getMoneyCost();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 25, 22);
	ss.str("");
	ss << StrengthPotion->getDescription();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.9, 0, 0.9), 2, 33, 22);
	ss.str("");
	ss << StrengthPotion->getDuration();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 58, 22);

	//speed
	modelStack.PushMatrix();
	modelStack.Translate(44, 25.5, 1);
	modelStack.Scale(5, 5, 1);
	RenderMesh(meshList[GEO_SPEEDPOT], false);
	modelStack.PopMatrix();

	ss.str("");
	ss << "Speed Potion";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 10, 14);
	ss.str("");
	ss << SpeedPotion->getMoneyCost();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 25, 14);
	ss.str("");
	ss << SpeedPotion->getDescription();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.8, 0.9, 1), 2, 33, 14);
	ss.str("");
	ss << SpeedPotion->getDuration();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 58, 14);

	//selector
	ss.str("");
	ss << ">";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.1, 0.7, 1), 4, 8, 29 - shopbuttonhighlight * 8.2);

}

void SceneShop::renderComputerMenu()
{
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth / 2, 50, 1);
	modelStack.Scale(m_worldWidth - 40, 75, 1);
	RenderMesh(meshList[GEO_SHOPMENUBG], false);
	modelStack.PopMatrix();

	std::ostringstream ss;
	ss.str("");
	ss << "[R]";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 66, 49);

	ss.str("");
	ss << "X";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 3, 68, 48.5);

	if (computerPage == 1)
	{
		ss.str("");
		ss << "Weapons [Z]/[X]";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 12, 48);

		ss.str("");
		ss << "Owned";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 32, 48);

		ss.str("");
		ss << "Damage";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 52, 48);

		ss.str("");
		ss << "Sword";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 15, 43);

		ss.str("");
		ss << "Boxing Glove";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 15, 36);

		ss.str("");
		ss << "Rifle";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 15, 29);

		ss.str("");
		ss << "Flamethrower";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 15, 22);

		ss.str("");
		ss << "Crossbow";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 15, 15);

		ss.str("");
		ss << sword->GetDamage();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 54, 43);

		ss.str("");
		ss << boxingGlove->GetDamage();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 54, 36);

		ss.str("");
		ss << rifle->GetDamage();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 54, 29);

		ss.str("");
		ss << flamethrower->GetDamage();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 54, 22);

		ss.str("");
		ss << crossbow->GetDamage();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 54, 15);
		//selctor
		ss.str("");
		ss << ">";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.1, 0.7, 1), 4, 13, 42 - shopbuttonhighlight * 7);
	}
	else if (computerPage == 2)
	{
		ss.str("");
		ss << "Skills [Z]/[X]";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 12, 48);

		ss.str("");
		ss << "Owned";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 32, 48);

		ss.str("");
		ss << "Description";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 47, 48);

		ss.str("");
		ss << "EMP Skill";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 15, 43);

		ss.str("");
		ss << "Hack Skill";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 15, 36);

		ss.str("");
		ss << "Heal Skill";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 15, 29);

		ss.str("");
		ss << "Immortal Skill";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 15, 22);

		ss.str("");
		ss << "Overdrive Skill";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5, 15, 15);


		ss.str("");
		ss << emp->getDescription();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 1.5, 43, 43.5);

		ss.str("");
		ss << hack->getDescription();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 1.5, 43, 36.5);

		ss.str("");
		ss << heal->getDescription();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 1.5, 43, 29.5);

		ss.str("");
		ss << immortal->getDescription();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 1.5, 43, 22.5);

		ss.str("");
		ss << overdrive->getDescription();
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 1.5, 43, 15.5);

		//selctor
		ss.str("");
		ss << ">";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.1, 0.7, 1), 4, 13, 42 - shopbuttonhighlight * 7);
	}
}

void SceneShop::renderUI()
{
	std::ostringstream ss;

	//render money
	modelStack.PushMatrix();
	modelStack.Translate(169, 97, 1);
	modelStack.Scale(14, 5, 1);
	RenderMesh(meshList[GEO_SHOPMENUBG], false);
	modelStack.PopMatrix();

	ss.str("");
	ss << "$";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.1, 1, 0.1), 3, 73, 56.7);

	ss.str("");
	ss << player->getMoney();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 74.5, 56.7);


	Vector3 wep1 = Vector3(16, 15, 1);
	Vector3 wep2 = Vector3(26, 15, 1);
	Vector3 scale = Vector3(10, 10, 1);

	// render hotbar
	modelStack.PushMatrix();
	modelStack.Translate(wep1.x, wep1.y, wep1.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderMesh(meshList[GEO_HOTBAR_SELECTED], false);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(wep2.x, wep2.y, wep2.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderMesh(meshList[GEO_HOTBAR], false);
	modelStack.PopMatrix();

	if (player->GetWeapon() != nullptr)
		renderWeaponUI(wep1, scale, cGameManager->weptype);
	if (player->GetSideWeapon() != nullptr)
		renderWeaponUI(wep2, scale, cGameManager->sideweptype);

	//potions
	modelStack.PushMatrix();
	modelStack.Translate(140, wep1.y, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderMesh(meshList[GEO_HOTBAR], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(150, wep1.y, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderMesh(meshList[GEO_HOTBAR], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(160, wep1.y, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderMesh(meshList[GEO_HOTBAR], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(140, wep1.y, 1);
	modelStack.Scale(5, 5, 1);
	RenderMesh(meshList[GEO_HEALTHPOT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(150, wep1.y, 1);
	modelStack.Scale(5, 5, 1);
	RenderMesh(meshList[GEO_STRENGTHPOT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(160, wep1.y, 1);
	modelStack.Scale(5, 5, 1);
	RenderMesh(meshList[GEO_SPEEDPOT], false);
	modelStack.PopMatrix();

	//ss.str("");
	//ss << "[1]";
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 61, 10);

	//ss.str("");
	//ss << "[2]";
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 65.5, 10);

	//ss.str("");
	//ss << "[3]";
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 70, 10);

	ss.str("");
	cInventoryItem = cInventoryManager->GetItem("healthpotion");
	ss << cInventoryItem->GetCount();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 2, 62.7, 5.7);

	ss.str("");
	cInventoryItem = cInventoryManager->GetItem("strengthpotion");
	ss << cInventoryItem->GetCount();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 2, 67.2, 5.7);

	ss.str("");
	cInventoryItem = cInventoryManager->GetItem("speedpotion");
	ss << cInventoryItem->GetCount();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 2, 71.7, 5.7);
}

GameObject* SceneShop::Checkborder(GameObject* go)
{
	if (go->pos.x + go->scale.x / 2 > m_worldWidth)
	{
		go->pos.x = m_worldWidth - go->scale.x / 2;
	}

	if (go->pos.x - go->scale.x / 2 < 0)
	{
		go->pos.x = 0 + go->scale.x / 2;
	}

	if (go->pos.y + go->scale.y / 2 > m_worldHeight)
	{
		go->pos.y = m_worldHeight - go->scale.y / 2;
	}

	if (go->pos.y - go->scale.y / 2 < 0)
	{
		go->pos.y = 0 + go->scale.y / 2;
	}

	return go;
}

void SceneShop::renderWeaponUI(Vector3 pos, Vector3 scale, int object)
{
	switch (object)
	{
	case Weapon::SWORD:
		modelStack.PushMatrix();
		modelStack.Translate(pos.x, pos.y, pos.z);
		modelStack.Scale(scale.x, scale.y, scale.z);
		meshList[GEO_SWORDR]->material.kAmbient.Set(1,1,1);
		RenderMesh(meshList[GEO_SWORDR], true);
		modelStack.PopMatrix();
		break;
	case Weapon::RIFLE:
		modelStack.PushMatrix();
		modelStack.Translate(pos.x, pos.y, pos.z);
		modelStack.Scale(scale.x - 2, scale.y / 3, scale.z);
		meshList[GEO_RIFLE_RIGHT]->material.kAmbient.Set(1, 1, 1);
		RenderMesh(meshList[GEO_RIFLE_RIGHT], true);
		modelStack.PopMatrix();
		break;
	case Weapon::FLAMETHROWER:
		modelStack.PushMatrix();
		modelStack.Translate(pos.x, pos.y, pos.z);
		modelStack.Scale(scale.x - 2, scale.y / 3, scale.z);
		meshList[GEO_FLAMETHROWER]->material.kAmbient.Set(1, 1, 1);
		RenderMesh(meshList[GEO_FLAMETHROWER], true);
		modelStack.PopMatrix();
		break;
	case Weapon::CROSSBOW:
		modelStack.PushMatrix();
		modelStack.Translate(pos.x, pos.y, pos.z);
		modelStack.Scale(scale.x - 2, scale.y - 2, scale.z);
		meshList[GEO_CROSSBOW]->material.kAmbient.Set(1, 1, 1);
		RenderMesh(meshList[GEO_CROSSBOW], true);
		modelStack.PopMatrix();
		break;
	}
}


bool SceneShop::CheckEquip(Weapon::WEAPONTYPE wep)
{
	if ((cGameManager->weptype == wep) || (cGameManager->sideweptype == wep))
	{
		return false;
	}
	return true;
}

