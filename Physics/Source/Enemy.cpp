#include "Enemy.h"

Player* Enemy::PlayerPointer = Player::GetInstance();

Enemy::Enemy()
{
    health = 0;
    redTimer = 0;
    attackdt = 0;
    movementSpeed = 0;
    energyDropped = 0;
    moneyDropped = 0;
    attackDamage = 0;
    affectedByKnockback = true;
    enemyGameObject = nullptr;
    PlayerPointer = nullptr;
}

Enemy::~Enemy()
{
}

bool Enemy::Update(double dt)
{
    return false;
}

void Enemy::Init()
{
}

void Enemy::SetGO(GameObject* newGameObject)
{
    enemyGameObject = newGameObject;
}

bool Enemy::ChangeHealth(int changeInHealth)
{
    health -= changeInHealth;
    redTimer = 0.5;
    if (health <= 0)
    {
        return true;
    }
    return false;
}
