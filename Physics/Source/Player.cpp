#include "Player.h"

Player::Player()
{

}

void Player::SetGameObject(GameObject* player)
{
	m_player = player;
}

void Player::update()
{

}

Player::~Player()
{

}

GameObject* Player::getPlayer()
{
	return m_player;
}