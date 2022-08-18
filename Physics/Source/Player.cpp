#include "Player.h"

Player::Player()
{
	greenTimer = 0;
	redTimer = 0;
	maxHealth = 100;
	health = maxHealth;
	m_player = nullptr;
	weapon = nullptr;
	cGameManager = GameManger::GetInstance();
	cSoundController = CSoundController::GetInstance();
}

Player::~Player()
{
	if (weapon)
	{
		delete weapon;
		weapon = nullptr;
	}
}

void Player::SetGameObject(GameObject* player)
{
	m_player = player;
}

void Player::Update(double dt, Vector3 mousepos)
{
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
		m_player->angle = 0;
	}

	else if (movementDirection.x < 0)
	{
		m_player->angle = 180;
	}

	m_player->pos += movementDirection.Normalize() * 40 * dt;

	if (weapon)
	{
		static bool attack = false;
		if (Application::IsMousePressed(0) && !attack)
		{
			weapon->attack();
			attack = true;
		}
		else if (!Application::IsMousePressed(0) && attack)
			attack = false;
		weapon->Update(dt, mousepos, movementDirection, m_player);
	}

	if (redTimer > 0)
	{
		m_player->color.Set(1, 0, 0);
		redTimer -= dt;
	}
	else
		m_player->color.Set(1, 1, 1);

	if (greenTimer > 0)
	{
		m_player->color.Set(0, 1, 0);
		greenTimer -= dt;
	}
	else if (redTimer <= 0)
		m_player->color.Set(1, 1, 1);
}

void Player::ChangeHealth(int ChangeAmount)
{
	if (ChangeAmount > 0 && health != maxHealth)
		greenTimer = 0.5;
	else if (ChangeAmount < 0)
	{

	}
		redTimer = 0.5;

	if (ChangeAmount > 0 && health == maxHealth)
		return;

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
	this->weapon = weapon;
}

Weapon* Player::GetWeapon()
{
	return weapon;
}


GameObject* Player::getPlayer()
{
	return m_player;
}