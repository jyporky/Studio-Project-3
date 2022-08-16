#include "Swordsman.h"

Swordsman::Swordsman()
{
    health = 20;
    redTimer = 0;
    attackdt = 0;
    movementSpeed = 6;
    energyDropped = 2;
    moneyDropped = 2;
    attackDamage = 0;
    affectedByKnockback = true;
    enemyGameObject = nullptr;
    PlayerPointer = nullptr;
    sCurrState = CHASE;
    attackRange = 5;
}

Swordsman::~Swordsman()
{
    enemyGameObject = nullptr;
    PlayerPointer = nullptr;
}

bool Swordsman::Update(double dt)
{
    if (redTimer > 0)
    {
        enemyGameObject->color.Set(1, 0, 0);
        redTimer -= dt;
    }
    else
        enemyGameObject->color.Set(1, 1, 1);

    if ((PlayerPointer->getPlayer()->pos - enemyGameObject->pos).LengthSquared() > attackRange * attackRange)
        sCurrState = CHASE;
    switch (sCurrState)
    {
    case IDLE:
        break;
    case CHASE:
        //chase the player
        enemyGameObject->pos += (PlayerPointer->getPlayer()->pos - enemyGameObject->pos).Normalize() * dt * movementSpeed;
        if ((PlayerPointer->getPlayer()->pos - enemyGameObject->pos).LengthSquared() <= attackRange * attackRange)
        {
            sCurrState = ATTACK;
        }
        break;
    case ATTACK:
        break;
    default:
        break;
    }
    return false;
}

void Swordsman::Init()
{
    //get the revelant pointers
    PlayerPointer = Player::GetInstance();
}

