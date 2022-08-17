#include "Enemy.h"

Player* Enemy::PlayerPointer = Player::GetInstance();
GameManger* Enemy::cGameManager = GameManger::GetInstance();

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
    attackSpeed = 0;
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

void Enemy::SetEnemyGameObject(GameObject* enemyGO)
{
    enemyGameObject = enemyGO;
}

GameObject* Enemy::GetEnemyGameObject()
{
    return enemyGameObject;
}

void Enemy::SetGO(GameObject* newGameObject)
{
    enemyGameObject = newGameObject;
}

bool Enemy::ChangeHealth(int changeInHealth)
{
    if (changeInHealth < 0)
        redTimer = 0.5;
    else if (changeInHealth > 0)
        greenTimer = 0.5;

    health += changeInHealth;
    if (health <= 0)
    {
        return true;
    }
    return false;
}
