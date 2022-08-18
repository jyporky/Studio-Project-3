#include "Player.h"

Player::Player()
{
	greenTimer = 0;
	redTimer = 0;
	maxHealth = 100;
	health = maxHealth;
	gameobject = nullptr;
	CurrWeapon = nullptr;
	cGameManager = GameManger::GetInstance();
	cSoundController = CSoundController::GetInstance();
	movementspeed = 40;
	isSpawningBullet = false;
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

	if (movementDirection.x > 0)
	{
		gameobject->angle = 0;
	}

	else if (movementDirection.x < 0)
	{
		gameobject->angle = 180;
	}

	gameobject->pos += movementDirection.Normalize() * movementspeed * dt;

	//check if the player is holding a weapon
	if (CurrWeapon)
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
		CurrWeapon->Update(dt, mousepos, movementDirection, gameobject);
	}


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

void Player::SetMaxHealth(unsigned newMaxHealth)
{
	maxHealth = newMaxHealth;
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

			//check if the angle of vector is less than half the attack angle
			dotproduct = Math::RadianToDegree(acos(dotproduct / (m_enemyList[idx]->GetGameObject()->pos.Distance(gameobject->pos) * mousepos.Distance(gameobject->pos))));
			if (dotproduct > CurrWeapon->GetAttackAngle() * 0.5f)
				continue;

			m_enemyList[idx]->ChangeHealth(-CurrWeapon->GetDamage());
			hitlist.push_back(m_enemyList[idx]);
		}
	}
}

bool Player::IsSpawningBullet()
{
	return isSpawningBullet;
}

Weapon* Player::GetWeapon()
{
	return CurrWeapon;
}


GameObject* Player::getPlayer()
{
	return gameobject;
}