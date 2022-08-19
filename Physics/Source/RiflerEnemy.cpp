#include "RiflerEnemy.h"

Rifler::Rifler()
{
    health = 15;
    redTimer = 0;
    attackdt = 0;
    movementSpeed = 4;
    energyDropped = 2;
    moneyDropped = 5;
    attackDamage = 3;
    affectedByKnockback = true;
    gameobject = nullptr;
    PlayerPointer = nullptr;
    CurrWeapon = nullptr;
    sCurrState = CHASE;
    attackRange = 50;
    attackSpeed = 1.5;
    isSpawningBullet = false;
}

Rifler::~Rifler()
{
    gameobject = nullptr;
    PlayerPointer = nullptr;
    if (CurrWeapon)
    {
        delete CurrWeapon;
        CurrWeapon = nullptr;
    }
}
bool Rifler::IsSpawningBullet() {
    return isSpawningBullet;
}
bool Rifler::Update(double dt)
{
    isSpawningBullet = false;
    //check if the enemy is dead
    if (health <= 0)
        return true;

    if (redTimer > 0)
    {
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
        isSpawningBullet = false;
        gameobject->pos += (PlayerPointer->getPlayer()->pos - gameobject->pos).Normalize() * dt * movementSpeed;
        if ((PlayerPointer->getPlayer()->pos - gameobject->pos).LengthSquared() <= attackRange * attackRange)
        {
            sCurrState = ATTACK;
        }
        break;
    case ATTACK:
        if ((PlayerPointer->getPlayer()->pos - gameobject->pos).LengthSquared() > attackRange * attackRange) {
            sCurrState = CHASE;
           
        }
       // Attack the player
       if(CurrWeapon->attack())
         isSpawningBullet = true;
        
        break;
    }

    // Make the sword point to the player
    CurrWeapon->Update(dt, PlayerPointer->getPlayer()->pos, 0, gameobject);
    return false;
}

void Rifler::Init()
{
    //get the revelant pointers
    PlayerPointer = Player::GetInstance();
}