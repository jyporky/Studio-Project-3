#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

bool Enemy::Update()
{
    return false;
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
