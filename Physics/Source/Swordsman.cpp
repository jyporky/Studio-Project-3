#include "Swordsman.h"

Swordsman::Swordsman()
{
    health = 20;
    redTimer = 0;
    movementSpeed = 20;
    energyDropped = 2;
    moneyDropped = 10;
    attackDamage = 5;
    affectedByKnockback = true;
    gameobject = nullptr;
    PlayerPointer = nullptr;
    CurrWeapon = nullptr;
    sCurrState = CHASE;
    attackRange = 12;
    attackSpeed = 1.5;
    iFrameTimer = 0;
    enemytype = SWORDMAN;
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
    if (iFrameTimer > 0)
    {
        iFrameTimer -= dt;
    }
    //check if the enemy is dead
    if (health <= 0)
    {
        return true;
    }

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
    if (isStunned)
        sCurrState = IDLE;
    if (cGameManager->bPlayerLost)
        sCurrState = IDLE;
    
    Entity* Target = PlayerPointer;
    if (turned)
    {
        //find an enemy to target
    }

    
    
    Vector3 direction;
    direction.SetZero();
    switch (sCurrState)
    {
    case IDLE:
        //do nothing
        break;
    case CHASE:
        //chase the player
        leftdt += dt;
        if (!moveleft)
        {
            direction = (gameobject->pos - Target->GetGameObject()->pos).Normalize();
            direction = Vector3(-direction.y, direction.x, 0);
            if (leftdt > 2)
            {
                moveleft = !moveleft;
                leftdt = 0;
            }
        }
        else
        {
            direction = (gameobject->pos - Target->GetGameObject()->pos).Normalize();
            direction = -(Vector3(-direction.y, direction.x, 0));
            if (leftdt > 2)
            {
                moveleft = !moveleft;
                leftdt = 0;
            }
        }
        gameobject->pos += ((Target->GetGameObject()->pos - gameobject->pos).Normalize() + direction) * dt * movementSpeed;
        if ((Target->GetGameObject()->pos - gameobject->pos).LengthSquared() <= attackRange * attackRange)
        {
            sCurrState = ATTACK;
        }
        break;
    case ATTACK:
        leftdt = 0;
        if ((Target->GetGameObject()->pos - gameobject->pos).LengthSquared() > attackRange * attackRange)
        {
            sCurrState = CHASE;
            break;
        }
        //Attack the player
        if (CurrWeapon->attack())
        {
            //deal damage to the player
            if (PlayerPointer->iFrame == false)
            {
                PlayerPointer->ChangeHealth(-attackDamage);
            }
            sCurrState = BACK_OFF;
        }
        break;
    case BACK_OFF:
        leftdt += dt;
        if (!moveleft)
        {
            direction = (gameobject->pos - Target->GetGameObject()->pos).Normalize();
            direction = Vector3(-direction.y, direction.x, 0);
            if (leftdt > switchtime)
            {
                moveleft = !moveleft;
                leftdt = 0;
            }
        }
        else
        {
            direction = (gameobject->pos - Target->GetGameObject()->pos).Normalize();
            direction = -(Vector3(-direction.y, direction.x, 0));
            if (leftdt > switchtime)
            {
                moveleft = !moveleft;
                leftdt = 0;
            }
        }
        //check if the enemy can attack or not
        if (CurrWeapon->attacktest())
        {
            leftdt = 0;
            sCurrState = CHASE;
            break;
        }

        //move the enemy away from the target
        gameobject->pos -= (Target->GetGameObject()->pos - gameobject->pos).Normalize() * dt * movementSpeed;
        break;
    }

    gameobject->pos.z = 0;
    // Make the sword point to the player
    CurrWeapon->Update(dt, Target->GetGameObject()->pos, 0, gameobject);
    return false;
}

void Swordsman::Init()
{
    switchtime = Math::RandFloatMinMax(0.5, 8);
    //get the revelant pointer
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
bool Swordsman::getStunned() {
    return isStunned;
}
void Swordsman::makeEnemyStunned() {
    isStunned = true;
}