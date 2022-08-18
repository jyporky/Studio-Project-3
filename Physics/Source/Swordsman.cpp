#include "Swordsman.h"

Swordsman::Swordsman()
{
    health = 20;
    redTimer = 0;
    attackdt = 0;
    movementSpeed = 20;
    energyDropped = 2;
    moneyDropped = 2;
    attackDamage = 5;
    affectedByKnockback = true;
    gameobject = nullptr;
    PlayerPointer = nullptr;
    CurrWeapon = nullptr;
    sCurrState = CHASE;
    attackRange = 12;
    attackSpeed = 1.5;
}

Swordsman::~Swordsman()
{
    gameobject = nullptr;
    PlayerPointer = nullptr;
    if (CurrWeapon)
    {
        delete CurrWeapon;
        CurrWeapon = nullptr;
    }
}

bool Swordsman::Update(double dt)
{
    //check if the enemy is dead
    if (health <= 0)
    {
        cSoundController->PlaySoundByID(4);
        return true;
    }

    if (redTimer > 0)
    {
        cSoundController->PlaySoundByID(3);
        gameobject->color.Set(1, 0, 0);
        redTimer -= dt;
    }
    else
        gameobject->color.Set(1, 1, 1);

    if (greenTimer > 0)
    {
        gameobject->color.Set(0, 1, 0);
        greenTimer -= dt;
    }
    else if (redTimer <= 0)
        gameobject->color.Set(1, 1, 1);

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
        gameobject->pos += (PlayerPointer->getPlayer()->pos - gameobject->pos).Normalize() * dt * movementSpeed;
        if ((PlayerPointer->getPlayer()->pos - gameobject->pos).LengthSquared() <= attackRange * attackRange)
        {
            sCurrState = ATTACK;
        }
        break;
    case ATTACK:
        if ((PlayerPointer->getPlayer()->pos - gameobject->pos).LengthSquared() > attackRange * attackRange)
            sCurrState = CHASE;
        //Attack the player
        if (attackdt <= 0)
        {
            //deal damage to the player
            CurrWeapon->attack();
            attackdt = attackSpeed;
            if (PlayerPointer->iFrame == false)
            {
                PlayerPointer->ChangeHealth(-attackDamage);
            }
            
        }
        break;
    }

    // Make the sword point to the player
    CurrWeapon->Update(dt, PlayerPointer->getPlayer()->pos, 0, gameobject);
    return false;
}

void Swordsman::Init()
{
    //get the revelant pointers
    PlayerPointer = Player::GetInstance();
}

void Swordsman::SetWeapon(Weapon* weapon)
{
    CurrWeapon = weapon;
}

Weapon* Swordsman::GetWeapon()
{
    return CurrWeapon;
}