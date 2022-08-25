#include "DoppelgangerAlly.h"

DoppelgangerAlly::DoppelgangerAlly()
{
    health = 1;
    maxhealth = health;
    redTimer = 0;
    movementSpeed = 10;
    attackDamage = 3;
    gameobject = nullptr;
    CurrWeapon = nullptr;
    sCurrState = CHASE;
    attackRange = 12;
    attackSpeed = 2.0;
    iFrameTimer = 0;
  /*  enemytype = SWORDMAN;*/
}

DoppelgangerAlly::~DoppelgangerAlly()
{
    gameobject = nullptr;
    if (CurrWeapon)
    {
        delete CurrWeapon;
        CurrWeapon = nullptr;
    }
}
void DoppelgangerAlly::SetEnemyVector(std::vector<Entity*> m_enemyList)
{
    this->m_enemyList = m_enemyList;
}
bool DoppelgangerAlly::Update(double dt)
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

    
    Entity* Target = nullptr;
    //if (m_enemyList.size() > 0) {
        for (unsigned idx = 0; idx < m_enemyList.size(); idx++) {
            if (m_enemyList[idx]->GetGameObject() == gameobject)
                continue;
            if (Target == nullptr) {
                Target = m_enemyList[idx];
                continue;
            }
            if ((m_enemyList[idx]->GetGameObject()->pos - gameobject->pos).LengthSquared() < (Target->GetGameObject()->pos - gameobject->pos).LengthSquared()) {
                Target = m_enemyList[idx];
            }
        }
   // }
    
    if (Target == nullptr)
        return false;
    switch (sCurrState)
    {
    case IDLE:
        //do nothing
        break;
    case CHASE:
        //chase the player
        gameobject->pos += (Target->GetGameObject()->pos - gameobject->pos).Normalize() * dt * movementSpeed;
        if ((Target->GetGameObject()->pos - gameobject->pos).LengthSquared() <= attackRange * attackRange)
        {
            sCurrState = ATTACK;
        }
        break;
    case ATTACK:
        if ((Target->GetGameObject()->pos - gameobject->pos).LengthSquared() > attackRange * attackRange)
        {
            sCurrState = CHASE;
            break;
        }
        //Attack the enemy
        if (CurrWeapon->attack())
        {
           /* for (unsigned idx = 0; idx < m_enemyList.size(); idx++) {
                m_enemyList[idx]->ChangeHealth(-attackDamage);
            }*/
          Target->ChangeHealth(-attackDamage);
        }
        break;
    }

    gameobject->pos.z = 0;
    // Make the sword point to the player
    CurrWeapon->Update(dt, Target->GetGameObject()->pos, 0, gameobject);
    return false;
}

void DoppelgangerAlly::Init()
{
    //get the revelant pointers
}

void DoppelgangerAlly::SetWeapon(Weapon* weapon)
{
    CurrWeapon = weapon;
}

Weapon* DoppelgangerAlly::GetWeapon()
{
    return CurrWeapon;
}
bool DoppelgangerAlly::ChangeHealth(int changeInHealth) {
    if (changeInHealth < 0)
    {
        if (iFrameTimer > 0)
        {
            return false;
        }
        cSoundController->PlaySoundByID(3);
        redTimer = 0.5;
        iFrameTimer = 0.15;
    }

    health += changeInHealth;
    if (health <= 0)
    {
        cSoundController->StopPlayByID(4);
        cSoundController->PlaySoundByID(4);
        return true;
    }
    return false;
}