#include "Player.h"

Player::Player()
{

}

void Player::SetGameObject(GameObject* player)
{
	m_player = player;
}

void Player::Update(double dt)
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
}

Player::~Player()
{

}

GameObject* Player::getPlayer()
{
	return m_player;
}