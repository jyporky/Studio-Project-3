#include "Player.h"

Player::Player()
{
	greenTimer = 0;
	redTimer = 0;
	blueTimer = 0;
	maxHealth = 100;
	health = maxHealth;
	gameobject = nullptr;
	CurrWeapon = nullptr;
	SideWeapon = nullptr;
	cGameManager = GameManger::GetInstance();
	cSoundController = CSoundController::GetInstance();
	movementspeed = 40;
	dashBoost = 80;
	iFrame = false;
	money = 0;
	energy = 0;
	isSpawningBullet = false;
	dashDirection.Set(1, 0, 0);

	meleeDmgBoost = 0;
	rangeDmgBoost = 0;
	dashing = false;
	rotate = 0;
}

Player::~Player()
{
	if (CurrWeapon)
	{
		delete CurrWeapon;
		CurrWeapon = nullptr;
	}
}

void Player::SetGameObject(GameObject* player)
{
	gameobject = player;
}

void Player::Update(double dt, Vector3 mousepos)
{
	if (redTimer > 0)
	{
		gameobject->color.Set(1, 0, 0);
		redTimer -= dt;
	}
	else
		gameobject->color.Set(1, 1, 1);

	if (greenTimer > 0)
	{
		gameobject->color.Set(0, 1, 0);
		greenTimer -= dt;
	}
	else if (redTimer <= 0)
		gameobject->color.Set(1, 1, 1);
	else if (redTimer <= 0)
		gameobject->color.Set(1, 1, 1);

	//for dashing and i frame
	if (blueTimer > 0)
	{
		blueTimer -= dt;
		gameobject->color.Set(0.6, 0.6, 1);
		iFrame = true;
	}
	else
		iFrame = false;

	if (dashcd > 0)
		dashcd -= dt;

	isSpawningBullet = false;
	//deal with the player movement
	Vector3 movementDirection;
	movementDirection.SetZero();

	if (Application::IsKeyPressed('W'))
	{
		movementDirection.y += 1;
	}

	if (Application::IsKeyPressed('S'))
	{
		movementDirection.y -= 1;
	}

	if (Application::IsKeyPressed('A'))
	{
		movementDirection.x -= 1;
	}

	if (Application::IsKeyPressed('D'))
	{
		movementDirection.x += 1;
	}
	gameobject->vel = movementDirection.Normalize() * movementspeed;

	if (movementDirection != Vector3(0, 0, 0) && !dashing)
		dashDirection = movementDirection;

	if (!dashing)
	{
		if (movementDirection.x > 0)
		{
			gameobject->angle = 0;
		}
		else if (movementDirection.x < 0)
		{
			gameobject->angle = 180;
		}
		rotate = 0;
	}
	else
	{
		if (dashDirection.x > 0)
		{
			gameobject->angle = 0;
			rotate -= 1800 * dt;
		}
		else if (dashDirection.x < 0)
		{
			gameobject->angle = 180;
			rotate += 1800 * dt;
		}
		else
		{
			rotate += 1800 * dt;
		}
	}
	//dashing
	if(movementspeed > dashBoost && blueTimer <= 0)
	{
		movementspeed -= dashBoost;
		blueTimer = 0;
		dashing = false;
		CurrWeapon->GetGameObject()->visible = true;
	}
	if (Application::IsKeyPressed(' ') && blueTimer <= 0 && dashcd <= 0)
	{
		movementspeed += dashBoost;
		blueTimer = 0.2;
		cSoundController->StopPlayByID(6);
		cSoundController->PlaySoundByID(6);
		dashing = true;
		CurrWeapon->GetGameObject()->visible = false;
		dashcd = 1;
	}

	static bool switch_weapon = false;
	if (Application::IsMousePressed(1) && !switch_weapon && SideWeapon != nullptr && !dashing)
	{
		CurrWeapon->GetGameObject()->visible = false;
		SwapWeapon();
		CurrWeapon->GetGameObject()->visible = true;
		switch_weapon = true;
	}
	else if (!Application::IsMousePressed(1) && switch_weapon)
		switch_weapon = false;


	if (!dashing)
		gameobject->pos += movementDirection.Normalize() * movementspeed * dt;
	else
		gameobject->pos += dashDirection.Normalize() * movementspeed * dt;

	//check if the player is holding a weapon
	if (CurrWeapon)
	{
		if (!dashing)
		{
			//if the weapon is animating and is a melee weapon
			if (CurrWeapon->Animate && CurrWeapon->IsMelee)
			{
				Attack(clickMousePos);
			}

			static bool attack = false;
			if (Application::IsMousePressed(0) && (!attack || !CurrWeapon->IsMelee))
			{
				//do the damage to the enemies
				if (CurrWeapon->attack())
				{
					clickMousePos = mousepos;
					if (CurrWeapon->IsMelee)
						hitlist.clear();
					else
					{
						//spawn a bullet
						isSpawningBullet = true;
					}
				}
				attack = true;
			}
			else if (!Application::IsMousePressed(0) && attack)
				attack = false;
		}
		CurrWeapon->Update(dt, mousepos, movementDirection, gameobject);
	}
}

bool Player::ChangeHealth(int ChangeAmount)
{
	bool godmode = false;
	if (godmode)
		return false;
	if (ChangeAmount > 0 && health != maxHealth)
		greenTimer = 0.5;
	else if (ChangeAmount < 0)
	{
		redTimer = 0.5;
		cSoundController->StopPlayByID(1);
		cSoundController->PlaySoundByID(1);
	}

	if (ChangeAmount > 0 && health == maxHealth)
		return false;

	health += ChangeAmount;
	if (health <= 0)
	{
		cGameManager->bPlayerLost = true;
		health = 0;
	}
	else if (health > maxHealth)
	{
		health = maxHealth;
	}
	return false;
}

void Player::SetEnemyVector(std::vector<Entity*> m_enemyList)
{
	this->m_enemyList = m_enemyList;
}

unsigned Player::GetHealth()
{
	return health;
}

unsigned Player::GetMaxHealth()
{
	return maxHealth;
}

void Player::SetMaxHealth(unsigned changedMaxHealth)
{
	maxHealth = changedMaxHealth;
	health = maxHealth;
}

void Player::SetWeapon(Weapon* weapon)
{
	this->CurrWeapon = weapon;
}

//a function to deal with damaging enemies with the weapon
void Player::Attack(Vector3 mousepos)
{
	if (CurrWeapon->IsMelee)
	{
		//for melee weapons
		for (unsigned idx = 0; idx < m_enemyList.size(); idx++)
		{
			
			//check if the enemy has been hit before
			bool hitb4 = false;
			for (unsigned idx1 = 0; idx1 < hitlist.size(); idx1++)
			{
				if (m_enemyList[idx] == hitlist[idx1])
				{
					hitb4 = true;
					break;
				}
			}
			if (hitb4)
				continue;

			//check through each enemy for distance
			if (m_enemyList[idx]->GetGameObject()->pos.DistanceSquared(gameobject->pos) > CurrWeapon->GetRange() * CurrWeapon->GetRange())
			{
				continue;
			}
			Vector3 enemy2player = m_enemyList[idx]->GetGameObject()->pos - gameobject->pos;
			Vector3 mouse2player = clickMousePos - gameobject->pos;
			float dotproduct = enemy2player.Dot(mouse2player);
			//check if the angles are pointing in the same direction
			if (dotproduct < 0)
				continue;
			//check if the shield enemy is blocking the attack
			if (m_enemyList[idx]->GetEnemyType() == Entity::SHIELDMAN)
			{
				Bullet* testbullet = new Bullet();
				testbullet->SetGameObject(new GameObject);
				testbullet->GetGameObject()->pos = gameobject->pos;
				//testbullet->GetGameObject()->pos += (m_enemyList[idx]->GetGameObject()->pos - gameobject->pos).Normalize() * 2;

				if (Entity::CheckShieldCollision(testbullet, m_enemyList[idx]))
				{
					//attack blocked by the enemy
					delete testbullet->GetGameObject();
					delete testbullet;
					//add enemy to the hit list
					hitlist.push_back(m_enemyList[idx]);
					continue;
				}
				delete testbullet->GetGameObject();
				delete testbullet;
			}

			//check if the angle of vector is less than half the attack angle
			dotproduct = Math::RadianToDegree(acos(dotproduct / (m_enemyList[idx]->GetGameObject()->pos.Distance(gameobject->pos) * mousepos.Distance(gameobject->pos))));
			if (dotproduct > CurrWeapon->GetAttackAngle() * 0.5f)
				continue;

			m_enemyList[idx]->ChangeHealth(-CurrWeapon->GetDamage() - meleeDmgBoost);
			hitlist.push_back(m_enemyList[idx]);
		}
	}
}

bool Player::IsSpawningBullet()
{
	return isSpawningBullet;
}

int Player::getMoney()
{
	return money;
}

void Player::changeMoney(float moneyChange)
{
	money += moneyChange;
}

int Player::getEnergy()
{
	return energy;
}

void Player::changeMovementSpeed(float change)
{
	movementspeed += change;
}

void Player::changeEnergy(float energyChange)
{
	energy += energyChange;
}

void Player::SwapWeapon()
{
	Weapon* tempwep = CurrWeapon;
	CurrWeapon = SideWeapon;
	SideWeapon = tempwep;
	cSoundController->StopPlayByID(12);
	cSoundController->PlaySoundByID(12);

	if (cGameManager->weptype != 0)
	{
		int temptype = cGameManager->weptype;
		cGameManager->weptype = cGameManager->sideweptype;
		cGameManager->sideweptype = temptype;
	}
}

Weapon* Player::GetWeapon()
{
	return CurrWeapon;
}

Weapon* Player::GetSideWeapon()
{
	return SideWeapon;
}


GameObject* Player::getPlayer()
{
	return gameobject;
}

