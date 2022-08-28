#include "RiflerEnemy.h"

Rifler::Rifler()
{
    health = 15;
    maxhealth = health;
    movementSpeed = 14;
    energyDropped = 8;
    moneyDropped = 20;
    attackDamage = 3;
    affectedByKnockback = true;
    gameobject = nullptr;
    PlayerPointer = nullptr;
    CurrWeapon = nullptr;
    sCurrState = CHASE;
    attackRange = 50;
    attackSpeed = 1.5;
    isSpawningBullet = false;
    runRange = 20;
    iFrameTimer = 0;
    enemytype = RIFER;
    isStunned = false;
    turned = false;
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

    if (iFrameTimer > 0)
    {
        iFrameTimer -= dt;
    }
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

    if (kbTimer > 0)
    {
        gameobject->direction -= kbEffect;
        kbEffect -= dt;
    }

    if (greenTimer > 0)
    {
        gameobject->color.Set(0, 1, 0);
        greenTimer -= dt;
    }
    else if (redTimer <= 0)
        gameobject->color.Set(1, 1, 1);

    if (isStunned) {
        sCurrState = IDLE;
    }
    else if (sCurrState != IDLE) {
        if (CurrWeapon->attack())
            isSpawningBullet = true;
    }

    Target = nullptr;

    if (cGameManager->bPlayerLost)
        sCurrState = IDLE;
    if (!turned) {
        Target = PlayerPointer;
    }
    else
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
    if (Target) {
        if (!isCamper)
        {
            switch (sCurrState)
            {
            case IDLE:
            {
                if ((Target->GetGameObject()->pos - gameobject->pos).LengthSquared() >= attackRange * attackRange)
                    sCurrState = CHASE;
                else if ((Target->GetGameObject()->pos - gameobject->pos).LengthSquared() <= attackRange * attackRange)
                {
                    sCurrState = ATTACK;
                }
                else if ((Target->GetGameObject()->pos - gameobject->pos).LengthSquared() < runRange * runRange) {
                    sCurrState = RUN;
                }
                break;
            }
            case CHASE:
            {
                //chase the player
                gameobject->pos += (Target->GetGameObject()->pos - gameobject->pos).Normalize() * dt * movementSpeed;
                if ((Target->GetGameObject()->pos - gameobject->pos).LengthSquared() <= attackRange * attackRange)
                {
                    sCurrState = ATTACK;
                }
                break;
            }
            case ATTACK:
            {
                if ((Target->GetGameObject()->pos - gameobject->pos).LengthSquared() > attackRange * attackRange) {
                    sCurrState = CHASE;

                }
                if ((Target->GetGameObject()->pos - gameobject->pos).LengthSquared() < runRange * runRange) {
                    sCurrState = RUN;
                }
                break;
            }
            case RUN:
            {
                Vector3 runAway = gameobject->pos - Target->GetGameObject()->pos;
                gameobject->pos += runAway.Normalized() * movementSpeed * dt;
                //gameobject->pos -= (PlayerPointer->getPlayer()->pos - gameobject->pos).Normalize() * dt * movementSpeed;
                if ((Target->GetGameObject()->pos - gameobject->pos).LengthSquared() >= runRange * runRange) {
                    sCurrState = ATTACK;
                }
                break;
            }
            case CAMP:
            {
                sCurrState = ATTACK;
            }
            }
        }
        else
        {
            //find the best corner to camp
            Vector3 targetcorner;
            targetcorner.SetZero();
            unsigned windowHeight = 100.f;
            unsigned windowWidth = windowHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

            //check the quadrant the player is in
            if (PlayerPointer->getPlayer()->pos.x > windowHeight * 0.5f && PlayerPointer->getPlayer()->pos.y > windowHeight * 0.5f)
                targetcorner = Vector3(windowWidth * 0.1f, windowHeight * 0.1f, 0);
            else if (PlayerPointer->getPlayer()->pos.x > windowHeight * 0.5f && PlayerPointer->getPlayer()->pos.y <= windowHeight * 0.5f)
                targetcorner = Vector3(windowWidth * 0.1f, windowHeight * 0.9f, 0);
            else if (PlayerPointer->getPlayer()->pos.x <= windowHeight * 0.5f && PlayerPointer->getPlayer()->pos.y > windowHeight * 0.5f)
                targetcorner = Vector3(windowWidth * 0.9f, windowHeight * 0.1f, 0);
            else
                targetcorner = Vector3(windowWidth * 0.9f, windowHeight * 0.9f, 0);



            switch (sCurrState)
            {
            case IDLE:
                break;
            case CHASE:
                //go to the camping corner
                gameobject->pos += (targetcorner - gameobject->pos).Normalize() * movementSpeed * dt;
                if ((gameobject->pos - targetcorner).Length() <= 5)
                    sCurrState = CAMP;
                break;
            case CAMP:
                if ((Target->GetGameObject()->pos - gameobject->pos).LengthSquared() <= runRange * runRange)
                    sCurrState = CHASE;
                break;
            }
        }
        gameobject->pos.z = 0;
        // Make the sword point to the player
        CurrWeapon->Update(dt, PlayerPointer->getPlayer()->pos, 0, gameobject);
    }
    return false;
}

void Rifler::Init()
{
    float temp = Math::RandFloatMinMax(0,1);
    temp = round(temp);
    isCamper = temp;
    //get the revelant pointers
    PlayerPointer = Player::GetInstance();
}
bool Rifler::getStunned() {
    return isStunned;
}
void Rifler::makeEnemyStunned() {
    isStunned = true;
}
void Rifler::resetEnemyStunned() {
    isStunned = false;
}
bool Rifler::getTurnedState() {
    return turned;
}
void Rifler::turnEnemy() {
    turned = true;
}
void Rifler::resetEnemyTurned()
{
    if(turned)
      turned = false;
}
Entity* Rifler::getTarget() {
    return Target;
}