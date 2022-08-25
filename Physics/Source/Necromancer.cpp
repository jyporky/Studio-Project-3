#include "Necromancer.h"

Necromancer::Necromancer()
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
    turned = false;
}

Necromancer::~Necromancer()
{
    gameobject = nullptr;
    PlayerPointer = nullptr;
    if (CurrWeapon)
    {
        delete CurrWeapon;
        CurrWeapon = nullptr;
    }
}

bool Necromancer::Update(double dt)
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
    Target = nullptr;

    //find an enemy to target
    Vector3 direction;
    direction.SetZero();

    if (turned)
    {
        for (unsigned idx = 0; idx < m_enemyList.size(); idx++) {
            if (m_enemyList[idx] == this)
                continue;
            if (Target == nullptr) {
                Target = m_enemyList[idx];
                continue;
            }
            if ((m_enemyList[idx]->GetGameObject()->pos - gameobject->pos).LengthSquared() < (Target->GetGameObject()->pos - gameobject->pos).LengthSquared()) {
                Target = m_enemyList[idx];
            }
        }
    }
    else {
        Target = PlayerPointer;
    }

    if (Target)
    {
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
                //sCurrState = ATTACK;
            }
            break;
        }


        gameobject->pos.z = 0;
        // Make the sword point to the player
        CurrWeapon->Update(dt, Target->GetGameObject()->pos, 0, gameobject);
    }
    return false;
}

void Necromancer::Init()
{
    switchtime = Math::RandFloatMinMax(0.5, 8);
    //get the revelant pointer
    PlayerPointer = Player::GetInstance();
}

void Necromancer::SetWeapon(Weapon* weapon)
{
    CurrWeapon = weapon;
}

Weapon* Necromancer::GetWeapon()
{
    return CurrWeapon;
}
bool Necromancer::getStunned() {
    return isStunned;
}
void Necromancer::makeEnemyStunned() {
    isStunned = true;
}
void Necromancer::turnEnemy() {
    turned = true;
}