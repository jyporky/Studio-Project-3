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
    attackSpeed = 1.5;
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

    if (greenTimer > 0)
    {
        enemyGameObject->color.Set(0, 1, 0);
        greenTimer -= dt;
    }
    else
        enemyGameObject->color.Set(1, 1, 1);

    if (attackdt > 0)
        attackdt -= dt;

    if ((PlayerPointer->getPlayer()->pos - enemyGameObject->pos).LengthSquared() > attackRange * attackRange)
        sCurrState = CHASE;
    switch (sCurrState)
    {
    case IDLE:
        //do nothing
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
        //Attack the player
        if (attackdt <= 0)
        {
            attackdt = 1.5;
        }
        break;
    }
    return false;
}

void Swordsman::Init()
{
    //get the revelant pointers
    PlayerPointer = Player::GetInstance();
}

