#include "Swordsman.h"

Swordsman::Swordsman()
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
    sCurrState = IDLE;
}

Swordsman::~Swordsman()
{
    enemyGameObject = nullptr;
    PlayerPointer = nullptr;
}

bool Swordsman::Update(double dt)
{
    switch (sCurrState)
    {
    case IDLE:
        break;
    case CHASE:
        //chase the player
        enemyGameObject->pos += (PlayerPointer->getPlayer()->pos - enemyGameObject->pos).Normalize() * dt * movementSpeed;
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

