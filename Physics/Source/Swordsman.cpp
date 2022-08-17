#include "Swordsman.h"

Swordsman::Swordsman()
{
    health = 20;
    redTimer = 0;
    attackdt = 0;
    movementSpeed = 6;
    energyDropped = 2;
    moneyDropped = 2;
    attackDamage = 5;
    affectedByKnockback = true;
    enemyGameObject = nullptr;
    PlayerPointer = nullptr;
    weapon = nullptr;
    sCurrState = CHASE;
    attackRange = 12;
    attackSpeed = 1.5;
}

Swordsman::~Swordsman()
{
    enemyGameObject = nullptr;
    PlayerPointer = nullptr;
    if (weapon)
    {
        delete weapon;
        weapon = nullptr;
    }
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
    else if (redTimer <= 0)
        enemyGameObject->color.Set(1, 1, 1);

    if (attackdt > 0)
        attackdt -= dt;

    if (cGameManager->bPlayerLost)
        sCurrState = IDLE;
    
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
        if ((PlayerPointer->getPlayer()->pos - enemyGameObject->pos).LengthSquared() > attackRange * attackRange)
            sCurrState = CHASE;
        //Attack the player
        if (attackdt <= 0)
        {
            //deal damage to the player
            weapon->attack();
            PlayerPointer->ChangeHealth(-attackDamage);
            attackdt = attackSpeed;
        }
        break;
    }

    // Make the sword point to the player
    weapon->Update(dt, PlayerPointer->getPlayer()->pos, 0, enemyGameObject);
    return false;
}

void Swordsman::Init()
{
    //get the revelant pointers
    PlayerPointer = Player::GetInstance();
}

void Swordsman::SetWeapon(Weapon* weapon)
{
    this->weapon = weapon;
}

Weapon* Swordsman::GetWeapon()
{
    return weapon;
}