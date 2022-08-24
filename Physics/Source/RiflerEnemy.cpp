#include "RiflerEnemy.h"

Rifler::Rifler()
{
    health = 15;
    movementSpeed = 14;
    energyDropped = 2;
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

  

    if (cGameManager->bPlayerLost)
        sCurrState = IDLE;


    if (!isCamper)
    {
        switch (sCurrState)
        {
        case IDLE:
        {
            //do nothing
            break; 
        }
        case CHASE:
        {
            //chase the player
            gameobject->pos += (PlayerPointer->getPlayer()->pos - gameobject->pos).Normalize() * dt * movementSpeed;
            if ((PlayerPointer->getPlayer()->pos - gameobject->pos).LengthSquared() <= attackRange * attackRange)
            {
                sCurrState = ATTACK;
            }
            break;
        }
        case ATTACK:
        {
            if ((PlayerPointer->getPlayer()->pos - gameobject->pos).LengthSquared() > attackRange * attackRange) {
                sCurrState = CHASE;

            }
            if ((PlayerPointer->getPlayer()->pos - gameobject->pos).LengthSquared() < runRange * runRange) {
                sCurrState = RUN;
            }
            break; 
        }
        case RUN:
        {
            Vector3 runAway = gameobject->pos - PlayerPointer->getPlayer()->pos;
            gameobject->pos += runAway.Normalized() * movementSpeed * dt;
            //gameobject->pos -= (PlayerPointer->getPlayer()->pos - gameobject->pos).Normalize() * dt * movementSpeed;
            if ((PlayerPointer->getPlayer()->pos - gameobject->pos).LengthSquared() >= runRange * runRange) {
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
            if ((PlayerPointer->getPlayer()->pos - gameobject->pos).LengthSquared() <= runRange * runRange)
                sCurrState = CHASE;
            break;
        }
    }

    if (kbTimer > 0)
    {
        gameobject->pos -= kbEffect;
        kbTimer -= dt;
    }

    gameobject->pos.z = 0;
    if (CurrWeapon->attack())
        isSpawningBullet = true;
    // Make the sword point to the player
    CurrWeapon->Update(dt, PlayerPointer->getPlayer()->pos, 0, gameobject);
    return false;
}

void Rifler::Init()
{
    isCamper = Math::RandInt();
    //get the revelant pointers
    PlayerPointer = Player::GetInstance();
}
bool Rifler::getStunned() {
    return isStunned;
}
void Rifler::makeEnemyStunned() {
    isStunned = true;
}